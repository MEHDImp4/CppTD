
param(
    [string]$Output = "src\output\weather.exe",
    [string]$Std = "c++17",
    [switch]$UseCMake,
    [switch]$Run,
    [switch]$Deploy
)


$OutDir = Split-Path -Parent $Output
if (-not (Test-Path $OutDir)) {
    New-Item -ItemType Directory -Path $OutDir -Force | Out-Null
}

# Helper: detect Qt prefix under C:\Qt (prefer numeric versions)
$qtRoot = 'C:\Qt'
$Global:QtPrefix = $null
if (Test-Path $qtRoot) {
    $versions = Get-ChildItem $qtRoot -Directory | Where-Object { $_.Name -match '^[0-9]' } | Sort-Object LastWriteTime -Descending
    foreach ($v in $versions) {
        if (Test-Path (Join-Path $v.FullName 'mingw_64')) { $Global:QtPrefix = Join-Path $v.FullName 'mingw_64'; break }
        $msvc = Get-ChildItem $v.FullName -Directory -ErrorAction SilentlyContinue | Where-Object { $_.Name -match 'msvc' } | Select-Object -First 1
        if ($msvc) { $Global:QtPrefix = Join-Path $v.FullName $msvc.Name; break }
    }
}

if ($UseCMake) {
    Write-Host "Configuring and building with CMake..."
    $qtRoot = 'C:\Qt'
    $qtPrefix = $null
    if (Test-Path $qtRoot) {
        $versions = Get-ChildItem $qtRoot -Directory | Where-Object { $_.Name -match '^[0-9]' } | Sort-Object LastWriteTime -Descending
        foreach ($v in $versions) {
            if (Test-Path (Join-Path $v.FullName 'mingw_64')) { $qtPrefix = Join-Path $v.FullName 'mingw_64'; break }
            $msvc = Get-ChildItem $v.FullName -Directory -ErrorAction SilentlyContinue | Where-Object { $_.Name -match 'msvc' } | Select-Object -First 1
            if ($msvc) { $qtPrefix = Join-Path $v.FullName $msvc.Name; break }
        }
    }

    if ($Global:QtPrefix) {
        $qt6Dir = Join-Path $Global:QtPrefix 'lib\cmake\Qt6'
        $cmakePrefix = Join-Path $Global:QtPrefix 'lib\cmake'
        Write-Host "Detected Qt prefix: $Global:QtPrefix"
        if (Test-Path $qt6Dir) {
            Write-Host "Using Qt6_DIR: $qt6Dir"
            if ($Global:QtPrefix -match 'mingw') {
                Write-Host 'Detected MinGW Qt; forcing MinGW Makefiles generator'
                if (Test-Path 'build') { Remove-Item -Recurse -Force 'build' }
                & cmake -S . -B build -G "MinGW Makefiles" "-DQt6_DIR=$qt6Dir"
            } else {
                & cmake -S . -B build "-DQt6_DIR=$qt6Dir"
            }
        } else {
            Write-Host "Using CMAKE_PREFIX_PATH: $cmakePrefix"
            if ($Global:QtPrefix -match 'mingw') {
                if (Test-Path 'build') { Remove-Item -Recurse -Force 'build' }
                & cmake -S . -B build -G "MinGW Makefiles" "-DCMAKE_PREFIX_PATH=$cmakePrefix"
            } else {
                & cmake -S . -B build "-DCMAKE_PREFIX_PATH=$cmakePrefix"
            }
        }
    } else {
        Write-Host "No Qt install detected under C:\\Qt — running CMake without Qt hints"
        & cmake -S . -B build
    }

    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configure failed (code $LASTEXITCODE)"
        exit $LASTEXITCODE
    }

    Write-Host "Building..."
    & cmake --build build --config Release
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed (code $LASTEXITCODE)"
        exit $LASTEXITCODE
    }

    Write-Host "Build succeeded. Binary available in build directory."
    if ($Run) {
        # Run with Qt bin on PATH so DLLs are found
        if ($Global:QtPrefix) {
            $qtBin = Join-Path $Global:QtPrefix 'bin'
            Write-Host "Launching with PATH including $qtBin"
            $env:PATH = "$qtBin;$env:PATH"
        }
        & .\build\CppTD_Qt.exe
        exit $LASTEXITCODE
    }
    exit 0
}


$sources = Get-ChildItem -Path src -Filter *.cpp -Recurse | ForEach-Object { $_.FullName }
if ($sources.Count -eq 0) {
    Write-Error "Aucun fichier source trouvé dans src"
    exit 1
}


$args = @("-Wall", "-Wextra", "-g3", "-std=$Std", "-I", "include")
$args += $sources
$args += @("-o", $Output)

Write-Host "Compilation des sources vers $Output..."
& g++ @args
if ($LASTEXITCODE -ne 0) {
    Write-Error "Compilation échouée (code $LASTEXITCODE)"
    exit $LASTEXITCODE
}

Write-Host "Compilation réussie : $Output"

if ($Deploy) {
    if (-not $Global:QtPrefix) { Write-Error "Qt not detected under C:\\Qt, cannot deploy runtime"; exit 1 }
    $qtBin = Join-Path $Global:QtPrefix 'bin'
    $target = (Resolve-Path .\build).Path
    if (-not (Test-Path $target)) { New-Item -ItemType Directory -Path $target -Force | Out-Null }

    $windeploy = Join-Path $qtBin 'windeployqt.exe'
    if (Test-Path $windeploy) {
        Write-Host "Using windeployqt to deploy runtime into $target"
        & "$windeploy" --no-compiler-runtime --dir $target "$target\CppTD_Qt.exe"
        if ($LASTEXITCODE -ne 0) { Write-Error "windeployqt failed (code $LASTEXITCODE)"; exit $LASTEXITCODE }
    } else {
        Write-Host "windeployqt not found; copying Qt DLLs and plugins manually"
        $dlls = Get-ChildItem $qtBin -Filter 'Qt6*.dll' -File -ErrorAction SilentlyContinue
        foreach ($d in $dlls) { Copy-Item $d.FullName -Destination $target -Force }

        $pluginsRoot = Join-Path $Global:QtPrefix 'plugins'
        if (Test-Path $pluginsRoot) {
            Get-ChildItem $pluginsRoot -Directory | ForEach-Object {
                $dst = Join-Path $target $_.Name
                if (-not (Test-Path $dst)) { New-Item -ItemType Directory -Path $dst -Force | Out-Null }
                Copy-Item $_.FullName -Destination $dst -Recurse -Force
            }
        }
    }

    Write-Host "Deployment complete. You can run .\build\CppTD_Qt.exe (in the project root) or .\build\CppTD_Qt.exe (from build folder)"
}
