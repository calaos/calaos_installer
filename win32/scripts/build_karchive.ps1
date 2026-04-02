# Build KArchive KF6 from source for MinGW
# This script is intended to run in GitHub Actions on windows-latest
# Prerequisites: Qt6 installed (install-qt-action), MinGW in PATH, CMake available

$ErrorActionPreference = "Stop"

$INSTALL_PREFIX = "C:/karchive"
$BUILD_ROOT = "$env:RUNNER_TEMP/karchive_build"

# Use Qt installation from install-qt-action
$QT_DIR = $env:QT_ROOT_DIR
if (-not $QT_DIR) {
    Write-Error "QT_ROOT_DIR is not set. Ensure install-qt-action ran before this script."
    exit 1
}

Write-Host "=== Building KArchive KF6 ===" -ForegroundColor Cyan
Write-Host "Install prefix: $INSTALL_PREFIX"
Write-Host "Qt directory: $QT_DIR"
Write-Host "Build root: $BUILD_ROOT"

# Add Qt bin directory to PATH so that qtpaths.exe is found by ECM
$env:PATH = "$QT_DIR/bin;$env:PATH"

New-Item -ItemType Directory -Force -Path $BUILD_ROOT | Out-Null

# --- Build extra-cmake-modules (ECM) ---
Write-Host "`n=== Building extra-cmake-modules ===" -ForegroundColor Cyan
Set-Location $BUILD_ROOT
git clone --depth 1 --branch master https://invent.kde.org/frameworks/extra-cmake-modules.git
New-Item -ItemType Directory -Force -Path "$BUILD_ROOT/ecm_build" | Out-Null
Set-Location "$BUILD_ROOT/ecm_build"

cmake -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" `
    -DBUILD_TESTING=OFF `
    -DBUILD_HTML_DOCS=OFF `
    -DBUILD_MAN_DOCS=OFF `
    "$BUILD_ROOT/extra-cmake-modules"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make -j $env:NUMBER_OF_PROCESSORS
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make install
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# --- Build KArchive ---
Write-Host "`n=== Building KArchive ===" -ForegroundColor Cyan
Set-Location $BUILD_ROOT
git clone --depth 1 --branch master https://invent.kde.org/frameworks/karchive.git
New-Item -ItemType Directory -Force -Path "$BUILD_ROOT/karchive_build" | Out-Null
Set-Location "$BUILD_ROOT/karchive_build"

cmake -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" `
    -DCMAKE_PREFIX_PATH="$QT_DIR;$INSTALL_PREFIX" `
    -DBUILD_TESTING=OFF `
    -DBUILD_WITH_QT6=ON `
    "$BUILD_ROOT/karchive"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make -j $env:NUMBER_OF_PROCESSORS
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

mingw32-make install
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n=== KArchive build complete ===" -ForegroundColor Green
Write-Host "Installed to: $INSTALL_PREFIX"
Get-ChildItem -Recurse "$INSTALL_PREFIX" -Include "*.dll","*.a" | ForEach-Object { Write-Host "  $_" }
