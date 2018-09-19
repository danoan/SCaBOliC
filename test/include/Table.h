#ifndef SCABOLIC_TABLE_H
#define SCABOLIC_TABLE_H

#include <vector>
#include "string"

namespace SCaBOliC
{
    namespace Experiment
    {

        class Table
        {
        public:

        public:
            Table(std::string tableName):tableName(tableName){};

            template<class TIteratorType>
            void addRow(std::string rowName,
                        TIteratorType begin,
                        TIteratorType end);

            void setColName(int colIndex,std::string colName);

            void print(std::ostream& os);

        private:
            std::string tableName;

            int rows;
            int cols;

            std::vector<std::string> rowNames;
            std::vector<std::string> colNames;

            std::vector< std::vector<double> > elements;
        };
    }
}

#include "Table.hpp"

#endif //SCABOLIC_TABLE_H
