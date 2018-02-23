#ifndef STEP_H
#define STEP_H
#include "Stone.h"

class Step
{
public:
    Step();
    int move_row;
    int move_col;
    int prev_row;
    int prev_col;
    int selectid;
    int killid;
    Step *next;

};

#endif // STEP_H
