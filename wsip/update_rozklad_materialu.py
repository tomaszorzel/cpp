#!/usr/bin/env python3
"""
Fill a monthly "szczegółówy_rozkład_materiału" DOCX from a detailed plan DOCX.

The script reads:
- day titles from the "Temat dnia" column,
- child activities from the "Działania dzieci" column.

It writes the day titles, activity names, and "Punkty podstawy programowej"
values into the corresponding weekly tables of the target material plan.
Destination cell formatting is preserved by changing only existing text nodes
where possible.
"""

from __future__ import annotations

import argparse
import copy
import difflib
import importlib
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, NoReturn, cast
from zipfile import ZipFile

try:
    from lxml import etree as ET  # type: ignore
except ImportError:
    from xml.etree import ElementTree as ET  # type: ignore


W = "http://schemas.openxmlformats.org/wordprocessingml/2006/main"
XML = "http://www.w3.org/XML/1998/namespace"
NS = {"w": W}
XmlElement = Any

ET.register_namespace("w", W)

DAY_TITLE_RE = re.compile(r"^\s*(\d+)\.\s*(.+?)\s*$")
TARGET_DAY_RE = re.compile(r"^\s*Dzień\s+(\d+)\s*:\s*.*$", re.IGNORECASE)
AREA_HEADER_RE = re.compile(r"^\s*([1-9])")
ACTIVITY_START_RE = re.compile(r"(?<![\d.])([1-9])\s+(?=\d+\.\d)")
CODE_PREFIX_RE = re.compile(
    r"^\s*(?P<codes>\d+\.\d+[a-z]?(?:(?:\s*,\s*)+\d+\.\d+[a-z]?)*(?:\s*,\s*)?)\s+",
    re.IGNORECASE,
)
CODE_RE = re.compile(r"\d+\.\d+[a-z]?", re.IGNORECASE)
WEEK_RE = re.compile(r"Tydzie[ńn]\s+([IVXLCDM]+)", re.IGNORECASE)


@dataclass(frozen=True)
class DayTitle:
    week: int
    day: int
    title: str


@dataclass(frozen=True)
class Activity:
    week: int
    day: int
    area: int
    codes: list[str]
    title: str
    source_row: int
    aids: list[str] | None = None


@dataclass(frozen=True)
class AidEntry:
    week: int
    title: str
    aids: list[str]
    page: int


@dataclass(frozen=True)
class ActivityPattern:
    area: int
    codes: list[str]
    title_pattern: str
    regex: re.Pattern[str]


@dataclass(frozen=True)
class MonthConfig:
    key: str
    display_name: str
    file_prefix: str
    start_week: int
    pdf: Path


@dataclass(frozen=True)
class MonthJob:
    config: MonthConfig
    source_plan: Path
    target_rozklad: Path
    output_docx: Path
    audit_json: Path


MONTH_CONFIGS = [
    MonthConfig("pazdziernik", "PAŹDZIERNIK", "PAŹDZIERNIK", 5, Path("1827J1_Druzyna_4latek_cz1.pdf")),
    MonthConfig("listopad", "LISTOPAD", "LISTOPAD", 9, Path("1827J1_Druzyna_4latek_cz1.pdf")),
    MonthConfig("grudzien", "GRUDZIEŃ", "GRUDZIEŃ", 13, Path("1827J1_Druzyna_4latek_cz1.pdf")),
    MonthConfig("styczen", "STYCZEŃ", "STYCZEŃ", 17, Path("1827J1_Druzyna_4latek_cz1.pdf")),
    MonthConfig("luty", "LUTY", "LUTY", 21, Path("1827J0_Przewodnik_4-latek_cz2_kk.pdf")),
    MonthConfig("marzec", "MARZEC", "MARZEC", 25, Path("1827J0_Przewodnik_4-latek_cz2_kk.pdf")),
    MonthConfig("kwiecien", "KWIECIEŃ", "KWIECIEŃ", 29, Path("1827J0_Przewodnik_4-latek_cz2_kk.pdf")),
    MonthConfig("maj", "MAJ", "MAJ", 33, Path("1827J0_Przewodnik_4-latek_cz2_kk.pdf")),
    MonthConfig("czerwiec", "CZERWIEC", "CZERWIEC", 37, Path("1827J0_Przewodnik_4-latek_cz2_kk.pdf")),
]


@dataclass(frozen=True)
class TargetDayBlock:
    day: int
    activity_rows: list[int]
    aids_row: int
    points_row: int


def die(message: str, code: int = 2) -> NoReturn:
    print(f"ERROR: {message}", file=sys.stderr)
    raise SystemExit(code)


def norm_space(text: str | None) -> str:
    if not text:
        return ""
    text = text.replace("\xa0", " ").replace("\u00ad", "").replace("\uf03d", " ")
    return re.sub(r"\s+", " ", text).strip()


def clean_xml_text(text: str) -> str:
    return "".join(char for char in text if char in "\t\n\r" or ord(char) >= 32)


def normalize_for_match(text: str | None) -> str:
    text = norm_space(text).lower()
    text = re.sub(r"^[\d\s\.)-]+", "", text)
    text = re.sub(r"[^\wąćęłńóśźż]+", " ", text, flags=re.IGNORECASE)
    return norm_space(text)


def match_score(left: str, right: str) -> float:
    if left == right:
        return 1.0
    if left in right or right in left:
        return 0.94
    return difflib.SequenceMatcher(None, left, right).ratio()


def item_key(item: str) -> str:
    return re.sub(r"\s+", "", item.casefold())


def split_aid_items(text: str) -> list[str]:
    items: list[str] = []
    current: list[str] = []
    depth = 0
    for char in text:
        if char == "(":
            depth += 1
        elif char == ")" and depth:
            depth -= 1
        if char in ",;" and depth == 0:
            items.append("".join(current))
            current = []
        else:
            current.append(char)
    if current:
        items.append("".join(current))
    return items


def split_aids(text: str) -> list[str]:
    seen: set[str] = set()
    aids: list[str] = []
    for item in split_aid_items(norm_space(clean_xml_text(text))):
        item = item.strip(" .")
        item = item.replace("czytelni czego", "czytelniczego")
        item = item.replace("za bawki", "zabawki")
        item = item.replace("gimna styczne", "gimnastyczne")
        item = item.replace("róż nych", "różnych")
        item = item.replace("obrę cze", "obręcze")
        item = item.replace("ob ręcze", "obręcze")
        if not item:
            continue
        key = item_key(item)
        if key not in seen:
            seen.add(key)
            aids.append(item)
    return aids


def quoted_titles(text: str) -> list[str]:
    return [normalize_for_match(title) for title in re.findall(r"„([^”]+)”", text)]


def title_key(title: str) -> str:
    return item_key(normalize_for_match(title))


def title_matches(left_title: str, right_title: str) -> bool:
    left_quotes = quoted_titles(left_title)
    right_quotes = quoted_titles(right_title)
    if left_quotes and right_quotes:
        return any(
            title_key(left_quote) == title_key(right_quote)
            or title_key(left_quote) in title_key(right_quote)
            or title_key(right_quote) in title_key(left_quote)
            for left_quote in left_quotes
            for right_quote in right_quotes
        )

    left_norm = normalize_for_match(left_title)
    right_norm = normalize_for_match(right_title)
    if not left_norm or not right_norm:
        return False
    return match_score(left_norm, right_norm) >= 0.86


def roman_to_int(value: str) -> int:
    numerals = {"I": 1, "V": 5, "X": 10, "L": 50, "C": 100, "D": 500, "M": 1000}
    total = 0
    previous = 0
    for char in reversed(value.upper()):
        current = numerals[char]
        if current < previous:
            total -= current
        else:
            total += current
            previous = current
    return total


def generated_file(path: Path) -> bool:
    stem = path.stem.casefold()
    return path.name.startswith("~$") or any(
        marker in stem for marker in ("_updated", "_audit", "_test")
    )


def first_matching_file(patterns: list[str]) -> Path | None:
    for pattern in patterns:
        matches = sorted(path for path in Path(".").glob(pattern) if not generated_file(path))
        if matches:
            return matches[0]
    return None


def month_config_by_key(key: str) -> MonthConfig | None:
    normalized = normalize_for_match(key)
    for config in MONTH_CONFIGS:
        if normalized in {config.key, normalize_for_match(config.display_name)}:
            return config
    return None


def month_output_path(target_rozklad: Path) -> Path:
    return target_rozklad.with_name(f"{target_rozklad.stem}_updated{target_rozklad.suffix}")


def month_audit_path(target_rozklad: Path) -> Path:
    return target_rozklad.with_name(f"{target_rozklad.stem}_audit.json")


def discover_month_job(config: MonthConfig) -> MonthJob:
    source_plan = first_matching_file(
        [
            f"{config.file_prefix}-Szczeg*plan_pracy.docx",
            f"{config.file_prefix}*Szczeg*plan_pracy.docx",
        ]
    )
    target_rozklad = first_matching_file(
        [
            f"{config.file_prefix}-*rozk*materia*.docx",
            f"{config.file_prefix}*rozk*materia*.docx",
        ]
    )
    if source_plan is None:
        die(f"No source plan DOCX found for {config.display_name}")
    if target_rozklad is None:
        die(f"No target rozkład DOCX found for {config.display_name}")

    return MonthJob(
        config=config,
        source_plan=source_plan,
        target_rozklad=target_rozklad,
        output_docx=month_output_path(target_rozklad),
        audit_json=month_audit_path(target_rozklad),
    )


def qname(ns: str, tag: str) -> str:
    return f"{{{ns}}}{tag}"


def cell_text(cell: XmlElement) -> str:
    return "".join(node.text or "" for node in cell.findall(".//w:t", NS))


def row_cells(row: XmlElement) -> list[XmlElement]:
    return list(row.findall("w:tc", NS))


def replace_cell_text(cell: XmlElement, text: str, run_properties: XmlElement | None = None) -> None:
    text_nodes = cell.findall(".//w:t", NS)
    if not text_nodes:
        paragraph = cell.find("w:p", NS)
        if paragraph is None:
            paragraph = ET.SubElement(cell, qname(W, "p"))
        run = ET.SubElement(paragraph, qname(W, "r"))
        if run_properties is not None:
            run.insert(0, copy.deepcopy(run_properties))
        text_node = ET.SubElement(run, qname(W, "t"))
        text_nodes = [text_node]

    first_text = text_nodes[0]
    if run_properties is not None:
        for run in cell.findall(".//w:r", NS):
            if first_text in run.findall(".//w:t", NS) and run.find("w:rPr", NS) is None:
                run.insert(0, copy.deepcopy(run_properties))
                break

    first_text.text = text
    if text.startswith(" ") or text.endswith(" "):
        first_text.set(qname(XML, "space"), "preserve")
    elif qname(XML, "space") in first_text.attrib:
        del first_text.attrib[qname(XML, "space")]

    for extra_text in text_nodes[1:]:
        extra_text.text = ""


def run_font_properties(run_properties: XmlElement) -> XmlElement | None:
    result = ET.Element(qname(W, "rPr"))
    for tag in ("rFonts", "sz", "szCs"):
        element = run_properties.find(f"w:{tag}", NS)
        if element is not None:
            result.append(copy.deepcopy(element))
    return result if len(result) else None


def table_activity_run_properties(table: XmlElement, area_columns: dict[int, int]) -> XmlElement | None:
    area_column_indexes = set(area_columns.values())
    for row in table.findall("w:tr", NS):
        cells = row_cells(row)
        for column_index in area_column_indexes:
            if column_index >= len(cells):
                continue
            for run_properties in cells[column_index].findall(".//w:rPr", NS):
                font_properties = run_font_properties(run_properties)
                if font_properties is None:
                    continue
                if font_properties.find("w:sz", NS) is not None:
                    return font_properties
    return None


def calibri_10_run_properties() -> XmlElement:
    run_properties = ET.Element(qname(W, "rPr"))
    fonts = ET.SubElement(run_properties, qname(W, "rFonts"))
    for attr in ("ascii", "hAnsi", "eastAsia", "cs"):
        fonts.set(qname(W, attr), "Calibri")
    size = ET.SubElement(run_properties, qname(W, "sz"))
    size.set(qname(W, "val"), "20")
    complex_size = ET.SubElement(run_properties, qname(W, "szCs"))
    complex_size.set(qname(W, "val"), "20")
    return run_properties


def replace_cell_with_code_links(
    cell: XmlElement,
    codes: list[str],
    tooltip_by_code: dict[str, str],
    run_properties: XmlElement | None = None,
) -> None:
    paragraphs = cell.findall("w:p", NS)
    if paragraphs:
        paragraph = paragraphs[0]
        for extra_paragraph in paragraphs[1:]:
            cell.remove(extra_paragraph)
        for child in list(paragraph):
            if child.tag != qname(W, "pPr"):
                paragraph.remove(child)
    else:
        paragraph = ET.SubElement(cell, qname(W, "p"))

    for index, code in enumerate(codes):
        if index:
            paragraph.append(make_text_run(", ", run_properties))
        paragraph.append(make_hyperlink(code, tooltip_by_code, run_properties))


def make_text_run(text: str, run_properties: XmlElement | None = None) -> XmlElement:
    run = ET.Element(qname(W, "r"))
    if run_properties is not None:
        run.insert(0, copy.deepcopy(run_properties))
    text_node = ET.SubElement(run, qname(W, "t"))
    text_node.text = text
    if text.startswith(" ") or text.endswith(" "):
        text_node.set(qname(XML, "space"), "preserve")
    return run


def make_hyperlink(
    code: str,
    tooltip_by_code: dict[str, str],
    run_properties: XmlElement | None = None,
) -> XmlElement:
    hyperlink = ET.Element(qname(W, "hyperlink"))
    hyperlink.set(qname(W, "anchor"), "_top")
    hyperlink.set(qname(W, "history"), "1")

    tooltip = tooltip_by_code.get(code.lower())
    if tooltip:
        hyperlink.set(qname(W, "tooltip"), tooltip)

    run = make_text_run(code, run_properties)
    hyperlink_run_properties = run.find("w:rPr", NS)
    if hyperlink_run_properties is None:
        hyperlink_run_properties = ET.Element(qname(W, "rPr"))
        run.insert(0, hyperlink_run_properties)
    style = hyperlink_run_properties.find("w:rStyle", NS)
    if style is None:
        style = ET.SubElement(hyperlink_run_properties, qname(W, "rStyle"))
    style.set(qname(W, "val"), "Hipercze")
    hyperlink.append(run)
    return hyperlink


def curriculum_hyperlinks(docx_path: Path) -> dict[str, str]:
    with ZipFile(docx_path) as archive:
        root = ET.fromstring(archive.read("word/document.xml"))

    links: dict[str, str] = {}
    for hyperlink in root.findall(".//w:hyperlink", NS):
        text = norm_space(cell_text(hyperlink))
        if CODE_RE.fullmatch(text):
            links[text.lower()] = hyperlink.get(qname(W, "tooltip")) or ""
    return links


def week_on_page(page_text: str) -> int | None:
    matches = WEEK_RE.findall(page_text or "")
    return roman_to_int(matches[-1]) if matches else None


def is_activity_bullet(text: str) -> bool:
    text = norm_space(text)
    if not text:
        return False
    prefixes = (
        "„",
        '"',
        "Zabawy ",
        "Zestaw ",
        "Praca ",
        "Kształtowanie ",
        "Aktywna ",
        "Spacer ",
        "Wyjście ",
        "Słuchanie ",
    )
    return text.startswith(prefixes)


def block_title(block: str) -> str:
    block = norm_space(block)
    block = re.sub(r"^•\s*", "", block)
    block = block.split(" • ", 1)[0]
    for marker in (". N. ", ". Dzieci ", ". Uczestnicy ", ". Przedszkolaki "):
        if marker in block:
            return norm_space(block.split(marker, 1)[0] + ".")
    return block


def parse_aid_block(block: str, week: int, page: int) -> AidEntry | None:
    block = norm_space(block)
    if not block.startswith("•"):
        return None

    parts = block.split(" • ")
    if len(parts) < 2:
        return None

    title = block_title(parts[0])
    aid_text = parts[-1]
    for stop_pattern in (
        r"\s+Wiodąca aktywność:",
        r"\s+Tydzień\s+[IVXLCDM]+",
        r"\s+Drużyna marzeń",
        r"\s+ZABAWY\s+",
        r"\s+[IVX]+\s+CZĘŚĆ\s+DNIA",
        r"\s+Ref\.:",
        r"\s+–\s+",
        r"\s+[A-ZĄĆĘŁŃÓŚŹŻ][^,.;]{2,80}\s+sł\.",
    ):
        aid_text = re.split(stop_pattern, aid_text, maxsplit=1)[0]

    aids = split_aids(aid_text)
    if not title or not aids:
        return None
    return AidEntry(week=week, title=title, aids=aids, page=page)


def parse_pdf_aids_page(page_text: str, week: int, page: int) -> list[AidEntry]:
    aids: list[AidEntry] = []
    current: list[str] = []

    def current_has_aid_marker() -> bool:
        return " • " in norm_space(" ".join(current))

    def is_aid_continuation(line: str) -> bool:
        if not line:
            return False
        first = line[0]
        return first.islower() or first in "([/"

    def flush() -> None:
        if not current:
            return
        entry = parse_aid_block(" ".join(current), week, page)
        if entry is not None:
            aids.append(entry)

    for raw_line in page_text.splitlines():
        line = norm_space(raw_line)
        if not line:
            continue
        if line.startswith("•"):
            bullet_text = norm_space(line[1:])
            if current and is_activity_bullet(bullet_text):
                flush()
                current = [line]
            else:
                current.append(line)
            continue
        if current:
            if current_has_aid_marker() and not is_aid_continuation(line):
                flush()
                current = []
                continue
            current.append(line)

    flush()
    return aids


def extract_pdf_aids(pdf_path: Path, start_week: int = 1) -> list[AidEntry]:
    try:
        fitz: Any = importlib.import_module("fitz")
    except ImportError:
        die("PyMuPDF is required. Install it with: python -m pip install PyMuPDF")

    aid_entries: list[AidEntry] = []
    current_week: int | None = None

    with fitz.open(str(pdf_path)) as doc:
        for page_offset in range(doc.page_count):
            page_number = page_offset + 1
            page = doc.load_page(page_offset)
            page_text = page.get_text("text")
            page_week = week_on_page(page_text)
            if page_week is not None:
                current_week = page_week
            if current_week is None or current_week < start_week:
                continue
            aid_entries.extend(parse_pdf_aids_page(page_text, current_week, page_number))

    if not aid_entries:
        die(f"No PDF aids were extracted from: {pdf_path}")
    return aid_entries


def attach_activity_aids(
    activities_by_day: dict[tuple[int, int], list[Activity]],
    pdf_aids: list[AidEntry],
    start_week: int,
) -> tuple[dict[tuple[int, int], list[Activity]], list[dict[str, object]]]:
    aid_index: dict[int, list[AidEntry]] = {}
    for aid_entry in pdf_aids:
        aid_index.setdefault(aid_entry.week, []).append(aid_entry)

    with_aids: dict[tuple[int, int], list[Activity]] = {}
    unmatched: list[dict[str, object]] = []

    for key, activities in activities_by_day.items():
        week, day = key
        pdf_week = start_week + week - 1
        week_aids = aid_index.get(pdf_week, [])
        for activity in activities:
            matched_aids: list[str] = []
            matched_titles: list[str] = []
            for aid_entry in week_aids:
                if not title_matches(activity.title, aid_entry.title):
                    continue
                matched_titles.append(aid_entry.title)
                matched_aids.extend(aid_entry.aids)

            aids = unique_aids(matched_aids)
            if not aids:
                unmatched.append(
                    {
                        "week": week,
                        "pdf_week": pdf_week,
                        "day": day,
                        "area": activity.area,
                        "title": activity.title,
                        "reason": "no_matching_pdf_aids",
                    }
                )
            with_aids.setdefault(key, []).append(
                Activity(
                    week=activity.week,
                    day=activity.day,
                    area=activity.area,
                    codes=activity.codes,
                    title=activity.title,
                    source_row=activity.source_row,
                    aids=aids,
                )
            )

    return with_aids, unmatched


def unique_aids(values: list[str]) -> list[str]:
    seen: set[str] = set()
    result: list[str] = []
    for value in values:
        key = item_key(value)
        if key in seen:
            continue
        seen.add(key)
        result.append(value)
    return result


def unique_in_order(values: list[str]) -> list[str]:
    seen: set[str] = set()
    result: list[str] = []
    for value in values:
        if value in seen:
            continue
        seen.add(value)
        result.append(value)
    return result


def code_sort_key(code: str) -> tuple[int, int, str]:
    match = re.fullmatch(r"(\d+)\.(\d+)([a-z]?)", code, re.IGNORECASE)
    if not match:
        return (999, 999, code.lower())
    return (int(match.group(1)), int(match.group(2)), match.group(3).lower())


def unique_sorted_codes(values: list[str]) -> list[str]:
    return sorted(set(values), key=code_sort_key)


def extract_source_plan(
    plan_docx: Path,
) -> tuple[dict[tuple[int, int], DayTitle], dict[tuple[int, int], list[Activity]], list[dict[str, object]]]:
    docx_module: Any = importlib.import_module("docx")
    document = docx_module.Document(str(plan_docx))
    activity_patterns = extract_activity_patterns(document)

    titles: dict[tuple[int, int], DayTitle] = {}
    activities: dict[tuple[int, int], list[Activity]] = {}
    parse_warnings: list[dict[str, object]] = []

    for week_index, table in enumerate(document.element.findall(".//w:tbl", NS), start=1):
        current_day: int | None = None
        current_day_marker = ""
        next_unlabeled_day = 1
        for row_index, row in enumerate(table.findall("w:tr", NS), start=1):
            cells = row_cells(row)
            if len(cells) < 2:
                continue

            left_text = norm_space(cell_text(cells[0]))
            day_match = DAY_TITLE_RE.match(left_text)
            if day_match:
                current_day = int(day_match.group(1))
                current_day_marker = left_text
                title = norm_space(day_match.group(2))
                titles[(week_index, current_day)] = DayTitle(
                    week=week_index,
                    day=current_day,
                    title=title,
                )
                next_unlabeled_day = max(next_unlabeled_day, current_day + 1)
            elif (
                left_text
                and left_text != current_day_marker
                and "temat dnia" not in normalize_for_match(left_text)
            ):
                current_day = next_unlabeled_day
                current_day_marker = left_text
                title = left_text
                titles[(week_index, current_day)] = DayTitle(
                    week=week_index,
                    day=current_day,
                    title=title,
                )
                next_unlabeled_day += 1

            if current_day is None:
                continue

            actions_text = norm_space(cell_text(cells[1]))
            if not actions_text:
                continue

            parsed = merge_activities(
                parse_activities(actions_text, week_index, current_day, row_index)
                + parse_fallback_activities(
                    actions_text,
                    week_index,
                    current_day,
                    row_index,
                    activity_patterns,
                )
                + parse_area_only_activity(actions_text, week_index, current_day, row_index)
            )
            if not parsed:
                parse_warnings.append(
                    {
                        "week": week_index,
                        "day": current_day,
                        "source_row": row_index,
                        "text": actions_text,
                        "reason": "no_activity_pattern_found",
                    }
                )
                continue

            activities.setdefault((week_index, current_day), []).extend(parsed)

    if not titles:
        die(f"No day titles found in source plan: {plan_docx}")
    return titles, activities, parse_warnings


def parse_activities(text: str, week: int, day: int, source_row: int) -> list[Activity]:
    starts = list(ACTIVITY_START_RE.finditer(text))
    parsed: list[Activity] = []

    for index, start in enumerate(starts):
        area = int(start.group(1))
        segment_start = start.end()
        segment_end = starts[index + 1].start() if index + 1 < len(starts) else len(text)
        segment = text[segment_start:segment_end].strip()
        codes_match = CODE_PREFIX_RE.match(segment)
        if not codes_match:
            continue

        codes = CODE_RE.findall(codes_match.group("codes"))
        title = segment[codes_match.end() :].strip()
        title = title.rstrip(" .")
        if not codes or not title:
            continue

        parsed.append(
            Activity(
                week=week,
                day=day,
                area=area,
                codes=unique_in_order(codes),
                title=title,
                source_row=source_row,
            )
        )

    return parsed


def parse_activity_pattern_line(text: str) -> ActivityPattern | None:
    match = re.match(r"^\s*([1-9])\s+(.+)$", norm_space(text))
    if not match:
        return None

    area = int(match.group(1))
    rest = match.group(2)
    code_matches = list(CODE_RE.finditer(rest))
    if not code_matches:
        return None

    codes = unique_in_order([code_match.group(0) for code_match in code_matches])
    title_pattern_raw = rest[code_matches[-1].end() :].strip()
    title_pattern = title_pattern_raw.rstrip(" .")
    if not title_pattern:
        return None

    return ActivityPattern(
        area=area,
        codes=codes,
        title_pattern=title_pattern,
        regex=activity_pattern_regex(title_pattern_raw),
    )


def activity_pattern_regex(title_pattern: str) -> re.Pattern[str]:
    pattern_source = re.sub(r"nr\s*\.", "__NUMBERED_SET__", title_pattern)
    pattern_source = pattern_source.replace("„”", "__QUOTED_TITLE__")
    pattern = re.escape(pattern_source)
    pattern = pattern.replace("__QUOTED_TITLE__", r"„[^”]+”")
    pattern = pattern.replace("__NUMBERED_SET__", r"nr\s+\d+")
    pattern = pattern.replace(r"\ ", r"\s+")
    return re.compile(pattern, re.IGNORECASE)


def extract_activity_patterns(plan_docx: Any) -> list[ActivityPattern]:
    patterns: list[ActivityPattern] = []
    for paragraph in plan_docx.paragraphs:
        pattern = parse_activity_pattern_line(paragraph.text)
        if pattern is not None:
            patterns.append(pattern)
    return patterns


def parse_fallback_activities(
    text: str,
    week: int,
    day: int,
    source_row: int,
    patterns: list[ActivityPattern],
) -> list[Activity]:
    matches: list[tuple[int, Activity]] = []
    for pattern in patterns:
        match = pattern.regex.search(text)
        if not match:
            continue
        title = norm_space(match.group(0)).rstrip(" .")
        matches.append(
            (
                match.start(),
                Activity(
                    week=week,
                    day=day,
                    area=pattern.area,
                    codes=pattern.codes,
                    title=title,
                    source_row=source_row,
                ),
            )
        )

    return [activity for _, activity in sorted(matches, key=lambda item: item[0])]


def parse_area_only_activity(text: str, week: int, day: int, source_row: int) -> list[Activity]:
    match = re.match(r"^\s*([1-9])\s+([^\d].+?)\s*$", text)
    if not match:
        return []

    title = norm_space(match.group(2)).rstrip(" .")
    if not title:
        return []

    return [
        Activity(
            week=week,
            day=day,
            area=int(match.group(1)),
            codes=[],
            title=title,
            source_row=source_row,
        )
    ]


def merge_activities(activities: list[Activity]) -> list[Activity]:
    seen: set[tuple[int, str, tuple[str, ...]]] = set()
    merged: list[Activity] = []
    for activity in activities:
        activity = normalize_activity(activity)
        key = (
            activity.area,
            title_key(activity.title),
            tuple(unique_sorted_codes(activity.codes)),
        )
        if key in seen:
            continue
        seen.add(key)
        merged.append(activity)
    return merged


def normalize_activity(activity: Activity) -> Activity:
    if title_key(activity.title) == title_key("Zabawy swobodne i konstrukcyjne w grupach"):
        return Activity(
            week=activity.week,
            day=activity.day,
            area=6,
            codes=activity.codes,
            title=activity.title,
            source_row=activity.source_row,
            aids=activity.aids,
        )
    return activity


def sentence_text(text: str) -> str:
    text = norm_space(text)
    if not text:
        return ""
    if text.endswith((".", "!", "?")):
        return text
    return f"{text}."


def extract_area_columns(table: XmlElement) -> dict[int, int]:
    rows = table.findall("w:tr", NS)
    if len(rows) < 2:
        return {}

    area_columns: dict[int, int] = {}
    for column_index, cell in enumerate(row_cells(rows[1])):
        match = AREA_HEADER_RE.match(norm_space(cell_text(cell)))
        if match:
            area_columns[int(match.group(1))] = column_index
    return area_columns


def find_day_blocks(table: XmlElement) -> dict[int, TargetDayBlock]:
    rows = table.findall("w:tr", NS)
    blocks: dict[int, TargetDayBlock] = {}
    row_index = 0

    while row_index < len(rows):
        cells = row_cells(rows[row_index])
        left_text = norm_space(cell_text(cells[0])) if cells else ""
        day_match = TARGET_DAY_RE.match(left_text)
        if not day_match:
            row_index += 1
            continue

        day = int(day_match.group(1))
        activity_rows: list[int] = []
        while row_index < len(rows):
            cells = row_cells(rows[row_index])
            left_text = norm_space(cell_text(cells[0])) if cells else ""
            if left_text.lower() == "pomoce":
                break
            activity_rows.append(row_index)
            row_index += 1

        aids_row = row_index if row_index < len(rows) else None
        points_row: int | None = None
        search_index = row_index + 1
        while search_index < len(rows):
            cells = row_cells(rows[search_index])
            left_text = norm_space(cell_text(cells[0])) if cells else ""
            if left_text.lower() == "punkty podstawy programowej":
                points_row = search_index
                break
            if TARGET_DAY_RE.match(left_text):
                break
            search_index += 1

        if aids_row is not None and points_row is not None:
            blocks[day] = TargetDayBlock(
                day=day,
                activity_rows=activity_rows,
                aids_row=aids_row,
                points_row=points_row,
            )
            row_index = points_row + 1
        else:
            row_index += 1

    return blocks


def remove_empty_activity_rows(
    table: XmlElement,
    day_blocks: dict[int, TargetDayBlock],
    area_columns: dict[int, int],
) -> int:
    rows = table.findall("w:tr", NS)
    area_column_indexes = set(area_columns.values())
    rows_to_remove: list[XmlElement] = []

    for block in day_blocks.values():
        for row_number in block.activity_rows:
            if row_number >= len(rows):
                continue
            row = rows[row_number]
            cells = row_cells(row)
            has_area_content = any(
                column_index < len(cells) and norm_space(cell_text(cells[column_index]))
                for column_index in area_column_indexes
            )
            if not has_area_content:
                rows_to_remove.append(row)

    for row in reversed(rows_to_remove):
        table.remove(row)
    return len(rows_to_remove)


def update_rozkład(
    source_plan: Path,
    target_rozkład: Path,
    output_docx: Path,
    audit_json: Path,
    curriculum_docx: Path,
    pdf_path: Path,
    start_week: int = 1,
) -> dict[str, list[dict[str, object]]]:
    docx_module: Any = importlib.import_module("docx")
    tooltip_by_code = curriculum_hyperlinks(curriculum_docx)
    day_titles, source_activities, parse_warnings = extract_source_plan(source_plan)
    pdf_aids = extract_pdf_aids(pdf_path, start_week=start_week)
    source_activities, aid_match_warnings = attach_activity_aids(source_activities, pdf_aids, start_week)
    document = docx_module.Document(str(target_rozkład))

    day_cells_updated: list[dict[str, object]] = []
    activities_written: list[dict[str, object]] = []
    aids_written: list[dict[str, object]] = []
    points_written: list[dict[str, object]] = []
    rows_removed: list[dict[str, object]] = []
    unmatched: list[dict[str, object]] = []

    for week_index, table in enumerate(document.element.findall(".//w:tbl", NS), start=1):
        rows = table.findall("w:tr", NS)
        area_columns = extract_area_columns(table)
        day_blocks = find_day_blocks(table)
        if not area_columns:
            unmatched.append({"week": week_index, "reason": "no_area_columns_found"})
            continue
        activity_run_properties = table_activity_run_properties(table, area_columns)
        if activity_run_properties is None:
            activity_run_properties = calibri_10_run_properties()

        for row_index, row in enumerate(rows, start=1):
            cells = row_cells(row)
            if not cells:
                continue
            left_text = norm_space(cell_text(cells[0]))
            match = TARGET_DAY_RE.match(left_text)
            if not match:
                continue

            day = int(match.group(1))
            title = day_titles.get((week_index, day))
            if title is None:
                unmatched.append(
                    {
                        "week": week_index,
                        "row": row_index,
                        "day": day,
                        "current_text": left_text,
                        "reason": "no_matching_source_day_title",
                    }
                )
                continue

            new_text = f"Dzień {day}: {title.title}"
            replace_cell_text(cells[0], new_text)
            day_cells_updated.append(
                {
                    "week": week_index,
                    "row": row_index,
                    "day": day,
                    "old_text": left_text,
                    "new_text": new_text,
                }
            )

        for day, block in sorted(day_blocks.items()):
            activities = source_activities.get((week_index, day), [])
            if not activities:
                unmatched.append({"week": week_index, "day": day, "reason": "no_source_activities_found"})
                continue

            activities_by_area: dict[int, list[Activity]] = {}
            for activity in activities:
                activities_by_area.setdefault(activity.area, []).append(activity)

            for area, column_index in sorted(area_columns.items()):
                for row_number in block.activity_rows:
                    cells = row_cells(rows[row_number])
                    if column_index < len(cells):
                        replace_cell_text(cells[column_index], "")

                points_cells = row_cells(rows[block.points_row])
                if column_index < len(points_cells):
                    replace_cell_text(points_cells[column_index], "")
                aids_cells = row_cells(rows[block.aids_row])
                if column_index < len(aids_cells):
                    replace_cell_text(aids_cells[column_index], "")

                area_activities = activities_by_area.get(area, [])
                if not area_activities:
                    continue

                if len(area_activities) > len(block.activity_rows):
                    unmatched.append(
                        {
                            "week": week_index,
                            "day": day,
                            "area": area,
                            "reason": "not_enough_activity_rows",
                            "activities": len(area_activities),
                            "available_rows": len(block.activity_rows),
                        }
                    )

                for offset, activity in enumerate(area_activities[: len(block.activity_rows)]):
                    row_number = block.activity_rows[offset]
                    cells = row_cells(rows[row_number])
                    if column_index >= len(cells):
                        unmatched.append(
                            {
                                "week": week_index,
                                "day": day,
                                "area": area,
                                "row": row_number + 1,
                                "reason": "target_activity_cell_missing",
                            }
                        )
                        continue

                    activity_text = sentence_text(activity.title)
                    replace_cell_text(cells[column_index], activity_text, activity_run_properties)
                    activities_written.append(
                        {
                            "week": week_index,
                            "day": day,
                            "area": area,
                            "row": row_number + 1,
                            "source_row": activity.source_row,
                            "title": activity_text,
                        }
                    )

                aids_cells = row_cells(rows[block.aids_row])
                aids_run_properties = None
                if aids_cells:
                    for run_properties in aids_cells[0].findall(".//w:rPr", NS):
                        aids_run_properties = run_font_properties(run_properties)
                        if aids_run_properties is not None:
                            break
                if column_index >= len(aids_cells):
                    unmatched.append(
                        {
                            "week": week_index,
                            "day": day,
                            "area": area,
                            "row": block.aids_row + 1,
                            "reason": "target_aids_cell_missing",
                        }
                    )
                else:
                    all_aids = unique_aids(
                        [aid for activity in area_activities for aid in (activity.aids or [])]
                    )
                    if all_aids:
                        aids_text = ", ".join(all_aids)
                        replace_cell_text(aids_cells[column_index], aids_text, aids_run_properties)
                        aids_written.append(
                            {
                                "week": week_index,
                                "day": day,
                                "area": area,
                                "row": block.aids_row + 1,
                                "aids": all_aids,
                            }
                        )

                all_codes = unique_sorted_codes(
                    [code for activity in area_activities for code in activity.codes]
                )
                points_cells = row_cells(rows[block.points_row])
                points_run_properties = None
                if points_cells:
                    for run_properties in points_cells[0].findall(".//w:rPr", NS):
                        points_run_properties = run_font_properties(run_properties)
                        if points_run_properties is not None:
                            break
                if column_index >= len(points_cells):
                    unmatched.append(
                        {
                            "week": week_index,
                            "day": day,
                            "area": area,
                            "row": block.points_row + 1,
                            "reason": "target_points_cell_missing",
                        }
                    )
                    continue

                replace_cell_with_code_links(
                    points_cells[column_index],
                    all_codes,
                    tooltip_by_code,
                    points_run_properties,
                )
                points_written.append(
                    {
                        "week": week_index,
                        "day": day,
                        "area": area,
                        "row": block.points_row + 1,
                        "codes": all_codes,
                    }
                )

        removed_count = remove_empty_activity_rows(table, day_blocks, area_columns)
        if removed_count:
            rows_removed.append({"week": week_index, "rows_removed": removed_count})

    if not day_cells_updated:
        die(f"No target day cells were updated in: {target_rozkład}")

    if output_docx.exists():
        output_docx.unlink()
    document.save(str(output_docx))

    audit = {
        "inputs": {
            "source_plan": str(source_plan),
            "target_rozkład": str(target_rozkład),
            "output_docx": str(output_docx),
            "curriculum_docx": str(curriculum_docx),
            "pdf": str(pdf_path),
            "start_week": start_week,
        },
        "source_day_titles": [
            {"week": value.week, "day": value.day, "title": value.title}
            for value in sorted(day_titles.values(), key=lambda item: (item.week, item.day))
        ],
        "source_activities": [
            {
                "week": activity.week,
                "day": activity.day,
                "area": activity.area,
                "codes": activity.codes,
                "title": activity.title,
                "source_row": activity.source_row,
                "aids": activity.aids or [],
            }
            for day_activities in source_activities.values()
            for activity in day_activities
        ],
        "pdf_aids": [
            {
                "week": aid_entry.week,
                "title": aid_entry.title,
                "aids": aid_entry.aids,
                "page": aid_entry.page,
            }
            for aid_entry in pdf_aids
        ],
        "application": {
            "day_cells_updated": day_cells_updated,
            "activities_written": activities_written,
            "aids_written": aids_written,
            "points_written": points_written,
            "rows_removed": rows_removed,
            "parse_warnings": parse_warnings,
            "aid_match_warnings": aid_match_warnings,
            "unmatched": unmatched,
        },
    }
    audit_json.write_text(json.dumps(audit, ensure_ascii=False, indent=2), encoding="utf-8")
    return audit["application"]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Fill a rozkład materiału DOCX from a detailed plan DOCX.")
    parser.add_argument(
        "--source-plan",
        type=Path,
        default=Path("WRZESIEŃ-Szczegółowy_plan_pracy.docx"),
        help="Detailed plan DOCX to read from.",
    )
    parser.add_argument(
        "--target-rozkład",
        dest="target_rozklad",
        type=Path,
        default=Path("WRZESIEŃ-szczegółówy_rozkład_materiału.docx"),
        help="Rozkład materiału DOCX to update.",
    )
    parser.add_argument(
        "--output-docx",
        type=Path,
        default=Path("WRZESIEŃ-szczegółówy_rozkład_materiału_updated.docx"),
        help="Updated DOCX output path.",
    )
    parser.add_argument(
        "--audit-json",
        type=Path,
        default=Path("WRZESIEŃ-szczegółówy_rozkład_materiału_audit.json"),
        help="Audit JSON output path.",
    )
    parser.add_argument(
        "--curriculum-docx",
        type=Path,
        default=Path("Podstawa_programowa_wychowania_przedszkolnego.docx"),
        help="Curriculum DOCX to read code hyperlink tooltips from.",
    )
    parser.add_argument(
        "--pdf",
        type=Path,
        default=Path("1827J1_Druzyna_4latek_cz1.pdf"),
        help="Guide PDF to read activity aids from.",
    )
    parser.add_argument(
        "--start-week",
        type=int,
        default=1,
        help="Actual PDF week number for the first table in the source/target files.",
    )
    parser.add_argument(
        "--batch",
        action="store_true",
        help="Process all configured months from October through June.",
    )
    parser.add_argument(
        "--months",
        nargs="+",
        help=(
            "Optional month keys for batch mode, e.g. pazdziernik listopad maj. "
            "If omitted with --batch, October through June are processed."
        ),
    )
    return parser.parse_args()


def print_result_summary(result: dict[str, list[dict[str, object]]]) -> None:
    print(f"Day cells updated: {len(result['day_cells_updated'])}")
    print(f"Activities written: {len(result['activities_written'])}")
    print(f"Aids rows written: {len(result['aids_written'])}")
    print(f"Points rows written: {len(result['points_written'])}")
    rows_removed_total = sum(cast(int, item["rows_removed"]) for item in result["rows_removed"])
    print(f"Empty rows removed: {rows_removed_total}")
    print(f"Parse warnings: {len(result['parse_warnings'])}")
    print(f"Aid match warnings: {len(result['aid_match_warnings'])}")
    print(f"Unmatched: {len(result['unmatched'])}")


def selected_month_configs(month_keys: list[str] | None) -> list[MonthConfig]:
    if not month_keys:
        return MONTH_CONFIGS

    selected: list[MonthConfig] = []
    for key in month_keys:
        config = month_config_by_key(key)
        if config is None:
            valid = ", ".join(config.key for config in MONTH_CONFIGS)
            die(f"Unknown month: {key}. Valid keys: {valid}")
        selected.append(config)
    return selected


def run_batch(args: argparse.Namespace) -> None:
    if not args.curriculum_docx.exists():
        die(f"Input file not found: {args.curriculum_docx}")

    for config in selected_month_configs(args.months):
        job = discover_month_job(config)
        for path in (job.source_plan, job.target_rozklad, args.curriculum_docx, config.pdf):
            if not path.exists():
                die(f"Input file not found: {path}")

        print(f"\n== {config.display_name} ==")
        result = update_rozkład(
            source_plan=job.source_plan,
            target_rozkład=job.target_rozklad,
            output_docx=job.output_docx,
            audit_json=job.audit_json,
            curriculum_docx=args.curriculum_docx,
            pdf_path=config.pdf,
            start_week=config.start_week,
        )
        print(f"Updated DOCX: {job.output_docx}")
        print(f"Audit JSON:   {job.audit_json}")
        print_result_summary(result)


def main() -> None:
    args = parse_args()
    if args.batch or args.months:
        run_batch(args)
        return

    for path in (args.source_plan, args.target_rozklad, args.curriculum_docx, args.pdf):
        if not path.exists():
            die(f"Input file not found: {path}")

    result = update_rozkład(
        source_plan=args.source_plan,
        target_rozkład=args.target_rozklad,
        output_docx=args.output_docx,
        audit_json=args.audit_json,
        curriculum_docx=args.curriculum_docx,
        pdf_path=args.pdf,
        start_week=args.start_week,
    )
    print(f"Updated DOCX: {args.output_docx}")
    print(f"Audit JSON:   {args.audit_json}")
    print_result_summary(result)


if __name__ == "__main__":
    main()
