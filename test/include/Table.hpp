#include "Table.h"

using namespace SCaBOliC::Experiment;

template<class TIteratorType>
void Table::addRow<TIteratorType>(std::string rowName,
                                  TIteratorType begin,
                                  TIteratorType end)
{
    rowNames.push_back(rowName);
    int cr = rowNames.size()-1;

    colNames.push_back("Noname-" + std::to_string(cr) );
    elements[cr].insert(begin,end);
}