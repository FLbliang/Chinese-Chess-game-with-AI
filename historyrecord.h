#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H

#include <iostream>
#include <QString>
#include "Step.h"

class HistoryRecord
{
private:
    int historyTable[90][90];

public:
    HistoryRecord();
    void resetHistoryTable();
    int getHistoryScore(Step *step);
    void pushHistoryTable(Step *step, int level);
    void sort(Step *& test);
};

#endif // HISTORYRECORD_H
