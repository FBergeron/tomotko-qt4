/****************************************************************************
* HistoryField.h
* 
* The code of this class is borrowed/inspired from qhistorylineedit.h found in 
* ZTen project.
****************************************************************************/


#ifndef HISTORY_FIELD_H
#define HISTORY_FIELD_H

using namespace std;

#include <iostream>
#include <QComboBox>
#include <QWidget>

class HistoryField : public QComboBox {

    Q_OBJECT

public:

    HistoryField( QWidget* parent = 0 );
    void activate();

};

#endif
