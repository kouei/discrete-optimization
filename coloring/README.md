## Graph Coloring

![graph-coloring](./graph-coloring.png)

Week 3 Assignment of the Coursera Course [Discrete Optimization](https://www.coursera.org/learn/discrete-optimization/home/info)  
This code can get highest score for the assignment **60/60**  
The implementation is written in C++, the `solver.py` will feed data into the C++ program and
get result back from it.  
To run the program you need to compile `main.cpp`, here I use clang as my compiler

`$ clang++ main.cpp -o main -std=c++11 -O2 -Wall`

This will generate executable file `main`, then you can run `submit.py` to submit the result

## Optimization Strategy

* Local Search
* Tabu Search
* Restart
