import argparse
import json
import re
from pathlib import Path
from typing import Any, Dict, Iterable, Tuple


def walk(obj: Any, prefix: str = "") -> Iterable[Tuple[str, Any]]:
    """Flatten a JSON-like structure into (path, value) pairs.

    Paths: dict keys joined by '.', lists indexed as [i].
    """
    if isinstance(obj, dict):
        for k, v in obj.items():
            new_prefix = f"{prefix}.{k}" if prefix else k
            yield from walk(v, new_prefix)
    elif isinstance(obj, list):
        for i, v in enumerate(obj):
            new_prefix = f"{prefix}[{i}]"
            yield from walk(v, new_prefix)
    else:
        yield prefix, obj


def try_number(x: Any):
    try:
        return float(x)
    except Exception:
        return None


def generate_report(
    base_map: Dict[str, Any],
    sdr_map: Dict[str, Any],
    out_dir: Path,
    report_path: Path = None,
    top: int = 30,
):
    base_keys = set(base_map)
    sdr_keys = set(sdr_map)

    only_in_base = sorted(base_keys - sdr_keys)
    only_in_sdr = sorted(sdr_keys - base_keys)
    common = base_keys & sdr_keys

    value_diffs = []
    for k in sorted(common):
        bv = base_map[k]
        sv = sdr_map[k]
        if bv != sv:
            # compute numeric diffs when possible
            bnum = try_number(bv)
            snum = try_number(sv)
            if bnum is not None and snum is not None:
                abs_diff = abs(bnum - snum)
                rel = abs_diff / abs(bnum) if bnum != 0 else None
            else:
                abs_diff = None
                rel = None
            value_diffs.append({
                "path": k,
                "base": bv,
                "sdr": sv,
                "abs_diff": abs_diff,
                "rel_diff": rel,
            })

    # helper: prettify paths by replacing tokens like KEY[index] with KEY[<string>]
    # It will try to find a representative field inside the indexed element (ID/NAME/value/alias...)
    def pretty_path(path: str) -> str:
        fields_priority = [
            "ID",
            "Id",
            "id",
            "NAME",
            "Name",
            "name",
            "ALIAS",
            "Alias",
            "alias",
            "VALUE",
            "Value",
            "value",
            "TITLE",
            "Title",
            "title",
        ]

        def replace_match(m: re.Match) -> str:
            full = m.group(0)  # e.g. 'ENUM_ITEM[3]'
            key = m.group(1)   # e.g. 'ENUM_ITEM'
            idx = m.group(2)   # e.g. '3'
            # prefix up to and including this token
            start = m.start()
            prefix = path[: start + len(full)]

            # try to find a representative field under this element, e.g. prefix + '.ID'
            id_val = None
            for fld in fields_priority:
                candidate = prefix + f".{fld}"
                if candidate in base_map:
                    id_val = base_map[candidate]
                    break
                if candidate in sdr_map:
                    id_val = sdr_map[candidate]
                    break

            if id_val is not None:
                return f"{key}[{id_val}]"
            # fallback: keep numeric index
            return full

        return re.sub(r"([A-Za-z0-9_]+)\[(\d+)\]", replace_match, path)

    # summary
    summary_lines = []
    summary_lines.append(f"Only in base: {len(only_in_base)}")
    summary_lines.append(f"Only in sdr:  {len(only_in_sdr)}")
    summary_lines.append(f"Value diffs: {len(value_diffs)}")

    # --- New: aggregate ALIAS/ID/VALUE per parent path ---
    from collections import defaultdict

    fields_of_interest = ["ALIAS", "ID", "VALUE"]
    parent_base = defaultdict(dict)
    parent_sdr = defaultdict(dict)

    for path, val in base_map.items():
        for f in fields_of_interest:
            if path.upper().endswith("." + f):
                parent = path[: - (len(f) + 1)]
                parent_base[parent][f] = val

    for path, val in sdr_map.items():
        for f in fields_of_interest:
            if path.upper().endswith("." + f):
                parent = path[: - (len(f) + 1)]
                parent_sdr[parent][f] = val

    parents_both = set(parent_base) & set(parent_sdr)
    parents_only_base = set(parent_base) - set(parent_sdr)
    parents_only_sdr = set(parent_sdr) - set(parent_base)

    common_field_diffs = []
    for p in sorted(parents_both):
        row = {"parent": p}
        diff = False
        for f in fields_of_interest:
            bv = parent_base[p].get(f)
            sv = parent_sdr[p].get(f)
            row[f + "_base"] = bv
            row[f + "_sdr"] = sv
            if bv != sv:
                diff = True
        if diff:
            common_field_diffs.append(row)


    # sort numeric diffs primarily by relative diff (desc), then by absolute diff (desc)
    numeric_with_rel = [d for d in value_diffs if d["rel_diff"] is not None]
    numeric_without_rel = [d for d in value_diffs if d["rel_diff"] is None and d["abs_diff"] is not None]
    non_numeric = [d for d in value_diffs if d["abs_diff"] is None]

    numeric_with_rel_sorted = sorted(numeric_with_rel, key=lambda x: x["rel_diff"], reverse=True)
    numeric_without_rel_sorted = sorted(numeric_without_rel, key=lambda x: x["abs_diff"], reverse=True)

    numeric_sorted_all = numeric_with_rel_sorted + numeric_without_rel_sorted
    ordered_diffs = numeric_sorted_all + non_numeric

    md_lines = []
    md_lines.append("# Comparison Report")
    md_lines.append("")
    md_lines.append("## Summary")
    md_lines.extend([f"- {l}" for l in summary_lines])
    md_lines.append("")

    # Add focused section for ALIAS/ID/VALUE diffs
    md_lines.append("## Common ALIAS/ID/VALUE differences")
    if common_field_diffs:
        md_lines.append("| Parent | Base ALIAS | SDR ALIAS | Base ID | SDR ID | Base VALUE | SDR VALUE |")
        md_lines.append("|---|---|---|---|---|---:|---:|")
        for r in common_field_diffs:
            md_lines.append(
                "| {} | {} | {} | {} | {} | {} | {} |".format(
                    pretty_path(r["parent"]),
                    r.get("ALIAS_base", ""),
                    r.get("ALIAS_sdr", ""),
                    r.get("ID_base", ""),
                    r.get("ID_sdr", ""),
                    r.get("VALUE_base", ""),
                    r.get("VALUE_sdr", ""),
                )
            )
    else:
        md_lines.append("(no ALIAS/ID/VALUE differences found among common parents)")

    md_lines.append("")

    md_lines.append("## Only-in-base: ALIAS/ID/VALUE (sample)")
    if parents_only_base:
        for p in sorted(list(parents_only_base)[:top]):
            vals = parent_base[p]
            md_lines.append(
                f"- {pretty_path(p)} — ALIAS={vals.get('ALIAS','')}, ID={vals.get('ID','')}, VALUE={vals.get('VALUE','')}"
            )
    else:
        md_lines.append("(none)")

    md_lines.append("")

    md_lines.append("## Only-in-sdr: ALIAS/ID/VALUE (sample)")
    if parents_only_sdr:
        for p in sorted(list(parents_only_sdr)[:top]):
            vals = parent_sdr[p]
            md_lines.append(
                f"- {pretty_path(p)} — ALIAS={vals.get('ALIAS','')}, ID={vals.get('ID','')}, VALUE={vals.get('VALUE','')}"
            )
    else:
        md_lines.append("(none)")

    md_lines.append("")

    # write auxiliary files for these focused lists
    (out_dir / "_diff_common_alias_id_value.txt").write_text(
        "\n".join(
            f"{pretty_path(r['parent'])}\t{r.get('ALIAS_base','')}\t{r.get('ALIAS_sdr','')}\t{r.get('ID_base','')}\t{r.get('ID_sdr','')}\t{r.get('VALUE_base','')}\t{r.get('VALUE_sdr','')}"
            for r in common_field_diffs
        ),
        encoding="utf-8",
    )
    (out_dir / "_diff_only_in_base_alias_id_value.txt").write_text(
        "\n".join(
            f"{pretty_path(p)}\t{parent_base[p].get('ALIAS','')}\t{parent_base[p].get('ID','')}\t{parent_base[p].get('VALUE','')}"
            for p in sorted(parents_only_base)
        ),
        encoding="utf-8",
    )
    (out_dir / "_diff_only_in_sdr_alias_id_value.txt").write_text(
        "\n".join(
            f"{pretty_path(p)}\t{parent_sdr[p].get('ALIAS','')}\t{parent_sdr[p].get('ID','')}\t{parent_sdr[p].get('VALUE','')}"
            for p in sorted(parents_only_sdr)
        ),
        encoding="utf-8",
    )

    md_lines.append("## Top numerical differences")
    if numeric_sorted_all:
        md_lines.append("| Path | Base | SDR | Abs diff | Rel diff (%) |")
        md_lines.append("|---|---:|---:|---:|---:|")
        for d in numeric_sorted_all[:top]:
            rel_str = (
                f"{d['rel_diff']*100:.3f}%" if d["rel_diff"] is not None else "-"
            )
            display_path = pretty_path(d["path"])
            md_lines.append(
                f"| {display_path} | {d['base']} | {d['sdr']} | {d['abs_diff']} | {rel_str} |"
            )
    else:
        md_lines.append("(no numeric diffs found)")

    md_lines.append("")
    md_lines.append("## Other value differences (non-numeric)")
    if non_numeric:
        md_lines.append("Path — base => sdr")
        for d in non_numeric[:top]:
            md_lines.append(f"- {pretty_path(d['path'])} — {d['base']} => {d['sdr']}")
    else:
        md_lines.append("(none)")

    md_lines.append("")
    md_lines.append("## Only in base (sample)")
    for p in only_in_base[:top]:
        md_lines.append(f"- {pretty_path(p)}")

    md_lines.append("")
    md_lines.append("## Only in sdr (sample)")
    for p in only_in_sdr[:top]:
        md_lines.append(f"- {pretty_path(p)}")

    # write markdown report
    if report_path is None:
        report_path = out_dir / "comparison_report.md"
    report_path.write_text("\n".join(md_lines), encoding="utf-8")

    # also keep the plain lists like before
    (out_dir / "_diff_only_in_base.txt").write_text("\n".join(pretty_path(p) for p in only_in_base), encoding="utf-8")
    (out_dir / "_diff_only_in_sdr.txt").write_text("\n".join(pretty_path(p) for p in only_in_sdr), encoding="utf-8")
    (out_dir / "_diff_value_diffs.txt").write_text(
        "\n".join(
            f"{pretty_path(d['path'])}\t{d['base']}\t{d['sdr']}\t{d.get('abs_diff')}\t{d.get('rel_diff')}"
            for d in ordered_diffs
        ),
        encoding="utf-8",
    )

    return report_path


def extract_save_data_map(json_obj: dict) -> Dict[str, Any]:
    """Extract a flat map of PATH -> VALUE from the `SAVE DATA` section.

    The `SAVE DATA` section is a list of groups where each group is a
    dict mapping a group-name to a list of {"PATH":..., "VALUE":...} entries.
    """
    out = {}
    for group in json_obj.get("SAVE DATA", []):
        if not isinstance(group, dict):
            continue
        for _, entries in group.items():
            if not isinstance(entries, list):
                continue
            for e in entries:
                if not isinstance(e, dict):
                    continue
                p = e.get("PATH")
                if p is None:
                    continue
                out[p] = e.get("VALUE")
    return out


def find_default_files(cwd: Path) -> Tuple[Path, Path]:
    # try to find reasonable defaults in current directory
    candidates = list(cwd.glob("*.json"))
    base = None
    sdr = None
    for p in candidates:
        name = p.name.lower()
        if "base" in name and base is None:
            base = p
        if ("sdr" in name or "ov_" in name) and sdr is None:
            sdr = p
    return base, sdr


def main():
    p = argparse.ArgumentParser(description="Compare two JSON files and produce a human-readable report")
    p.add_argument("base", nargs="?", help="base JSON file path")
    p.add_argument("sdr", nargs="?", help="sdr JSON file path")
    p.add_argument("-o", "--out", help="output report path (markdown)")
    p.add_argument("--only-save-data", action="store_true", help="compare only the SAVE DATA sections (use PATH/ VALUE entries)")
    p.add_argument("--top", type=int, default=30, help="how many top diffs/samples to show")
    args = p.parse_args()

    cwd = Path(__file__).parent
    base_path = Path(args.base) if args.base else None
    sdr_path = Path(args.sdr) if args.sdr else None
    if base_path is None or sdr_path is None:
        dbase, dsdr = find_default_files(cwd)
        base_path = base_path or dbase
        sdr_path = sdr_path or dsdr

    if base_path is None or sdr_path is None:
        p.print_help()
        print("\nError: could not determine base/sdr files. Provide the paths or make sure files exist in the script directory.")
        return

    base = json.loads(base_path.read_text(encoding="utf-8"))
    sdr = json.loads(sdr_path.read_text(encoding="utf-8"))

    if getattr(args, "only_save_data", False) or getattr(args, "only-save-data", False):
        base_map = extract_save_data_map(base)
        sdr_map = extract_save_data_map(sdr)
    else:
        base_map = dict(walk(base))
        sdr_map = dict(walk(sdr))

    out_dir = base_path.parent
    report_path = Path(args.out) if args.out else None
    result = generate_report(base_map, sdr_map, out_dir, report_path=report_path, top=args.top)

    print("Saved full lists to:", out_dir)
    print("Wrote human-readable report to:", result)


if __name__ == "__main__":
    main()
