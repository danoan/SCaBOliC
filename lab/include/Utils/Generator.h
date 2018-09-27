#ifndef SCABOLIC_GENERATOR_H
#define SCABOLIC_GENERATOR_H

#include <cstdlib>
#include <stdexcept>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Utils
        {
            template<int totalIndexes>
            class Generator
            {
            public:


            private:
                enum QO{Expand,Increment,Done,Invalid};

                struct QC
                {
                    QC(){};

                    QC(int level,int value,QO op):level(level),value(value)
                    {
                        if(level>totalIndexes) op=Invalid;
                        else this->op = op;
                    }

                    int level;
                    int value;
                    QO op;
                };

            public:
                Generator(const int* pIndexLimits):currIndex(0),
                                                   currLim(0)
                {
                    indexLimits = (int*) malloc(sizeof(int)*totalIndexes);
                    copySequence(indexLimits,pIndexLimits);

                    previousSequence = (int*) malloc(sizeof(int)*totalIndexes);
                    memset(previousSequence,0,sizeof(int)*totalIndexes);

                    stack.push(QC(0,0,QO::Expand));
                    stack.push(QC(0,0,QO::Done));
                };

                Generator(const Generator&){throw std::runtime_error("Operation not allowed!");}
                Generator& operator=(const Generator&){throw std::runtime_error("Operation not allowed!");}
                ~Generator(){free(previousSequence); free(indexLimits);}


                bool next(int* sequence)
                {
                    QC qc;
                    do
                    {
                        qc = stack.top();
                        stack.pop();

                        switch (qc.op)
                        {
                            case QO::Expand:
                            {
                                stack.push(QC(qc.level, qc.value, QO::Increment));

                                if(qc.level!=totalIndexes-1)
                                    stack.push(QC(qc.level + 1, 0,QO::Expand));

                                break;
                            }
                            case QO::Increment:
                            {
                                if(qc.value!=indexLimits[qc.level])
                                {
                                    ++qc.value;
                                    sequence[qc.level] = qc.value;
                                    stack.push(QC(qc.level,qc.value,QO::Expand));
                                    stack.push(QC(0,0,QO::Done));
                                }
                                else
                                {
                                    sequence[qc.level] = 0;
                                }
                            }
                        }

                    }while(!stack.empty() && qc.op!=QO::Done);

                    return !stack.empty();

                }

            private:
                inline void copySequence(int* cp, const int* old)
                {
                    for(int i=0;i<totalIndexes;++i) cp[i]=old[i];
                }

            private:
                int* indexLimits;

                std::stack<QC> stack;

                int* previousSequence;
                int currIndex;
                int currLim;
            };
        }
    }
}

#endif //SCABOLIC_GENERATOR_H
