import json
import sys
from pathlib import Path

jpath = Path(__file__).with_name('1_error_green.json')
if len(sys.argv) > 1:
    target = sys.argv[1]
else:
    print('Usage: python inspect_param.py <PATH>')
    sys.exit(1)

data = json.loads(jpath.read_text(encoding='utf-8'))

# helpers
structs = {}
for s in data.get('DEFINITIONS', {}).get('STRUCT_DEF', {}).get('STRUCT', []):
    structs[s.get('ID')] = s

enums = {}
for e in data.get('DEFINITIONS', {}).get('ENUM_DEF', {}).get('ENUM', []):
    enums[e.get('ID')] = e

# target may be like 'ISP_TNR_ATTR_S.Enable' or 'DEFINITIONS.ENUM_DEF.ENUM[XYZ].ENUM_ITEM[ABC]'
parts = target.split('.')
if len(parts) >= 2 and parts[0] in structs:
    struct_name = parts[0]
    member_path = '.'.join(parts[1:])
    struct = structs.get(struct_name)
    members = struct.get('MEMBER', [])
    # try to find member with ID equal to next part (simple)
    next_key = parts[1]
    found = None
    for m in members:
        if m.get('ID') == next_key or m.get('ALIAS') == next_key:
            found = m
            break
    if found:
        print(f"Found member in struct {struct_name}:\n")
        for k, v in found.items():
            print(f"{k}: {v}")
    else:
        # try nested fields
        # search by joining remainder
        rem = '.'.join(parts[1:])
        for m in members:
            if m.get('ID') == rem or m.get('ALIAS') == rem:
                found = m
                break
        if found:
            print(f"Found member by full match {struct_name}:\n")
            for k, v in found.items():
                print(f"{k}: {v}")
        else:
            print(f"Member {next_key} not found directly in struct {struct_name}.\nMembers available: {[m.get('ID') for m in members]}")
elif target in enums:
    e = enums[target]
    print(f"Found enum {target}:\nALIAS: {e.get('ALIAS')}, ID: {e.get('ID')}, COMMENT: {e.get('COMMENT')}")
    print('ENUM_ITEM:')
    for it in e.get('ENUM_ITEM', []):
        print(' -', it)
else:
    print('Could not resolve target via simple rules. You can pass a struct.member or an enum ID.')
