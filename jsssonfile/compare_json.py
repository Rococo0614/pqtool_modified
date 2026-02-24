import json
from pathlib import Path

base_path = Path(r"c:\Users\rui.jiang\Desktop\CviPQtool_20260206\jsonnnfile\ISO54721_base.json")
sdr_path = Path(r"c:\Users\rui.jiang\Desktop\CviPQtool_20260206\jsonnnfile\ov_os04a10_sdr.json")

base = json.loads(base_path.read_text(encoding="utf-8"))
sdr = json.loads(sdr_path.read_text(encoding="utf-8"))


def walk(obj, prefix=""):
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


base_map = dict(walk(base))
sdr_map = dict(walk(sdr))

base_keys = set(base_map)
sdr_keys = set(sdr_map)

only_in_base = sorted(base_keys - sdr_keys)
only_in_sdr = sorted(sdr_keys - base_keys)
common = base_keys & sdr_keys

value_diffs = []
for k in sorted(common):
    if base_map[k] != sdr_map[k]:
        value_diffs.append((k, base_map[k], sdr_map[k]))

print("ONLY_IN_BASE", len(only_in_base))
print("ONLY_IN_SDR", len(only_in_sdr))
print("VALUE_DIFFS", len(value_diffs))

preview = 50
print("\n-- only in base (first 50) --")
for k in only_in_base[:preview]:
    print(k)

print("\n-- only in sdr (first 50) --")
for k in only_in_sdr[:preview]:
    print(k)

print("\n-- value diffs (first 50) --")
for k, bv, sv in value_diffs[:preview]:
    print(k, "::", bv, "=>", sv)

out_dir = base_path.parent
(out_dir / "_diff_only_in_base.txt").write_text("\n".join(only_in_base), encoding="utf-8")
(out_dir / "_diff_only_in_sdr.txt").write_text("\n".join(only_in_sdr), encoding="utf-8")
(out_dir / "_diff_value_diffs.txt").write_text(
    "\n".join(f"{k}\t{base_map[k]}\t{sdr_map[k]}" for k, _, _ in value_diffs),
    encoding="utf-8",
)
print("\nSaved full lists to:", out_dir)
