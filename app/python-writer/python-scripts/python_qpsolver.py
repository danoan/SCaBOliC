import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers
import model as M

def f(x,U,P):
    s= M.constantFactor*(U.trans()*x + x.trans()*P*x ) + M.constantTerm
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

X=solvers.qp(P,U,G,h)['x']
print(X)
print(solvers.qp(P,U))
rebuild_shape(X)
print(f(X,U,P))
print(M.constantTerm,M.constantFactor)
