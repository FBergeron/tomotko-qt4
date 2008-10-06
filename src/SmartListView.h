#ifndef SMART_LISTVIEW_H
#define SMART_LISTVIEW_H

#include <iostream>
#include <QResizeEvent>
#include <QTreeWidget>

using namespace std;

class SmartListView : public QTreeWidget {

public:

    SmartListView( QWidget* parent = 0 );
    virtual ~SmartListView();

    //int getStretchColumn() const;
    //void setStretchColumn( int col );
    bool areAllColumnsWide() const;
    void setAllColumnsWide( bool isOn );
    void updateColumnsWidth();

protected:

    void resizeEvent( QResizeEvent* event );
    virtual int sizeHintForColumn( int column ) const;

private:

    //int stretchColumn;
    bool allColumnsWide;

};

#endif
