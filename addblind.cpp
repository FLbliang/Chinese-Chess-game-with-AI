#include "addblind.h"
#include <QPainter>
#include <ctime>
#include <QString>
#include <QDebug>

#define cout qDebug()

AddBlind::AddBlind()
{
    srand((unsigned)time(NULL));
    this->resize(600,600);


    this->initTypes(this->red_types);
    this->initTypes(this->black_types);

    for(int i = 0; i < 32; i++){
        this->haveMove[i] = false;
    }

    this->isChessMove = false;
    this->current_boards = this->Boards;
    this->isMove = false;


}

void AddBlind::Click(const int &clickid, const int &row, const int &col){

    Board::Click(clickid, row, col);
    if(this->isChessMove){

        bool get = false;
        int type;
        int id = this->Board_record[this->to_row][this->to_col];
        bool red = this->_stone[id]._red;
        if(this->haveEat()){
            if(this->deadId != -1 && this->haveMove[this->deadId]){
                switch(this->dead_type){
                case Stone::CHE: type = 0;break;
                case Stone::MA: type = 1; break;
                case Stone::PAO: type = 2;break;
                case Stone::SHI: type = 3;break;
                case Stone::XIANG: type = 4;break;
                case Stone::BING: type = 5;break;
                default:return;
                }
                this->updateText(type);
                this->isChessMove = false;
                this->deadId = -1;
            }else /*if(!this->haveMove[this->deadId])*/ {
                if(this->_stone[this->deadId]._type == Stone::JIANGE){
                    return ;
                }
                while(!get){
                    type = rand() % 6;

                    if(this->judgeHaveType(this->_stone[this->deadId]._red, type)){
                       this->_stone[this->deadId]._red ? this->red_types[type]-- : this->black_types[type]--;
                       this->updateText(type);
                       get = true;
                    }
                }
                get = false;

            }

        }

           if(this->haveMove[id] || this->_stone[id]._type == Stone::JIANGE){
               this->isChessMove = false;
                 return;
            }


        while(!get){
            type = rand() % 6;
            if(this->judgeHaveType(red, type)){
                red ? this->red_types[type]-- : this->black_types[type]--;
//                this->test(red);
                this->giveType(id, type);
                get = true;
            }
        }

        this->haveMove[id] = true;
        this->isChessMove = false;

    }
}

void AddBlind::test(bool red){
    if(red){
        int sum = 0;
        cout << "red:";
        for(int i = 0;i < 6; i++){
            sum += this->red_types[i];
        }
        cout << sum;
    }else{
        int sum = 0;
        cout << "black:";
        for(int i = 0;i < 6;i++){
            sum += this->black_types[i];
        }
        cout << sum;
    }
}

bool AddBlind::haveEat(){
    bool flag = this->current_boards != this->Boards;
    this->current_boards = this->Boards;
    return flag;
}

void AddBlind::updateText(const int &index){
    QString str;
    switch(index){
    case 0:str = "你一支真车队伍被灭了!!!";break;
    case 1:str = "你一支真马队伍被灭了!!!";break;
    case 2:str = "你一支真炮队伍被灭了!!!";break;
    case 3:str = "你一支真护卫队伍被灭了!!!";break;
    case 4:str = "你一支真象队伍被灭了!!!";break;
    case 5:str = "你一支真士兵队伍被灭了!!!";break;
    }

    this->label->setText(str);
}

void AddBlind::giveType(const int &id, const int &index){
    switch(index){
    case 0:this->_stone[id]._type = Stone::CHE;break;
    case 1:this->_stone[id]._type = Stone::MA;break;
    case 2:this->_stone[id]._type = Stone::PAO;break;
    case 3:this->_stone[id]._type = Stone::SHI;break;
    case 4:this->_stone[id]._type = Stone::XIANG;break;
    case 5:this->_stone[id]._type = Stone::BING;break;
    }
}

bool AddBlind::judgeHaveType(bool red, const int & index){
    return red ? this->red_types[index] > 0 : this->black_types[index] > 0;
}

void AddBlind::drawStone(QPainter &painter, const int &id){
    if(this->_stone[id]._dead){
        return;
    }

    QPoint c = center(id);
    QPen pen;
    QPen flag_pen;
    QFont font;
    font.setWeight(QFont::Bold);
    pen.setWidth(3);

    QRect rect = QRect(c.rx() - this->_r, c.ry() - this->_r, 2*this->_r, 2*this->_r);
    painter.setBrush(QBrush(QColor(255, 255, 0)));
      if(_stone[id]._red){
        pen.setColor(Qt::red);
    }else{
          pen.setColor(Qt::black);

    }

    if(this->_selectid == id){
        painter.setBrush(Qt::gray);
    }
    painter.setPen(pen);
    painter.drawEllipse(center(id),_r,_r);
    QString middle = _stone[id].getText(this->_stone[id]._red);
    painter.setFont(font);
    painter.drawText(rect, middle,QTextOption(Qt::AlignCenter));
    if(this->haveMove[id]){
        return ;
    }
    pen.setWidth(8);
    QBrush no;
    painter.setBrush(no);
    painter.setPen(pen);
    painter.drawEllipse(center(id), this->_r, this->_r);



}

void AddBlind::initTypes(QMap<int, int> &types){
    types[0] = 2; // cars
    types[1] = 2; // horses
    types[2] = 2; //cans
    types[3] = 2; //pro_soliders
    types[4] = 2; //elephants
    types[5] = 5; // soliders

}


























