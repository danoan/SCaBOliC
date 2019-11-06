import numpy as np
import scipy
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers
import model as M

import wrapper_mosek as WM

def f(x,U,P):
    s= M.constantFactor*(U.trans()*x + 0.5*x.trans()*P*x ) + M.constantTerm
    return s[0]


def roundDown(X,U,P):
    Nx=matrix(X)
    for i in range(M.numVars):
        X0 = matrix(Nx)
        X1 = matrix(Nx)

        X0[i]=0
        X1[i]=1

        dx0 = f(X0,U,P)
        dx1 = f(X1,U,P)
        dx=(dx1-dx0)

        # print(f(Nx,U,P),dx1,dx0,dx)

        if dx < 0:
            Nx[i]=1
        else:
            Nx[i]=0

    return Nx

def rebuild_shape(X):
    digShape = M.digitalModel
    for i,x in enumerate(M.pixelMap.keys()):
        row,col = M.pixelMap[x]['row'],M.pixelMap[x]['col']

        if X[x]>=0.5:
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
    NX=roundDown(X,U,P)
    rebuild_shape(NX)

    print(f(X,U,P))
    print(f(NX,U,P))
    # print(M.constantTerm,M.constantFactor)



normally()