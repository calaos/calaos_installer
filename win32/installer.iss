#define MyAppName "Calaos Installer"
#define MyAppID "{534F1446-12F3-4030-A524-990A43FA003E}"
#define MyAppPublisher "Calaos"
#define MyAppURL "http://www.calaos.fr/"
;#define MyAppVersion "2.0-beta1"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{#MyAppID}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName=Calaos
DisableProgramGroupPage=no
OutputDir=build
OutputBaseFilename=calaos_installer_setup_{#MyAppVersion}
Compression=lzma
SolidCompression=no
AppCopyright=Copyright (c) Calaos Team
WizardImageFile=calaos.bmp
WizardSmallImageFile=WizModernSmallImage-IS.bmp
SetupIconFile=Setup.ico
UninstallDisplayIcon={app}\calaos_installer.exe
MinVersion=0,5.01sp3
PrivilegesRequired=admin

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "C:\calaos_installer_build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: calaosinstaller;
;Source: "Bonjour.msi"; DestDir: "{app}\redist"; Flags: ignoreversion; Components: calaosinstaller;
;Source: "Bonjour64.msi"; DestDir: "{app}\redist"; Flags: ignoreversion; Components: calaosinstaller;
Source: "psvince.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: calaosinstaller;

[Icons]
Name: "{group}\Calaos Installer"; Filename: "{app}\calaos_installer.exe"; Components: calaosinstaller
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Calaos Installer"; Filename: "{app}\calaos_installer.exe"; Components: calaosinstaller; Tasks: desktopicon
Name: "{group}\Calaos Help"; Filename: "http://calaos.fr/wiki"; Components: calaosinstaller; IconFilename: "{app}\question.ico";

[Types]
Name: "Full"; Description: "Full installation"

[Components]
Name: "calaosinstaller"; Description: "Calaos Installer"; Types: Full

[Run]
;Not needed as we ship openssl dll, and the dll are now built using mingw64, no vc runtime needed too
;Filename: "{app}\redist\vcredist_sp1_x86.exe"; Parameters: "/q /NoSetupVersionCheck"; WorkingDir: "{app}\redist"; StatusMsg: "Installing Visual Studio 2010 C++ CRT Libraries..."; Components: calaosinstaller
;Filename: "{app}\redist\Win32OpenSSL_Light-1_0_2L.exe"; Parameters: "/silent /verysilent /sp- /suppressmsgboxes"; WorkingDir: "{app}\redist"; StatusMsg: "Installing OpenSSL libraries..."; Components: calaosinstaller
;For bonjour when we need it one day
;Filename: "msiexec.exe"; Parameters: "/i ""{app}\redist\Bonjour.msi"" /qn"; WorkingDir: "{app}\redist"; StatusMsg: "Installing Bonjour32..."; Components: calaosinstaller; Check: not IsWin64
;Filename: "msiexec.exe"; Parameters: "/i ""{app}\redist\Bonjour64.msi"" /qn"; WorkingDir: "{app}\redist"; StatusMsg: "Installing Bonjour64..."; Components: calaosinstaller; Check: IsWin64
Filename: "{app}\calaos_installer.exe"; WorkingDir: "{app}"; Description: "Start Calaos Installer"; Flags: postinstall nowait skipifsilent runascurrentuser  

[Code]
// function IsModuleLoaded to call at install time
// added also setuponly flag
function IsModuleLoaded(modulename: String ):  Boolean;
external 'IsModuleLoaded@files:psvince.dll stdcall setuponly';
 
// function IsModuleLoadedU to call at uninstall time
// added also uninstallonly flag
function IsModuleLoadedU(modulename: String ):  Boolean;
external 'IsModuleLoaded@{app}\psvince.dll stdcall uninstallonly' ;
 
function CompareVersion(str1, str2: String): Integer;
var
  temp1, temp2: String;
begin
    temp1 := str1;
    temp2 := str2;
	  if temp1 <> temp2 then
	  begin
	    Result := -1;
    end
    else
	  begin
	    Result := 0;
	  end;
end;
 
function InitializeSetup(): Boolean;
var
  oldVersion: String;
  uninstaller: String;
  ErrorCode: Integer;
begin
  if IsModuleLoaded( 'calaos_installer.exe' ) then
  begin
    MsgBox( 'Calaos Installer is running, please close it and run setup again.',
             mbError, MB_OK );
    Result := False;
    Exit;
  end;
 
  if RegKeyExists(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#MyAppID}_is1') then
  begin
    RegQueryStringValue(HKEY_LOCAL_MACHINE,
      'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#MyAppID}_is1',
      'DisplayVersion', oldVersion);
    if (CompareVersion(oldVersion, '{#MyAppVersion}') < 0) then
    begin
      if MsgBox('Version ' + oldVersion + ' of Calaos Installer is already installed. Do you want to replace this version?',
        mbConfirmation, MB_YESNO) = IDNO then
      begin
        Result := False;
      end
      else
      begin
          RegQueryStringValue(HKEY_LOCAL_MACHINE,
            'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#MyAppID}_is1',
            'UninstallString', uninstaller);
          ShellExec('runas', uninstaller, '/SILENT', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
          if (ErrorCode <> 0) then
          begin
            MsgBox( 'Failed to uninstall Calaos Installer version ' + oldVersion + '. Please restart Windows and run setup again.',
             mbError, MB_OK );
            Result := False;
          end
          else
          begin
            Result := True;
          end;
      end;
    end
    else
    begin
      MsgBox('Version ' + oldVersion + ' of Calaos Installer is already installed. This installer will exit.',
        mbInformation, MB_OK);
      Result := False;
    end;
  end
  else
  begin
    Result := True;
  end;
end;
 
function InitializeUninstall(): Boolean;
begin
 
  if IsModuleLoadedU( 'calaos_installer.exe' ) then
  begin
    MsgBox( 'Calaos Installer is running, please close it and run again uninstall.',
             mbError, MB_OK );
    Result := false;
  end
  else Result := true;

  // Unload the DLL, otherwise the dll psvince is not deleted
  UnloadDLL(ExpandConstant('{app}\psvince.dll'));
 
end;
