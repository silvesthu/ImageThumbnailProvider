$dll_path = (get-location).path + "\ImageThumbnailProvider.dll"

New-PSDrive -Name HKCR -PSProvider Registry -Root HKEY_CLASSES_ROOT

New-Item -Path "HKCR:\CLSID\{84F5E367-E0A3-4957-B6B5-CA9A2BADC57B}"
Set-Item -Path "HKCR:\CLSID\{84F5E367-E0A3-4957-B6B5-CA9A2BADC57B}" -Value "Image Thumbnail Provider"

New-Item -Path "HKCR:\CLSID\{84F5E367-E0A3-4957-B6B5-CA9A2BADC57B}\InprocServer32"
Set-Item -Path "HKCR:\CLSID\{84F5E367-E0A3-4957-B6B5-CA9A2BADC57B}\InprocServer32" -Value "$dll_path"

# Specify Thumbnail Handlers https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/cc144118(v=vs.85)

New-Item -Path "HKCR:\.exr\shellex"
New-Item -Path "HKCR:\.exr\shellex\{e357fccd-a995-4576-b01f-234630154e96}"
Set-Item -Path "HKCR:\.exr\shellex\{e357fccd-a995-4576-b01f-234630154e96}" -Value "{84F5E367-E0A3-4957-B6B5-CA9A2BADC57B}"