import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers
import model as M

def f(X):
    s=0

    for x,u in zip(X,M._U):
        s+=x*u

    for x,y,v in zip(M._P_dim0,M._P_dim1,M._P_values):
        s+=X[x]*X[y]*v

    return s

def rebuild_shape(X):
    digShape = M.digitalModel
    for i,x in enumerate(M.pixelMap.keys()):
        row,col = M.pixelMap[i]['row'],M.pixelMap[i]['col']

        if X[x]==1:
            digShape[row][col] = 255
        else:
            digShape[row][col] = 0

    aaa = np.array( digShape, dtype=np.uint8 )
    plt.imsave("out.png",aaa)


U = matrix( M._U, (M.numVars,1),'d' )
P = spmatrix( M._P_values, M._P_dim0,M._P_dim1, (M.numVars,M.numVars),'d' )

G = sparse( [ spdiag( [-1 for r in range(M.numVars)] ) ] )
h = matrix( [ [0.0]*M.numVars ], (M.numVars,1), 'd' )

X= np.array( [ 1 if v > 0 else 0 for v in solvers.qp(P,U,G,h)['x'] ] )
print(X)
rebuild_shape(X)
print(f(X))