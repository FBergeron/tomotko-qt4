#include "SequenceListItem.h"

SequenceListItem::SequenceListItem( QTreeWidget* parent, const QString& text, Sequence sequence )
    : QTreeWidgetItem( parent ), sequence( sequence ) {
    setText( 0, text );
}

SequenceListItem::~SequenceListItem() {
}

void SequenceListItem::setOn( bool isOn ) {
    setCheckState( 0, isOn ? Qt::Checked : Qt::Unchecked );
}

Sequence& SequenceListItem::getSequence() {
    return( sequence );
}
