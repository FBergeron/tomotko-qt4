#ifndef CONTENT_LISTVIEW_H
#define CONTENT_LISTVIEW_H

#include <iostream>
#include <QHeaderView>
#include <QResizeEvent>
#include <QTreeWidget>

using namespace std;

class ContentListView : public QTreeWidget {

public:

    ContentListView( QWidget* parent = 0 );
    virtual ~ContentListView();

    QSize sizeHint() const;
    virtual int sizeHintForColumn( int column ) const;

protected:

    void resizeEvent( QResizeEvent* event );
};

#endif
