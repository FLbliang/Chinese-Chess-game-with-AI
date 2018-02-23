#include "hashtable.h"

HashTable::HashTable()
{
    this->initHashKeys();
}

void HashTable::initHashKeys(){
    srand((unsigned)time(NULL));

    for(int id = 0; id < 32; id++){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                this->hashKey32[id][i][j] = this->get32keys();
                this->hashKey64[id][i][j] = this->get64keys();
            }
        }
    }

    for(int i = 0; i < 2; i++){
        this->tableRecord[i] = new keyRecord[1024*1024];
    }
}

uint32_t HashTable::get32keys(){
    return rand() ^ ((uint32_t)rand() << 15) ^ ((uint32_t)rand() << 30);
}

uint64_t HashTable::get64keys(){
    return rand() ^ ((uint64_t)rand() << 15)
                  ^ ((uint64_t)rand() << 30)
                  ^ ((uint64_t)rand() << 45)
                  ^ ((uint64_t)rand() << 60);

}

void HashTable::calculateInitKeys(const int Board_record[10][10]){

    this->hashSum32 = 0;
    this->hashSum64 = 0;
    int id;

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(Board_record[i][j] != -1){
                id = Board_record[i][j];
                this->hashSum32 = this->hashSum32 ^ this->hashKey32[id][i][j];
                this->hashSum64 = this->hashSum64 ^ this->hashKey64[id][i][j];
            }
        }
    }
}

void HashTable::hashFakeMove(Step *moveStep){
    int fromRow = moveStep->prev_row;
    int fromCol = moveStep->prev_col;
    int toRow = moveStep->move_row;
    int toCol = moveStep->move_col;
    int moveId = moveStep->selectid;
    int killId = moveStep->killid;

    this->hashSum32 = this->hashSum32 ^ this->hashKey32[moveId][fromRow][fromCol];
    this->hashSum64 = this->hashSum64 ^ this->hashKey64[moveId][fromRow][fromCol];

    if(killId != -1){
        this->hashSum32 = this->hashSum32 ^ this->hashKey32[killId][fromRow][fromCol];
        this->hashSum64 = this->hashSum64 ^ this->hashKey64[killId][fromRow][fromCol];
    }

    this->hashSum32 = this->hashSum32 ^ this->hashKey32[moveId][toRow][toCol];
    this->hashSum64 = this->hashSum64 ^ this->hashKey64[moveId][toRow][toCol];
}

void HashTable::hashFakeRelase(Step *relaseStep){
    int fromRow = relaseStep->prev_row;
    int fromCol = relaseStep->prev_col;
    int toRow = relaseStep->move_row;
    int toCol = relaseStep->move_col;
    int moveId = relaseStep->selectid;
    int killId = relaseStep->killid;

    this->hashSum32 = this->hashSum32 ^ this->hashKey32[moveId][toRow][toCol];
    this->hashSum64 = this->hashSum64 ^ this->hashKey64[moveId][toRow][toCol];

    this->hashSum32 = this->hashSum32 ^ this->hashKey32[moveId][fromRow][fromCol];
    this->hashSum64 = this->hashSum64 ^ this->hashKey64[moveId][fromRow][fromCol];

    if(killId != -1){
        this->hashSum32 = this->hashSum32 ^ this->hashKey32[killId][toRow][toCol];
        this->hashSum64 = this->hashSum64 ^ this->hashKey64[killId][toRow][toCol];
    }
}

long int HashTable::lookUpHashTable(const long & alpha, const long & beta, const int & level, const int & tableNo){

    int address = this->hashSum32 & 0xFFFFF;
    keyRecord *item = & this->tableRecord[tableNo][address];

    if(item->level >= level && item->checkNumber == this->hashSum64){
        switch(item->type){
        case TYPE::exact:return item->value;
        case TYPE::lower_bound:
            if(item->value >= beta){
                return item->value;
            }
            break;
        case TYPE::upper_bound:
            if(item->value <= alpha){
                return item->value;
            }
            break;
        }
    }

    return 676767;
}

void HashTable::tablePush(int type, const long int & value, const int & level, const int & tableNo){
    int address = this->hashSum32 & 0xFFFFF;
    keyRecord *item = & this->tableRecord[tableNo][address];

    item->checkNumber = this->hashSum64;
    item->level = level;
    item->type = type;
    item->value = value;
}














