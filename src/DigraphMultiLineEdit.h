#ifndef DIGRAPH_MULTI_LINE_EDIT_H
#define DIGRAPH_MULTI_LINE_EDIT_H

#include <iostream>
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextEdit>
#include "Util.h"

class DigraphMultiLineEdit : public QTextEdit {

    Q_OBJECT

public:

    DigraphMultiLineEdit( QWidget* parent = 0 );
    ~DigraphMultiLineEdit();

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
