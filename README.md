# Pictures AR #

I realized this project for the project of my Compute Vision course at Ca' Foscari university.

This project is a simple AR program that replace two markers with the pictures related to them.

In particular Van marker will be substituted with Van Gohg's picture and Leo with Leonardo's picture.

All code is commented in order to explain how it works and why I have done some choices instead of others

### Project Author ###
Marco Signoretto

### GOALS ###
I have tried to write code efficient and reusable and group it into multiple files with different goals.
Definition and declaration are splitted into different files, so ```CMakeLists.txt``` and ```src/CMakeLists.txt``` have been edited as well 

### File ###
In the project there is the original ```data``` folder provided by the teacher with all the files used by the program and the files used to test the program

# Setup #

## How to build: ##

```sh
mkdir build
cd build
cmake ../ -DOpenCV_DIR="<insert the path of your opencv/build directory>"
make install
```

for example in my computer is 

```sh
cmake  ../ -DOpenCV_DIR="/usr/local/Cellar/opencv3/3.2.0" 
```

## How to run: ##

### CMake target run ###
In order to run the project you need to perform the following commands:
```sh
cd build
make install
make run.....
```
The preconfigured target which has been created are:

#### Webcam ####
```sh
make run
make run_D
```

#### Video ####
```sh
make run_video1
make run_video1_D
make run_video2
make run_video2_D
```

#### Image ####
```sh
make run_image1
make run_image1_D
make run_image2
make run_image2_D
make run_image3
make run_image3_D
make run_image4
make run_image4_D
```

### Executable run ###

and then invoke the program with some arguments, for example:
```sh
./PictureAR list_of_args...
```

#### Program arguments available: ###
1. ```-D``` or ```--Debug``` enable debug info, in this way we can observe how boundaries have been detected and other debug info (to have fast execution you shouldn't add it)

2. ```-s``` or ```--source``` this argument require an additional parameter which is the source of the frame. The possible sources are: ```image```,```video```,```webcam``` the last one is the default source.

3. ```-f``` or ```--filename``` this argument must be present if the "source" is  ```image``` or ```video``` and it must be followed by the filename of the source.

4. ```-c``` or ```--camera``` this argument allows to specify the camera number ( default is 0 which is the main camera ) for ```webcam``` source.

