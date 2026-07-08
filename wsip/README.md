# WSiP Plan Updater

This folder contains a Python script that fills selected columns in
monthly plan `.docx` files using information extracted from
`1827J0_Przewodnik_4-latek_cz2_kk.pdf`.

The script writes curriculum codes to `Osiągnięcia dziecka`. The inserted codes are:

- matched by week and activity, starting from `Tydzień XXIX`
- sorted in ascending order
- separated with `, `
- inserted as Word hyperlinks
- formatted as Calibri 11

The script also extracts teacher-aid/material bullets from the PDF activity
descriptions and appends missing values to the `Pomoce` column. Existing `Pomoce`
content is preserved, and duplicate values are skipped.

## Requirements

- Python 3.14.6
- Microsoft Word-compatible `.docx` files in this folder
- Python packages:
  - `PyMuPDF`
  - `lxml`
  - `python-docx`

## Install Dependencies

Run:

```powershell
python -m pip install PyMuPDF lxml python-docx
```

## Required Input Files

Place these files in the same folder as the script:

```text
1827J0_Przewodnik_4-latek_cz2_kk.pdf
Podstawa_programowa_wychowania_przedszkolnego.docx
KWIECIEŃ-Szczegółowy_plan_pracy.docx
MAJ*.docx
CZERWIEC*.docx
update_kwiecien_plan.py
```

## Run

```powershell
python update_kwiecien_plan.py
```

By default, this creates:

```text
<input-name>_updated.docx
<input-name>_audit.json
```

Without `--plan-docx`, the script processes matching April, May and June files:

```text
KWIECIE*.docx
MAJ*.docx
CZERWIEC*.docx
```

Generated/temporary outputs such as files containing `_updated` or `_spacing0`
are skipped automatically.

The command prints summary counters:

```text
Rows updated: ...
Unmatched: ...
Aid appends: ...
Aid errors: ...
```

`Aid errors` should be `0`. If it is not, inspect the audit JSON for details.

## Optional Arguments

```powershell
python update_kwiecien_plan.py --output-docx output.docx
```

Useful options:

- `--plan-docx file.docx` processes one or more explicit plan files.
- `--output-docx output.docx` sets the output path when processing one input file.
- `--audit-json file.json` sets the audit output path when processing one input file.
- `--start-week XXIX` changes the starting week. By default it is inferred from
  the filename: April `XXIX`, May `XXXIII`, June `XXXVII`.
- `--min-score 0.82` changes the fuzzy matching threshold.

## Notes

Close the output `.docx` in Word before rerunning the script. Word may lock open
documents and prevent the script from overwriting them.
