#!/usr/bin/env python3
"""check.py on hand-made server outputs (the server itself is not needed)."""
import os, subprocess, sys, tempfile, unittest

CHECK = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'check.py')
DEVICE = ('<calaos:ioconfig xmlns:calaos="http://www.calaos.fr"><calaos:home><calaos:room name="r">'
          '<calaos:device id="d" type="climate" impl="thermostat" name="D">'
          '<calaos:link name="current_temp" io="t"/></calaos:device></calaos:room></calaos:home></calaos:ioconfig>')


def run(work):
    return subprocess.run([sys.executable, '-I', CHECK, work], capture_output=True, text=True)


class CheckTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.work = self.tmp.name
        os.makedirs(os.path.join(self.work, 'xml'))
        os.makedirs(os.path.join(self.work, 'out'))

    def tearDown(self):
        self.tmp.cleanup()

    def variant(self, name, log, saved=DEVICE):
        for path, text in ((('xml', name + '.xml'), DEVICE), (('out', name + '.log'), log),
                           (('out', name + '.saved.xml'), saved)):
            with open(os.path.join(self.work, *path), 'w') as f:
                f.write(text)

    def test_nothing_to_check_fails(self):
        r = run(self.work)
        self.assertNotEqual(r.returncode, 0)
        self.assertIn('no device', r.stdout)

    def test_a_clean_load_passes(self):
        self.variant('a', '[INF] x\nAll services started\n[WRN] later\n')
        self.assertEqual(run(self.work).returncode, 0)

    def test_a_loading_error_fails(self):
        self.variant('a', '[ERR] device (Device.cpp:1) bad link\nAll services started\n')
        r = run(self.work)
        self.assertEqual(r.returncode, 1)
        self.assertIn('bad link', r.stdout)

    def test_a_dropped_attribute_fails(self):
        self.variant('a', 'All services started\n', DEVICE.replace(' name="D"', ''))
        r = run(self.work)
        self.assertEqual(r.returncode, 1)
        self.assertIn("name='D' became None", r.stdout)


if __name__ == '__main__':
    unittest.main()
