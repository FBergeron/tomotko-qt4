#include "SmartListView.h"

SmartListView::SmartListView( QWidget *parent /* = 0 */ )
    : QTreeWidget( parent ), /*stretchColumn( -1 ),*/ allColumnsWide( false ) {
}

SmartListView::~SmartListView() {
}

//void SmartListView::setStretchColumn( int col ) {
//    stretchColumn = col;
//}
//
//int SmartListView::getStretchColumn() const {
//    return( stretchColumn );
//}

void SmartListView::setAllColumnsWide( bool isOn ) {
    allColumnsWide = isOn;
}

bool SmartListView::areAllColumnsWide() const {
    return( allColumnsWide );
}

void SmartListView::updateColumnsWidth() {
//    //cerr << "update w=" << width() << " vp.w=" << viewport()->width();
//    //cerr << " vw=" << visibleWidth();
//    //cerr << " cw=" << contentsWidth();
//    //cerr << endl;
//    //resizeContents( width() + 20, height() );
//    viewport()->adjustSize();
//    if( stretchColumn != -1 ) {
//        int otherColumnsWidth = 0;
//        for( int i = 0; i < columnCount(); i++ ) {
//            if( i != stretchColumn )
//                otherColumnsWidth += columnWidth( i );
//        }
//        setColumnWidth( stretchColumn, viewport()->width() - otherColumnsWidth );
//    }
    if( allColumnsWide ) {
        for( int i = 0; i < columnCount(); i++ )
            resizeColumnToContents( i );
    }
}

int SmartListView::sizeHintForColumn( int column ) const {
    if( allColumnsWide )
        return( ( width() - 2 * frameWidth() ) / columnCount() );
    else 
        return( QTreeWidget::sizeHintForColumn( 0 ) );
}

void SmartListView::resizeEvent( QResizeEvent* event ) {
    QTreeWidget::resizeEvent( event );
    updateColumnsWidth();
}
