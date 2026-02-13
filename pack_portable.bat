set build_dir=.\
set src_dir=%1
set qt_lib_path=%2
set branch=%3
set build_mode=%4

set src_dir=%src_dir:/=\%
set qt_lib_path=%qt_lib_path:/=\%

set exe_release_path=%build_dir%\release\CviPQTool.exe
set exe_debug_path=%build_dir%\debug\CviPQTool.exe

if "%build_mode%"=="debug" (
	set exe_path=%exe_debug_path%
) else (
	set exe_path=%exe_release_path%
)

set dll_path=%qt_lib_path%\bin
set plugin_path=%qt_lib_path%\plugins
set dll_path=%dll_path:/=\%
set plugin_path=%plugin_path:/=\%

mkdir %build_dir%\libs
xcopy %src_dir%\libs %build_dir%\libs /E /Y

rd %build_dir%\templates /S /Q
mkdir %build_dir%\templates
xcopy %src_dir%\jsons\templates %build_dir%\templates /E /Y

rd %build_dir%\portable /S /Q
mkdir %build_dir%\portable
mkdir %build_dir%\portable\platforms
mkdir %build_dir%\portable\imageformats
mkdir %build_dir%\portable\styles
mkdir %build_dir%\portable\templates

copy %exe_path% %build_dir%\portable

xcopy %build_dir%\libs\*.dll %build_dir%\portable /E /Y
xcopy %build_dir%\templates %build_dir%\portable\templates /E /Y

xcopy %dll_path%\libgcc_s_seh-1.dll %build_dir%\portable /Y
xcopy %dll_path%\libstdc++-6.dll %build_dir%\portable /Y
xcopy %dll_path%\libwinpthread-1.dll %build_dir%\portable /Y

if "%build_mode%"=="debug" (
	xcopy %dll_path%\Qt5Chartsd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Cored.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Guid.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Networkd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Widgetsd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5WebSocketsd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Qmld.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Quickd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5QuickWidgetsd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5PrintSupportd.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5SerialPortd.dll %build_dir%\portable /Y
) else (
	xcopy %dll_path%\Qt5Charts.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Core.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Gui.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Network.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Widgets.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5WebSockets.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Qml.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5Quick.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5QuickWidgets.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5PrintSupport.dll %build_dir%\portable /Y
	xcopy %dll_path%\Qt5SerialPort.dll %build_dir%\portable /Y
)

xcopy %plugin_path%\platforms\qdirect2d.dll %build_dir%\portable\platforms /Y
xcopy %plugin_path%\platforms\qminimal.dll %build_dir%\portable\platforms /Y
xcopy %plugin_path%\platforms\qoffscreen.dll %build_dir%\portable\platforms /Y
xcopy %plugin_path%\platforms\qwebgl.dll %build_dir%\portable\platforms /Y
xcopy %plugin_path%\platforms\qwindows.dll %build_dir%\portable\platforms /Y
xcopy %plugin_path%\imageformats\qgif.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qicns.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qico.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qjpeg.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qsvg.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qtga.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qtiff.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qwbmp.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\imageformats\qwebp.dll %build_dir%\portable\imageformats /Y
xcopy %plugin_path%\styles\qwindowsvistastyle.dll %build_dir%\portable\styles /Y
