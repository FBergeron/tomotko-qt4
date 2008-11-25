#include "ContentListView.h"

ContentListView::ContentListView( QWidget *parent /* = 0 */ )
    : SmartListView( parent ) {
    setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum ) );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

ContentListView::~ContentListView() {
}

QSize ContentListView::sizeHint() const {
    // Not perfect but good enough.
    QSize defaultSizeHint( SmartListView::sizeHint() );
    QFontMetrics fm( font() );
    int height = ( fm.height() + 6 ) * ( topLevelItemCount() + 1 );
    return( QSize( defaultSizeHint.width(), height ) );
}
