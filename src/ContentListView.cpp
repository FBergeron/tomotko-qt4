#include "ContentListView.h"

ContentListView::ContentListView( QWidget *parent /* = 0 */ )
    : QTreeWidget( parent ) {
    setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum ) );
    header()->setMovable( false );
    header()->setResizeMode( QHeaderView::Fixed );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

ContentListView::~ContentListView() {
}

QSize ContentListView::sizeHint() const {
    // Not perfect but good enough.
    QSize defaultSizeHint( QTreeWidget::sizeHint() );
    QFontMetrics fm( font() );
    int height = ( fm.height() + 6 ) * ( topLevelItemCount() + 1 );
    return( QSize( defaultSizeHint.width(), height ) );
}

int ContentListView::sizeHintForColumn( int column ) const {
    int actualWidth = width() - 2 * frameWidth();
    int counterColumnWidth = (int)( actualWidth * 0.2 );
    if( column == 0 ) 
        return( actualWidth - 3 * counterColumnWidth );
    else
        return( counterColumnWidth );
}

void ContentListView::resizeEvent( QResizeEvent* event ) {
    QTreeWidget::resizeEvent( event );
    for( int i = 0; i < columnCount(); i++ )
        resizeColumnToContents( i );
}
