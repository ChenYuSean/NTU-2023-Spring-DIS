
## Build
```bash
cd ./pbrt-v4

cmake -S ./ -B ./build/ -DCMAKE_BUILD_TYPE=Debug

make -C build/ -j8
```

## Execute
```bash
cd ./pbrt-v4

./build/pbrt --outfile sphere.png --log-level verbose ../scene/sphere.pbrt

./build/pbrt --outfile sphere.png ../scene/pbrt-v4-scenes-master/pbrt-book/book.pbrt

./build/pbrt --outfile sphere.png --log-level verbose ../scene/teapot/scene-v4.pbrt

./build/pbrt --outfile sphere.png --log-level verbose ../scene/veach-mis/scene-v4.pbrt
```

### Command Options

- `--outfile`: saved filename
- `--write-partial-images`: output during rendering
- `--log-level verbose`: show all logging text 

### Ref
[pbrt, Version 4](https://github.com/mmp/pbrt-v4)  
[pbrt guide](https://pbrt.org/users-guide-v4)   
[ubuntu22.04编译PBRT-v4](https://blog.csdn.net/jinshelj/article/details/126458035)  
