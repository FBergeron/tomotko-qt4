#include "FolderTreeItem.h"
#include "icons/closedFolder.xpm"
#include "icons/openedFolder.xpm"

FolderTreeItem::FolderTreeItem( QTreeWidget* parent, Folder* folder, Preferences& prefs, PropertiesPanel* propsPanel )
    : TreeItem( (QTreeWidget*) parent ), folder( folder ), prefs( prefs ), propsPanel( propsPanel ) {
    // It's important to setCheckState() first.  Otherwise, if a change is made too soon, 
    // it may reset the markedForStudy property by an event.
    setCheckState( 0, folder->isMarkedForStudy() ? Qt::Checked : Qt::Unchecked );
    setText( 0, folder->getTitle() );
    setOpen( prefs.isFolderOpen( folder->getId() ) );
}

FolderTreeItem::FolderTreeItem( QTreeWidgetItem* parent, Folder* folder, Preferences& prefs, PropertiesPanel* propsPanel )
    : TreeItem( parent ), folder( folder ), prefs( prefs ), propsPanel( propsPanel ) {
    // It's important to setCheckState() first.  Otherwise, if a change is made too soon, 
    // it may reset the markedForStudy property by an event.
    setCheckState( 0, folder->isMarkedForStudy() ? Qt::Checked : Qt::Unchecked );
    setText( 0, folder->getTitle() );
}

FolderTreeItem::~FolderTreeItem() {
}

bool FolderTreeItem::isFolder() const {
    return( true );
}

void FolderTreeItem::setPropertiesPanel( PropertiesPanel* propsPanel ) {
    this->propsPanel = propsPanel;
}

void FolderTreeItem::setOpen( bool isOpened ) {
    // Ignore close event for root.
    if( parent() || isOpened ) {
        setExpanded( isOpened );
        updateIcon();
        prefs.setFolderOpen( folder->getId(), isOpened );
    }
}

void FolderTreeItem::setOn( bool isOn ) {
    // We clear the selection in order to effectively reset it after
    // changing the state of the item.  This is needed because the
    // item is updated when a selection event occurs.
    //bool isUpdateHackEnabled = listView()->isUpdatesEnabled();
    //if( isUpdateHackEnabled )
    //    listView()->clearSelection();
    //setCheckState( 0, isOn ? Qt::Checked : Qt::Unchecked );
    folder->setMarkedForStudy( isOn );
    // Force updating the event.
    //if( isUpdateHackEnabled )
    //    listView()->setSelected( this, true );
}

Folder* FolderTreeItem::getFolder() {
    return( folder );
}

void FolderTreeItem::update() {
    if( propsPanel )
        propsPanel->updateCounters();
}

void FolderTreeItem::updateIcon() {
    setIcon( 0, QIcon( QPixmap( isExpanded() ? ZPIXMAP( openedFolder_xpm ) : ZPIXMAP( closedFolder_xpm ) ) ) );
}
