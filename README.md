# MULTI-SCALE PHYSICS SANDBOX

hopefully this turns into something cool before i start uni

## CLONE

```bash
git clone https://github.com/sl4Jd/multiscale_physics_sandbox.git
```
```bash
cd multiscale_physics_sandbox
```
## BUILD FOR WINDOWS

### WITH MSVC 
must be in VS dev cmd
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```
```bash
cmake --build build
```
### WITH GCC
vcpkg triplets for gcc are much more unreliable so i recommend using conan here
```bash
conan install . --profile=default --output-folder=build --build=missing --settings build_type=Release
```
```bash
cd build  
```
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
```
```bash
cmake --build . --config Release 
```
## BUILD FOR LINUX
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```
```bash
cmake --build build
```
you shoud now have multi-scale-sandbox in build/bin directory
