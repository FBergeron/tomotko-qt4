#ifndef RESULT_LIST_ITEM_H
#define RESULT_LIST_ITEM_H

#include <iostream>
#include <QColorGroup>
#include <QFont>
#include <QFontMetrics>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QObject>
#include <QPainter>
#include <QString>
#include <QWidget>
#include "Term.h"

using namespace std;

class ResultListItem : public QTreeWidgetItem {

public:

    ResultListItem( QTreeWidget* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, const QString& vocabTitle, const QString& location, bool isAltShown = true );
    ~ResultListItem();

    Term* getTerm();
    void updateUi();
    void setFont( int column, const QFont& font ); 
    QFont font( int column ) const;

private:

    Term*               term;
    QString             firstLanguage;
    QString             testLanguage;
    QString             vocabTitle;
    QString             location;
    bool                altShown;

    QMap<int,QFont>     fonts;

};

#endif
