
from cvxopt import spmatrix


def pim(rows,cols):
    m=rows*(cols+1)
    n=(cols+1)*rows

    c=cols

    I=[]
    J=[]
    values=[]
    for j in range(m):
        J.append(j)
        I.append( 2*(j//c)*(c+1)+j%c+1 )
        values.append( -1 )

        J.append(j)
        I.append( (2*(j//c) + 1)*(c+1)+j%c )
        values.append( -1 )

        J.append(j)
        I.append( (2*(j//c) + 1)*(c+1)+j%c + 1 )
        values.append( 1 )

        J.append(j)
        I.append( (2*(j//c+1))*(c+1)+j%c+1 )
        values.append( 1 )

    return spmatrix(values,I,J)

def main():
    PIM = pim(3,4)[:,11]
    print(PIM)
    pass


if __name__=='__main__':
    main()