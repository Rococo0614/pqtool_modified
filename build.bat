mkdir build
cd build
"qmake.exe" "../ISPTool.pro" -spec win32-g++ "CONFIG+=qtquickcompiler"
"mingw32-make.exe" -j20