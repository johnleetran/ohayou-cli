# Build
## Deps
vcpkg - https://github.com/microsoft/vcpkg
```
git clone --branch master --single-branch https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install harfbuzz
./vcpkg install opencv4[contrib]
```

## MAC

### libgsf
```
brew install libgsf
```

### openmp - https://www.mathworks.com/help/coder/ug/install-openmp-library-on-macos-platform.html
```
 curl -O https://github.com/llvm/llvm-project/releases/download/llvmorg-11.1.0/openmp-11.1.0.src.tar.xz -L
gunzip -c openmp-11.1.0.src.tar.xz | tar xv
cd openmp-11.1.0.src
mkdir build
cmake ..
make -j4
make install
```
### liVIPS
https://libvips.github.io/libvips/API/current/using-from-cpp.html
```
cur -O https://github.com/libvips/libvips/releases/download/v8.10.6/vips-8.10.6.tar.gz -F
gunzip -c vips-8.10.6.tar.gz | tar xv
cd vips-8.10.6
export CXX=/usr/local/opt/llvm/bin/clang++
export CC=/usr/local/opt/llvm/bin/clang
export CPPFLAGS="-I/usr/local/opt/llvm/include -fopenmp"
export LDFLAGS="-L/usr/local/opt/llvm/lib"
./configure 
make -j4
make install

#example to link with vips "g++ -g -Wall example.cc `pkg-config vips-cpp --cflags --libs`"
```

# Super resolution models can be found here

## models
https://github.com/Saafke/EDSR_Tensorflow/tree/master/models
https://github.com/fannymonori/TF-ESPCN/tree/master/export
https://github.com/Saafke/FSRCNN_Tensorflow/tree/master/models
https://github.com/fannymonori/TF-LapSRN/tree/master/export

## tutorils
https://learnopencv.com/super-resolution-in-opencv/
https://towardsdatascience.com/deep-learning-based-super-resolution-with-opencv-4fd736678066

```
ln -s /usr/local/Cellar/harfbuzz/2.8.0  /usr/local/Cellar/harfbuzz/2.7.4
mkdir build
cd build
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=/Users/john/Documents/code/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

## TODO
* cartooning
* emboss
* pencil sketch
* some special effects filter like wind, pinch, or spin
* refactor ui that adjusts HSV with sliders 
* libvips integration - create prymaid for zooming
* how to stream video from a docker container
* seam carving
* content aware fill using seam carving
* intergrate eigen lib

docker run -it --rm --device=/dev/video0 -e DISPLAY=$DISPLAY -v $XSOCK:$XSOCK -v $XAUTH:$XAUTH -e XAUTHORITY=$XAUTH

docker run --device=/dev/video0:/dev/video0 -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -p 5000:5000 -p 8888:8888 -it ohayou /bin/bash -l