/*
 * This class contains code borrowed from 
 * keyboard_shortcuts.h (rev. 1.2) and
 * keyboard_shortcuts.cpp (rev. 1.3) of ZBEDic.
 * Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
 *
 * It has been slightly modified and adapted for Qt4.
 */
#ifndef KEY_ACTION_LIST_VIEW_ITEM
#define KEY_ACTION_LIST_VIEW_ITEM

#include <QAction>
#include <QKeySequence>
#include <QTreeWidgetItem>
#include "Util.h"

class KeyActionListViewItem : public QTreeWidgetItem {

public:

    KeyActionListViewItem( QTreeWidget *parent, QAction *action, Action actionIndex ) 
        : QTreeWidgetItem( parent ), action( action ), actionIndex( actionIndex ) {
        keyCode = action->shortcut();
        updateText();
    }

    QAction* getAction() {
        return action;
    }

    Action getActionIndex() {
        return( actionIndex );
    }

    void setKey( int keyCode ) {
        this->keyCode = keyCode;
        updateText();
    }

    QKeySequence getKey() {
        return keyCode;
    }

    void updateText() {
        setText( 1, keyCode ? keyCode.toString() : QString::null );
    }  

private:

    QAction*        action;
    QKeySequence    keyCode;
    Action          actionIndex;

};

#endif
