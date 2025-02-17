# GRID

## Setup environment

```bash
git submodule update --init
# Configure and build libastro
cmake -DCMAKE_BUILD_TYPE=Release -S 3rd/ephem -B 3rd/ephem/build && cmake --build 3rd/ephem/build --parallel
```

## Build

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel
cd ..
```
