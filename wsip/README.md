# WSiP DOCX Automation Scripts

This folder contains Python scripts for filling WSiP kindergarten planning
documents from guide PDFs and DOCX source files.

## Requirements

- Python 3.14.6
- Microsoft Word-compatible `.docx` files in this folder
- Python packages:
  - `PyMuPDF`
  - `lxml`
  - `python-docx`

Install dependencies:

```powershell
python -m pip install PyMuPDF lxml python-docx
```

Close generated `.docx` files in Word before rerunning a script. Word locks open
documents and can prevent the script from overwriting the output file.

## update_rozklad_materialu.py

`update_rozklad_materialu.py` fills a monthly
`szczegółówy_rozkład_materiału` DOCX from a corresponding
`Szczegółowy_plan_pracy` DOCX and guide PDF.

Default input files:

```text
WRZESIEŃ-Szczegółowy_plan_pracy.docx
WRZESIEŃ-szczegółówy_rozkład_materiału.docx
1827J1_Druzyna_4latek_cz1.pdf
Podstawa_programowa_wychowania_przedszkolnego.docx
```

Default output files:

```text
WRZESIEŃ-szczegółówy_rozkład_materiału_updated.docx
WRZESIEŃ-szczegółówy_rozkład_materiału_audit.json
```

Run with defaults:

```powershell
python update_rozklad_materialu.py
```

Run all configured months from October through June:

```powershell
python update_rozklad_materialu.py --batch
```

Run selected months:

```powershell
python update_rozklad_materialu.py --batch --months pazdziernik listopad maj
```

The script:

- fills `Dzień n: ...` labels from `Temat dnia`,
- copies activities from `Działania dzieci` into the matching numbered area
  columns,
- supports normal activity rows in the form
  `<area> <section numbers> <activity name>`,
- supports recurring activity patterns from the last page of the source plan,
- writes activity names with a final period,
- writes sorted, de-duplicated, hyperlinked section numbers into
  `Punkty podstawy programowej`,
- extracts aids from `1827J1_Druzyna_4latek_cz1.pdf` and writes them into
  `Pomoce`,
- removes empty activity rows after filling the tables,
- writes an audit JSON with extracted data, placements, and warnings.

Useful options:

```powershell
python update_rozklad_materialu.py `
  --source-plan "WRZESIEŃ-Szczegółowy_plan_pracy.docx" `
  --target-rozkład "WRZESIEŃ-szczegółówy_rozkład_materiału.docx" `
  --output-docx "WRZESIEŃ-szczegółówy_rozkład_materiału_updated.docx" `
  --audit-json "WRZESIEŃ-szczegółówy_rozkład_materiału_audit.json" `
  --pdf "1827J1_Druzyna_4latek_cz1.pdf" `
  --curriculum-docx "Podstawa_programowa_wychowania_przedszkolnego.docx" `
  --start-week 1
```

Batch mode uses these configured first-week/PDF pairs:

```text
PAŹDZIERNIK  V      1827J1_Druzyna_4latek_cz1.pdf
LISTOPAD     IX     1827J1_Druzyna_4latek_cz1.pdf
GRUDZIEŃ     XIII   1827J1_Druzyna_4latek_cz1.pdf
STYCZEŃ      XVII   1827J1_Druzyna_4latek_cz1.pdf
LUTY         XXI    1827J0_Przewodnik_4-latek_cz2_kk.pdf
MARZEC       XXV    1827J0_Przewodnik_4-latek_cz2_kk.pdf
KWIECIEŃ     XXIX   1827J0_Przewodnik_4-latek_cz2_kk.pdf
MAJ          XXXIII 1827J0_Przewodnik_4-latek_cz2_kk.pdf
CZERWIEC     XXXVII 1827J0_Przewodnik_4-latek_cz2_kk.pdf
```

After running, check the printed counters:

```text
Day cells updated: ...
Activities written: ...
Aids rows written: ...
Points rows written: ...
Empty rows removed: ...
Parse warnings: ...
Aid match warnings: ...
Unmatched: ...
```

`Parse warnings` and `Unmatched` should normally be `0`. `Aid match warnings`
can be non-zero when the guide PDF does not provide a separate aid bullet for a
specific activity.

## update_kwiecien_plan.py

`update_kwiecien_plan.py` fills selected columns in April, May, and June monthly
plan DOCX files using `1827J0_Przewodnik_4-latek_cz2_kk.pdf`.

Default run:

```powershell
python update_kwiecien_plan.py
```

The script:

- extracts `Propozycja zapisu w dzienniku` to `Podstawa programowa` mappings
  from the PDF,
- writes sorted, comma-space separated, hyperlinked curriculum codes into
  `Osiągnięcia dziecka`,
- extracts teacher aids from PDF activity descriptions,
- appends missing aids to `Pomoce` without duplicating existing values,
- processes matching April, May, and June files when `--plan-docx` is not
  provided.

Common options:

```powershell
python update_kwiecien_plan.py --plan-docx "KWIECIEŃ-Szczegółowy_plan_pracy.docx"
python update_kwiecien_plan.py --start-week XXIX
python update_kwiecien_plan.py --min-score 0.82
```
