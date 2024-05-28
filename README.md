
## Build
```bash
cmake -S ./pbrt-v4 -B ./pbrt-v4/build/ -DCMAKE_BUILD_TYPE=Debug

make -C ./pbrt-v4/build/ -j8
```

## Execute
```bash
./pbrt-v4/build/pbrt --outfile sphere.png ./scene/sphere.pbrt

./pbrt-v4/build/pbrt --write-partial-images ./scene/teapot/scene-v4.pbrt

./pbrt-v4/build/pbrt ./scene/pbrt-v4-scenes-master/pbrt-book/book.pbrt

./pbrt-v4/build/pbrt ./scene/veach-mis/scene-v4.pbrt
```

### Options Arguments

- `--outfile`: saved filename
- `--write-partial-images`: output during rendering
- `--log-level verbose`: show all logging text 

### Ref
[pbrt, Version 4](https://github.com/mmp/pbrt-v4)  
[pbrt guide](https://pbrt.org/users-guide-v4)   
[ubuntu22.04编译PBRT-v4](https://blog.csdn.net/jinshelj/article/details/126458035)  
