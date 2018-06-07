#!/usr/bin/python
# -*- coding: utf-8 -*-

import math
import os
from collections import namedtuple

Point = namedtuple("Point", ['x', 'y'])

def length(point1, point2):
    return math.sqrt((point1.x - point2.x)**2 + (point1.y - point2.y)**2)

def solve_it(input_data):
    # Modify this code to run your optimization algorithm

    # parse the input
    lines = input_data.split('\n')

    nodeCount = int(lines[0])

    points = []
    for i in range(1, nodeCount+1):
        line = lines[i]
        parts = line.split()
        points.append(Point(float(parts[0]), float(parts[1])))

        
    with open('cpp_input.txt', mode='w') as f:
        f.write(input_data)
    
    os.system("./main")
    
    distance = None
    tour = None
    with open('cpp_output.txt', mode='r') as f:
        line = f.readline()
        line = line[:-1]
        distance = line
        line = f.readline()
        tour = line
        
    # prepare the solution in the specified output format
    output_data = distance + ' ' + str(0) + '\n'
    output_data += tour

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

