#ifndef SCROLLABLE_LINEEDIT_H
#define SCROLLABLE_LINEEDIT_H

#include <iostream>
#include <QLineEdit>

using namespace std;

class ScrollableLineEdit : public QLineEdit {

    Q_OBJECT

public:

    ScrollableLineEdit( QWidget* parent = 0 );
    ~ScrollableLineEdit();

public slots:

    void scrollLeft();
    void scrollRight();

};

#endif // QLINEEDIT_H
