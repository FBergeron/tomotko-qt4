#ifndef TERM_LIST_ITEM_H
#define TERM_LIST_ITEM_H

#include <iostream>
#include <QApplication>
#include <QColorGroup>
#include <QMap>
#include <QPainter>
#include <QTreeWidgetItem>
#include <QWidget>
#include "Term.h"

class TermListItem : public QTreeWidgetItem {

public:

    TermListItem( QTreeWidget* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, bool isAltShown = true );
    ~TermListItem();

    Term* getTerm();
    void updateUi();
    virtual void setOn( bool isOn );
    void setFont( int column, const QFont& font ); 
    QFont font( int column ) const;

private:

    Term*               term;
    QString             firstLanguage;
    QString             testLanguage;
    bool                altShown;

    QMap<int,QFont>     fonts;

};

#endif
