if not exist %~dp0\build\NUL mkdir %~dp0\build
cd build
cmake -G "Visual Studio 16 2019" -DMAYA_VERSION=2020 ../
cmake --build . --config Release --target Install
pause