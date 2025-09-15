hopefully this turns into something cool before i start uni

# BUILDING FROM SOURCE

```bash
git clone https://github.com/sl4Jd/game_engine.git
```
```bash
cd game_engine
```
## BUILD FOR VISUAL STUDIO
```bash
cmake -B build -S .
```
```bash
cmake --build build
```
## BUILD WITH MSVC AND VCPKG
if you like vcpkg but use some text editor and dont want visual studio bullshit  
i dont recommend using gcc compiler as vcpkg's x64-mingw triplets are quite unreliable  
all commands must be in developer cmd for visual studio
```bash
cmake --preset vcpkg
```
### Build app
```bash
cmake --build build
```
you shoud now have project.exe in build/bin directory
