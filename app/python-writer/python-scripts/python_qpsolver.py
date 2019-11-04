import numpy as np
import scipy
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers
import model as M

import wrapper_mosek as WM

def f(x,U,P):
    s= M.constantFactor*(U.trans()*x + 0.5*x.trans()*P*x ) + M.constantTerm
    return s


def roundDown(X,U,P):
    Nx=matrix(X)
    for i in range(M.numVars):
        print( f(Nx,U,P) )

        row = 0.5*P[i,:]

        X0 = matrix(Nx)
        X1 = matrix(Nx)

        X0[i]=0
        X1[i]=1

        dx0 = row[i]*X0[i] + row*X0 + U[i]
        dx1 = row[i]*X1[i] + row*X1 + U[i]
        dx=(dx1-dx0)[0]

        #print(dx1[0],dx0[0],(dx1-dx0)[0])

        if dx < 0:
            Nx[i]=1
        else:
            Nx[i]=0

    exit
    return Nx

def rebuild_shape(X):
    digShape = M.digitalModel
    for i,x in enumerate(M.pixelMap.keys()):
        row,col = M.pixelMap[x]['row'],M.pixelMap[x]['col']

        if X[x]>=0.9:
            digShape[row][col] = 255
        else:
            digShape[row][col] = 0

    aaa = np.array( digShape, dtype=np.uint8 )
    plt.imsave("out.png",aaa,cmap='gray')



def normally():
    U = matrix( M._U, (M.numVars,1),'d' )
    P = spmatrix( M._P_values, M._P_dim0,M._P_dim1, (M.numVars,M.numVars),'d' )

    G0 = sparse( spdiag( [-1 for r in range(M.numVars)] ) )
    G1 = sparse( spdiag( [1 for r in range(M.numVars)] ) )
    G = matrix([G0,G1])
    h = matrix( [ [0.0]*M.numVars,[1.0]*M.numVars ], (2*M.numVars,1), 'd' )


    X=solvers.qp(P,U,G,h)['x']
    rebuild_shape(X)

    print(f(X,U,P))
    print(f(roundDown(X,U,P),U,P))
    print(M.constantTerm,M.constantFactor)

    X=matrix( [0]*M.numVars)
    for i in [6,3,66,21,25,102,51,14,2,45,43,61,72,75,15,79,78,77,69,70,71,85,86,17,91,58,30,29,24,27,26,36,35,99,109,81,80,4,38,95,87,88,89,34,67,10,11,7,53,49,5,98,42,84,83,50,40,28,111,55,33,31,105]:
        X[i]=1
    print(f(X,U,P))





normally()