import json
from pathlib import Path

p = Path(__file__).with_name('1_error_green.json')
js = json.loads(p.read_text(encoding='utf-8'))

# collect structs members
structs = {}
for s in js.get('DEFINITIONS', {}).get('STRUCT_DEF', {}).get('STRUCT', []):
    sid = s.get('ID')
    members = [m.get('ID') for m in s.get('MEMBER', []) if 'ID' in m]
    structs[sid] = members

# collect functions-exposed members
exposed = set()
for f in js.get('FUNCTIONS', {}).get('FUNCTION', []):
    # CONTENTS.CONTENT may list struct ids
    for c in f.get('CONTENTS', {}).get('CONTENT', []) or []:
        # if it's a struct id, add all its members
        if c in structs:
            for m in structs[c]:
                exposed.add(f"{c}.{m}")
    # LAYOUTS -> LAYOUT -> MEMBER
    for layout in f.get('LAYOUTS', {}).get('LAYOUT', []) or []:
        for m in layout.get('MEMBER', []) or []:
            exposed.add(m)

# collect save data paths
save_paths = set()
for entry in js.get('SAVE DATA', []) or []:
    # entry may be a dict with key mapping to list
    for k, v in entry.items():
        if isinstance(v, list):
            for it in v:
                if isinstance(it, dict) and 'PATH' in it:
                    save_paths.add(it['PATH'])

# intersection
affected = sorted(save_paths & exposed)

out = Path('effective_params.txt')
out.write_text('\n'.join(affected), encoding='utf-8')
print('Found', len(affected), 'SAVE DATA paths that are exposed by FUNCTIONS (written to effective_params.txt)')

# also write examples
with Path('effective_params_sample.txt').open('w', encoding='utf-8') as f:
    for a in affected[:200]:
        f.write(a + '\n')
print('Wrote sample 200 entries to effective_params_sample.txt')
