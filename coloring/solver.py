#!/usr/bin/python
# -*- coding: utf-8 -*-

import os

def solve_it(input_data):
    # Modify this code to run your optimization algorithm
    
    # parse the input
    lines = input_data.split('\n')

    first_line = lines[0].split()
    node_count = int(first_line[0])
    edge_count = int(first_line[1])

    edges = []
    for i in range(1, edge_count + 1):
        line = lines[i]
        parts = line.split()
        edges.append((int(parts[0]), int(parts[1])))
    
    with open('python_input.txt', mode='w') as f:
        line = str(node_count) + ' ' + str(edge_count) + '\n'
        f.write(line)
        for a, b in edges:
            line = str(a) + ' ' + str(b) + '\n'
            f.write(line)
    
    os.system("./main")
    
    with open('cpp_output.txt', mode='r') as f:
        line = f.readline()
        line = line[:-1]
        color_count = int(line)
        line = f.readline()
        line = line[:-1]
        line = line.split(' ')
        solution = [int(num) for num in line]
    
    # prepare the solution in the specified output format
    output_data = str(color_count) + ' ' + str(0) + '\n'
    output_data += ' '.join(map(str, solution))

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
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/gc_4_1)')

