# AGENTS.md — Calaos Installer

## Project Overview

Calaos Installer is a Qt-based (Qt 5/6) C++ desktop GUI application for creating and
editing home automation configurations for the Calaos system. It includes a secondary
sub-application (machine_creator/) for writing Calaos OS images to USB/SD media.

- **Language:** C++ (C++11), QML, embedded C (Lua 5.1.4 vendored)
- **GUI Framework:** Qt Widgets + Qt Quick/QML
- **Build System:** qmake (`.pro` files)
- **License:** GPLv3
- **Platforms:** Linux, macOS, Windows (cross-compiled via Docker)

## Build Commands

```bash
# Full build (out-of-source in build/)
mkdir -p build && cd build
qmake ../calaos_installer.pro
make -j$(nproc)

# Machine Creator (secondary app)
mkdir -p build/machine_creator && cd build/machine_creator
qmake ../../machine_creator/machine_creator.pro
make -j$(nproc)

# Clean build
cd build && make clean
# or remove build/ entirely: rm -rf build/

# Intermediate files (objects, moc, uic, rcc) go to build-qt<Qt version>/ inside the
# build directory (.qmake.conf): a Qt update rebuilds everything instead of linking
# objects compiled against the previous Qt headers. qmake refuses a build directory
# that still holds generated files at its top (layout before this rule): empty it.

# Rebuild translations (.ts -> .qm)
# Handled automatically by qmake via lrelease during build
```

### Dependencies

- Qt 5 or Qt 6 (with core5compat for Qt 6)
- Qt modules: core gui network widgets xml printsupport quick qml quickwidgets quickcontrols2
- Linux: libdl
- Machine Creator additionally requires: KArchive (KF5/KF6), libudev (Linux)

## Testing

Unit tests use Qt Test and live in `tests/tests.pro` (standalone qmake project, not included
by `calaos_installer.pro`). Build out of the repository: qmake artefacts next to the sources
are not all gitignored.

```bash
mkdir -p /tmp/installer_tests_build && cd /tmp/installer_tests_build
qmake6 <repo>/tests/tests.pro
make -j$(nproc)
QT_QPA_PLATFORM=offscreen make check          # offscreen: no X server needed

# one suite / one case, verbose
QT_QPA_PLATFORM=offscreen ./tst_projectmanager/tst_projectmanager -v2 ioRoundTrip
```

- `tests/tst_projectmanager/` characterises the io.xml / rules.xml reader and writer
  (`src/projectmanager.cpp`) against the real model. Sample projects under
  `tests/<project>/` are discovered automatically: dropping a new `io.xml` / `rules.xml`
  in a new directory adds it to the round-trip tests. The remaining cases pin today's
  behaviour, including known defects (defaults injected at load, rule entries dropped
  without a word, ...). When a change
  fixes one of them, that test is expected to fail: update it in the same commit.
- `tests/tst_devices/` covers `<calaos:device>` (`Calaos/Device.*`): round trip of the
  spec example, `format="2"`, read-only loading of a newer format, cascade delete.
  Unknown elements are kept at every level (comments included, whitespace-only text
  aside), never dropped with what follows.
- `tests/tst_iomove/` covers `ListeRoom::moveIO` (every IO kind moved, saved, reloaded) and
  `moveIOInHome` (tree items re-parented, equal to a freshly populated tree).
- `tests/tst_deleteimpact/` covers `deletionImpact` (what a deletion does to rules and device
  links, computed before deleting); `tests/tst_hometree/` the room -> device -> owned IOs
  tree (`src/HomeTree.*`), cascade delete without double delete, drag and drop onto a room and
  the delete confirmation dialog.
- `tests/tst_onlineconfig/` covers the `config get`/`put` JSON helper (`src/OnlineConfig.*`): `io_format` declared in every put, announced format parsing, the server's refusal text, the per-host format memory behind the devices gate in `DialogSaveOnline`.
- `tests/tst_onlinesession/` drives `OnlineSession` (`src/OnlineSession.*`, the requests of the online open/save dialogs) against a fake `/api.php` over http and https (`QSslServer`, a self-signed certificate for 127.0.0.1): one `admin_unlock` when the server answers `bad_session`, `admin_session` in the body of `config get`/`put` only, one https retry on `https_required`, requests unchanged against a server without admin.
  `tests/tst_onlinedialogs/` drives the online open and save dialogs offscreen against the same fake server (`FakeApi.h`): the admin password typed for a host never saved before is the one unlocked, picking another saved host clears it.
- `tests/tst_backuparchive/` covers `BackupArchive` (`src/BackupArchive.*`, "Open a backup archive..."): the `calaos-backup/2` zip of the web app and a zipped project folder, against hand-written zips for what KZip would not write: size and entry limits, a size the headers lie about, paths leaving the archive. `io.xml` and `rules.xml` that are not what `Load()` can open are refused before it runs, and so are files from two folders. `local_config.xml` is noticed, never read. `BackupSaveGuard` decides where a save of an opened backup goes: never to the temp folder, so the first save asks for a folder, and whether it can be sent as it is: only untouched, while the temp folder holds the files of that archive. It also takes those files out of the temp folder once the project left it. `MainWindow` only applies its answers.
- `tests/tst_devicetemplates/` covers `DeviceTemplates` (`src/DeviceTemplates.*`), the device
  templates of calaos_base applied to the model: for every shipped template, each enum option and
  each `when` both ways builds a device whose roles, params and IO params are documented in
  `io_doc.json`, whose links resolve, and which survives a save and reload; free ids, validation,
  substitution, insertion in the tree. `tests/tst_dialognewdevice/` drives the "New device from a
  template" dialog offscreen; `CALAOS_SHOTS_DIR=<dir>` (and `CALAOS_SHOTS_LANG=fr`,
  `CALAOS_SHOTS_QM=<compiled calaos_installer_fr.qm>`) writes its screenshots.
  `tests/server_load/run.sh` (not in CI: it needs a calaos_base build) starts a real
  calaos_server on each device that `tst_devicetemplates` writes with `CALAOS_DEVICE_XML_DIR`, and
  fails on a loading warning or on an attribute the server's save drops.
- `tests/tst_deviceedit/` covers `DeviceEdit` (`src/DeviceEdit.*`) and its dialog
  `DialogEditDevice`, the properties of an existing device: links and documented params read
  from IODoc, changed and saved, rules and owned IOs untouched, refusals (decision 17, legacy
  types, IO kind), required roles; `CALAOS_SHOTS_DIR` writes its screenshots and
  `CALAOS_DEVICE_XML_DIR` the edited io.xml for `tests/server_load/run.sh`.
- `tests/tst_mqttclient/` covers the MQTT 3.1.1 client (`src/MqttPacket.*` packets and stream reader,
  `src/MqttClient.*` on `QTcpSocket`) against `FakeMqttBroker` and scripted byte streams, and against a real
  `eclipse-mosquitto:2.0.22` container that the test starts itself (skipped when `docker` is missing or does not
  answer). `tests/tst_zigbee2mqtt/` drives the Zigbee2MQTT dialog against the fake broker.
- `tests/tst_esphomediscovery/` covers `MqttListener` (`src/MqttListener.*`, `MqttClient` listening to a
  broker for a few seconds) against `FakeMqttBroker`, an in-process broker independent of `MqttClient`, and
  `EsphomeDiscovery` (`src/EsphomeDiscovery.*`): ESPHome climates read from their state topics, their Home
  Assistant discovery config, `esphome/discover` and `<prefix>/status`, and the template values they imply.
  `tst_dialognewdevice` drives the "Find it on the MQTT broker" panel (`src/WidgetEsphomeDiscovery.*`)
  against the same fake broker.
- `tests/tst_hadiscovery/` covers `HomeAssistantDiscovery` (`src/HomeAssistantDiscovery.*`), Home Assistant
  MQTT discovery read from `<prefix>/#` (abbreviations, `~`, availability, device discovery) and imported: an MQTT
  IO per sensor, switch, light, cover or number, with the params IODoc documents; a climate device of the ESPHome
  climate templates (filled by `EsphomeDiscovery::prefill`, then given the entity's own topics) for a climate.
  The dialog (`src/DialogHomeAssistantMqtt.*`) is driven against `FakeMqttBroker`;
  `CALAOS_DEVICE_XML_DIR` writes the io.xml it imports (`ha_*.xml`) for `tests/server_load/run.sh`.
- `tests/tst_replacedroles/` covers IODoc's `replaces` (a thermostat's zone valve `output` for
  `output_heat`/`output_cool`) in the editor, its dialog and the command form, against its own
  `io_doc.json` fixture (the thermostat of a server that publishes it), not the shipped one.
- `tests/tst_apptheme/` covers `AppTheme` (`src/AppTheme.*`, called from `main.cpp`): the installer is
  always light whatever the system scheme. The test starts from a dark GTK platform theme
  (`QT_QPA_PLATFORMTHEME=gtk3`, `GTK_THEME=Adwaita:dark`; offscreen ignores `setColorScheme`) and skips
  when the plugin is missing unless `CALAOS_REQUIRE_DARK_SYSTEM` is set (CI). `CALAOS_SHOTS_DIR=<dir>`
  with `CALAOS_SHOTS_FIX=0|1` writes the Remote UI editor screenshots, one scheme per process.
- `tests/tst_servertypes/` pins the IO types the installer may create to the ones the server
  registers (`server_io_types.txt`, regenerate it with the command in its header when the
  server adds or drops a type). A type the server dropped moves to a `REGISTER_LEGACY_*` line
  in `Calaos/IOBase.cpp`: still read and written back, shown as unsupported in the tree.
- `data/doc/{en,fr}` (`io_doc.json`, `io_doc.md`, `device_templates.json`) are generated by
  calaos_base, never edited. `./generate_iodoc.sh` refreshes them from the `io-doc` artifact of
  calaos_base's CI (latest green master; `CALAOS_BASE_BRANCH=<branch>` for another one), or from
  a directory (`calaos_base/scripts/create_iodoc`). The `iodoc-sync` CI job fails when the parsed
  JSON differs from the artifact (`scripts/check_iodoc_sync.py`, tested by
  `tests/iodoc_sync/test_check_iodoc_sync.py`); the download (`scripts/fetch_iodoc_artifact.sh`, tested
  with a fake `gh`) only trusts push, schedule and workflow_dispatch runs of calaos_base, never a
  pull request's (a fork's run carries the fork's branch name); `IODoc::getDeviceObject(impl)`
  reads `devices[impl]` of `io_doc.json`, a key that is not an IO type.
- The model and (de)serialisation sources are listed once in `calaos_model.pri`, shared by
  the application and the tests. A new file that the tests need goes there; one that only
  the GUI needs stays in `calaos_installer.pro`.
- `tests/tst_devicecommand/` covers the JSON command form (`src/DeviceCommand.*`). The `list_rule`
  of a param (`always_includes`, `default_when_linked`, `value_requires_values`, published by
  calaos_base) is applied to the `mode` field and the `modes` param pair only: no other field
  or param reads it. A param without `list_rule` (the proxy `modes`) is the whole list.
- `tests/tst_builddirs/` runs qmake on the projects and checks that every intermediate file
  lands in `build-qt<Qt version>/` and that a build directory of the former layout is refused;
  `tests/tst_timezonehelper/` covers the timezone list and builds the Remote UI editor offscreen.
- To add a suite: create `tests/tst_<name>/tst_<name>.pro` (copy `tst_projectmanager`) and
  add it to `SUBDIRS` in `tests/tests.pro`.
- CI (`.github/workflows/pr_ci.yml`) builds the application and runs these tests on every
  pull request. `tests/tst_widgetioproperties` drives the IO properties dialog widget offscreen; `tests/tst_roomteardown` and `tests/tst_teardownorder` run their crash/hang-prone cases in a forked child (the parent must never touch `ListeRoom`/`ListeRule` in the latter); the rest of the GUI has no test: verify UI changes by running the app.

## Code Style Guidelines

### Two Eras of Code

The codebase has two distinct style layers. **Follow the newer conventions for all new code.**

| Aspect | Old Code (Calaos/) | New Code (src/ newer files) |
|---|---|---|
| Methods | `get_name()`, `AddInput()` | `setIoId()`, `loadFromXml()` |
| Members | no prefix (`param`, `gui_type`) | `m_` prefix (`m_ioId`, `m_type`) |
| Static members | no prefix | `s_` prefix (`s_knownAttributes`) |
| Null | `NULL` | `nullptr` |
| File naming | PascalCase | PascalCase (preferred) |
| Header guards | `S_NAME_H` | `FILENAME_H` |
| Memory | manual `delete` | Qt parent-child ownership |

### Naming Conventions

- **Classes:** PascalCase — `WidgetModel`, `DialogRemoteUIEditor`, `PageModel`
- **Methods/functions:** camelCase — `loadFromXmlElement()`, `setIoId()`, `widgetAt()`
- **Member variables:** `m_` prefix + camelCase — `m_ioId`, `m_gridW`, `m_modified`
- **Static members:** `s_` prefix + camelCase — `s_knownAttributes`
- **Local variables:** camelCase, descriptive — `pageIndex`, `widgetType`
- **Constants/defines:** `UPPER_SNAKE_CASE` — `WAGO_HOST`, `BCAST_UDP_PORT`
- **Enums:** Plain enums (not `enum class`) with `UPPER_SNAKE_CASE` values
- **Files:** PascalCase matching the class name — `WidgetModel.h`, `DialogIOSelector.cpp`

### Formatting

- **Indentation:** 4 spaces (no tabs)
- **Braces:** Opening brace on its own line (Allman-influenced style):
  ```cpp
  if (condition)
  {
      doSomething();
  }
  ```
- **For loops:** No space after semicolons: `for (int i = 0;i < size;i++)`
- **Section separators:** `//----...----` lines used in Calaos/ domain classes
- **Line length:** No strict limit; keep under ~120 characters where practical
- **No linter or formatter configured** (.clang-format does not exist)

### Includes

- Own header first in `.cpp` files
- Project headers next, then Qt headers last
- Calaos/ headers use angle brackets: `#include <ListeRoom.h>`
- src/ headers use quotes: `#include "DialogIOSelector.h"`
- Qt headers always use angle brackets: `#include <QWidget>`
- Include paths configured in .pro: `src/common/`, `src/common/LuaScript`, `Calaos/`, `src/`

### Header Guards

Use `#ifndef` / `#define` / `#endif` (not `#pragma once`):
```cpp
#ifndef WIDGETMODEL_H
#define WIDGETMODEL_H
// ...
#endif // WIDGETMODEL_H
```
Guard name: uppercase filename + `_H` suffix.

### Qt Patterns

- **Signal/slot connections:** Use new-style syntax (pointer-to-member):
  ```cpp
  connect(sender, &SenderClass::signal, receiver, &ReceiverClass::slot);
  ```
  Lambda connections are acceptable for short inline handlers.
  Do NOT use old-style `SIGNAL()`/`SLOT()` macros.
- **Q_OBJECT:** First line inside any QObject-derived class body
- **Q_PROPERTY:** Standard getter/setter/notify pattern for QML-exposed properties
- **Q_INVOKABLE:** For methods callable from QML
- **Singletons:** `ClassName::Instance()` pattern with static local variable
  (`ListeRoom::Instance()`, `ListeRule::Instance()`, `WagoConnect::Instance()`)
- **Translations:** Use `tr()` for all user-visible strings
- **Unused params:** Use `Q_UNUSED()` macro
- **QML imports:** Qt 6 style without version numbers: `import QtQuick`

### Memory Management

- Use **Qt parent-child ownership** — pass `this` as parent to `new QObject(this)`
- Avoid raw `new`/`delete` for QObject-derived types; rely on the parent tree
- For non-QObject types, prefer RAII or scope-based lifetime
- Legacy code uses `DELETE_NULL()` macro (defined in `Utils.h`) — avoid in new code

### Error Handling

- No exceptions — the codebase uses virtually zero try/catch
- Return `nullptr` or `false` on failure; check return values
- Use `QMessageBox::critical()` / `QMessageBox::warning()` for user-facing errors
- Validate inputs with early returns

### Logging

- Use `qDebug()` with stream syntax: `qDebug() << "message" << variable;`
- Custom aliases exist (`cDebug`, `cInfo`, `cWarning`, `cError`, `cCritical`) but
  all map to `qDebug()` — no log-level differentiation
- Use `qWarning()` for genuine warnings
- Use `console.log()` in QML

## Project Structure

```
Calaos/          — Core domain model (Room, Rule, IOBase, Condition, Action)
src/             — Main application source (~168 files)
  common/        — Shared utilities (Utils, Params, LuaScript/)
  ftp/           — FTP client (qftp, vendored)
  wizards/hue/   — Philips Hue setup wizard
qml/             — QML UI for Remote UI Editor
  components/    — QML components (GridContainer, PageEditor, etc.)
  managers/      — DragDropManager, GridManager
  utils/         — GridUtils.js, WidgetColors.js
data/            — UI forms (.ui), resources (.qrc), images, IO docs
lang/            — Translations (7 languages: fr, de, es, nb, hi, pl, ru)
machine_creator/ — Secondary app: Calaos Machine Creator (separate .pro)
tests/           — Qt Test suites (tests.pro) and sample XML projects used as their data
scripts/         — CI/CD and build/deploy scripts
win32/           — Windows packaging (NSIS/Inno Setup)
macos/           — macOS packaging (DMG, signing)
debian/          — Debian packaging
```

## CI/CD

GitHub Actions workflows (manual trigger via `workflow_dispatch`):
- `.github/workflows/build_release_win.yml` — Windows (Docker cross-compile)
- `.github/workflows/build_release_macos.yml` — macOS (native build + codesign + notarize)

## Key Notes for Agents

- When adding new source files, register them in `calaos_installer.pro` (SOURCES/HEADERS)
- New `.ui` forms go in `data/` and must be added to FORMS in the `.pro` file
- New QML files go in `qml/` and must be added to `qml/qml.qrc`
- New resources/images go in `data/` and must be added to the appropriate `.qrc` file
- The `Calaos/` directory code is shared with calaos_server — modify with care
- Vendored code (Lua 5.1.4, qftp, drivelist, BLAKE2) should not be modified
- `using namespace std;` and `using namespace Calaos;` appear in older headers —
  avoid adding new `using namespace` directives in headers

## Pitfalls

Cross-repo notes (IODoc chain, merge order, rules, ESPHome): [calaos_base/docs/18_stack_notes.md](https://github.com/calaos/calaos_base/blob/master/docs/18_stack_notes.md) (in French).

- **Per-Qt build dirs.** Objects go to `build-qt<Qt version>/` (`.qmake.conf:8`), because qmake does not track Qt headers and a Qt update otherwise links stale objects with fresh ones (reason in `.qmake.conf:3-7`). Do not reuse an object dir across Qt versions.
- **Forced light theme.** `AppTheme::forceLight` (`src/AppTheme.cpp:43-51`) sets `ColorScheme::Light` from Qt 6.8 and pins a light palette before that. Hardcoded dark-theme assumptions in QML/widgets will break in the system dark mode.
- **`data/doc` is generated** by `generate_iodoc.sh` from the calaos_base CI artifact; never edit it by hand, and merge calaos_base first (see the linked notes).
- **MQTT is in-house.** `src/MqttClient.*` speaks MQTT 3.1.1 over `QTcpSocket` (no TLS, no MQTT 5): QtMqtt uses Qt private APIs, and a KDE runtime update made the Flatpak crash at startup until it was rebuilt. Reuse `MqttListener` (or `MqttClient`) for any MQTT feature, never add another client. Its slots may run modal dialogs: closed sockets are deleted only once the socket callback returns (`SocketCallback` in `MqttClient.cpp`).
