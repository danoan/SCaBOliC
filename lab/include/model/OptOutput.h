#ifndef SCABOLIC_LAB_MODEL_OPTOUTPUT_H
#define SCABOLIC_LAB_MODEL_OPTOUTPUT_H

#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/model/Solution.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Model
        {
            struct OptOutput
            {
                typedef Energy::ISQ::InputData ISQInputData;
                typedef Energy::Solution Solution;

                OptOutput(ISQInputData input,
                          Solution solution,
                          std::string prefix):input(input),
                                               solution(solution),
                                               prefix(prefix){}
                ISQInputData input;
                Solution solution;
                std::string prefix;
            };
        }
    }
}

#endif //SCABOLIC_LAB_MODEL_OPTOUTPUT_H
