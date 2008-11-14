#ifndef VOCAB_TREE_ITEM_H
#define VOCAB_TREE_ITEM_H

#include <iostream>
#include <QTreeWidgetItem>
#include <QWidget>
#include "Base.h"
#include "PixMap.h"
#include "PropertiesPanel.h"
#include "TreeItem.h"
#include "Vocabulary.h"

class VocabTreeView;

class VocabTreeItem : public TreeItem, public Base {

public:

    VocabTreeItem( QTreeWidgetItem* parent, Vocabulary* vocab, PropertiesPanel* propsPanel = NULL );
    ~VocabTreeItem();

    const char* className() const { return "VocabTreeItem"; }

    void setPropertiesPanel( PropertiesPanel* propsPanel );
    virtual void setOpen( bool isOpened );
    virtual void setOn( bool isOn );
    virtual void setSelected( bool isSelected );

    Vocabulary* getVocabulary();

    virtual void update();

private:

    Vocabulary*         vocab;
    PropertiesPanel*    propsPanel;

};

#endif
