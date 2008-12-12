#include "TreeItem.h"

TreeItem::TreeItem( QTreeWidget* parent )
    : QTreeWidgetItem( parent ) {
}

TreeItem::TreeItem( QTreeWidgetItem* parent )
    : QTreeWidgetItem( parent ) {
}

TreeItem::~TreeItem() {
}

bool TreeItem::isFolder() const {
    return( false );
}

void TreeItem::update() {
}

void TreeItem::setOpen( bool /*isOpened*/ ) {
}
