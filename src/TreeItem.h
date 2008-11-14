#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <QString>
//#include <QStringList>
#include <QTreeWidgetItem>

class TreeItem : public QTreeWidgetItem/*QCheckListItem*/ {

public:

    TreeItem( QTreeWidget* parent/*QListView* parent*//*, const QStringList& texts*//*, QCheckListItem::Type type*/ );
    TreeItem( QTreeWidgetItem* parent/*ListViewItem* parent*//*, const QStringList& texts*//*, QCheckListItem::Type type*/ );
    ~TreeItem();

    virtual bool isFolder() const;
    virtual void update();
    virtual void setOpen( bool isOpened );

};

#endif

