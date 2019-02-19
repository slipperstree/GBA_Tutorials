tasklist /fi "imagename eq mGBA.exe" |find ":" > nul

if errorlevel 1 taskkill /f /im "mGBA.exe"