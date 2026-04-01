# Deploy calaos_installer Windows package to calaos.fr
# Usage: deploy_installer.ps1 -UploadToken <token> [-RelType <stable|experimental>]

param(
    [Parameter(Mandatory=$true)]
    [string]$UploadToken,

    [string]$RelType = "stable"
)

$ErrorActionPreference = "Stop"

$ROOT_DIR = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path))
$PKG_DIR = Join-Path $ROOT_DIR "packages"

# Find the installer exe
$installerExe = Get-ChildItem -Path $PKG_DIR -Filter "calaos_installer_setup_*.exe" | Select-Object -First 1
if (-not $installerExe) {
    Write-Error "No installer .exe found in $PKG_DIR"
    exit 1
}

$filePath = $installerExe.FullName
$hash = (Get-FileHash -Algorithm SHA256 $filePath).Hash.ToLower()
$uploadFolder = "$RelType/calaos_installer/windows"

Write-Host "=== Deploying Calaos Installer ===" -ForegroundColor Cyan
Write-Host "File: $filePath"
Write-Host "SHA256: $hash"
Write-Host "Upload folder: $uploadFolder"

# Upload via curl (available natively on Windows)
curl.exe -X POST `
    -H "Content-Type: multipart/form-data" `
    -F "upload_key=$UploadToken" `
    -F "upload_folder=$uploadFolder" `
    -F "upload_sha256=$hash" `
    -F "upload_file=@$filePath" `
    https://calaos.fr/download/upload

if ($LASTEXITCODE -ne 0) {
    Write-Error "Upload failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

Write-Host "`n=== Deploy complete ===" -ForegroundColor Green
