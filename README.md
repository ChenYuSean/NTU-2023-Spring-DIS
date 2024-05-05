
## Build
```bash
cd ./pbrt-v4

cmake -S ./ -B ./build/ -DCMAKE_BUILD_TYPE=Debug

make -C build/ -j8
```

## Execute
```bash
./build/pbrt --outfile sphere.png --write-partial-images ../scene/sphere.pbrt

./build/pbrt --outfile teapot.png --write-partial-images ../scene/teapot/scene-v4.pbrt
```

### Ref
[pbrt, Version 4](https://github.com/mmp/pbrt-v4)
[pbrt guide](https://pbrt.org/users-guide-v4) 
[ubuntu22.04编译PBRT-v4](https://blog.csdn.net/jinshelj/article/details/126458035)
