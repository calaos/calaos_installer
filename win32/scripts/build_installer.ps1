# Build calaos_installer and machine_creator for Windows (MinGW 64-bit)
# Prerequisites: Qt6 + MinGW in PATH (install-qt-action), KArchive built

$ErrorActionPreference = "Stop"

$ROOT_DIR = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path))
$BUILD_DIR = Join-Path $ROOT_DIR "build_win"
$BUILD_MC_DIR = Join-Path $BUILD_DIR "mc"

# Set KArchive path for qmake .pro files
$env:KARCHIVE_PREFIX = "C:/karchive"

Write-Host "=== Building Calaos Installer ===" -ForegroundColor Cyan
Write-Host "Root: $ROOT_DIR"
Write-Host "Build dir: $BUILD_DIR"
Write-Host "KArchive: $env:KARCHIVE_PREFIX"

# Add KArchive bin to PATH so DLLs are found at link time
$env:PATH = "C:/karchive/bin;$env:PATH"

# --- Build calaos_installer ---
if (Test-Path $BUILD_DIR) { Remove-Item -Recurse -Force $BUILD_DIR }
New-Item -ItemType Directory -Force -Path $BUILD_MC_DIR | Out-Null
Set-Location $BUILD_DIR

Write-Host "`n=== qmake calaos_installer ===" -ForegroundColor Cyan
qmake "$ROOT_DIR/calaos_installer.pro"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n=== make calaos_installer ===" -ForegroundColor Cyan
mingw32-make -j $env:NUMBER_OF_PROCESSORS
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# --- Build machine_creator ---
Set-Location $BUILD_MC_DIR

Write-Host "`n=== qmake machine_creator ===" -ForegroundColor Cyan
qmake "$ROOT_DIR/machine_creator/machine_creator.pro"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n=== make machine_creator ===" -ForegroundColor Cyan
mingw32-make -j $env:NUMBER_OF_PROCESSORS
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n=== Build complete ===" -ForegroundColor Green
