#include "Singlegame.h"
#include "Step.h"
#include <QTimer>
#include <ctime>
#include <QDebug>
#include <QtAlgorithms>

#define cout qDebug()


const long int MAX = 600000;

const int Red_solider[10][9] = {
    {0, 0,  0,  0,  0,  0,  0, 0,  0},
    {90,90,110,120,120,120,110,90,90},
    {90,90,110,120,120,120,110,90,90},
    {70,90,110,110,110,110,110,90,70},
    {70,70,70, 70, 70, 70, 70, 70,70},
};

const int Black_solider[10][9] = {
     {0, 0,  0,  0,  0,  0,  0, 0,  0},
     {0, 0,  0,  0,  0,  0,  0, 0,  0},
     {0, 0,  0,  0,  0,  0,  0, 0,  0},
     {0, 0,  0,  0,  0,  0,  0, 0,  0},
     {0, 0,  0,  0,  0,  0,  0, 0,  0},
     {70,70,70, 70, 70, 70, 70, 70,70},
     {70,90,110,110,110,110,110,90,70},
     {90,90,110,120,120,120,110,90,90},
     {90,90,110,120,120,120,110,90,90},
     {0, 0,  0,  0,  0,  0,  0, 0,  0},
};

SingleGame::SingleGame()
{
    this->initRandNumber();
    this->hashTable = new HashTable;
    this->history = new HistoryRecord;
    this->selectid2 = -1;
    this->steps = new Step;
    this->steps->next = NULL;
    srand((unsigned)time(NULL));
    this->level = 5;
    this->cal = 0;
}

void SingleGame::initRandNumber(){
    for(int i = 0; i < 16; i++){
        this->randNumber[i] = i;
    }
}

int SingleGame::GetSoliderValue(const int & id){
    int row = this->_stone[id]._row;
    int col = this->_stone[id]._col;
    if(id < 16){
        return Black_solider[row][col];
    }else{
        return Red_solider[row][col];
    }
}

long int SingleGame::JudgeScore(){

    long int Red_sum = 0;
    long int Black_sum = 0;
    Step *test;
    Step *de;
    int id;


    bool isRed_turn = this->level%2 > 0;

    memset(this->beAttacked_Chess, 0, sizeof(this->beAttacked_Chess));
    memset(this->Protect_Chess, 0, sizeof(this->Protect_Chess));

    int killed, selected;
    int halfValue;

    for(int i = 0; i < 32; i++){
        if(this->_stone[i]._dead){
            continue;
        }
        test = new Step;
        test->next = NULL;
        this->Chess_Value[i] = 0;
        this->Flexibility_Chess[i] = 0;

        selected = this->_stone[i]._type;      // find the type of selected chess

        switch(selected){
        case Stone::CHE:
            this->Find_CarSteps(test, i, true);
            break;
        case Stone::PAO:
            this->Find_CanSteps(test, i, true);
            break;
        case Stone::MA:
            this->Find_HorseSteps(test, i, true);
            break;
        case Stone::XIANG:
            this->Find_ElephantSteps(test, i, true);
            break;
        case Stone::BING:
            this->Find_SolidersSteps(test, i, true);
            break;
        case Stone::SHI:
            this->Find_ProSolidersSteps(test, i, true);
            break;
        case Stone::JIANGE:
            this->Find_HeaderSteps(test, i, true);
            break;
        }

        de = test;
        test = test->next;
        delete de;


        // find the flexibility and calculate the weight of the chess that may be attacked
        while(test){
            id = test->killid;
            if(id == -1){      // calculate flexibility
                this->Flexibility_Chess[i]++;
            }else{

                if(this->_stone[i]._red == this->_stone[id]._red){
                    this->Protect_Chess[id]++;
                }else{

                    killed = this->_stone[id]._type;   // find the type of chess that may be attacked

                    this->beAttacked_Chess[id]++;
                    this->Flexibility_Chess[i]++;

                    switch(this->_stone[id]._type){
                    case Stone::JIANGE:
                        if(this->_stone[id]._red && !isRed_turn){
                            return MAX;
                        }else if(!this->_stone[id]._red && isRed_turn){
                            return MAX;
                        }
                        break;
                    default:   // calculate the attacked weight of the chess that may be attacked
                        this->beAttacked_Chess[id] += (30 + (Base_value[killed] - Base_value[selected])/10)/10;

                        break;
                    }
                }
            }
            de = test;
            test = test->next;
            delete de;
        }

        // calculate the weight of the selected chess
        this->Chess_Value[i] += Base_value[selected] + this->Flexibility_Chess[i]*Flex_value[selected];



    }

    for(int i = 0; i < 32; i++){
        if(this->_stone[i]._dead){
            continue;
        }

        selected = this->_stone[i]._type;
        halfValue = Base_value[selected]/16;
        if( this->_stone[i]._red ){   // calculate the red type chess weight
            if( this->beAttacked_Chess[i] ){ // judge the red chess may be attacked or not
                if( isRed_turn ){
                    // judge the type of chess
                    if(selected == Stone::JIANGE){ // judge if the selected chess is the king .

                        this->Chess_Value[i] -= 20;

                    }else{

                        this->Chess_Value[i] -= halfValue * 2;
                        // judge that if the selected chess be protected or not .
                        if( this->Protect_Chess[i] ){
                            this->Chess_Value[i] += halfValue;
                        }
                    }
                }else{
                    // the black chess turn
                    if(selected == Stone::JIANGE){
                        return MAX;
                    }

                    this->Chess_Value[i] -= halfValue * 10;
                    if(this->Protect_Chess[i]){
                        this->Chess_Value[i] += halfValue * 9;
                    }
                }

                this->Chess_Value[i] -= this->beAttacked_Chess[i];

            }else{
                // have not be attacked
                if(this->Protect_Chess[i]){
                    this->Chess_Value[i] += 5;
                }
            }
        }else{ // the black chess

            if(this->beAttacked_Chess[i]){ // may be attacked
                if( !isRed_turn ){
                    // the black turn
                    if(this->_stone[i]._type == Stone::JIANGE){
                        this->Chess_Value[i] -= 20;
                    }else{
                        this->Chess_Value[i] -= halfValue * 2;

                        if(this->Protect_Chess[i]){
                            this->Chess_Value[i] += halfValue;
                        }
                    }
                }else{
                    // the red turn
                    if(this->_stone[i]._type == Stone::JIANGE){
                        return MAX;
                    }else{
                        this->Chess_Value[i] -= halfValue * 10;

                        if(this->Protect_Chess[i]){
                            this->Chess_Value[i] += halfValue * 9;
                        }
                    }
                }

                this->Chess_Value[i] -= this->beAttacked_Chess[i];
            }else{
                // have not be attacked
                if(this->Protect_Chess[i]){
                    // be protected
                    this->Chess_Value[i] += 5;
                }
            }
        }

        if(this->_stone[i]._red){
            if(selected == Stone::BING){
                this->Chess_Value[i] += this->GetSoliderValue(i);
            }

            // add the value to the sum value of red chess
            Red_sum += this->Chess_Value[i];
        }else{
            if(selected == Stone::BING){
                this->Chess_Value[i] += this->GetSoliderValue(i);
            }

            //add the value to the sum value of black chess
            Black_sum += this->Chess_Value[i];
        }
    }

    if(isRed_turn){
        return Red_sum - Black_sum;
    }else{
        return Black_sum - Red_sum;
    }
}

void SingleGame::Push(Step *&step){
    step->next = this->steps->next;
    this->steps->next = step;
}

void SingleGame::Back_board(){
    if(this->steps->next){
        Step *step = this->steps->next;
        this->FakeRelase(step);
        this->steps->next = step->next;
        this->is_red = !this->is_red;
        if(step->killid != -1){
            this->Boards++;
        }
        delete step;
        update();
    }
}

void SingleGame::Click(const int & clickid, const int & row, const int & col){
    if(this->is_red){
        Board::Click(clickid, row, col);
        if(this->_selectid != -1){
            this->selectid2 = this->_selectid;
            this->pre_row = this->_stone[this->selectid2]._row;
            this->pre_col = this->_stone[this->selectid2]._col;
        }


        if(!this->is_red){
            Step *step = new Step;
            this->_stone[this->selectid2]._row = this->pre_row;
            this->_stone[this->selectid2]._col = this->pre_col;
            this->WriteInfo(clickid,row,col,this->selectid2,step);
            this->Push(step);
            this->_stone[this->selectid2]._row = row;
            this->_stone[this->selectid2]._col = col;

//           if(this->cal >= 3){
//               this->level = 4;
//           }
           if(this->Boards < 28){
               this->level = 5;
           }

            QTimer::singleShot(16,this,SLOT(ComputerMove()));
        }
    }

}

bool SingleGame::judgeSame(Step *&jud1, Step *&jud2){
    if(jud1->killid != jud2->killid || jud1->selectid != jud2->selectid){
        return false;
    }else if(jud1->move_col != jud2->move_col || jud1->move_row != jud2->move_row){
        return false;
    }
    return true;
}

bool SingleGame::isRepetition(Step *& red, Step *& black){

    if(this->_stone[black->selectid]._type == Stone::JIANGE){
        return false;
    }

    if(red->killid == -1 &&
            black->killid == -1)
    {
        Step *old_red, *old_black;
        old_red = red->next->next;
        old_black = red->next;

        if(old_red->killid == -1 &&
                old_black->killid == -1 &&
                this->_stone[old_black->selectid]._type == this->_stone[black->selectid]._type)
        {

            old_black = old_red->next;
            old_red = old_red->next->next;
            if(this->judgeSame(old_red, red) && this->judgeSame(old_black, black)){
                return true;
            }

        }
    }

    return false;
}

long int SingleGame::isGameover(int level){

    int flag = (this->level - level + 1) % 2;
    if(this->Red_killed){
        if(flag){
            return MAX + level;
        }else{
            return -MAX - level;
        }
    }

    if(this->Black_killed){
        if(flag){
            return -MAX - level;
        }else{
            return MAX + level;
        }
    }

    return 0;
}

long int SingleGame::alphabeta(const int &level, long int alpha, long int beta){
    int gameOver = this->isGameover(level);
    if(gameOver){
        return gameOver;
    }else if( level <= 0 ){
        long int value = this->JudgeScore();
        this->hashTable->tablePush(0, value, level, this->level%2);
        return value;
    }

    bool findRed = this->level % 2 > 0 ? level % 2 == 0 : level % 2 > 0;

    int tableValue = this->hashTable->lookUpHashTable(alpha, beta, level, this->level%2);

    if(tableValue != 676767){
        return tableValue;
    }



    Step *test = new Step;
    test->next = NULL;
    this->FindSteps(test, findRed, false);
    this->history->sort(test);
    Step *p = test;
    test = test->next;
    long int score;
    Step *bestMove = NULL;
    bool isExact = false;
//    long int current = -MAX;

    delete p;
    while(test){
        p = test;
        this->FakeMove(p);
        this->hashTable->hashFakeMove(p);
        score = -this->alphabeta(level-1, -beta, -alpha);
        this->FakeRelase(p);
        this->hashTable->hashFakeRelase(p);

        if(score > alpha){
            if(level == this->level){
                if(this->cal < 3 || ! this->isRepetition(this->steps->next, test) ){
                    if(this->select_step != NULL){
                        delete this->select_step;
                    }
                    this->select_step = p;
                    bestMove = p;
                    alpha = score;
                    isExact = true;
                }
            }else {
                alpha = score;
                bestMove = p;
                isExact = true;
            }
        }

        if(alpha >= beta){
            while(test){
                p = test;
                test = test->next;
                if(p != bestMove)
                    delete p;
            }
            this->hashTable->tablePush(1, alpha, level, this->level%2);
            if(bestMove){
                this->history->pushHistoryTable(bestMove, level);
                if(this->level != level){
                    delete bestMove;
                }
            }

            return alpha;
        }
       test = test->next;
       if(p != bestMove)
         delete p;
    }
    if(isExact){
       this->hashTable->tablePush(0, alpha, level, this->level%2);
    }else{
        this->hashTable->tablePush(2, alpha, level, this->level%2);
    }

    if(bestMove){
        this->history->pushHistoryTable(bestMove, level);
        if(this->level != level){
            delete bestMove;
        }
    }

    return alpha;
}

long int SingleGame::PVS(const int &level, long alpha, long beta){
    int gameOver = this->isGameover(level);
    if(gameOver){
        return gameOver;
    }else if( level <= 0 ){
        long int value = this->JudgeScore();
        this->hashTable->tablePush(0, value, level, this->level%2);
        return value;
    }

    bool findRed = this->level % 2 > 0 ? level % 2 == 0 : level % 2 > 0;

    int tableValue = this->hashTable->lookUpHashTable(alpha, beta, level, this->level%2);

    if(tableValue != 676767){
        return tableValue;
    }


    Step *test = new Step;
    test->next = NULL;
    this->FindSteps(test, findRed, false);
    this->history->sort(test);

    Step *p = test;
    test = test->next;
    delete p;
    p = test;
    test = test->next;
    this->FakeMove(p);
    this->hashTable->hashFakeMove(p);
    long int bestValue = -this->PVS(level-1, -beta, -alpha);
    this->FakeRelase(p);
    this->hashTable->hashFakeRelase(p);

    long int score;
    int judge = 2;
    Step *bestMove = p;

    if(this->level == level){
        this->select_step = p;
    }

    while(test){
        p = test;
        if(bestValue < beta){

            if(bestValue > alpha){
                judge = 0;
                alpha = bestValue;
            }

            this->FakeMove(p);
            this->hashTable->hashFakeMove(p);
            score = -this->PVS(level-1, -alpha-1, -alpha);

            if(score > alpha && score < beta){
                bestValue = -this->PVS(level-1, -beta, -score);
                if(this->level == level){
                    if(this->cal < 3 || ! this->isRepetition(this->steps->next, p) ){
                        if(this->select_step != NULL){
                            delete this->select_step;
                        }
                        this->select_step = p;
                        bestMove = p;

                    }

                }else{
                    bestMove = p;
                }
            }else if(score > bestValue){
                bestValue = score;
                if(this->level == level){
                    if(this->cal < 3 || ! this->isRepetition(this->steps->next, p) ){
                        if(this->select_step != NULL){
                            delete this->select_step;
                        }
                        this->select_step = p;
                        bestMove = p;
                    }
                }else{
                    bestMove = p;
                }
            }
            this->FakeRelase(p);
            this->hashTable->hashFakeRelase(p);
        }
        if(bestValue >= beta){

            judge = 1;
        }

        test = test->next;
        if(p != bestMove){
            delete p;
        }
    }

    this->hashTable->tablePush(judge, bestValue, level, this->level%2);
    this->history->pushHistoryTable(bestMove, level);
    if(this->level != level){
        delete bestMove;
    }

    return bestValue;


}

long int SingleGame::MTD(const long int &guess, const int &level){
    long int lower_bound = -MAX;
    long int upper_bound = MAX;

    long int score = guess;
    long int value;


    while(lower_bound < upper_bound){

        if(score == lower_bound){
            value = score + 1;
        }else{
            value = score;
        }

        score = this->alphabeta(level, value-1, value);

        if(score < value){
            upper_bound = score;
        }else{
            lower_bound = score;
        }
    }

    return score;
}

void SingleGame::ComputerMove(){
    this->select_step = NULL;
    this->hashTable->calculateInitKeys(this->Board_record);
    this->history->resetHistoryTable();
//    this->alphabeta(this->level, -MAX, MAX);

    this->PVS(this->level, -MAX, MAX);
//    long int guess = 0;
//    this->setLevel = this->level;
//    for(this->level = 1; this->level <= this->setLevel; this->level++){
//        testCal = 0;
//        guess = this->MTD(guess, this->level);
//        cout << testCal;
//	       cout << guess;
//    }
//    this->level--;
//    cout << testCal;
    this->Move(this->select_step);
    this->Push(this->select_step);
    this->cal++;
    update();

}



void SingleGame::FindSteps(Step *&test, const bool & find_red, const bool & judge_score){

//    testCal++;
    int sum = 16;
    int id;
    int flag;
    while(sum){
        flag = rand() % sum;
        id = this->randNumber[flag];
        if(find_red){
            id += 16;
        }
        swap(this->randNumber[flag], this->randNumber[sum-1]);
        if(this->_stone[id]._dead){
            sum--;
            continue;
        }
        switch(this->_stone[id]._type){
        case Stone::JIANGE:
            this->Find_HeaderSteps(test,id, judge_score);break;
        case Stone::BING:
            this->Find_SolidersSteps(test,id, judge_score);break;
       case Stone::CHE:
            this->Find_CarSteps(test,id, judge_score);break;
        case Stone::MA:
            this->Find_HorseSteps(test,id, judge_score);break;
        case Stone::PAO:
            this->Find_CanSteps(test,id, judge_score);break;
        case Stone::SHI:
            this->Find_ProSolidersSteps(test,id, judge_score);break;
        case Stone::XIANG:
            this->Find_ElephantSteps(test,id, judge_score);break;
        }
        sum--;

    }

}

void SingleGame::FakeMove(Step *&step){
    this->_stone[step->selectid]._row = step->move_row;
    this->_stone[step->selectid]._col = step->move_col;
    this->Board_record[step->prev_row][step->prev_col] = -1;
    this->Board_record[step->move_row][step->move_col] = step->selectid;

    if(step->killid !=  -1){
        this->_stone[step->killid]._dead = true;
        if(this->_stone[step->killid]._type == 0){
            this->_stone[step->killid]._red ? this->Red_killed = true : this->Black_killed = true;
        }
    }
    this->is_red = !this->is_red;

}

void SingleGame::FakeRelase(Step *&step){
    this->_stone[step->selectid]._row = step->prev_row;
    this->_stone[step->selectid]._col = step->prev_col;
    this->Board_record[step->prev_row][step->prev_col] = step->selectid;
    if(step->killid != -1){
        this->_stone[step->killid]._dead = false;
        if(this->_stone[step->killid]._type == 0){
            this->_stone[step->killid]._red ? this->Red_killed = false : this->Black_killed = false;
        }
    }
    this->Board_record[step->move_row][step->move_col] = step->killid;
    this->is_red = !this->is_red;
}

void SingleGame::Move(Step *&step){
    this->_selectid = step->selectid;
    Board::Click(step->killid,step->move_row,step->move_col);
    if(this->_stone[step->killid]._type == Stone::JIANGE){
        this->_stone[step->killid]._red ? this->Red_killed = true : this->Black_killed = true;
    }
}




void SingleGame::WriteInfo(const int & killid, const int & row, const int & col, const int & selectid, Step *&step){
   step->killid = killid;
   step->selectid = selectid;
   step->move_row = row;
   step->move_col = col;
   step->prev_row = this->_stone[selectid]._row;
   step->prev_col = this->_stone[selectid]._col;
}

void SingleGame::Find_Can_and_Car_Steps(Step *&test, const int & selectid, const bool & judge_score){

    int row = this->_stone[selectid]._row;
    int col = this->_stone[selectid]._col;
    int stone_num = 0;
    for(int i = col+1; i < 9; i++){   // right
        this->Can_Car_StepsPush(selectid, row, i, test, stone_num, judge_score);
        if(stone_num == 2){
            break;
        }else if(this->_stone[selectid]._type == Stone::CHE && stone_num){
            break;
        }
    }

    stone_num = 0;
    for(int i = col-1; i >= 0; i--){ // left
        this->Can_Car_StepsPush(selectid,row, i, test, stone_num, judge_score);
        if(stone_num == 2){
            break;
        }else if(this->_stone[selectid]._type == Stone::CHE && stone_num){
            break;
        }
    }

    stone_num = 0;
    for(int i = row+1; i < 10; i++){//down
        this->Can_Car_StepsPush(selectid, i, col, test, stone_num, judge_score);
        if(stone_num == 2){
            break;
        }else if(this->_stone[selectid]._type == Stone::CHE && stone_num){
            break;
        }
    }

    stone_num = 0;
    for(int i = row-1; i >= 0; i--){//up
        this->Can_Car_StepsPush(selectid, i, col, test, stone_num, judge_score);
        if(stone_num == 2){
            break;
        }else if(this->_stone[selectid]._type == Stone::CHE && stone_num){
            break;
        }
    }

}

void SingleGame::StepsPush(const int & selectid, const int & row, const int & col,Step *&test){
    Step *step = new Step;
    int killid = this->Board_record[row][col];
    this->WriteInfo(killid,row,col,selectid,step);
    step->next = test->next;
    test->next = step;
}

void SingleGame::Can_Car_StepsPush(const int & selectid,
                              const int & row,
                              const int  &col,
                              Step *& test,
                              int & stone_num,
                              const bool & judge_score)

{
    int killid = this->GetId(row,col);
    switch(stone_num){
    case 0:
        if(killid == -1){
            if(judge_score){
                this->Flexibility_Chess[selectid]++;
            }else{
                this->StepsPush(selectid,row,col,test);
            }

        }else if(killid != -1){
            stone_num++;
            if(this->_stone[selectid]._type == Stone::CHE){
                if(judge_score){
                    this->StepsPush(selectid, row, col, test);
                }else if(this->_stone[killid]._red != this->_stone[selectid]._red){
                    this->StepsPush(selectid, row, col, test);
                }
            }
        }
        return;
    case 1:
        if(killid == -1){
            return;
        }else if(killid != -1){
            if(judge_score){
                this->StepsPush(selectid,row,col,test);
            }else if(this->_stone[killid]._red != this->_stone[selectid]._red){
                this->StepsPush(selectid,row,col,test);
            }
            stone_num++;
        }
        return;
    default:return;

    }

}

void SingleGame::Find_CanSteps(Step *&test, const int & selectid, const bool & judge_score){
   this->Find_Can_and_Car_Steps(test, selectid, judge_score);
}

void SingleGame::Find_CarSteps(Step *&test, const int & selectid, const bool & judge_score){
    this->Find_Can_and_Car_Steps(test, selectid, judge_score);
}

void SingleGame::Find_ElephantSteps(Step *&test, const int & selectid, const bool & judge_score){
    int sel_row = this->_stone[selectid]._row;
    int sel_col = this->_stone[selectid]._col;

    int row1 = sel_row - 2;
    int col1 = sel_col - 2;

    int row2 = sel_row - 2;
    int col2 = sel_col + 2;

    int row3 = sel_row + 2;
    int col3 = sel_col - 2;

    int row4 = sel_row + 2;
    int col4 = sel_col + 2;

    if(row1 >= 0 && col1 >= 0){
        if(judge_score){
            if(this->JudgeElephant(row1, col1, selectid)){
                if( !this->isChess(row1, col1) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, row1, col1, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(row1,col1),row1,col1,selectid)){
             this->StepsPush(selectid, row1, col1, test);
        }

    }

    if(row2 >= 0 && col2 < 9){
        if(judge_score){
            if(this->JudgeElephant(row2, col2, selectid)){
                if( !this->isChess(row2, col2) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, row2, col2, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(row2,col2),row2,col2,selectid)){
            this->StepsPush(selectid,row2,col2,test);
        }

    }

    if(row3 < 10 && col3 >= 0){
        if(judge_score){
            if(this->JudgeElephant(row3, col3, selectid)){
                if( !this->isChess(row3, col3) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, row3, col3, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(row3,col3),row3,col3,selectid)){
             this->StepsPush(selectid,row3,col3,test);
        }

    }

    if(row4 < 10 && col4 < 9){
        if(judge_score){
            if(this->JudgeElephant(row4, col4, selectid)){
                if( !this->isChess(row4, col4) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, row4, col4, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(row4,col4),row4,col4,selectid)){
             this->StepsPush(selectid,row4,col4,test);
        }

    }

}

void SingleGame::Find_HeaderSteps(Step *&test, const int & selectid, const bool & judge_score){
    int sel_row = this->_stone[selectid]._row;
    int sel_col = this->_stone[selectid]._col;

    int row1 = sel_row;
    int col1 = sel_col - 1;

    int row2 = sel_row - 1;
    int col2 = sel_col;

    int row3 = sel_row;
    int col3 = sel_col + 1;

    int row4 = sel_row + 1;
    int col4 = sel_col;

    if(col1 >= 3){
        if(judge_score){
            if(this->JudgeHeader(row1, col1, selectid)){
                if( !this->isChess(row1, col1) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                     this->StepsPush(selectid, row1, col1, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(row1,col1),row1,col1,selectid)){
            this->StepsPush(selectid, row1, col1, test);
        }
    }

    if(!this->_stone[selectid]._red){
        if(row2 >= 0){
            if(judge_score){
                if(this->JudgeHeader(row2, col2, selectid)){
                    if( !this->isChess(row2, col2) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row2, col2, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row2,col2),row2,col2,selectid)){
                this->StepsPush(selectid,row2,col2,test);
            }
        }

        if(row4 < 3){
            if(judge_score){
                if(this->JudgeHeader(row4, col4, selectid)){
                    if( !this->isChess(row4, col4) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row4, col4, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row4,col4),row4,col4,selectid)){
                 this->StepsPush(selectid,row4,col4,test);
            }
        }
    }else{
        if(row2 >= 7){
            if(judge_score){
                if(this->JudgeHeader(row2, col2, selectid)){
                    if( !this->isChess(row2, col2) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row2, col2, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row2,col2),row2,col2,selectid)){
                 this->StepsPush(selectid,row2,col2,test);
            }
        }

        if(row4 < 10){
            if(judge_score){
                if(this->JudgeHeader(row4, col4, selectid)){
                    if( !this->isChess(row4, col4) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row4, col4, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row4,col4),row4,col4,selectid)){
                 this->StepsPush(selectid,row4,col4,test);
            }
        }
    }


    if(col3 < 6){
        if(judge_score){
            if(this->JudgeHeader(row3, col3, selectid)){
                if( !this->isChess(row3, col3) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, row3, col3, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(row3,col3),row3,col3,selectid)){
             this->StepsPush(selectid,row3,col3,test);
        }
    }

    int id = selectid == 20 ? 4 : 20;
    if(this->JudgeHeader(this->_stone[id]._row, this->_stone[id]._col, selectid)){
        this->StepsPush(selectid, this->_stone[id]._row, this->_stone[id]._col, test);

    }

}

void SingleGame::Find_HorseSteps(Step *&test, const int & selectid, const bool & judge_score){
    int sel_row = this->_stone[selectid]._row;
    int sel_col = this->_stone[selectid]._col;

    int hrow1 = sel_row - 2;
    int hcol1 = sel_col - 1;

    int hrow2 = sel_row - 2;
    int hcol2 = sel_col + 1;

    int hrow3 = sel_row + 2;
    int hcol3 = sel_col - 1;

    int hrow4 = sel_row + 2;
    int hcol4 = sel_col + 1;

    int lrow1 = sel_row - 1;
    int lcol1 = sel_col - 2;

    int lrow2 = sel_row - 1;
    int lcol2 = sel_col + 2;

    int lrow3 = sel_row + 1;
    int lcol3 = sel_col - 2;

    int lrow4 = sel_row + 1;
    int lcol4 = sel_col + 2;

    if(hrow1 >=  0 && hcol1 >= 0){
        if(judge_score){
            if(this->JudgeHorse(hrow1, hcol1, selectid)){
                if( !this->isChess(hrow1, hcol1) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, hrow1, hcol1, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(hrow1,hcol1),hrow1,hcol1,selectid)){
            this->StepsPush(selectid, hrow1, hcol1, test);
        }

    }

    if(hrow2 >= 0 && hcol2 < 9){
        if(judge_score){
            if(this->JudgeHorse(hrow2, hcol2, selectid)){
                if( !this->isChess(hrow2, hcol2) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, hrow2, hcol2, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(hrow2,hcol2),hrow2,hcol2,selectid)){
            this->StepsPush(selectid,hrow2,hcol2,test);
        }

    }

    if(hrow3 < 10 && hcol3 >= 0){
        if(judge_score){
            if(this->JudgeHorse(hrow3, hcol3, selectid)){
                if( !this->isChess(hrow3, hcol3) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, hrow3, hcol3, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(hrow3,hcol3),hrow3,hcol3,selectid)){
            this->StepsPush(selectid,hrow3,hcol3,test);
        }

    }

    if(hrow4 < 10 && hcol4 < 9){
        if(judge_score){
            if(this->JudgeHorse(hrow4, hcol4, selectid)){
                if( !this->isChess(hrow4, hcol4) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                     this->StepsPush(selectid, hrow4, hcol4, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(hrow4,hcol4),hrow4,hcol4,selectid)){
            this->StepsPush(selectid,hrow4,hcol4,test);
        }

    }

    if(lrow1 >= 0 && lcol1 >= 0){
        if(judge_score){
            if(this->JudgeHorse(lrow1, lcol1, selectid)){
                if( !this->isChess(lrow1, lcol1) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                     this->StepsPush(selectid, lrow1, lcol1, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(lrow1,lcol1),lrow1,lcol1,selectid)){
            this->StepsPush(selectid,lrow1,lcol1,test);
        }

    }

    if(lrow2 >= 0 && lcol2 < 9){
        if(judge_score){
            if(this->JudgeHorse(lrow2, lcol2, selectid)){
                if( !this->isChess(lrow2, lcol2) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                    this->StepsPush(selectid, lrow2, lcol2, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(lrow2,lcol2),lrow2,lcol2,selectid)){
            this->StepsPush(selectid,lrow2,lcol2,test);
        }

    }

    if(lrow3 < 10 && lcol3 >= 0){
        if(judge_score){
            if(this->JudgeHorse(lrow3, lcol3, selectid)){
                if( !this->isChess(lrow3, lcol3) ){
                    this->Flexibility_Chess[selectid]++;
                }else{
                     this->StepsPush(selectid, lrow3, lcol3, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(lrow3,lcol3),lrow3,lcol3,selectid)){
            this->StepsPush(selectid,lrow3,lcol3,test);
        }

    }

    if(lrow4 < 10 && lcol4 < 9){
        if(judge_score){
            if(this->JudgeHorse(lrow4, lcol4, selectid)){
                if( !this->isChess(lrow4, lcol4) ){
                    this->Flexibility_Chess[selectid]++;

                }else{
                     this->StepsPush(selectid, lrow4, lcol4, test);
                }

            }
        }
        else if(this->MoveJudge(this->GetId(lrow4,lcol4),lrow4,lcol4,selectid)){
            this->StepsPush(selectid,lrow4,lcol4,test);
        }

    }
}

void SingleGame::Find_ProSolidersSteps(Step *&test, const int & selectid, const bool & judge_score){
    int row = this->_stone[selectid]._row;
    int col = this->_stone[selectid]._col;
    int start,last;
    if(this->_stone[selectid]._red){
        start = 7;
        last = 10;
    }else{
        start = 0;
        last = 3;
    }

    for(int i = start;i < last;i++){
        for(int j = 3;j < 6;j++){
            if(i != row || j != col){
                if(judge_score){
                    if(this->JudgeProSolider(i, j, selectid)){
                        if( !this->isChess(i, j) ){
                            this->Flexibility_Chess[selectid]++;
                        }else{
                            this->StepsPush(selectid, i, j, test);
                        }

                    }
                }
                else if(this->MoveJudge(this->GetId(i,j),i,j,selectid)){
                    this->StepsPush(selectid,i,j,test);
                }
            }
        }
    }

}

void SingleGame::Find_SolidersSteps(Step *&test, const int & selectid, const bool & judge_score){
    int row = this->_stone[selectid]._row;
    int col = this->_stone[selectid]._col;

    if(this->_stone[selectid]._red){
        int row1 = row - 1;
        int col1 = col;
        if(row1 >= 0){
            if(judge_score){
                if(this->JudgeSolider(row1, col1, selectid)){
                    if( !this->isChess(row1, col1) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row1, col1, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row1,col1),row1,col1,selectid)){
                this->StepsPush(selectid,row1,col1,test);
            }
        }
    }else{
        int row1 = row + 1;
        int col1 = col;
        if(row1 < 10){
            if(judge_score){
                if(this->JudgeSolider(row1, col1, selectid)){
                    if( !this->isChess(row1, col1) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row1, col1, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row1,col1),row1,col1,selectid)){
                this->StepsPush(selectid,row1,col1,test);
            }
        }
    }

    if((row < 5 && this->_stone[selectid]._red) || (row > 4 && !this->_stone[selectid]._red)){
        int row2 = row;
        int col2 = col - 1;
        int row3 = row;
        int col3 = col + 1;
        if(col2 >= 0){
            if(judge_score){
                if(this->JudgeSolider(row2, col2, selectid)){
                    if( !this->isChess(row2, col2) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row2, col2, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row2,col2),row2,col2,selectid)){
                this->StepsPush(selectid,row2,col2,test);
            }
        }
        if(col3 < 9){
            if(judge_score){
                if(this->JudgeSolider(row3, col3, selectid)){
                    if( !this->isChess(row3, col3) ){
                        this->Flexibility_Chess[selectid]++;
                    }else{
                        this->StepsPush(selectid, row3, col3, test);
                    }

                }
            }
            else if(this->MoveJudge(this->GetId(row3,col3),row3,col3,selectid)){
                this->StepsPush(selectid,row3,col3,test);
            }
        }
    }

}















