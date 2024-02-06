#!/usr/bin/python
# -*- coding: utf-8 -*-
from ortools.sat.python import cp_model
import numpy as np
def solve(filename):
    with open(filename,'r') as f:
        n,K = map(int,f.readline().split(' '))
        v = np.zeros(n)
        w = np.zeros(n)
        x = []
        model = cp_model.CpModel()
        for i in range(n):
            v[i],w[i] = map(int,f.readline().split(' '))
            x.append(model.NewBoolVar(f'x{i}'))
        model.Add(np.dot(x,w) <= K)
        model.Maximize(np.dot(x,v))
        solver = cp_model.CpSolver()
        status = solver.Solve(model)
        answer = {}
        if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
            answer['objective'] = solver.ObjectiveValue()
            answer['x'] = [solver.Value(i) for i in x]
            return answer
    return None