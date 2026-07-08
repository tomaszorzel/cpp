#!/usr/bin/env python3
"""
Fill "Osiagniecia dziecka" in the April plan from the guide PDF.

Default inputs:
  - 1827J0_Przewodnik_4-latek_cz2_kk.pdf
  - Podstawa_programowa_wychowania_przedszkolnego.docx
  - KWIECIEŃ-Szczegółowy_plan_pracy.docx

The script:
  1. extracts "Propozycja zapisu w dzienniku" -> "Podstawa programowa"
     mappings from the PDF, starting at "Tydzien XXIX";
  2. reads tooltip text for matching curriculum codes from the curriculum DOCX;
  3. writes those linked codes into the target plan DOCX in the
     "Osiagniecia dziecka" column for matching week/action rows;
  4. writes an audit JSON showing what was extracted and where it was applied.

Dependency:
  python -m pip install PyMuPDF
"""

from __future__ import annotations

import argparse
import copy
import difflib
import importlib
import json
import re
import sys
import zipfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Iterable, NoReturn, cast

try:
    from lxml import etree as ET  # type: ignore
except ImportError:
    from xml.etree import ElementTree as ET  # type: ignore


W = "http://schemas.openxmlformats.org/wordprocessingml/2006/main"
R = "http://schemas.openxmlformats.org/officeDocument/2006/relationships"
XML = "http://www.w3.org/XML/1998/namespace"
NS = {"w": W, "r": R}
XmlElement = Any
XmlElementTree = Any

ET.register_namespace("w", W)
ET.register_namespace("r", R)


CODE_RE = re.compile(r"\b\d+\.\d+[a-z]?\b", re.IGNORECASE)
WEEK_RE = re.compile(r"Tydzie[ńn]\s+([IVXLCDM]+)", re.IGNORECASE)
POINT_RE = re.compile(r"^\s*(\d+)\s*[\.)]")


@dataclass
class PdfEntry:
    week: str
    point_no: str | None
    proposal: str
    codes: list[str]
    page: int
    aids: list[str] = field(default_factory=list)
    matched_target: str | None = None
    score: float | None = None


@dataclass
class AidEntry:
    week: str
    title: str
    aids: list[str]


@dataclass
class WeekEntries:
    entries: list[PdfEntry] = field(default_factory=list)

    def best_match(self, action_text: str, min_score: float) -> PdfEntry | None:
        action_norm = normalize_for_match(action_text)
        if not action_norm:
            return None

        best: tuple[float, PdfEntry] | None = None
        for entry in self.entries:
            proposal_norm = normalize_for_match(entry.proposal)
            if not proposal_norm:
                continue
            score = match_score(action_norm, proposal_norm)
            if best is None or score > best[0]:
                best = (score, entry)

        if best and best[0] >= min_score:
            matched = copy.copy(best[1])
            matched.score = best[0]
            matched.matched_target = action_text
            return matched
        return None


def die(message: str, code: int = 2) -> NoReturn:
    print(f"ERROR: {message}", file=sys.stderr)
    raise SystemExit(code)


def roman_to_int(value: str) -> int:
    numerals = {"I": 1, "V": 5, "X": 10, "L": 50, "C": 100, "D": 500, "M": 1000}
    total = 0
    prev = 0
    for char in reversed(value.upper()):
        current = numerals[char]
        if current < prev:
            total -= current
        else:
            total += current
            prev = current
    return total


def int_to_roman(value: int) -> str:
    parts: list[str] = []
    for number, numeral in (
        (1000, "M"),
        (900, "CM"),
        (500, "D"),
        (400, "CD"),
        (100, "C"),
        (90, "XC"),
        (50, "L"),
        (40, "XL"),
        (10, "X"),
        (9, "IX"),
        (5, "V"),
        (4, "IV"),
        (1, "I"),
    ):
        while value >= number:
            parts.append(numeral)
            value -= number
    return "".join(parts)


def infer_start_week(plan_path: Path) -> str:
    name = plan_path.name.upper()
    if name.startswith("MAJ"):
        return "XXXIII"
    if name.startswith("CZERWIEC"):
        return "XXXVII"
    return "XXIX"


def default_output_path(plan_path: Path) -> Path:
    return plan_path.with_name(f"{plan_path.stem}_updated{plan_path.suffix}")


def default_audit_path(plan_path: Path) -> Path:
    return plan_path.with_name(f"{plan_path.stem}_audit.json")


def discover_plan_files() -> list[Path]:
    files: list[Path] = []
    for pattern in ("KWIECIE*.docx", "MAJ*.docx", "CZERWIEC*.docx"):
        for path in sorted(Path(".").glob(pattern)):
            name = path.name
            if name.startswith("~$"):
                continue
            if any(marker in path.stem for marker in ("_updated", "_spacing0", "_audit", "_test")):
                continue
            if path not in files:
                files.append(path)
    return files


def norm_space(text: str | None) -> str:
    if not text:
        return ""
    text = text.replace("\xa0", " ").replace("\u00ad", "").replace("\uf03d", " ")
    return re.sub(r"\s+", " ", text).strip()


def clean_xml_text(text: str) -> str:
    return "".join(
        char for char in text if char in "\t\n\r" or ord(char) >= 32
    )


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


def extract_codes(text: str) -> list[str]:
    seen: set[str] = set()
    codes: list[str] = []
    for code in CODE_RE.findall(text or ""):
        code = code.lower()
        if code not in seen:
            seen.add(code)
            codes.append(code)
    return codes


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


def quoted_titles(text: str) -> list[str]:
    return [normalize_for_match(title) for title in re.findall(r"„([^”]+)”", text)]


def title_key(title: str) -> str:
    return item_key(normalize_for_match(title))


def title_matches_proposal(proposal: str, title: str) -> bool:
    proposal_quotes = quoted_titles(proposal)
    title_quotes = quoted_titles(title)
    if proposal_quotes and title_quotes:
        return any(
            title_key(proposal_quote) == title_key(title_quote)
            or title_key(proposal_quote) in title_key(title_quote)
            or title_key(title_quote) in title_key(proposal_quote)
            for proposal_quote in proposal_quotes
            for title_quote in title_quotes
        )

    proposal_norm = normalize_for_match(proposal)
    title_norm = normalize_for_match(title)
    if not title_norm:
        return False
    return match_score(proposal_norm, title_norm) >= 0.86


def code_sort_key(code: str) -> tuple[int, int, str]:
    match = re.fullmatch(r"(\d+)\.(\d+)([a-z]?)", code.lower())
    if not match:
        return (10_000, 10_000, code.lower())
    return (int(match.group(1)), int(match.group(2)), match.group(3))


def sorted_codes(codes: Iterable[str]) -> list[str]:
    return sorted(codes, key=code_sort_key)


def is_codes_only(text: str) -> bool:
    text = norm_space(text)
    if not text:
        return False
    return bool(re.fullmatch(r"(?:\d+\.\d+[a-z]?\s*,?\s*)+", text, flags=re.IGNORECASE))


def starts_numbered_item(text: str) -> bool:
    return bool(re.match(r"^\s*\d+\s*[\.)]?\s+", text or ""))


def strip_item_number(text: str) -> tuple[str | None, str]:
    match = re.match(r"^\s*(\d+)\s*[\.)]?\s*(.*)$", text or "")
    if not match:
        return None, norm_space(text)
    return match.group(1), norm_space(match.group(2))


def is_summary_noise(text: str) -> bool:
    normalized = normalize_for_match(text)
    if not normalized:
        return True
    noise = {
        "podstawa",
        "programowa",
        "i czesc dnia",
        "ii czesc dnia",
        "iii czesc dnia",
        "i część dnia",
        "ii część dnia",
        "iii część dnia",
    }
    return normalized in noise


def parse_pdf_summary_text(page_text: str, week: str, page_number: int) -> list[PdfEntry]:
    entries: list[PdfEntry] = []
    in_summary = False
    current_point: str | None = None
    current_lines: list[str] = []

    def flush(codes_line: str) -> None:
        nonlocal current_point, current_lines
        proposal = norm_space(" ".join(current_lines))
        codes = extract_codes(codes_line)
        if proposal and codes:
            entries.append(
                PdfEntry(
                    week=week,
                    point_no=current_point,
                    proposal=proposal,
                    codes=codes,
                    page=page_number,
                )
            )
        current_point = None
        current_lines = []

    for raw_line in page_text.splitlines():
        line = norm_space(raw_line)
        normalized = normalize_for_match(line)
        if "propozycja zapisu w dzienniku" in normalized:
            in_summary = True
            current_point = None
            current_lines = []
            continue
        if not in_summary:
            continue
        if normalized.startswith("srodki dydaktyczne") or normalized.startswith("środki dydaktyczne"):
            in_summary = False
            current_point = None
            current_lines = []
            continue
        if normalized.startswith("wiodaca aktywnosc") or normalized.startswith("wiodąca aktywność"):
            in_summary = False
            current_point = None
            current_lines = []
            continue
        if is_summary_noise(line):
            continue
        if is_codes_only(line):
            flush(line)
            continue
        if starts_numbered_item(line):
            current_point, first_line = strip_item_number(line)
            current_lines = [first_line] if first_line else []
            continue
        if current_lines:
            current_lines.append(line)

    return entries


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


def parse_aid_block(block: str, week: str) -> AidEntry | None:
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
    return AidEntry(week=week, title=title, aids=aids)


def parse_pdf_aids(week_text: str, week: str) -> list[AidEntry]:
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
        entry = parse_aid_block(" ".join(current), week)
        if entry is not None:
            aids.append(entry)

    for raw_line in week_text.splitlines():
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


def attach_aids(entries_by_week: dict[str, WeekEntries], text_by_week: dict[str, str]) -> None:
    for week, week_entries in entries_by_week.items():
        aid_entries = parse_pdf_aids(text_by_week.get(week, ""), week)
        for entry in week_entries.entries:
            seen: set[str] = set()
            for aid_entry in aid_entries:
                if not title_matches_proposal(entry.proposal, aid_entry.title):
                    continue
                for aid in aid_entry.aids:
                    key = aid.casefold()
                    if key not in seen:
                        seen.add(key)
                        entry.aids.append(aid)


def table_cell_text(cell: str | None) -> str:
    return norm_space(cell)


def find_column(header_cells: list[str], *needles: str) -> int | None:
    normalized = [normalize_for_match(cell) for cell in header_cells]
    for i, cell in enumerate(normalized):
        if all(needle in cell for needle in needles):
            return i
    return None


def week_on_page(page_text: str) -> str | None:
    matches = WEEK_RE.findall(page_text or "")
    return matches[-1].upper() if matches else None


def extract_pdf_entries(pdf_path: Path, start_week: str) -> dict[str, WeekEntries]:
    try:
        fitz: Any = importlib.import_module("fitz")
    except ImportError:
        die("PyMuPDF is required. Install it with: python -m pip install PyMuPDF")

    start_week_no = roman_to_int(start_week)
    by_week: dict[str, WeekEntries] = {}
    text_by_week: dict[str, str] = {}
    current_week: str | None = None

    with fitz.open(str(pdf_path)) as doc:
        for page_offset in range(doc.page_count):
            page_index = page_offset + 1
            page = doc.load_page(page_offset)
            page_week = week_on_page(page.get_text("text"))
            if page_week:
                current_week = page_week
            if not current_week or roman_to_int(current_week) < start_week_no:
                continue

            page_text = page.get_text("text")
            text_by_week[current_week] = text_by_week.get(current_week, "") + "\n" + page_text

            text_entries = parse_pdf_summary_text(page_text, current_week, page_index)
            for entry in text_entries:
                by_week.setdefault(current_week, WeekEntries()).entries.append(entry)

            if text_entries:
                continue

            try:
                tables = page.find_tables()
            except Exception:
                tables = []

            for table in tables:
                rows = table.extract()
                if not rows:
                    continue

                proposal_idx: int | None = None
                basis_idx: int | None = None
                data_start = 0

                for row_idx, raw_row in enumerate(rows[:5]):
                    row = [table_cell_text(cell) for cell in raw_row]
                    proposal_idx = find_column(row, "propozycja", "zapisu")
                    basis_idx = find_column(row, "podstawa", "programowa")
                    if proposal_idx is not None and basis_idx is not None:
                        data_start = row_idx + 1
                        break

                if proposal_idx is None or basis_idx is None:
                    continue

                for raw_row in rows[data_start:]:
                    row = [table_cell_text(cell) for cell in raw_row]
                    if max(proposal_idx, basis_idx) >= len(row):
                        continue
                    proposal = row[proposal_idx]
                    basis = row[basis_idx]
                    if not proposal or not basis:
                        continue
                    if "propozycja zapisu" in normalize_for_match(proposal):
                        continue

                    codes = extract_codes(basis)
                    if not codes:
                        continue
                    point_match = POINT_RE.match(proposal)
                    entry = PdfEntry(
                        week=current_week,
                        point_no=point_match.group(1) if point_match else None,
                        proposal=proposal,
                        codes=codes,
                        page=page_index,
                    )
                    by_week.setdefault(current_week, WeekEntries()).entries.append(entry)

    if not by_week:
        die(
            "No PDF mappings were extracted. Check that the PDF has selectable tables "
            'with "Propozycja zapisu w dzienniku" and "Podstawa programowa" columns.'
        )
    attach_aids(by_week, text_by_week)
    return by_week


def qname(ns: str, tag: str) -> str:
    return f"{{{ns}}}{tag}"


def iter_text(element: XmlElement) -> str:
    return "".join(node.text or "" for node in element.findall(".//w:t", NS))


def load_docx_xml(docx_path: Path) -> tuple[XmlElementTree, XmlElement]:
    with zipfile.ZipFile(docx_path) as zf:
        data = zf.read("word/document.xml")
    root = ET.fromstring(data)
    return ET.ElementTree(root), root


def curriculum_hyperlinks(docx_path: Path) -> dict[str, str]:
    _, root = load_docx_xml(docx_path)
    links: dict[str, str] = {}
    for hyperlink in root.findall(".//w:hyperlink", NS):
        text = norm_space(iter_text(hyperlink))
        if CODE_RE.fullmatch(text or ""):
            tooltip = hyperlink.get(qname(W, "tooltip"))
            if tooltip is None:
                tooltip = ""
            links[text.lower()] = tooltip
    if not links:
        die(f"No curriculum code hyperlinks found in {docx_path}")
    return links


def paragraph_text(paragraph: XmlElement) -> str:
    return iter_text(paragraph)


def cell_text(cell: XmlElement) -> str:
    return norm_space(iter_text(cell))


def row_cells(row: XmlElement) -> list[XmlElement]:
    return list(row.findall("w:tc", NS))


def get_week_from_text(text: str) -> str | None:
    match = WEEK_RE.search(text or "")
    return match.group(1).upper() if match else None


def ensure_run_format(run: XmlElement, style_id: str | None = None) -> None:
    rpr = run.find("w:rPr", NS)
    if rpr is None:
        rpr = ET.Element(qname(W, "rPr"))
        run.insert(0, rpr)

    for tag in ("rStyle", "rFonts", "sz", "szCs"):
        for existing in list(rpr.findall(f"w:{tag}", NS)):
            rpr.remove(existing)

    if style_id:
        style = ET.Element(qname(W, "rStyle"))
        style.set(qname(W, "val"), style_id)
        rpr.append(style)

    fonts = ET.Element(qname(W, "rFonts"))
    fonts.set(qname(W, "ascii"), "Calibri")
    fonts.set(qname(W, "hAnsi"), "Calibri")
    fonts.set(qname(W, "eastAsia"), "Calibri")
    fonts.set(qname(W, "cs"), "Calibri")
    rpr.append(fonts)

    size = ET.Element(qname(W, "sz"))
    size.set(qname(W, "val"), "22")
    rpr.append(size)

    complex_size = ET.Element(qname(W, "szCs"))
    complex_size.set(qname(W, "val"), "22")
    rpr.append(complex_size)


def make_text_run(text: str, style_id: str | None = None) -> XmlElement:
    run = ET.Element(qname(W, "r"))
    ensure_run_format(run, style_id)
    t = ET.SubElement(run, qname(W, "t"))
    if text.startswith(" ") or text.endswith(" "):
        t.set(qname(XML, "space"), "preserve")
    t.text = text
    return run


def make_hyperlink(code: str, tooltip_by_code: dict[str, str]) -> XmlElement:
    hyperlink = ET.Element(qname(W, "hyperlink"))
    hyperlink.set(qname(W, "anchor"), "_top")
    tooltip = tooltip_by_code.get(code.lower())
    if tooltip:
        hyperlink.set(qname(W, "tooltip"), tooltip)
    hyperlink.set(qname(W, "history"), "1")
    hyperlink.append(make_text_run(code, "Hipercze"))
    return hyperlink


def ensure_paragraph_format(paragraph: XmlElement) -> None:
    ppr = paragraph.find("w:pPr", NS)
    if ppr is None:
        ppr = ET.Element(qname(W, "pPr"))
        paragraph.insert(0, ppr)

    for existing in list(ppr.findall("w:spacing", NS)):
        ppr.remove(existing)

    spacing = ET.Element(qname(W, "spacing"))
    spacing.set(qname(W, "after"), "0")
    ppr.append(spacing)


def make_codes_paragraph(codes: Iterable[str], tooltip_by_code: dict[str, str]) -> XmlElement:
    paragraph = ET.Element(qname(W, "p"))
    ensure_paragraph_format(paragraph)
    first = True
    for code in sorted_codes(codes):
        if not first:
            paragraph.append(make_text_run(", "))
        first = False
        paragraph.append(make_hyperlink(code, tooltip_by_code))
    return paragraph


def make_plain_paragraph(text: str) -> XmlElement:
    paragraph = ET.Element(qname(W, "p"))
    ensure_paragraph_format(paragraph)
    paragraph.append(make_text_run(text))
    return paragraph


def replace_cell_with_codes(cell: XmlElement, codes: list[str], tooltip_by_code: dict[str, str]) -> None:
    for child in list(cell):
        if child.tag != qname(W, "tcPr"):
            cell.remove(child)
    cell.append(make_codes_paragraph(codes, tooltip_by_code))


def append_cell_items(cell: XmlElement, new_items: Iterable[str]) -> list[str]:
    existing_text = cell_text(cell)
    existing_items = split_aids(existing_text)
    seen = {item_key(item) for item in existing_items}
    appended: list[str] = []

    for item in new_items:
        item = norm_space(clean_xml_text(item)).strip(" .")
        if not item:
            continue
        key = item_key(item)
        if key in seen:
            continue
        seen.add(key)
        existing_items.append(item)
        appended.append(item)

    if not appended:
        return []

    for child in list(cell):
        if child.tag != qname(W, "tcPr"):
            cell.remove(child)
    cell.append(make_plain_paragraph(", ".join(existing_items)))
    return appended


def target_columns(table: XmlElement) -> tuple[int, int, int] | None:
    rows = table.findall("w:tr", NS)
    if not rows:
        return None
    header = [cell_text(cell) for cell in row_cells(rows[0])]
    action_idx = find_column(header, "dzialania", "dzieci")
    if action_idx is None:
        action_idx = find_column(header, "działania", "dzieci")
    aids_idx = find_column(header, "pomoce")
    achievement_idx = None
    for idx, header_text in enumerate(header):
        normalized = normalize_for_match(header_text)
        if "przewidywane" in normalized:
            continue
        if (
            ("osiagniecia" in normalized or "osiągnięcia" in normalized)
            and "dziecka" in normalized
        ):
            achievement_idx = idx
            break
    if action_idx is None or aids_idx is None or achievement_idx is None:
        return None
    return action_idx, aids_idx, achievement_idx


def update_plan_docx(
    plan_path: Path,
    output_path: Path,
    hyperlink_by_code: dict[str, str],
    entries_by_week: dict[str, WeekEntries],
    start_week: str,
    min_score: float,
) -> dict[str, list[dict[str, object]]]:
    try:
        docx_module: Any = importlib.import_module("docx")
    except ImportError:
        die("python-docx is required. Install it with: python -m pip install python-docx")

    document = docx_module.Document(str(plan_path))
    root = document.element
    body = root.find("w:body", NS)
    if body is None:
        die(f"No document body found in {plan_path}")
    body = cast(XmlElement, body)

    start_week_no = roman_to_int(start_week)
    current_week: str | None = None
    table_index = 0
    updated: list[dict[str, object]] = []
    unmatched: list[dict[str, object]] = []
    aid_appends: list[dict[str, object]] = []
    aid_errors: list[dict[str, object]] = []

    for child in list(body):
        if child.tag == qname(W, "p"):
            week = get_week_from_text(paragraph_text(child))
            if week:
                current_week = week
            continue

        if child.tag != qname(W, "tbl"):
            continue

        columns = target_columns(child)
        if columns is None:
            continue

        table_week = current_week
        if table_week is None or roman_to_int(table_week) < start_week_no:
            table_week = int_to_roman(start_week_no + table_index)
        table_index += 1

        if roman_to_int(table_week) < start_week_no:
            continue

        action_idx, aids_idx, achievement_idx = columns
        week_entries = entries_by_week.get(table_week)
        if not week_entries:
            unmatched.append({"week": table_week, "reason": "no_pdf_entries_for_week"})
            continue

        data_row_index = 0
        for row_number, row in enumerate(child.findall("w:tr", NS), start=1):
            cells = row_cells(row)
            if max(action_idx, aids_idx, achievement_idx) >= len(cells):
                continue
            action = cell_text(cells[action_idx])
            normalized_action = normalize_for_match(action)
            if (
                not action
                or "dzialania dzieci" in normalized_action
                or "działania dzieci" in normalized_action
            ):
                continue

            match = week_entries.best_match(action, min_score)
            if match is None and data_row_index < len(week_entries.entries):
                match = copy.copy(week_entries.entries[data_row_index])
                match.score = 1.0
                match.matched_target = action
            data_row_index += 1

            if match is None:
                unmatched.append(
                    {
                        "week": table_week,
                        "row": row_number,
                        "action": action,
                        "reason": "no_matching_pdf_proposal",
                    }
                )
                continue

            replace_cell_with_codes(cells[achievement_idx], match.codes, hyperlink_by_code)
            if match.aids:
                try:
                    appended_aids = append_cell_items(cells[aids_idx], match.aids)
                except Exception as exc:
                    aid_errors.append(
                        {
                            "week": table_week,
                            "row": row_number,
                            "action": action,
                            "aids": match.aids,
                            "error": str(exc),
                        }
                    )
                else:
                    if appended_aids:
                        aid_appends.append(
                            {
                                "week": table_week,
                                "row": row_number,
                                "action": action,
                                "aids": appended_aids,
                            }
                        )
            updated.append(
                {
                    "week": table_week,
                    "row": row_number,
                    "action": action,
                    "pdf_proposal": match.proposal,
                    "pdf_page": match.page,
                    "codes": match.codes,
                    "aids": match.aids,
                    "score": round(match.score or 0.0, 3),
                }
            )

    if not updated:
        die("No target rows were updated. Lower --min-score or inspect the audit output.")

    if output_path.exists():
        output_path.unlink()
    document.save(str(output_path))

    return {"updated": updated, "unmatched": unmatched, "aid_appends": aid_appends, "aid_errors": aid_errors}


def audit_pdf_entries(entries_by_week: dict[str, WeekEntries]) -> dict[str, list[dict[str, object]]]:
    return {
        week: [
            {
                "point_no": entry.point_no,
                "proposal": entry.proposal,
                "codes": entry.codes,
                "aids": entry.aids,
                "pdf_page": entry.page,
            }
            for entry in entries.entries
        ]
        for week, entries in sorted(entries_by_week.items(), key=lambda item: roman_to_int(item[0]))
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Copy linked curriculum codes and aids into monthly detailed work plans."
    )
    parser.add_argument("--pdf", type=Path, default=Path("1827J0_Przewodnik_4-latek_cz2_kk.pdf"))
    parser.add_argument(
        "--curriculum-docx",
        type=Path,
        default=Path("Podstawa_programowa_wychowania_przedszkolnego.docx"),
    )
    parser.add_argument(
        "--plan-docx",
        type=Path,
        nargs="*",
        default=None,
        help="Plan DOCX file(s). Defaults to KWIECIE*.docx, MAJ*.docx and CZERWIEC*.docx.",
    )
    parser.add_argument(
        "--output-docx",
        type=Path,
        default=None,
        help="Output DOCX path. Only valid when processing one --plan-docx file.",
    )
    parser.add_argument(
        "--audit-json",
        type=Path,
        default=None,
        help="Audit JSON path. Only valid when processing one --plan-docx file.",
    )
    parser.add_argument(
        "--start-week",
        default=None,
        help="Roman week number. Defaults by filename: April XXIX, May XXXIII, June XXXVII.",
    )
    parser.add_argument(
        "--min-score",
        type=float,
        default=0.82,
        help="Minimum fuzzy match score for PDF proposal -> plan action rows.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    for path in (args.pdf, args.curriculum_docx):
        if not path.exists():
            die(f"Input file not found: {path}")

    plan_paths = args.plan_docx if args.plan_docx else discover_plan_files()
    if not plan_paths:
        die("No plan DOCX files found. Pass --plan-docx explicitly or add KWIECIE*/MAJ*/CZERWIEC*.docx files.")
    if len(plan_paths) > 1 and args.output_docx is not None:
        die("--output-docx can only be used with a single --plan-docx file.")
    if len(plan_paths) > 1 and args.audit_json is not None:
        die("--audit-json can only be used with a single --plan-docx file.")
    for path in plan_paths:
        if not path.exists():
            die(f"Input file not found: {path}")

    extraction_start_week = args.start_week or min(
        (infer_start_week(path) for path in plan_paths),
        key=roman_to_int,
    )
    extraction_start_week = (
        extraction_start_week.upper().replace("TYDZIEŃ", "").replace("TYDZIEN", "").strip()
    )
    entries_by_week = extract_pdf_entries(args.pdf, extraction_start_week)
    hyperlink_by_code = curriculum_hyperlinks(args.curriculum_docx)

    for plan_path in plan_paths:
        plan_start_week = args.start_week or infer_start_week(plan_path)
        plan_start_week = (
            plan_start_week.upper().replace("TYDZIEŃ", "").replace("TYDZIEN", "").strip()
        )
        output_docx = args.output_docx or default_output_path(plan_path)
        audit_json = args.audit_json or default_audit_path(plan_path)

        result = update_plan_docx(
            plan_path=plan_path,
            output_path=output_docx,
            hyperlink_by_code=hyperlink_by_code,
            entries_by_week=entries_by_week,
            start_week=plan_start_week,
            min_score=args.min_score,
        )

        audit = {
            "inputs": {
                "pdf": str(args.pdf),
                "curriculum_docx": str(args.curriculum_docx),
                "plan_docx": str(plan_path),
                "start_week": plan_start_week,
                "min_score": args.min_score,
            },
            "extracted_pdf_entries": audit_pdf_entries(entries_by_week),
            "application": result,
        }
        audit_json.write_text(json.dumps(audit, ensure_ascii=False, indent=2), encoding="utf-8")

        print(f"Plan DOCX:    {plan_path}")
        print(f"Updated DOCX: {output_docx}")
        print(f"Audit JSON:   {audit_json}")
        print(f"Rows updated: {len(result['updated'])}")
        print(f"Unmatched:    {len(result['unmatched'])}")
        print(f"Aid appends:  {len(result['aid_appends'])}")
        print(f"Aid errors:   {len(result['aid_errors'])}")
        print()


if __name__ == "__main__":
    main()
