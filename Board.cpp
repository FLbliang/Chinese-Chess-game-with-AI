#include "Board.h"
#include <Qpainter>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QMouseEvent>
#include <QLabel>
#include <math.h>
#include <QDebug>

#define cout qDebug()

Board::Board(QWidget *parent) : QWidget(parent)
{

    for(int i = 0;i < 10;i++){
        for(int j = 0;j < 10;j++){
            this->Board_record[i][j] = -1;
        }
    }

    for(int i = 0;i < 32;i++){
        this->_stone[i].init(i);
        this->Board_record[this->_stone[i]._row][this->_stone[i]._col] = this->_stone[i]._id;
    }
    this->_selectid = -1;
    this->is_red =  true;

    this->Red_killed = false;
    this->Black_killed = false;
    this->Boards = 32;
    this->start = new QPushButton(this);
    this->back = new QPushButton(this);
    this->exit = new QPushButton(this);
    this->label = new QLabel(this);
    this->back->setStyleSheet("QPushButton{"
                            "font-weight:700;"
                            "background-color:blue;"
                            "color:white;"
                            "border-radius:20px;"
                            "padding:5px;"
                            "}");

    this->label->setStyleSheet("QLabel{ background-color: grey;"
                               "color: white;"
                               "border: 1px solid blue;"
                               "border-radius: 6px;"
                               "}");



    this->start->setStyleSheet(this->back->styleSheet());
    this->exit->setStyleSheet(this->back->styleSheet());
    this->start->setText(tr("start"));
    this->back->setText(tr("back"));
    this->exit->setText(tr("exit"));

    QFont font;
    font.setWeight(QFont::Bold);
    this->start->setFont(font);
    this->back->setFont(font);
    this->exit->setFont(font);
    this->label->setFont(font);


}

int Board::GetId(const int & row, const int & col){
    return this->Board_record[row][col];
}

void Board::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPen pen;
    pen.setWidth(5);
    painter.setPen(pen);
    int d = this->width()/12.6;
    this->_r = d/2;
    //draw cross line
    for(int i = 1;i <= 10;i++){
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //draw vertical line
    for(int i = 1;i <= 9;i++){
        if(i == 1 || i == 9){
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        }else{
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }
    //draw nine grids
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    for(int i = 0;i < 32;i++){
        drawStone(painter,i);
    }

    int wid = this->width()/6;
    int hei = this->width()/12.6 * 10 / 6;
    int h = this->width()/12.6 * 10;
    int move_x = this->width()-wid - 15;
    this->start->resize(wid, hei);
    this->back->resize(wid, hei);
    this->exit->resize(wid,hei);
    this->start->move(move_x, h/11.5);
    this->back->move(move_x, 0.5*(h/1.2 + h/11.5));
    this->exit->move(move_x, h/1.2);
    this->label->resize(this->width()-2*d, 1.5*d);
    this->label->move(d, 11*d);
//     if(this->is_red){
//        label->setText(tr("It's your turn now, my friend !!! "));
//    }else{
//        label->setText(tr("I am calculating, please wait for a minute !"));
//    }

    if(this->Red_killed){
        this->label->setText(tr("You failed !!"));
    }else if(this->Black_killed){
        this->label->setText(tr("Congratulation! You are the winner!"));
    }

    if(this->isMove){
        QPoint from_point = this->center(this->from_row, this->from_col);
        QPoint to_point = this->center(this->to_row, this->to_col);
       // from_point.rx() += pen.width()/2;
       // from_point.ry() += pen.width()/2;

        pen.setColor(Qt::green);
        painter.setPen(pen);
        QBrush no;
        painter.setBrush(no);
        painter.drawEllipse(from_point, this->_r, this->_r);
        painter.drawEllipse(to_point, this->_r, this->_r);
    }

}

void Board::mouseReleaseEvent(QMouseEvent *ev){
    QPoint pt = ev->pos();

    int d = 2*this->_r;
    int row1 = pt.ry()/d;
    int row2 = row1 - 1;
    int col1 = pt.rx()/d;
    int col2 = col1 - 1;
    QPoint pos1 = center(row1,col1);
    QPoint pos2 = center(row2,col2);
    QPoint pos3 = center(row1,col2);
    QPoint pos4 = center(row2,col1);
    int row,col;
    int distance1 = pow(abs(pt.rx() - pos1.rx()),2) + pow(abs(pt.ry() - pos1.ry()),2);
    int distance2 = pow(abs(pt.rx() - pos2.rx()),2) + pow(abs(pt.ry() - pos2.ry()),2);
    int distance3 = pow(abs(pt.rx() - pos3.rx()),2) + pow(abs(pt.ry() - pos3.ry()),2);
    int distance4 = pow(abs(pt.rx() - pos4.rx()),2) + pow(abs(pt.ry() - pos4.ry()),2);
    int limit = this->_r*this->_r;
    if(distance1 <= limit && distance1 >= 0){
        row = row1;
        col = col1;
    }else if(distance2 <= limit && distance2 >= 0){
        row = row2;
        col = col2;
    }else if(distance3 <= limit && distance3 >= 0){
        row = row1;
        col = col2;
    }else if(distance4 >= 0){
        row = row2;
        col = col1;
   }
   if(row < 0 || col < 0 || row > 9 || col > 8){
       this->_selectid = -1;
       update();
       return;
   }

   int clickid = this->Board_record[row][col];
   this->Click(clickid,row,col);

}

void Board::Click(const int & clickid, const int & row, const int & col){
    if(this->_selectid == -1){
        if(clickid == -1){
            return;
        }else if(this->is_red == this->_stone[clickid]._red){
            this->_selectid = clickid;

            update();
        }
    }else if(MoveJudge(clickid, row, col, this->_selectid)){
           this->Board_record[this->_stone[this->_selectid]._row][this->_stone[this->_selectid]._col] = -1;
           this->from_row = this->_stone[this->_selectid]._row;
           this->from_col = this->_stone[this->_selectid]._col;
           this->_stone[this->_selectid]._row = row;
           this->_stone[this->_selectid]._col = col;
          if(clickid != -1){
              this->_stone[clickid]._dead = true;
              this->dead_type = this->_stone[clickid]._type;
              this->deadId = clickid;
              --this->Boards;
          }
          this->Board_record[row][col] = this->_selectid;
          this->is_red = !this->is_red;
          this->isMove = true;
          this->isChessMove = true;
          this->to_col = col;
          this->to_row = row;
          update();
          this->_selectid = -1;
    }else{
        this->_selectid = -1;

        update();
    }
}

QPoint Board::center(const int & row, const int & col){
    QPoint rec;
    rec.rx() = (col+1)* 2*_r + 5;
    rec.ry() = (row+1)* 2*_r + 5;
    return rec;
}

QPoint Board::center(const int & id){
    return center(_stone[id]._row, _stone[id]._col);
}

void Board::drawStone(QPainter &painter, const int & id){
    if(this->_stone[id]._dead){
        return;
    }

    QPoint c = center(id);
    QPen pen;
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

}

bool Board::MoveJudge(const int & killid, const int & row, const int & col, const int & selectid){

    if(killid != -1){
        if(this->_stone[killid]._red == this->_stone[selectid]._red){
            return false;
        }
    }

    switch(this->_stone[selectid]._type){
    case Stone::JIANGE:
        return JudgeHeader(row, col, selectid);
    case Stone::CHE:
        return JudgeCar(row, col, selectid);
    case Stone::PAO:
        return JudgeCan(killid,row, col, selectid);
    case Stone::MA:
        return JudgeHorse(row, col, selectid);
    case Stone::BING:
        return JudgeSolider(row, col, selectid);
    case Stone::SHI:
        return JudgeProSolider(row, col, selectid);
    case Stone::XIANG:
        return JudgeElephant(row, col, selectid);
    default:return false;
    }

}

int Board::Relation(const int & row, const int & col, const int & selectid){
    return abs(this->_stone[selectid]._row - row)*10 + abs(this->_stone[selectid]._col - col);
}

int Board::Have_Boards(const int & row, const int & col, const int & selectid){
    int sum = 0;
    if(row == this->_stone[selectid]._row){
        int col1,col2;
        if(this->_stone[selectid]._col < col){
            col1 = this->_stone[selectid]._col;
            col2 = col;
        }else{
            col2 = this->_stone[selectid]._col;
            col1 = col;
        }

        for(int i = col1+1;i < col2;i++){
            if(this->Board_record[row][i] != -1){
                sum++;
            }
        }
    }else{
        int row1,row2;
        if(this->_stone[selectid]._row < row){
            row1 = this->_stone[selectid]._row;
            row2 = row;
        }else{
            row2 = this->_stone[selectid]._row;
            row1 = row;
        }

        for(int i = row1+1;i < row2;i++){
            if(this->Board_record[i][col] != -1){
                sum++;
            }
        }
    }
    return sum;
}

bool Board::JudgeHeader(const int & row, const int & col, const int & selectid){

    if(col < 3 || col > 5){
        return false;
    }
    if(this->_stone[selectid]._red){
        if(row < 3 && col == this->_stone[selectid]._col){
            for(int i = row + 1; i < this->_stone[selectid]._row; i++){
                if(this->Board_record[i][col] != -1){
                    return false;
                }
            }
            return true;
        }
        else if(row < 7){
            return false;
        }

    }else{
        if(row > 6 && col == this->_stone[selectid]._col){

            for(int i = this->_stone[selectid]._row + 1; i < row; i++){
                if(this->Board_record[i][col] != -1){
                    return false;
                }
            }

            return true;
        }
        else if(row > 2){
            return false;
        }
    }
    int rel = this->Relation(row,col,selectid);
    if(rel != 10 && rel != 1){
        return false;
    }
    return true;
}

bool Board::JudgeCar(const int & row, const int & col, const int & selectid){

    if(row != this->_stone[selectid]._row && col != this->_stone[selectid]._col){
        return false;
    }
    int sum = this->Have_Boards(row,col,selectid);
    if(sum){
        return false;
    }
    return true;
}

bool Board::JudgeCan(const int & killid, const int & row, const int & col, const int & selectid){
    if(row != this->_stone[selectid]._row && col != this->_stone[selectid]._col){
        return false;
    }
    int sum = this->Have_Boards(row,col,selectid);
    if(sum > 1){
        return false;
    }else if(!sum && killid != -1){
        return false;
    }else if(sum == 1 && killid == -1){
        return false;
    }
    return true;
}

bool Board::JudgeHorse(const int & row, const int & col, const int & selectid){

    int rec = this->Relation(row,col,selectid);
    if(rec != 21 && rec != 12){
        return false;
    }

    if(rec == 21){
        int row2 = this->_stone[selectid]._row + row;
       row2 /= 2;
       if(this->Board_record[row2][this->_stone[selectid]._col] != -1){
           return false;
       }

    }else{
        int col2 = this->_stone[selectid]._col + col;
        col2 /= 2;
        if(this->Board_record[this->_stone[selectid]._row][col2] != -1){
            return false;
        }

    }
    return true;
}

bool Board::JudgeSolider(const int & row, const int & col, const int & selectid){
    int row1 = this->_stone[selectid]._row;
    int rec = this->Relation(row,col,selectid);
    if(this->_stone[selectid]._red){
        if(row1 >= 5){
            if(rec != 10){
                return false;
            }
        }else{
            if(rec != 10 && rec != 1){
                return false;
            }
        }
        if(row > row1){
            return false;
        }
    }else{
        if(row1 <= 4){
            if(rec != 10){
                return false;
            }
        }else{
            if(rec != 10 && rec != 1){
                return false;
            }
        }
        if(row < row1){
            return false;
        }
    }
    return true;
}

bool Board::JudgeProSolider(const int & row, const int & col, const int & selectid){

    if(col < 3 || col > 5){
        return false;
    }
    if(this->_stone[selectid]._red){
        if(row < 7){
            return false;
        }

    }else{
        if(row > 2){
            return false;
        }
    }
    int rel = this->Relation(row,col,selectid);
    if(rel != 11){
        return false;
    }
    return true;
}

bool Board::JudgeElephant(const int & row, const int & col, const int & selectid){

    if(this->_stone[selectid]._red){
        if(row < 5){
            return false;
        }

    }else{
        if(row > 4){
            return false;
        }
    }
    int rec = this->Relation(row,col,selectid);
    if(rec != 22){
        return false;
    }
    int row2 = this->_stone[selectid]._row + row;
    int col2 = this->_stone[selectid]._col + col;
    row2 /= 2;
    col2 /= 2;
    if(this->Board_record[row2][col2] != -1){
        return false;
    }
    return true;
}





















