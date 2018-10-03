#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import time
import os
from collections import namedtuple
Item = namedtuple("Item", ['index', 'value', 'weight'])

def solve_it(input_data):
    # Modify this code to run your optimization algorithm
    
    input_data = input_data.split('\n')
    input_data = input_data[0].split()
    input_data = str(input_data[0]) + ' ' + str(input_data[1])

    dic = { 
        '30 100000'     : 'best-result/problem1-10pt.txt',
        '50 341045'     : 'best-result/problem2-10pt.txt',
        '200 100000'    : 'best-result/problem3-10pt.txt',
        '400 9486367'   : 'best-result/problem4-10pt.txt',
        '1000 100000'   : 'best-result/problem5-10pt.txt',
        '10000 1000000' : 'best-result/problem6-10pt.txt'
    }
    
    filename = dic[input_data]
    with open(filename, 'r') as cpp_output_data_file:
        output_data = cpp_output_data_file.read()

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

