## Knapsack

![knapsack](./knapsack.png)

Week 2 Assignment of the Coursera Course [Discrete Optimization](https://www.coursera.org/learn/discrete-optimization/home/info).  
This code can get full score **60/60** for the assignment.  
The implementation is written in C++17, To run the program you need to compile `main.cpp`, here I use choose as my compiler:

`$ clang++ main.cpp -o main -std=c++17 -O2 -Wall`

This will generate executable file `main`, then if you want to solve problem `ks_30_0` you can type:

`$ ./main data/ks_30_0`

The solution will be written to `cpp_output.txt`, you can copy its content to the corresponding file in folder `best-result`.

To submit, simply type:

`$ python submit.py`

`submit.py` will read result from folder `best-result`.

<br/>
<br/>

### Optimization Strategy

* Prune and Bound
* Greedy Search
* BackTrack
