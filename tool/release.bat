set OutFile=CviPQtool_%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%.zip

set BaseDir=%~p0
set ProjectDir=%BaseDir%\..
set TargetDir=%ProjectDir%\portable

del "%ProjectDir%\out" /S /Q
rmdir "%ProjectDir%\out" /S /Q
mkdir "%ProjectDir%\out"
mkdir "%ProjectDir%\out\temp"
xcopy "%TargetDir%\*" "%ProjectDir%\out\temp" /S /Y /I
powershell Compress-Archive -Path "%ProjectDir%\out\temp\*" -DestinationPath "%ProjectDir%\out\%OutFile%"
del "%ProjectDir%\out\temp" /S /Q
rmdir "%ProjectDir%\out\temp" /S /Q

echo "%ProjectDir%\out\%OutFile% is generated."
