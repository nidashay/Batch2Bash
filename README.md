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
cmake -G "MinGWMakefiles" -S .. -B .
cmake --build .
```

### AUTHOR

**Clinton Gethi**

### LICENSE

[GPL 3.0]("LICENSE")

