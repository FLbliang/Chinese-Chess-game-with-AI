#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "Singlegame.h"

namespace Ui {
class Form;
}

class Form : public SingleGame
{
    Q_OBJECT

public:

    Form(bool & flag);
    ~Form();

private:
    Ui::Form *ui;
    void TestBug();
};

#endif // FORM_H
