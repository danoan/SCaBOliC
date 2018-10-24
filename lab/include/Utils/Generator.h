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
                typedef unsigned long Index;

            private:
                enum QO{Expand,Increment,Done,First,Invalid};

                struct QC
                {
                    QC(){};

                    QC(Index level,Index value,QO op):level(level),value(value)
                    {
                        if(level>totalIndexes) op=Invalid;
                        else this->op = op;
                    }

                    Index level;
                    Index value;
                    QO op;
                };

            public:
                Generator(const Index* pIndexLimits)
                {
                    indexLimits = (Index*) malloc(sizeof(Index)*totalIndexes);
                    previousSequence = (Index*) malloc(sizeof(Index)*totalIndexes);

                    restart(pIndexLimits);
                };

                void restart(const Index* pIndexLimits)
                {
                    currIndex = 0;
                    currLim = 0;

                    copySequence(indexLimits,pIndexLimits);
                    memset(previousSequence,0,sizeof(Index)*totalIndexes);

                    stack.push(QC(0,0,QO::Expand));
                    stack.push(QC(0,0,QO::First));
                }

                Generator(const Generator&){throw std::runtime_error("Operation not allowed!");}
                Generator& operator=(const Generator&){throw std::runtime_error("Operation not allowed!");}
                ~Generator(){free(previousSequence); free(indexLimits);}


                bool next(Index* sequence)
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

                                break;
                            }
                            case QO::First:
                            {
                                for(Index i=0;i<totalIndexes;++i) sequence[i] = 0;
                                stack.push(QC(0,0,QO::Done));
                                break;
                            }
                        }

                    }while(!stack.empty() && qc.op!=QO::Done);

                    return !stack.empty();

                }

            private:
                inline void copySequence(Index* cp, const Index* old)
                {
                    for(Index i=0;i<totalIndexes;++i) cp[i]=old[i];
                }

            private:
                Index* indexLimits;

                std::stack<QC> stack;

                Index* previousSequence;
                Index currIndex;
                Index currLim;
            };
        }
    }
}

#endif //SCABOLIC_GENERATOR_H
