#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "Stone.h"
//#include "Step.h"


class Board : public QWidget
{
    Q_OBJECT

public:
    int from_col;
    int from_row;
    int to_col;
    int to_row;
    bool isMove;
    bool isChessMove;
    Stone _stone[32];
    int Board_record[10][10];
    int Boards;
    bool Red_killed;
    bool Black_killed;
    int _r;
    int _selectid;
    bool is_red;
    Stone::TYPE dead_type;
    int deadId;
    QPushButton *start;
    QPushButton *back;
    QPushButton *exit;
    QLabel *label;

    explicit Board(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *);
    virtual void drawStone(QPainter &painter , const int & id);
    QPoint center(const int & id);
    QPoint center(const int & row, const int & col);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void Click(const int & clickid, const int & row, const int & col);

    //rules
    bool MoveJudge(const int & killid, const int & row, const int & col, const int & selectid);
    bool JudgeHorse(const int & row, const int & col, const int & selectid);
    bool JudgeHeader(const int & row, const int & col, const int & selectid);
    bool JudgeElephant(const int & row, const int & col, const int & selectid);
    bool JudgeCan(const int & killid, const int & row, const int & col, const int & selectid);
    bool JudgeCar(const int & row, const int & col, const int & selectid);
    bool JudgeProSolider(const int & row, const int & col, const int & selectid);
    bool JudgeSolider(const int & row, const int & col, const int & selectid);

    //help function
    int Relation(const int & row, const int & col, const int & selectid);
    int Have_Boards(const int & row, const int & col, const int & selectid);
    int GetId(const int & row, const int & col);


signals:

public slots:
};

#endif // BOARD_H
