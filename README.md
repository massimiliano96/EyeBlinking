# Requirements
- Opencv (version > 4.x)
    ```
    sudo apt install libopencv-dev python3-opencv
    ```
- Cmake (version > 3.22.x)
    ```
    sudo apt install cmake
    ```
- dlib (version > 19.24.x)
    ```
    wget http://dlib.net/files/dlib-19.24.tar.bz2
    tar xvf dlib-19.24.tar.bz2
    cd dlib-19.24/
    mkdir build
    cd build
    cmake ..
    sudo cmake --build . --target install
    sudo make install
    sudo ldconfig
    ```
- gTest (version > 1.11.x)
    ```
    sudo apt-get install libgtest-dev
    cd /usr/src/gtest
    sudo make
    sudo cp lib/*.a /usr/lib
    ```

## WORK IN PROGRESS