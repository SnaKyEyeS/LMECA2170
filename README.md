# LMECA2170

## Todo
### V0
* [x] Have a working Algo -> I think it's ok
* [x] Have a basic plot
* [x] Free everything correctly
* [ ] Benchmark the V0
* [ ] Should be select random input of n points
* [ ] Take an gif of the working algorithm to see the evolution and add it to the git

### V1
* [ ] Clean the code
  * [ ] Better use of functions
  * [ ] Better name for functions
  * [ ] Add comment to the code
* [ ] Improve the speed 
  * [ ] Improve processSite
  * [ ] Improve processCircle
  * [ ] Improve circle creation
  * [ ] Benchmark after the upgrade 
  * [ ] Tool to check function call and function duration (min, max, average, total) ?
  * [ ] Rebalance tree (maybe variable with depth ?) and benchmark -> Would be very nice to do :3
* [ ] Add a verbose environnement
  * [ ] Add general option for verbose
  * [ ] Add relevant print
* [ ] Check if memory is clean (valgrind)
* [ ] Add nice tool for visual
  * [ ] Each element shoulde be toggable
  * [ ] Help should with shortcut
  * [ ] Speed of sweep line should be variable
  * [ ] Sweep line should be able to stop
  * [ ] manual increase for sweep line should be ok
  * [ ] Should be able to adjuste resolution of lines (beachLine, cicle) (during ?)
* [ ] Add nice visual
  * [ ] Sweepline
  * [ ] Beachline
  * [ ] Site event
  * [ ] Circle Event
  * [ ] Circle from circle event (dashed)
  * [ ] Next Event should be highlighted
  * [ ] Finite vertex
  * [ ] Vertex in construction 
  * [ ] semi-finite vertex

### V2
* [ ] Add -O3 to make
  * [ ] Benchmark this 
  * [ ] Test othe flags and benchmark them
* [ ] Add a realtime ploting execution time / data-size
  * [ ] Should be able to plot with different color different option and choose which one to plot. (Data should be the same).
* [ ] Add tool to save anim to gif/save pictures
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
bin/myexec
```
