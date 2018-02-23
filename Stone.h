#ifndef STONE_H
#define STONE_H


#include <QString>


class Stone
{
public:
    Stone();
    enum TYPE{JIANGE, CHE, PAO, MA, BING, SHI, XIANG};
    int _row;
    int _col;
    int _id;

    bool _dead;
    bool _red;
    TYPE _type;

    void init(const int & id){
        this->_dead = false;
        this->_id = id;
        this->_red = id >= 16;

        struct Pos{
            int row,col;
            Stone::TYPE type;
        }pos[16] = {
        {0, 0, Stone::CHE},
        {0, 1, Stone::MA},
        {0, 2, Stone::XIANG},
        {0, 3, Stone::SHI},
        {0, 4, Stone::JIANGE},
        {0, 5, Stone::SHI},
        {0, 6, Stone::XIANG},
        {0, 7, Stone::MA},
        {0, 8, Stone::CHE},

        {2, 1, Stone::PAO},
        {2, 7, Stone::PAO},
        {3, 0, Stone::BING},
        {3, 2, Stone::BING},
        {3, 4, Stone::BING},
        {3, 6, Stone::BING},
        {3, 8, Stone::BING}
        };

        if(id < 16){

            this->_row = pos[id].row;
            this->_col = pos[id].col;
            this->_type = pos[id].type;
        }else{
            this->_row = 9 - pos[id-16].row;
            this->_col = 8 - pos[id-16].col;
            this->_type = pos[id-16].type;
        }
    }

    QString getText(bool isRed){

        if(!isRed){
            switch(this->_type){
            case 0:return "将";
            case 1:return "車";
            case 2:return "炮";
            case 3:return "馬";
            case 4:return "卒";
            case 5:return "士";
            case 6:return "象";
            default:return "wrong";
            }
        }else{
            switch(this->_type){
            case 0:return "帥";
            case 1:return "車";
            case 2:return "炮";
            case 3:return "馬";
            case 4:return "兵";
            case 5:return "仕";
            case 6:return "相";
            default:return "wrong";
            }
        }

    }

};

#endif // STONE_H
