#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <ctime>
#include "Stone.h"
#include "Step.h"

using namespace std;

enum TYPE{ exact, lower_bound, upper_bound };

struct keyRecord{
    int type;
    uint64_t checkNumber;
    int level;
    long int value;
};

class HashTable
{
public:
    HashTable();

    uint32_t hashKey32[32][10][10];
    uint64_t hashKey64[32][10][10];

    keyRecord *tableRecord[2];
    uint32_t hashSum32;
    uint64_t hashSum64;

    uint32_t get32keys();
    uint64_t get64keys();
    void initHashKeys();
    void calculateInitKeys(const int Board_record[10][10]);
    void hashFakeMove(Step *moveStep);
    void hashFakeRelase(Step *relaseStep);

    long int lookUpHashTable(const long int & alpha, const long int & beta, const int & level, const int & tableNo);
    void tablePush(int type, const long int & value, const int & level, const int & tableNo);

};

#endif // HASHTABLE_H
