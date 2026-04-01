# Package calaos_installer for Windows using windeployqt and Inno Setup
# Prerequisites: Build completed (build_installer.ps1), KArchive available, Inno Setup installed

$ErrorActionPreference = "Stop"

$ROOT_DIR = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path))
$BUILD_DIR = Join-Path $ROOT_DIR "build_win"
$BUILD_MC_DIR = Join-Path $BUILD_DIR "mc"
$STAGING_DIR = "C:\calaos_installer_build"
$PKG_DIR = Join-Path $ROOT_DIR "packages"

# Extract version from git tags
Set-Location $ROOT_DIR
$VERSION = git describe --tags --abbrev=0
if ($LASTEXITCODE -ne 0 -or -not $VERSION) {
    Write-Warning "No git tag found, falling back to git describe"
    $VERSION = git describe --long --tags --always
}
$VERSION = $VERSION.Trim()
$FILENAME = "calaos_installer_setup_$VERSION"

Write-Host "=== Packaging Calaos Installer ===" -ForegroundColor Cyan
Write-Host "Version: $VERSION"
Write-Host "Staging dir: $STAGING_DIR"
Write-Host "Output: $PKG_DIR/$FILENAME.exe"

# --- Prepare staging directory ---
if (Test-Path $STAGING_DIR) { Remove-Item -Recurse -Force $STAGING_DIR }
New-Item -ItemType Directory -Force -Path $STAGING_DIR | Out-Null
New-Item -ItemType Directory -Force -Path $PKG_DIR | Out-Null

# --- Find compiled executables ---
# MinGW release builds output to release/ subdirectory
$INSTALLER_EXE = $null
$MC_EXE = $null
foreach ($subdir in @("release", ".")) {
    $candidate = Join-Path $BUILD_DIR "$subdir/calaos_installer.exe"
    if (Test-Path $candidate) { $INSTALLER_EXE = $candidate; break }
}
foreach ($subdir in @("release", ".")) {
    $candidate = Join-Path $BUILD_MC_DIR "$subdir/calaos_machinecreator.exe"
    if (Test-Path $candidate) { $MC_EXE = $candidate; break }
}
if (-not $INSTALLER_EXE) { Write-Error "calaos_installer.exe not found in $BUILD_DIR"; exit 1 }
if (-not $MC_EXE) { Write-Error "calaos_machinecreator.exe not found in $BUILD_MC_DIR"; exit 1 }

Write-Host "Found: $INSTALLER_EXE"
Write-Host "Found: $MC_EXE"

# --- Copy executables to staging ---
Copy-Item $INSTALLER_EXE -Destination $STAGING_DIR
Copy-Item $MC_EXE -Destination $STAGING_DIR

# --- Run windeployqt on main executable ---
Write-Host "`n=== Running windeployqt ===" -ForegroundColor Cyan
$QML_DIR = Join-Path $ROOT_DIR "qml"

windeployqt6 `
    --dir $STAGING_DIR `
    --qmldir $QML_DIR `
    --no-translations `
    (Join-Path $STAGING_DIR "calaos_installer.exe")
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# Also deploy for machine_creator (may pull additional plugins)
windeployqt6 `
    --dir $STAGING_DIR `
    --no-translations `
    (Join-Path $STAGING_DIR "calaos_machinecreator.exe")
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# --- Copy KArchive DLL ---
Write-Host "`n=== Copying KArchive DLL ===" -ForegroundColor Cyan
$KARCHIVE_PREFIX = "C:/karchive"
# Try common MinGW naming patterns
$karchiveDll = $null
foreach ($name in @("libKF6Archive.dll", "KF6Archive.dll")) {
    $candidate = Join-Path $KARCHIVE_PREFIX "bin/$name"
    if (Test-Path $candidate) { $karchiveDll = $candidate; break }
}
if (-not $karchiveDll) {
    # Fallback: search recursively
    $karchiveDll = Get-ChildItem -Recurse $KARCHIVE_PREFIX -Filter "*KArchive*.dll" | Select-Object -First 1
    if ($karchiveDll) { $karchiveDll = $karchiveDll.FullName }
}
if (-not $karchiveDll) { Write-Error "KArchive DLL not found in $KARCHIVE_PREFIX"; exit 1 }
Write-Host "KArchive DLL: $karchiveDll"
Copy-Item $karchiveDll -Destination $STAGING_DIR

# --- Copy QtMqtt DLL ---
Write-Host "`n=== Copying QtMqtt DLL ===" -ForegroundColor Cyan
$QTMQTT_PREFIX = "C:/qtmqtt"
$qtmqttDll = $null
foreach ($name in @("Qt6Mqtt.dll", "libQt6Mqtt.dll")) {
    $candidate = Join-Path $QTMQTT_PREFIX "bin/$name"
    if (Test-Path $candidate) { $qtmqttDll = $candidate; break }
}
if (-not $qtmqttDll) {
    $qtmqttDll = Get-ChildItem -Recurse $QTMQTT_PREFIX -Filter "*Mqtt*.dll" | Select-Object -First 1
    if ($qtmqttDll) { $qtmqttDll = $qtmqttDll.FullName }
}
if ($qtmqttDll) {
    Write-Host "QtMqtt DLL: $qtmqttDll"
    Copy-Item $qtmqttDll -Destination $STAGING_DIR
} else {
    Write-Warning "QtMqtt DLL not found in $QTMQTT_PREFIX - Zigbee2MQTT support may be missing"
}

# --- Build Inno Setup installer ---
Write-Host "`n=== Building Inno Setup installer ===" -ForegroundColor Cyan
$ISS_DIR = Join-Path $ROOT_DIR "win32"
$BUILD_ISS = Join-Path $ISS_DIR "build.iss"

# Generate build.iss with version define prepended
$issContent = "#define MyAppVersion `"$VERSION`"`r`n" + (Get-Content -Raw (Join-Path $ISS_DIR "installer.iss"))
Set-Content -Path $BUILD_ISS -Value $issContent -Encoding UTF8

# Run Inno Setup Compiler
iscc $BUILD_ISS
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# Move the output installer to packages/
$builtInstaller = Join-Path $ISS_DIR "build/$FILENAME.exe"
if (-not (Test-Path $builtInstaller)) {
    Write-Error "Inno Setup output not found: $builtInstaller"
    exit 1
}
Move-Item $builtInstaller -Destination $PKG_DIR -Force

Write-Host "`n=== Packaging complete ===" -ForegroundColor Green
Write-Host "Installer: $PKG_DIR/$FILENAME.exe"
