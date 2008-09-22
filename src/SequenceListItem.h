#ifndef SEQUENCE_LIST_ITEM_H
#define SEQUENCE_LIST_ITEM_H

#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include "Sequence.h"

class SequenceListItem : public QTreeWidgetItem {

public:

    SequenceListItem( QTreeWidget* parent, const QString& text, Sequence sequence );
    ~SequenceListItem();

    Sequence& getSequence();

    void setOn( bool isOn );

private:

    Sequence sequence;

};

#endif

