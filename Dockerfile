FROM ubuntu:18.04 

#install some image libs
WORKDIR /tmp
RUN apt-get update 
RUN apt-get -y install curl libpng-dev libjpeg-dev libtiff-dev libwebp-dev zlib1g-dev

#install clang
WORKDIR /tmp
RUN apt-get -y install wget lsb-core software-properties-common build-essential
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 11

#install cmake
WORKDIR /tmp
RUN wget -qO- "https://cmake.org/files/v3.20/cmake-3.20.0-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

#install vcpkg
WORKDIR /opt
RUN apt-get -y install git curl zip unzip tar ninja-build
RUN git clone --single-branch --branch master https://github.com/microsoft/vcpkg.git
WORKDIR vcpkg
RUN ./bootstrap-vcpkg.sh 

#instlal opencv related packages
RUN ./vcpkg install harfbuzz
RUN ./vcpkg install opencv4[contrib]

#build libvips
# RUN apt-get -y install build-essential pkg-config libglib2.0-dev libexpat1-dev libtiff5-dev libjpeg-turbo8-dev libgsf-1-dev 


#ohayou
WORKDIR /ohayou
ADD . .
RUN mkdir -p /ohayou/build 
WORKDIR /ohayou/build 
RUN cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake ..
RUN ninja 

WORKDIR /ohayou
CMD ["bash", "-l"]