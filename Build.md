# Configure
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;$env:PATH"
cmake -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64"

# Build both versions
cmake --build build

# Run Qt version
$env:PATH = "C:\Qt\6.10.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;$env:PATH"
.\build\weather_station_qt.exe