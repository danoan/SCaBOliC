#include <iostream>
#include "Table.h"

using namespace SCaBOliC::Experiment;

void Table::setColName(int colIndex, std::string colName)
{
    colNames[colIndex] = colName;
}

void Table::print(std::ostream &os)
{
    os << tableName;
    for(auto it=colNames.begin();it!=colNames.end();++it)
    {
        os << std::string("\t") << *it;
    }
    os << std::endl;

    for(int i=0;rowNames.size();++i)
    {
        os << rowNames[i];
        for(auto it=elements[i].begin();it!=elements[i].end();++it)
            os << std::string("\t") << *it;
    }
    os << std::endl;
}