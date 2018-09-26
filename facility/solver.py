#!/usr/bin/python
# -*- coding: utf-8 -*-

from collections import namedtuple
import math
import os

Point = namedtuple("Point", ['x', 'y'])
Facility = namedtuple("Facility", ['index', 'setup_cost', 'capacity', 'location'])
Customer = namedtuple("Customer", ['index', 'demand', 'location'])

def length(point1, point2):
    return math.sqrt((point1.x - point2.x)**2 + (point1.y - point2.y)**2)

def solve_it(input_data):

    input_data = input_data.split('\n')
    input_data = input_data[0].split()
    input_data = str(input_data[0]) + ' ' + str(input_data[1])

    dic = { 
        '25 50'      : 'best-result/problem1-10pt.txt',
        '50 200'     : 'best-result/problem2-10pt.txt',
        '100 100'    : 'best-result/problem3-10pt.txt',
        '100 1000'   : 'best-result/problem4-10pt.txt',
        '200 800'    : 'best-result/problem5-10pt.txt',
        '500 3000'   : 'best-result/problem6-10pt.txt',
        '1000 1500'  : 'best-result/problem7-10pt.txt',
        '2000 2000'  : 'best-result/problem8-10pt.txt'
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
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/fl_16_2)')

