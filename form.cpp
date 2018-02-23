#include "form.h"
#include "ui_form.h"
#include <QPushButton>
#include <QFont>
int i = 0;

Form::Form(bool &flag)
{
    ui = new Ui::Form;
    ui->setupUi(this);
    this->resize(600,600);
    connect(this->back,&QPushButton::clicked,[=]{
        this->Back_board();
        this->Back_board();
        if(this->steps->next){
            this->from_row = this->steps->next->prev_row;
            this->from_col = this->steps->next->prev_col;
            this->to_row = this->steps->next->move_row;
            this->to_col = this->steps->next->move_col;
        }else{
            this->isMove = false;
        }
        this->cal--;
    });

    connect(this->exit,&QPushButton::clicked,[&]{
        this->close();
        flag = false;
    });

    connect(this->start,&QPushButton::clicked,[=]{
        this->close();
        i = 1;
        this->isMove = false;
    });

    connect(this,&QPushButton::windowIconChanged,[=]{
        this->close();
    });
    if(i){
        return;
    }
   // this->TestBug();
   // this->level = 5;
//    this->ComputerMove();
//    this->is_red = true;
//   int id = 9;
//     this->Board_record[this->_stone[id]._row][this->_stone[id]._col] = -1;
//    this->_stone[id]._row = 2;
//    this->_stone[id]._col = 4;
//    this->Board_record[this->_stone[id]._row][this->_stone[id]._col] = 1;



}

Form::~Form()
{
    delete ui;
}

void Form::TestBug(){
    this->Boards = 14;
    for(int i = 0; i < 32; i++){
        this->_stone[i]._dead = true;
        this->Board_record[this->_stone[i]._row][this->_stone[i]._col] = -1;
    }



    struct Test{
        int id;
        int row;
        int col;
    }rec[14] = {
    {2, 0, 2},
    {3, 1, 4},
    {4, 0, 3},
    {5, 0, 5},
    {8, 4, 3},
    {10, 9, 7},
    {11, 3, 0},
    {13, 3, 4},
    {15, 3, 8},
    {7, 4, 7},
    {16, 0, 1},
    {31, 6, 0},
    {17, 2, 4},
    {20, 9, 4},
    };

    for(int i = 0; i < 14; i++){
        this->_stone[rec[i].id]._dead = false;
        this->_stone[rec[i].id]._row = rec[i].row;
        this->_stone[rec[i].id]._col = rec[i].col;
        this->Board_record[rec[i].row][rec[i].col] = rec[i].id;
    }

}





















