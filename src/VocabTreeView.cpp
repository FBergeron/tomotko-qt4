#include "VocabTreeView.h"

VocabTreeView::VocabTreeView( Controller& controller )
    : QTreeWidget(), dirty( false ), controller( controller ) {
    connect( this, SIGNAL( itemExpanded( QTreeWidgetItem* ) ), this, SLOT( updateTreeItemIcon( QTreeWidgetItem* ) ) );
    connect( this, SIGNAL( itemCollapsed( QTreeWidgetItem* ) ), this, SLOT( updateTreeItemIcon( QTreeWidgetItem* ) ) );
}

VocabTreeView::~VocabTreeView() {
}

VocabTreeItem* VocabTreeView::currentItem() const {
    return( (VocabTreeItem*)QTreeWidget::currentItem() );
}

FolderTreeItem* VocabTreeView::addFolder( Folder* folder /*= NULL*/, QMap<int,Vocabulary>* vocabularies /*= NULL*/ ) {
    FolderTreeItem* newFolderItem = NULL;
    if( folder )
        setUpdatesEnabled( false );
    TreeItem* selectedItem = (TreeItem*)currentItem();
    if( selectedItem && selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        newFolderItem = addFolder( folderItem, folder, vocabularies );
    }
    if( folder )
        setUpdatesEnabled( true );
    return( newFolderItem );
}

FolderTreeItem* VocabTreeView::addFolder( FolderTreeItem* parentFolderItem, Folder* folder /*= NULL*/, QMap<int,Vocabulary>* vocabularies /*= NULL*/ ) {
    Folder* parentFolder = parentFolderItem->getFolder();
    Folder* newFolder = controller.addFolder( parentFolder, folder );    
    if( !newFolder )
        return( NULL );
    FolderTreeItem* newFolderItem = new FolderTreeItem( parentFolderItem, newFolder, controller.getPreferences() );
    newFolderItem->setOpen( false );
    newFolderItem->setOn( !folder );
    parentFolderItem->setOpen( !folder );
    if( folder ) {
        for( int i = 0; i < folder->getChildrenCount(); i++ ) {
            Base* folderChild = folder->childAt( i );
            if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                Folder* childFolder = (Folder*)folderChild;
                addFolder( newFolderItem, childFolder, vocabularies );
            }
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* childVocab = (Vocabulary*)folderChild;
                // childVocab contains only a reference to a vocabulary.  The actual data is found in the map.
                if( vocabularies && vocabularies->contains( childVocab->getId() ) ) {
                    Vocabulary& actualVocab = (*vocabularies)[ childVocab->getId() ];
                    addVocab( newFolderItem, &actualVocab );
                }
            }
        }
    }
    return( newFolderItem );
}

VocabTreeItem* VocabTreeView::addVocab( Vocabulary* vocab /*= NULL*/ ) {
    VocabTreeItem* newVocabItem = NULL;
    if( vocab )
        setUpdatesEnabled( false );
    TreeItem* selectedItem = (TreeItem*)currentItem();
    if( selectedItem && selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        newVocabItem = addVocab( folderItem, vocab );
    }
    if( vocab )
        setUpdatesEnabled( true );
    return( newVocabItem );
}

VocabTreeItem* VocabTreeView::addVocab( FolderTreeItem* parentFolderItem, Vocabulary* vocab /*= NULL*/ ) {
    Folder* parentFolder = parentFolderItem->getFolder();
    Vocabulary* newVocab = controller.addVocabulary( parentFolder, vocab );
    if( !newVocab )
        return( NULL );

    VocabTreeItem* newVocabItem = new VocabTreeItem( parentFolderItem, newVocab );
    newVocabItem->setOpen( false );
    newVocabItem->setOn( !vocab );
    parentFolderItem->setOpen( true );
    return( newVocabItem );
}

void VocabTreeView::removeItem() {
    QTreeWidgetItem* currItem = currentItem();
    if( currItem ) {
        QTreeWidgetItem* itemToSelect = this->itemAbove( currItem );
        setCurrentItem( itemToSelect ? itemToSelect : topLevelItem( 0 ) );
        delete( currItem );
    }
}

VocabTreeItem* VocabTreeView::getVocabTreeItem( int id ) {
    return( getVocabTreeItemRec( (TreeItem*)topLevelItem( 0 ), id ) );
}

VocabTreeItem* VocabTreeView::getVocabTreeItemRec( TreeItem* item, int id ) {
    if( item ) {
        if( item->isFolder() ) {
            for( int i = 0; i < item->childCount(); i++ ) {
                TreeItem* child = (TreeItem*)item->child( i );
                VocabTreeItem* vocabTreeItem = getVocabTreeItemRec( child, id );
                if( vocabTreeItem )
                    return( vocabTreeItem );
            }
        }
        else {
            VocabTreeItem* vocabTreeItem = (VocabTreeItem*)item;
            Vocabulary* vocab = vocabTreeItem->getVocabulary();
            return( vocab->getId() == id ? vocabTreeItem : NULL );
        }
    }
    return( NULL );
}

IdList VocabTreeView::getAllChildrenVocab( TreeItem* item ) const {
    IdList vocabList;
    getAllChildrenVocabRec( item, vocabList );
    return( vocabList );
}

void VocabTreeView::getAllChildrenVocabRec( TreeItem* item, IdList& vocabList ) const {
    if( item->isFolder() ) {
        for( int i = 0; item->childCount(); i++ ) {
            TreeItem* child = (TreeItem*)item->child( i );
            getAllChildrenVocabRec( child, vocabList );
        }
    }
    else {
        VocabTreeItem* vocabItem = (VocabTreeItem*)item;
        Vocabulary* vocab = vocabItem->getVocabulary();
        vocabList.append( vocab->getId() );
    }
}

Folder* VocabTreeView::getRootFolder() const {
    FolderTreeItem* rootFolderItem = (FolderTreeItem*)topLevelItem( 0 );
    return( rootFolderItem->getFolder() );
}

int VocabTreeView::getMaxFolderId() const {
    return( getRootFolder()->getMaxId() );
}

int VocabTreeView::getMaxVocabId() const {
    return( getRootFolder()->getMaxVocabId() );
}

void VocabTreeView::updateTreeItemIcon( QTreeWidgetItem* item ) {
    TreeItem* treeItem = (TreeItem*)item;
    if( treeItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)treeItem;
        folderItem->updateIcon();
    }
}
