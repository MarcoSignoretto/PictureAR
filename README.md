
# GENERAL #

All code is commented in order to explain how it work and why I have done some choice instead of others

### Project Author ###
Marco Signoretto 850484

### GOALS ###
I have tried to write code efficient and reusable.
Definition and declaration are splitted into different files, so ```CMakeLists.txt``` has been edited as well 

### File ###
In the project there is following file:

1. ```speed.txt``` that contains all speed observation of the video

### Image ###

1. ```acceleration.png``` that contain the graph of the acceleration of the car

### Video ###

1. ```output.mp4``` is original video with the line of th pointer are drawn

WARNING: the algorithm produce the video file ONLY if not already present.

## Generate plot ##
In order to generate plot I have done the following: 

1. Import ```speed.txt``` has data and rename as ```speed```
2. Execute the following commands in MATLAB:
```MATLAB
    time = 1:0.033:60.4;
    plot(time,speed)
    xlabel('time (sec)')
    ylabel('speed (Km/h)')
    title('BMW M5 F80 acceleration graph')
```

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

## How to run: ##
```sh
cd build
make run
```


or, alternatively:

```sh
make install
cd dist/bin
invoke the generated executable
```

--------------------------------------------------------------------------------
NOTE: 
Before submitting any assignment, remember to delete both the generated 
"build" and "dist" directories
--------------------------------------------------------------------------------




