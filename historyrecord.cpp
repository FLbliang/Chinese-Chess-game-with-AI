#include "historyrecord.h"

#include <QDebug>

#define cout qDebug()

HistoryRecord::HistoryRecord()
{
  }

void HistoryRecord::resetHistoryTable(){
    memset(this->historyTable, 0, 8100*4);

}

void HistoryRecord::pushHistoryTable(Step *step, int level){

    int fromRecord = step->prev_row*9 + step->prev_col;
    int toRecord = step->move_row*9 + step->move_col;
    this->historyTable[fromRecord][toRecord] += 2 << level;
}

int HistoryRecord::getHistoryScore(Step *step){
    int fromRecord = step->prev_row*9 + step->prev_col;
    int toRecord = step->move_row*9 + step->move_col;
    return this->historyTable[fromRecord][toRecord];
}


void HistoryRecord::sort(Step *&test){

    Step *head;
    Step *mid;
    Step *tail;
    Step *tmp;

    Step *get = test->next;
    head = test;
    head->next = NULL;
    tail = head->next;

    int headScore = 0;
    int tailScore = 0;
    int getScore = 0;

    while(get){
        getScore = this->getHistoryScore(get);
        tmp = get->next;
        if(head->next){
            headScore = this->getHistoryScore(head->next);
        }

        if(tail){
            tailScore = this->getHistoryScore(tail);
        }

        if(getScore >= headScore){
            get->next = head->next;
            head->next = get;
        }else if(getScore <= tailScore){
            get->next = tail->next;
            tail->next = get;
            tail = get;
        }else {
            mid = head->next;
            while(mid->next && getScore < this->getHistoryScore(mid->next)){
                mid = mid->next;
            }
            get->next = mid->next;
            mid->next = get;
        }

        if(tail == NULL){
            tail = head->next;
        }
        get = tmp;
    }




}
















