## Graph Coloring

![graph-coloring](./graph-coloring.png)

Week 3 Assignment of the Coursera Course [Discrete Optimization](https://www.coursera.org/learn/discrete-optimization/home/info).  
This code can get full score **60/60** for the assignment.  
The implementation is written in C++17. To run the program you need to compile `main.cpp`, here I use clang as my compiler:

`$ clang++ main.cpp -o main -std=c++17 -O2 -Wall`

This will generate executable file `main`, then if you want to solve problem `gc_50_3` you can type:

`$ ./main data/gc_50_3`

The solution will be written to `cpp_output.txt`, you can copy its content to the corresponding file in folder `best-result`.

To submit, simply type:

`$ python submit.py`

`submit.py` will read result from folder `best-result`.

<br/>
<br/>

### Optimization Strategy

* Local Search
* Tabu Search
* Restart

<br/>
<br/>

### Visualization
#### Graph Coloring - 100 Nodes
![coloring-visualization](../visualization/coloring-visualization.png)