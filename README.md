## Batch2Bash
* A simple tool that converts .bat scripts to .sh

### How to build

**REQUIREMENTS**
- GNU C COMPILER (gcc)
- CMake
- Make (Linux)


* Run the following commands

**LINUX**

```bash
mkdir build
cd build
cmake ..
make 

```

**WINDOWS**

```powershell
mkdir build
cd build
cmake -G "MinGW Makefiles" -S .. -B .
cmake --build .
```
### Usage

```bash
./bat2sh <path/to/bat/file> <path/to/existing/sh/file> #If null, it will create a new one
```

### Project Structure

```md

Batch2Bash/
|
|_.git/
|_include/
|__|parser.h
|_src/
|__|main.c
|__|parser.c
|_.gitignore
|_CMakeLists.txt
|_LICENSE
|_README.md

```

<br>

### AUTHOR

*Clinton Gethi*

### LICENSE

[GPL 3.0](LICENSE)

