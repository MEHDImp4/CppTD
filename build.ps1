
param(
    [string]$Output = "src\output\weather.exe",
    [string]$Std = "c++17"
)


$OutDir = Split-Path -Parent $Output
if (-not (Test-Path $OutDir)) {
    New-Item -ItemType Directory -Path $OutDir -Force | Out-Null
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
