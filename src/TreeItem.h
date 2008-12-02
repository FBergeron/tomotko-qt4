#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <QString>
#include <QTreeWidgetItem>

class TreeItem : public QTreeWidgetItem {

public:

    TreeItem( QTreeWidget* parent );
    TreeItem( QTreeWidgetItem* parent );
    ~TreeItem();

    virtual bool isFolder() const;
    virtual void update();
    virtual void setOpen( bool isOpened );

};

#endif

