#!/usr/bin/env python3
"""usage: check.py <dir>. Reads what load-all.sh left in <dir>: a warning or an error while loading (before the main loop)
other than the fixture's own device, a device the server did not keep, or an attribute of
the installer's device that the server's save dropped or changed, fails."""
import glob, os, re, sys
import xml.etree.ElementTree as ET
NS = '{http://www.calaos.fr}'
here = sys.argv[1]
def device(path):
    root = ET.parse(path).getroot()
    for d in root.iter(NS + 'device'):
        if d.get('id') != 'other':
            return d
bad = 0
names = sorted(os.path.basename(p)[:-4] for p in glob.glob(os.path.join(here, 'xml', '*.xml')))
if not names:
    print('no device to check in %s/xml' % here)
    sys.exit(1)
for n in names:
    log = open(os.path.join(here, 'out', n + '.log'), errors='replace').read()
    loading = log.split('All services started')[0]
    # The container has no sidecar, no firmware, no state cache and no MCP wrapper installed.
    noise = ('Device(other)', 'Process error: no such file', 'iostates.cache', 'Firmware directory', 'calaos_mcp wrapper')
    issues = [l for l in loading.splitlines() if re.match(r'\[(WRN|ERR|CRI)\]', l) and not any(x in l for x in noise)]
    if 'All services started' not in log:
        issues.append('server did not start')
    mine, saved = device(os.path.join(here, 'xml', n + '.xml')), device(os.path.join(here, 'out', n + '.saved.xml'))
    if saved is None:
        issues.append('device not saved back')
    else:
        a = [(c.tag, dict(c.attrib)) for c in [mine] + list(mine)]
        b = {(c.tag, c.get('id') or c.get('name')): dict(c.attrib) for c in [saved] + list(saved)}
        for tag, attrs in a:
            got = b.get((tag, attrs.get('id') or attrs.get('name')))
            if got is None:
                issues.append('dropped %s %s' % (tag, attrs))
                continue
            for k, v in attrs.items():
                if got.get(k) != v:
                    issues.append('%s %s: %s=%r became %r' % (tag, attrs.get('id') or attrs.get('name'), k, v, got.get(k)))
    print(('OK   ' if not issues else 'FAIL ') + n)
    for i in issues:
        print('     ' + i)
    bad += bool(issues)
print('%d loaded, %d failed' % (len(names), bad))
sys.exit(1 if bad else 0)
