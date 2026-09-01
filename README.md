# Calaos Installer

[![CI Build](https://github.com/calaos/calaos_installer/actions/workflows/ci_build.yml/badge.svg)](https://github.com/calaos/calaos_installer/actions/workflows/ci_build.yml)
[![License: GPL v3+](https://img.shields.io/badge/license-GPL--3.0--or--later-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

The desktop configuration tool for [Calaos](https://www.calaos.fr), a free and open source
home automation platform.

Calaos Installer is where an installation is designed: you declare the rooms of the house,
the inputs and outputs wired to each of them, and the rules that tie them together. The
resulting configuration (`io.xml` + `rules.xml`) is what `calaos_server` runs on the
controller. The application can talk to the controller directly, so a configuration can be
downloaded, edited and sent back without leaving the tool.

It ships with **Calaos Machine Creator**, a companion application that writes Calaos OS
images to USB drives and SD cards.

---

## Features

**Configuration**

- Organise the installation into rooms, each holding its inputs and outputs
- Build automation rules from conditions and actions, with a graphical editor
- Scenarios, timers, time ranges and internal variables (bool / int / string)
- Lua scripting for conditions and actions that need more than the rule editor offers
- Printable reports of the installation, by room or by input/output

**Hardware**

- Connect to a Wago PLC, upload the Calaos program, update its firmware
  (750-841, 750-842, 750-849, 750-880, 750-881, 750-889), or flash a custom one
- DALI and DMX lighting configuration, including the DALI Master 647 CSV import and
  DMX4ALL addressing
- Auto-detection wizards for MySensors, xPL, Squeezebox and Philips Hue
- Zigbee2MQTT device import

**Interfaces**

- Remote UI Editor: design touchscreen panel interfaces on a drag-and-drop grid
- Flash Remote UI firmware onto supported ESP32-P4 panels (Waveshare 86 Panel,
  Touch LCD 7" / 8" / 10.1"), see [`boards.json`](boards.json)

**Deployment**

- Download from and upload to a Calaos controller
- Write Calaos OS images to USB/SD media with Calaos Machine Creator

## Supported devices

| Family | Devices |
|---|---|
| Wago PLC | Digital in/out, analog in/out, temperature, shutters (simple and smart), DALI/DMX dimming and RGB |
| KNX | Switches, dimmers, RGB, shutters, analog and temperature, via `knxd` — also KNX/TP1 through a 750-849 |
| MQTT | Generic MQTT I/O and Zigbee2MQTT |
| MySensors | Switches, dimmers, RGB, shutters, analog, temperature, strings |
| Lighting | Philips Hue, OLA / DMX |
| Sensors | 1-Wire temperature, GPIO, ping |
| Cameras | Axis, Foscam, Gadspot, Planet, Reolink, Synology Surveillance Station, generic MJPEG |
| Audio / AV | Squeezebox, Roon, A/V receivers |
| Generic | HTTP/Web I/O, Wake-on-LAN |

Legacy support for xPL, Zibase and X10 is still present in the codebase.

## Installing

Prebuilt Windows installers, macOS application bundles and Flatpak bundles are published on
[calaos.fr](https://www.calaos.fr). Development builds for all three platforms are attached
as artifacts to every
[CI Build run](https://github.com/calaos/calaos_installer/actions/workflows/ci_build.yml).

To build it yourself, read on.

## Building from source

### Dependencies

- **Qt 6** (recommended) or Qt 5 — modules: `core gui network widgets xml printsupport
  quick qml quickwidgets quickcontrols2 serialport`, plus `core5compat` on Qt 6
- **KArchive** (KF6)
- **Qt MQTT** *(optional)* — enables Zigbee2MQTT support when present, detected
  automatically by qmake
- Linux: `libdl`; Calaos Machine Creator additionally needs `libudev` and Qt DBus

Lua 5.1.4, the FTP client and a few other third-party pieces are vendored in-tree; nothing
else to install for them.

On Fedora:

```bash
sudo dnf install qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qttools-devel \
                 qt6-qt5compat-devel qt6-qtserialport-devel kf6-karchive-devel \
                 systemd-devel
```

On Debian/Ubuntu (package names vary between releases):

```bash
sudo apt install qt6-base-dev qt6-declarative-dev qt6-tools-dev qt6-tools-dev-tools \
                 qt6-5compat-dev qt6-serialport-dev libkf6archive-dev libudev-dev
```

### Build

```bash
mkdir -p build && cd build
qmake ../calaos_installer.pro
make -j$(nproc)
```

The binary lands in `build/calaos_installer`. Translations are compiled from `lang/*.ts` to
`.qm` automatically during the build.

Calaos Machine Creator builds separately:

```bash
mkdir -p build/machine_creator && cd build/machine_creator
qmake ../../machine_creator/machine_creator.pro
make -j$(nproc)
```

### Install (Linux)

`PREFIX` is read at qmake time and defaults to `/usr/local`:

```bash
qmake PREFIX=/usr ../calaos_installer.pro
make -j$(nproc)
sudo make install
```

This installs the binary, the `.desktop` entry, the AppStream metainfo and the icon.

## Repository layout

```
Calaos/          Core domain model (Room, IOBase, Rule, Condition, Action) — shared with calaos_server
src/             Application source
  common/        Shared utilities, vendored Lua
  ftp/           FTP client used to push firmware and files to the Wago PLC
  wizards/       Setup wizards (Philips Hue)
qml/             Remote UI Editor
data/            .ui forms, Qt resources, images, IO documentation
lang/            Translations — French, German, Spanish, Norwegian, Hindi, Polish, Russian
machine_creator/ Calaos Machine Creator (separate .pro)
tests/           Sample Calaos projects used as manual test data
scripts/         Build and release helpers
flatpak/         Flatpak manifest, desktop entry, AppStream metainfo
win32/ macos/    Platform packaging
```

## Contributing

Patches are welcome — open an issue or a pull request.

[`AGENTS.md`](AGENTS.md) documents the build, the code style and the project conventions in
detail; it is written for AI coding agents but is just as useful to a human contributor.
The short version: 4-space indentation, opening brace on its own line, `m_` prefix on
members, new-style `connect()`, `tr()` on every user-visible string, and new source files
must be registered in `calaos_installer.pro`.

Two things worth knowing before you start:

- `Calaos/` is shared with `calaos_server`. Changes there affect both, so tread carefully.
- Vendored code (Lua 5.1.4, `qftp`, drivelist, BLAKE2) should be left alone.

There is no automated test suite. `tests/` holds sample XML projects to exercise the
application by hand; verify your changes by building and running it.

### Translations

Strings are extracted with `lupdate` and compiled by the build. After adding or changing
translatable strings:

```bash
lupdate calaos_installer.pro
# then translate lang/calaos_installer_<lang>.ts with Qt Linguist
```

## Related projects

- [calaos_base](https://github.com/calaos/calaos_base) — `calaos_server`, the controller daemon that runs the configuration
- [calaos_wago](https://github.com/calaos/calaos_wago) — the CodeSys program this tool uploads to the Wago PLC
- [calaos_remote_ui](https://github.com/calaos/calaos_remote_ui) — firmware for the ESP32-P4 touchscreen panels
- [calaos-os](https://github.com/calaos/calaos-os) — the OS images Calaos Machine Creator writes
- [calaos_mobile](https://github.com/calaos/calaos_mobile) / [calaos-web-app](https://github.com/calaos/calaos-web-app) — end-user clients
- [calaos.fr](https://www.calaos.fr) — documentation, downloads and community

## License

GPL-3.0-or-later. Copyright © Raoul Hecky and the Calaos contributors.
