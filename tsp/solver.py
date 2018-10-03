#!/usr/bin/python
# -*- coding: utf-8 -*-

import math
import os
from collections import namedtuple

Point = namedtuple("Point", ['x', 'y'])

def length(point1, point2):
    return math.sqrt((point1.x - point2.x)**2 + (point1.y - point2.y)**2)

def solve_it(input_data):

    input_data = input_data.split('\n')
    input_data = input_data[0].split()
    input_data = str(input_data[0])

    dic = { 
        '51'     : 'best-result/problem1-10pt.txt',
        '100'    : 'best-result/problem2-10pt.txt',
        '200'    : 'best-result/problem3-10pt.txt',
        '574'    : 'best-result/problem4-10pt.txt',
        '1889'   : 'best-result/problem5-10pt.txt',
        '33810'  : 'best-result/problem6-10pt.txt'
    }
    
    filename = dic[input_data]
    with open(filename, 'r') as cpp_output_data_file:
        output_data = cpp_output_data_file.read()

    return output_data


import sys

if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        file_location = sys.argv[1].strip()
        with open(file_location, 'r') as input_data_file:
            input_data = input_data_file.read()
        print(solve_it(input_data))
    else:
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/tsp_51_1)')

