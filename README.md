hopefully this turns into something cool before i start uni

# BUILDING FROM SOURCE

```bash
git clone https://github.com/sl4Jd/multiscale_physics_sandbox.git
```
```bash
cd multiscale_physics_sandbox
```
## BUILD DEPS

### BUILD FOR VISUAL STUDIO
be sure to have VCPKG_ROOT as environmental variable or put full path in CMakePresets.json file 
```bash
cmake -B build -S .
```
```bash
cmake --build build
```
### BUILD WITH MSVC AND VCPKG
if you like vcpkg but use some text editor and dont want visual studio bullshit  
i dont recommend using gcc compiler as vcpkg's x64-mingw triplets are quite unreliable  
all commands must be in developer cmd for visual studio  
be sure to have VCPKG_ROOT as environmental variable or put full path in CMakePresets.json file 
```bash
cmake --preset vcpkg
```
## BUILD APP
```bash
cmake --build build
```
you shoud now have multi-scale-sandbox.exe in build/bin directory
