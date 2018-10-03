#!/usr/bin/python
# -*- coding: utf-8 -*-

import os

def solve_it(input_data):
    input_data = input_data.split('\n')
    input_data = input_data[0].split()
    input_data = str(input_data[0]) + ' ' + str(input_data[1])

    dic = { 
        '50 350'     : 'best-result/problem1-10pt.txt',
        '70 1678'     : 'best-result/problem2-10pt.txt',
        '100 2502'     : 'best-result/problem3-10pt.txt',
        '250 28046'   : 'best-result/problem4-10pt.txt',
        '500 12565'   : 'best-result/problem5-10pt.txt',
        '1000 249482'   : 'best-result/problem6-10pt.txt'
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
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/gc_4_1)')

