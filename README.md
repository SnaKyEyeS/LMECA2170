# LMECA2170 - Fortune's algorithm

## How to use 

Simply execute the file `Devillez_Poncelet_Fortunes` and it will show our implementation of fortune algorithm with 20 points and a sweep animation. Some options are available. You can have the following list with `Devillez_Poncelet_Fortunes -h`:

```
-b, --benchmark [n]: output the time taken to apply the algorithm on the [n] points
-a, --animation [mode]: specify the level of animation
 * 0: No animation 
 * 1: Manual mode 
 * 2: Step mode 
 * 3: Sweep mode 
-h, --help: Print this help
-r, --resolution [n]: specify the max number of points [n] to draw the beachline
-p, --points [n]: specify the number of points [n] to use (default: 20) 
-s, --save: specify to save the animation
-f, --fps [n]: specify the number of fps to save (default: 10)
-v, --version: Print the version number
```

### --benchmark
Will apply the fortune algorithm on [n] points and return the time taken to initialise the structures and to apply the algorithm in microseconds.

If other flags are specified with -b, they will not be taken into account.

### Animation
There are 4 mode of display:
- No animation: it will directly output the voronoid
- Manual: you can move the sweep line step by step or event by event
- Step mode: the sweep line will move event by event
- Sweep mode: the sweep line will move at constant speed

## Shortcuts
Shortcuts are design for a AZERTY keyboard but the are mapped to the physical key (so on a QWERTY press Q instead of A for example)


## Disclaimer
Two function of BOV were overwrited:
- The shortcut function
- The help function

## Reference 
- Computanional Geometry book for the main idea of the algorithm
- Course's slides
- [Henri Devillez implementation](https://github.com/hdevillez/VoronoiVisualization): to check the implementation of the sorted queue
- [Steven Fortune implentation](https://9p.io/who/sjf/voronoi.tar): To compare how the best implentation was done


## V0

![V0 gif](Vid/V0.gif)

## V1

![V1 gif](Vid/V1.gif)


## Todo
### V0 - It's done
* [x] Have a working Algo -> I think it's ok
* [x] Have a basic plot
* [x] Free everything correctly
* [x] Benchmark the V0
* [x] Should be able to select random input of n points
* [x] Take an gif of the working algorithm to see the evolution and add it to the git

### V1
* [x] Clean the code
  * [x] Better use of functions
  * [x] Better name for functions
  * [x] Add comment to the code
* [x] Improve the speed 
  * [x] Improve the heap
  * [x] Improve processSite
  * [x] Improve processCircle
  * [x] Improve circle creation
  * [x] Benchmark after the upgrade 
  * [x] Tool to check function call and function duration (min, max, average, total) ?
  * [x] Rebalance tree (maybe variable with depth ?) and benchmark -> Would be very nice to do :3 -> partially done
* [ ] Add a verbose environnement
  * [ ] Add general option for verbose
  * [ ] Add relevant print
* [x] Check if memory is clean (valgrind)
* [x] Add nice tool for visual
  * [x] Each element shoulde be toggable
  * [x] Help should gives shortcut
  * [x] Speed of sweep line should be variable
  * [x] Sweep line should be able to stop
  * [x] manual increase for sweep line should be ok
  * [x] Should be able to adjuste resolution of lines (beachLine, cicle) (during ?)
  * [x] should be able to switch between different animation type
* [ ] Add nice visual
  * [x] Sweepline
  * [x] Beachline
  * [x] Site event
  * [ ] Circle Event (differentiate the valid one from the others) (and keep memory of those events)
  * [ ] Circle from circle event (dashed)
  * [x] Next Event should be highlighted
  * [x] Finite vertex
  * [x] Vertex in construction 
  * [x] semi-finite vertex
  * [x] Bound everything to a box
  * [x] Vertex of the He
  * [ ] Finish animation when all bpx are outside the box (add flag ?)
* [ ] Clean the data
  * [ ] verify if when add points
* [x] Flags
  * [x] Add a flag to save the animations 
  * [x] Flag for listing the help

### V2
* [x] Add -O3 to make
  * [x] Benchmark this 
  * [x] Test othe flags and benchmark them -> No need
* [ ] Add a realtime ploting execution time / data-size
  * [ ] Should be able to plot with different color different option and choose which one to plot. (Data should be the same).
* [x] Add tool to save anim to gif/save pictures
* [ ] Should be able to go back in time during animation
* [ ] Should be able to add point (and auto update)
* [ ] Process images
  * [ ] Basic input with pictures http://phrogz.net/js/image-voronoi/
  * [ ] Should process any image in input


## Installation
```
mkdir build
cd build
cmake ..
make
bin/Devillez_Poncelet_Fortune
```
