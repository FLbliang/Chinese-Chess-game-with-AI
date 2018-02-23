#ifndef ADDBLIND_H
#define ADDBLIND_H

#include "Board.h"
#include "Stone.h"
#include <QMap>

class AddBlind : public Board
{
    Q_OBJECT

public:
    AddBlind();
    void initTypes(QMap<int, int> & types);
    virtual void Click(const int & clickid, const int & row,const int & col);
    virtual void drawStone(QPainter &painter , const int & id);
    bool judgeHaveType(bool red, const int & index);
    void giveType(const int & id, const int & index);
    void updateText(const int & index);
    void test(bool red);
    bool haveEat();


private:
    QMap<int, int> red_types;
    QMap<int, int> black_types;
    bool haveMove[32];
    int current_boards;
};

#endif // ADDBLIND_H
