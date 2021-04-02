```
mkdir build
cd build
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=/Users/john/Documents/code/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

## TODO
* cartooning
* emboss
* pencil sketch
* some special effects filter like wind, pinch, or spin
* cli option 
* maybe create a ui that can adjust HSV 
* libvips integration - create prymaid for zooming
* dockerfile
* how to stream video from a docker container

docker run -it --rm --device=/dev/video0 -e DISPLAY=$DISPLAY -v $XSOCK:$XSOCK -v $XAUTH:$XAUTH -e XAUTHORITY=$XAUTH

docker run --device=/dev/video0:/dev/video0 -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -p 5000:5000 -p 8888:8888 -it ohayou /bin/bash -l