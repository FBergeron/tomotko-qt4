#ifndef VOCAB_TREEVIEW_H
#define VOCAB_TREEVIEW_H

#include <iostream>
#include <QTreeWidget>
#include "Base.h"
#include "Controller.h"
#include "FolderTreeItem.h"
#include "Preferences.h"
#include "Util.h"
#include "VocabTreeItem.h"
#include "Vocabulary.h"

using namespace std;

class VocabTreeView : public QTreeWidget {

    Q_OBJECT

public:

    VocabTreeView( Controller& controller );
    virtual ~VocabTreeView();

    VocabTreeItem* currentItem() const;

    FolderTreeItem* addFolder( Folder* folder = NULL, QMap<int,Vocabulary>* vocabularies = NULL );
    FolderTreeItem* addFolder( FolderTreeItem* parentFolderItem, Folder* folder = NULL, QMap<int,Vocabulary>* vocabularies = NULL );
    VocabTreeItem* addVocab( Vocabulary* vocab = NULL );
    VocabTreeItem* addVocab( FolderTreeItem* parentFolderItem, Vocabulary* vocab = NULL );
    void removeItem();

    VocabTreeItem* getVocabTreeItem( int id );

    IdList getAllChildrenVocab( TreeItem* item ) const;

    Folder* getRootFolder() const;
    int getMaxFolderId() const;
    int getMaxVocabId() const;

private slots:

    void updateTreeItemIcon( QTreeWidgetItem* item );

private:

    VocabTreeItem* getVocabTreeItemRec( TreeItem* item, int id );

    void getAllChildrenVocabRec( TreeItem* item, IdList& vocabList ) const;

    bool dirty;

    Controller& controller;

};

#endif
