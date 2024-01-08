;
; This file is part of Converseen, an open-source batch image converter
; and resizer.
;
; (C) Francesco Mondello 2009 - 2024
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
; Contact e-mail: Francesco Mondello <faster3ck@gmail.com>
;
;

#define MyAppName "Converseen"
#define MyAppVersion "0.12.0.2"
#define MyAppBuild "1"
#define MyAppPublisher "Francesco Mondello"
#define MyAppURL "http://converseen.fasterland.net/"
#define MyAppExeName "converseen.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B35C58D3-FBD2-4A81-8371-588F51EC180F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
InfoBeforeFile=Converseen\ImageMagick_License.txt
LicenseFile=Converseen\COPYING.txt
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppName}.exe
OutputBaseFilename=Converseen-{#MyAppVersion}-{#MyAppBuild}-win32-setup
Compression=lzma2/max
SolidCompression=yes

; Installer details
VersionInfoDescription={#MyAppName} {#MyAppVersion} - Windows Installer
VersionInfoVersion={#MyAppVersion}
AppCopyright="(C) 2009 - 2024 Francesco Mondello GNU - GPL"

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "Converseen\converseen.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Converseen\*"; Excludes: "settings"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function gsInstalled(): Boolean;
begin
  if RegKeyExists(HKEY_CURRENT_USER, 'SOFTWARE\GPL Ghostscript') then
  begin
	Result := true;
  end
  else
	  begin
		Result := false;
	  end
end;

procedure installGs();
var
    ErrCode: integer;
begin
	if (msgbox('In order to enable Converseen to manage PDF files you have to download and install Ghostscript. Do you want to download it now?', mbConfirmation, MB_YESNO) = IDYES) then
	begin
		ShellExec('open', 'https://ghostscript.com/releases/gsdnld.html', '', '', SW_SHOW, ewNoWait, ErrCode);
	end
end;

procedure CurPageChanged(CurPageID: Integer);
var
	gsIsInstalled : boolean;
begin
	if CurPageID = wpFinished then
	begin
		gsIsInstalled := gsInstalled();
		
		if gsIsInstalled = false then
			installGs();
	end
end;

[Code]
function GetNumber(var temp: String): Integer;
var
  part: String;
  pos1: Integer;
begin
  if Length(temp) = 0 then
  begin
    Result := -1;
    Exit;
  end;
    pos1 := Pos('.', temp);
    if (pos1 = 0) then
    begin
      Result := StrToInt(temp);
    temp := '';
    end
    else
    begin
    part := Copy(temp, 1, pos1 - 1);
      temp := Copy(temp, pos1 + 1, Length(temp));
      Result := StrToInt(part);
    end;
end;

function CompareInner(var temp1, temp2: String): Integer;
var
  num1, num2: Integer;
begin
    num1 := GetNumber(temp1);
  num2 := GetNumber(temp2);
  if (num1 = -1) or (num2 = -1) then
  begin
    Result := 0;
    Exit;
  end;
      if (num1 > num2) then
      begin
        Result := 1;
      end
      else if (num1 < num2) then
      begin
        Result := -1;
      end
      else
      begin
        Result := CompareInner(temp1, temp2);
      end;
end;

function CompareVersion(str1, str2: String): Integer;
var
  temp1, temp2: String;
begin
    temp1 := str1;
    temp2 := str2;
    Result := CompareInner(temp1, temp2);
end;

function InitializeSetup(): Boolean;
var
  oldVersion: String;
  uninstaller: String;
  ErrorCode: Integer;
  vCurID      :String;
  vCurAppName :String;
begin
  vCurID:= '{#SetupSetting("AppId")}';
  vCurAppName:= '{#SetupSetting("AppName")}';
  //remove first "{" of ID
  vCurID:= Copy(vCurID, 2, Length(vCurID) - 1);
  //
  if RegKeyExists(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + vCurID + '_is1') then
  begin
    RegQueryStringValue(HKEY_LOCAL_MACHINE,
      'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + vCurID + '_is1',
      'DisplayVersion', oldVersion);
    if (CompareVersion(oldVersion, '{#SetupSetting("AppVersion")}') < 0) then      
    begin
      if MsgBox('Version ' + oldVersion + ' of ' + vCurAppName + ' is already installed. In order to continue with the installation the old version of ' + vCurAppName + ' will be removed. Continue?',
        mbConfirmation, MB_YESNO) = IDNO then
      begin
        Result := False;
      end
      else
      begin
          RegQueryStringValue(HKEY_LOCAL_MACHINE,
            'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + vCurID + '_is1',
            'UninstallString', uninstaller);
          ShellExec('runas', uninstaller, '/SILENT', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
          Result := True;
      end;
    end
    else
      begin
        Result := True;
      end;
  end
  else
  begin
    Result := True;
  end;
end;
