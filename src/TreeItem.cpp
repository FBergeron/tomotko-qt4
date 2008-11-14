#include "TreeItem.h"

TreeItem::TreeItem( QTreeWidget* parent/*QListView* parent*//*, const QStringList& texts*//*, QCheckListItem::Type type*/ )
    : QTreeWidgetItem( parent/*, texts*/ )/*QCheckListItem( parent, text, type )*/ {
}

TreeItem::TreeItem( QTreeWidgetItem* parent/*QListViewItem* parent*//*, const QStringList& texts*//*, QCheckListItem::Type type*/ )
    : QTreeWidgetItem( parent/*, texts*/ )/*QCheckListItem( parent, text, type )*/ {
}

TreeItem::~TreeItem() {
}

bool TreeItem::isFolder() const {
    return( false );
}

void TreeItem::update() {
}

void TreeItem::setOpen( bool isOpened ) {
}
