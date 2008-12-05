#ifndef FOLDER_TREE_ITEM_H
#define FOLDER_TREE_ITEM_H

#include <QTreeWidget>
#include <QWidget>
#include "Base.h"
#include "Preferences.h"
#include "Folder.h"
#include "PixMap.h"
#include "PropertiesPanel.h"
#include "TreeItem.h"
#include "VocabTreeItem.h"

class FolderTreeItem : public TreeItem, public Base {

public:

    FolderTreeItem( QTreeWidget* parent, Folder* folder, Preferences& prefs, PropertiesPanel* propsPanel = NULL );
    FolderTreeItem( QTreeWidgetItem* parent, Folder* folder, Preferences& prefs, PropertiesPanel* propsPanel = NULL );
    ~FolderTreeItem();

    const char* className() const { return "FolderTreeItem"; }

    bool isFolder() const;
    void setPropertiesPanel( PropertiesPanel* propsPanel );
    virtual void setOpen( bool isOpened );
    virtual void setOn( bool isOn );

    Folder* getFolder();

    virtual void update();
    void updateIcon();

private:

    Folder*             folder;
    Preferences&        prefs;
    PropertiesPanel*    propsPanel;

};

#endif
