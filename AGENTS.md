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

# Rebuild translations (.ts -> .qm)
# Handled automatically by qmake via lrelease during build
```

### Dependencies

- Qt 5 or Qt 6 (with core5compat for Qt 6)
- Qt modules: core gui network widgets xml printsupport quick qml quickwidgets quickcontrols2
- Optional: Qt MQTT module (enables Zigbee2MQTT support, defines `QT_MQTT_AVAILABLE`)
- Linux: libdl
- Machine Creator additionally requires: KArchive (KF5/KF6), libudev (Linux)

## Testing

There is **no automated test framework** for this project. The `tests/` directory contains
sample Calaos XML project files (io.xml, rules.xml) used as test data for manual testing —
not executable test code. Verify changes by building successfully and running the application.

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
tests/           — Sample XML project files (test data, not test code)
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
