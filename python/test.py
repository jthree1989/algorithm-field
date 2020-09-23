#! /usr/bin/python

#%%
import numpy as np
from scipy.linalg import null_space
from scipy.linalg import lu
from sympy import Matrix
import pprint
A = np.array([
    [1, 2],
    [3, 6]
    ])
ns_a = null_space(A)
print(ns_a)
#%%
B = np.array([
    [1, 2, 3]
    ])
ns_b = null_space(B)
print(ns_b)

mx = Matrix(A)
print(mx.rref())

P, L, U = lu(A)
pprint.pprint(L)
pprint.pprint(U)
pprint.pprint(P)
pprint.pprint(P@L@U)
