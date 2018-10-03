## Vehicle Routing

![vehicle-routing](./vehicle-routing.png)

Week 7 Assignment of the Coursera Course [Discrete Optimization](https://www.coursera.org/learn/discrete-optimization/home/info).  
This code can get full score **60/60** for the assignment.  
The implementation is written in C++17. To run the program you need to compile `main.cpp`, here I use clang as my compiler:

`$ clang++ main.cpp -o main -std=c++17 -O2 -Wall`

This will generate executable file `main`, then if you want to solve problem `vrp_16_3_1` you can type:

`$ ./main data/vrp_16_3_1`

The solution will be written to `cpp_output.txt`, you can copy its content to the corresponding file in folder `best-result`.

To submit, simply type:

`$ python submit.py`

`submit.py` will read result from folder `best-result`.

<br/>
<br/>

### Optimization Strategy

* 2-opt
* Relocate
* Exchange
* Cross
* Guided Local Search

<br/>
<br/>

### Visualization
#### Vehicle Routing - 421 Customers / 41 Vehicles
![vehicle-routing-visualization](../visualization/vehicle-routing-visualization.png)