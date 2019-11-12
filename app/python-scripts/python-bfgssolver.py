import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers
from scipy.optimize import minimize
import model as M

U = matrix( M._U, (M.numVars,1),'d' )
P = spmatrix( M._P_values, M._P_dim0,M._P_dim1, (M.numVars,M.numVars),'d' )

def f(X):
    s=0

    for x,u in zip(X,M._U):
        s+=x*u

    for x,y,v in zip(M._P_dim0,M._P_dim1,M._P_values):
        s+=X[x]*X[y]*v

    return s


def rebuild_shape(X):
    digShape = M.digitalModel
    for i,k in enumerate(M.pixelMap.keys()):
        row,col = M.pixelMap[k]['row'],M.pixelMap[k]['col']

        if X[k]==1:
            digShape[row][col] = 255
        else:
            digShape[row][col] = 0

    aaa = np.array( digShape, dtype=np.uint8 )
    plt.imsave("out.png",aaa)




x0 = np.array( [0]*M.numVars )
fres = minimize(f, x0, method='BFGS', options={'disp': True})


res = fres['x']
res[res>=1] = 0
res[res<1] = 1

rebuild_shape(res)



