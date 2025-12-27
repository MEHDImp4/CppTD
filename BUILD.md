# Building CppTD_Qt

## Prerequisites
- Qt installed (example: `C:\Qt\6.10.1\mingw_64`)
- `cmake` on PATH
- Matching compiler for the Qt kit (for the example above use MSYS2/MinGW)
- PowerShell (Windows)

## Quick (recommended)
Use the repo helper script which detects Qt and configures CMake:

```powershell
# Configure + build
.\build.ps1 -UseCMake

# Build and run (adds Qt bin to PATH for this run)
.\build.ps1 -UseCMake -Run

# Build and create a portable folder (copies required Qt DLLs/plugins into build/)
.\build.ps1 -UseCMake -Deploy
```

## Manual CMake workflow
Remove old cache if switching generator/kit:

```powershell
Remove-Item -Recurse -Force build
```

Configure (example forcing MinGW generator for a MinGW Qt kit):

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DQt6_DIR="C:/Qt/6.10.1/mingw_64/lib/cmake/Qt6"
```

Build:

```powershell
cmake --build build --config Release
```

Run:

```powershell
# from project root
.\build\CppTD_Qt.exe
# or
cd build
.\CppTD_Qt.exe
```

## Using MSYS2 (MinGW64) — recommended for MinGW Qt
If your Qt installation is the MinGW build (example: `C:\Qt\6.10.1\mingw_64`), the simplest, most reliable way to build is inside the MSYS2 "MinGW 64-bit" shell so the MinGW toolchain and runtime are available.

1. Install MSYS2 (https://www.msys2.org/) and open the **MSYS2 MinGW 64-bit** shell from the Start Menu.
2. Update the system and install the toolchain/helpers:

```bash
# Update packages (you may need to close/reopen the shell after the first command)
pacman -Syu
# After reopening the MinGW64 shell, run:
pacman -Su

# Install required build tools and CMake (and Qt via pacman if you want, but not required
# if you already installed Qt from the Qt installer into C:\Qt)
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake
```

3. Configure and build the project from the MSYS2 MinGW64 shell (adjust Qt path as needed):

```bash
cd /c/Users/mehdi/Documents/GitHub/CppTD
cmake -S . -B build -G "MinGW Makefiles" -DQt6_DIR="C:/Qt/6.10.1/mingw_64/lib/cmake/Qt6"
cmake --build build --config Release -j $(nproc)
```

4. Deploy or run the built binary (use `windeployqt` to assemble a portable folder):

```bash
# From MSYS2 or PowerShell, run windeployqt from your Qt bin folder (example):
"C:/Qt/6.10.1/mingw_64/bin/windeployqt.exe" --no-compiler-runtime --dir build build/CppTD_Qt.exe
./build/CppTD_Qt.exe
```

Notes:
- If you installed Qt with the official Qt installer (to `C:\Qt`), you do not need to install `qt6` via pacman — just ensure the MSYS2 MinGW toolchain is installed and that you run CMake from the MSYS2 MinGW64 shell so the compiler matches the Qt kit.
- If you switch between MSVC and MinGW generators, remove the `build/` folder before reconfiguring to avoid generator conflicts:

```bash
rm -rf build
```

## Notes
- If you get a runtime error about missing `Qt6Widgets.dll` (or similar), use the `-Deploy` option or run with Qt's `bin` directory on PATH (the helper `-Run` does that).
- The helper script attempts to detect a Qt install under `C:\Qt` and passes `Qt6_DIR` or `CMAKE_PREFIX_PATH` to CMake.
- When switching between MSVC and MinGW Qt kits, remove the `build/` folder before reconfiguring to avoid generator conflicts.

If you'd like, I can also add a small `README.md` section pointing to this file or zip the `build/` folder for distribution.