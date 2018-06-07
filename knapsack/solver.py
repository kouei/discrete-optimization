#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import time
import os
from collections import namedtuple
Item = namedtuple("Item", ['index', 'value', 'weight'])

def solve_it(input_data):
    # Modify this code to run your optimization algorithm
    
    with open('cpp_input.txt', mode='w') as f:
        for line in input_data:
            f.write(line)
            
    os.system("./main")
    
    with open('cpp_output.txt', mode='r') as f:
        line = f.readline()
        line = line[:-1]
        value = int(line)
        line = f.readline()
        line = line[:-1]
        line = line.split(' ')
        taken = [int(num) for num in line]
    
    # prepare the solution in the specified output format
    
    output_data = str(value) + ' ' + str(1) + '\n'
    output_data += ' '.join(map(str, taken))
    return output_data
    

if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        file_location = sys.argv[1].strip()
        with open(file_location, 'r') as input_data_file:
            input_data = input_data_file.read()
        print(solve_it(input_data))
    else:
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/ks_4_0)')

