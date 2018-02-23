#ifndef SINGLEGAME_H
#define SINGLEGAME_H
#include "Board.h"
#include <QVector>
#include "Step.h"
#include "hashtable.h"
#include "historyrecord.h"

const int Header_Chess = 0;
const int Car_Chess = 1;
const int Can_Chess = 2;
const int horse_Chess = 3;
const int Solider_Chess = 4;
const int Pro_solider_Chess = 5;
const int Elephant_Chess = 6;
const long int Base_value[7] = {66666,550,350,350,100,200,200};
const long int Flex_value[7] = {0, 9, 6, 12, 15, 1, 1};

struct Pos_record{
    int row;
    int col;
    int type;
    bool is_red;
};

class SingleGame : public Board
{
    Q_OBJECT
public:
    int cal;
    Step *steps;
    SingleGame();
    Step *select_step;
    int selectid2;
    int pre_row;
    int pre_col;
    int level;
    int Chess_Value[32];
    int beAttacked_Chess[32];
    int Protect_Chess[32];
    int Flexibility_Chess[32];
    int randNumber[16];
    int setLevel;

    HashTable *hashTable;
    HistoryRecord *history;


    void Click(const int & clickid, const int & row,const int & col);
      //

    // Find steps functions
    void FindSteps(Step *&test, const bool & find_red, const bool & judge_score);
    void Find_HeaderSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_ProSolidersSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_SolidersSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_HorseSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_ElephantSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_CarSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_CanSteps(Step *&test, const int & selectid, const bool & judge_score);
    void Find_Can_and_Car_Steps(Step *&test, const int & selectid, const bool & judge_score);

    void Move(Step *&step);

    //Computer imitate the steps
    void FakeMove(Step *&step);
    void FakeRelase(Step *&step);

    //Judge functions
    long int JudgeScore();
    int GetSoliderValue(const int & id);
    long int isGameover(int level);

    //help judge value functions
    void Chess_relation(const int & chess_id);
    void Header_relation(const int & id, Pos_record *&record);
    void ProSoliders_relation(const int & id, Pos_record *&record);
    void Soliders_relation(const int & id, QVector<Pos_record *> &record);
    void Horse_relation(const int & id, QVector<Pos_record *> &record);
    void Elephant_relation(const int & id, QVector<Pos_record *> &record);
    void Car_relation(const int & id, QVector<Pos_record *> &record);
    void Can_relation(const int & id, QVector<Pos_record *> &record);

    //alphabeta function
    long int alphabeta(const int & level, long int alpha, long int beta);

    //PVS algorithm
    long int PVS(const int & level, long int alpha, long int beta);

    //MTD function
    long int MTD(const long int & guess, const int & level);


    //help functions
    void WriteInfo(const int & killid, const int & row,const int & col,const int & selectid, Step *&step);
    void StepsPush(const int & selectid, const int & row, const int & col, Step *&test);
    void Can_Car_StepsPush(const int & selectid,
                           const int & row,
                           const int & col,
                           Step *& test,
                           int & stone_num,
                           const bool & judge_score);

    bool isRepetition(Step *& red, Step *& black);
    bool judgeSame(Step *& jud1, Step *& jud2);
    void initRandNumber();

    //back functions
    void Push(Step *&step);
    void Back_board();
    bool isChess(const int & row, const int & col){ return this->Board_record[row][col] != -1 ? true : false; }

    void RelasePlace(QVector<Step* > &test);

public slots:
     void ComputerMove();


};

#endif // SINGLEGAME_H
