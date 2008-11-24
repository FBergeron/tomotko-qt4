#ifndef SCROLLABLE_MULTILINEEDIT_H
#define SCROLLABLE_MULTILINEEDIT_H

#include <iostream>
#include <QTextEdit>

using namespace std;

class ScrollableMultiLineEdit : public QTextEdit {
   
   Q_OBJECT

public:

    ScrollableMultiLineEdit( QWidget* parent = 0 );
   ~ScrollableMultiLineEdit();

public slots:

    void scrollUp();
    void scrollDown();
    void scrollPageUp();
    void scrollPageDown();

};

#endif
