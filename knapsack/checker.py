#!/usr/bin/python
# -*- coding: utf-8 -*-
from solver import solve
def check():
    dic = { 
        'data/ks_30_0'     : 'best-result/problem1-10pt.txt',
        'data/ks_50_0'     : 'best-result/problem2-10pt.txt',
        'data/ks_200_0'    : 'best-result/problem3-10pt.txt',
        'data/ks_400_0'   : 'best-result/problem4-10pt.txt',
        'data/ks_1000_0'   : 'best-result/problem5-10pt.txt',
        'data/ks_10000_0' : 'best-result/problem6-10pt.txt'
    }
    for i,j in dic.items():
        answer = solve(i)
        invalid = True
        with open(j,'r') as f:
            if answer['objective'] >= int(f.readline().split(' ')[0]):
                invalid = False
        if invalid:
            return False
    return True
if __name__ == '__main__':
    print(check())
