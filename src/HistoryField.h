/****************************************************************************
* HistoryField.h
* 
* The code of this class is borrowed/inspired from qhistorylineedit.h found in 
* ZTen project.
****************************************************************************/


#ifndef HISTORY_FIELD_H
#define HISTORY_FIELD_H

#include <iostream>
#include <QComboBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QWidget>
#include "Util.h"

class HistoryField : public QComboBox {

    Q_OBJECT

public:

    HistoryField( QWidget* parent = 0 );
    void activate();
    bool isDigraphEnabled() const;

public slots:

    void setDigraphEnabled( bool isEnabled );

protected:

    virtual void keyPressEvent( QKeyEvent* event );

private:

    bool digraphEnabled;
    QString buffer;

};

#endif
