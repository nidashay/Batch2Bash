# Batch2Bash

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C11-00599C.svg?logo=c&logoColor=white)](#)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey.svg)](#)

A high-performance, lightweight transpiler written in C for converting Windows Batch scripts (`.bat`) to POSIX-compliant Bash scripts (`.sh`).

## Overview

**Batch2Bash** is a dedicated utility designed to streamline the migration of automation workflows from Windows environments to Linux and other POSIX-compatible systems. By performing intelligent lexical translation of commands, environment variables, and control flow structures, it minimizes the manual effort required to port legacy Batch scripts.

## Key Features

- **Comprehensive Command Mapping**: Direct translation of Batch primitives to their coreutils counterparts:
  - `del` &rarr; `rm`
  - `copy` &rarr; `cp`
  - `move` &rarr; `mv`
  - `md`/`mkdir` &rarr; `mkdir -p`
  - `rd`/`rmdir` &rarr; `rmdir`
  - `cls` &rarr; `clear`
- **Intelligent Variable Substitution**:
  - Global variable conversion: `%VAR%` &rarr; `$VAR`
  - Positional argument support: `%1`–`%9` &rarr; `$1`–`$9`
  - Context-aware dynamic variables: `%CD%` &rarr; `$PWD`, `%~dp0` &rarr; `$(dirname "$0")`
- **Robust Conditional Logic**:
  - Filesystem checks: `if exist` and `if not exist`.
  - String evaluation: Support for standard `if "a"=="b"` equality checks.
- **Environment Management**: Accurate translation of `set` assignments with automatic POSIX-style quoting.
- **Path Normalization**: Heuristic-based conversion of Windows backslashes (`\`) to POSIX forward slashes (`/`) for improved compatibility.

## Build Requirements

To build Batch2Bash from source, ensure the following dependencies are installed:

- **Compiler**: GCC, Clang, or any C11-compliant compiler.
- **Build System**: [CMake](https://cmake.org/) 3.10 or higher.
- **Build Tool**: `make` (for Linux) or `ninja`/`mingw32-make` (for Windows).

## Installation

### Linux / POSIX

```bash
git clone https://github.com/nidashay/Batch2Bash.git
cd Batch2Bash
mkdir build && cd build
cmake ..
make
```

### Windows (MinGW-w64 / MSYS2)

```powershell
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

## Usage

The `bat2sh` executable requires the source script path and the desired output path as arguments:

```bash
./bat2sh <source_path.bat> <output_path.sh>
```

### Example

**Input (`script.bat`):**
```batch
@echo off
set PROJECT=Batch2Bash
echo Building %PROJECT%...
if not exist build mkdir build
```

**Output (`script.sh`):**
```bash
#!/bin/bash

PROJECT="Batch2Bash"
echo Building $PROJECT...
if [ ! -e "build" ]; then mkdir -p build; fi
```

## Project Structure

```text
Batch2Bash/
├── include/       # Public header files
├── src/           # Implementation source code
├── CMakeLists.txt # Build configuration
├── LICENSE        # GPL v3.0 License
└── README.md      # Project documentation
```

## Contributing

We welcome contributions of all kinds! Whether you are fixing bugs, adding new command mappings, or improving documentation:

1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

## Author

**Clinton Gethi** - [GitHub Profile](https://github.com/nidashay)

## License

This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for full details.
