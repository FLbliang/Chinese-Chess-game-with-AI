#include <QApplication>
#include "Board.h"
#include "Singlegame.h"
#include "form.h"
#include "addblind.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool flag = true;
    while(flag){
        Form *test = new Form(flag);
        test->show();
        a.exec();
        delete test;
    }

//    AddBlind game;
//    game.show();
//    a.exec();


    return 0;
}
