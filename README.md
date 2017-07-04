
# GENERAL #

All code is commented in order to explain how it work and why I have done some choice instead of others

### Project Author ###
Marco Signoretto 850484

### GOALS ###
I have tried to write code efficient and reusable and group it into file with different goals.
Definition and declaration are splitted into different files, so ```CMakeLists.txt``` has been edited as well 

### File ###
In the project there is the original ```data``` folder with all the files used by the progrma and the files used to test program:

# Setup #
The code must be build and setup has default template for this course as the following:

## How to build: ##

```sh
mkdir build
cd build
cmake ../ -DOpenCV_DIR="<insert the path of your opencv/build directory>"
make install
```

for example mine is 

```sh
cmake  ../ -DOpenCV_DIR="/usr/local/Cellar/opencv3/HEAD-c1007c7" 
```

//TODO -DCMAKE_BUILD_TYPE=Release vedere come aggiungere

## How to run: ##
In order to run the project you need to perform the following commands:
```sh
make install
cd dist/bin
```
and then invoke the program with some arguments, for example:
```sh
//TODO COMPLETE
```

-DCMAKE_BUILD_TYPE=Release
#### Program arguments: ###
1. ```-D``` or ```--Debug``` enable debug info, in this way we can observe how boundaries has been detected and other debug info (to have fast execution you shouldn't add it)

2. ```-s``` or ```--source``` this argument required an additional parameter which is the source of the frame. The possible sourse are: ```image```,```video```,```webcam``` the last one is the defualt source.

3. ```-f``` or ```--filename``` this argument must be present if the "source" is  ```image``` or ```video``` and must be following by the filename of the source.

4. ```-c``` or ```--camera``` this argument allow to specify the camera number ( default is 0 which is the main camera ) for ```webcam``` source.

--------------------------------------------------------------------------------
NOTE: 
Before submitting any assignment, remember to delete both the generated 
"build" and "dist" directories
--------------------------------------------------------------------------------




