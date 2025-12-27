# Weather Station Layout/Setup Script

$ErrorActionPreference = "Stop"

$qtPath = "C:\Qt\6.10.1\mingw_64\bin"
$mingwPath = "C:\Qt\Tools\mingw1310_64\bin"
$buildDir = "build_release"
$distDir = "WeatherStation_Setup"

Write-Host "Started Deployment Process..." -ForegroundColor Cyan

# 1. Clean and Configure (Release Mode)
if (Test-Path $buildDir) { Remove-Item -Recurse -Force $buildDir }
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null

$env:PATH = "$qtPath;$mingwPath;$env:PATH"

Write-Host "Building project..." -ForegroundColor Yellow
cmake -B $buildDir -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64"
cmake --build $buildDir --config Release

# 2. Prepare Dist Directory
if (Test-Path $distDir) { Remove-Item -Recurse -Force $distDir }
New-Item -ItemType Directory -Force -Path $distDir | Out-Null
New-Item -ItemType Directory -Force -Path "$distDir/data" | Out-Null

# 3. Copy Executable
Copy-Item "$buildDir/weather_station_qt.exe" -Destination "$distDir/WeatherStation.exe"

# 4. Copy Default Data (Empty file if not exists)
if (Test-Path "data/measurements.txt") {
    Copy-Item "data/measurements.txt" -Destination "$distDir/data/measurements.txt"
}
else {
    New-Item -ItemType File -Path "$distDir/data/measurements.txt" | Out-Null
}

# 5. Run windeployqt
Write-Host "Bundling Qt dependencies..." -ForegroundColor Yellow
if (Get-Command windeployqt -ErrorAction SilentlyContinue) {
    & windeployqt --release --compiler-runtime --no-translations --no-opengl-sw "$distDir/WeatherStation.exe"
}
else {
    Write-Host "Error: windeployqt not found in PATH." -ForegroundColor Red
    Exit 1
}

# 6. Create Zip Archive (Optional)
Write-Host "Creating Zip Archive..." -ForegroundColor Yellow
$zipFile = "WeatherStation_v1.0.zip"
if (Test-Path $zipFile) { Remove-Item $zipFile }
Compress-Archive -Path "$distDir/*" -DestinationPath $zipFile

Write-Host "Setup completed successfully!" -ForegroundColor Green
Write-Host "Portable folder: $distDir"
Write-Host "Zip file: $zipFile"
