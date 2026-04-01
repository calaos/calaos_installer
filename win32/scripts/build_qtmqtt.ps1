# Build QtMqtt from source for MinGW
# QtMqtt is not available via aqtinstall, so we compile it from the Qt project sources
# Prerequisites: Qt6 installed (install-qt-action), MinGW in PATH, CMake available

$ErrorActionPreference = "Stop"

$INSTALL_PREFIX = "C:/qtmqtt"
$BUILD_ROOT = "$env:RUNNER_TEMP/qtmqtt_build"

# Use Qt installation from install-qt-action
$QT_DIR = $env:QT_ROOT_DIR
if (-not $QT_DIR) {
    Write-Error "QT_ROOT_DIR is not set. Ensure install-qt-action ran before this script."
    exit 1
}

# Determine Qt version for matching source branch
$qtVersion = (qmake -query QT_VERSION)
if ($LASTEXITCODE -ne 0 -or -not $qtVersion) {
    Write-Error "Failed to query Qt version from qmake"
    exit 1
}

Write-Host "=== Building QtMqtt ===" -ForegroundColor Cyan
Write-Host "Install prefix: $INSTALL_PREFIX"
Write-Host "Qt directory: $QT_DIR"
Write-Host "Qt version: $qtVersion"
Write-Host "Build root: $BUILD_ROOT"

New-Item -ItemType Directory -Force -Path $BUILD_ROOT | Out-Null

# --- Clone QtMqtt sources ---
Write-Host "`n=== Cloning QtMqtt v$qtVersion ===" -ForegroundColor Cyan
Set-Location $BUILD_ROOT
git clone --depth 1 --branch "v$qtVersion" https://code.qt.io/qt/qtmqtt.git
if ($LASTEXITCODE -ne 0) {
    Write-Warning "Tag v$qtVersion not found, trying branch $qtVersion"
    git clone --depth 1 --branch $qtVersion https://code.qt.io/qt/qtmqtt.git
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

# --- Build QtMqtt ---
Write-Host "`n=== Building QtMqtt ===" -ForegroundColor Cyan
New-Item -ItemType Directory -Force -Path "$BUILD_ROOT/qtmqtt_build" | Out-Null
Set-Location "$BUILD_ROOT/qtmqtt_build"

cmake -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" `
    -DCMAKE_PREFIX_PATH="$QT_DIR" `
    -DBUILD_TESTING=OFF `
    -DBUILD_EXAMPLES=OFF `
    "$BUILD_ROOT/qtmqtt"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make -j $env:NUMBER_OF_PROCESSORS
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make install
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n=== QtMqtt build complete ===" -ForegroundColor Green
Write-Host "Installed to: $INSTALL_PREFIX"
Get-ChildItem -Recurse "$INSTALL_PREFIX" -Include "*.dll","*.a" | ForEach-Object { Write-Host "  $_" }
