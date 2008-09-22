#include "PixMap.h"

int small_display_cache = -1;

int small_display_p () {
    if( small_display_cache < 0 ) {
        QWidget *d = QApplication::desktop();
        small_display_cache = ( d->width() * d->height() < 100000 );
    }
    return small_display_cache;
}

QPixmap small_pixmap( const char *xpm[] ) {
    QImage qim( xpm );
    //QPixmap pm;
    //pm.convertFromImage( qim.scaled( 12, 12 )/*qim.smoothScale( 12, 12 )*/ );
    //return pm;
    return( QPixmap::fromImage( qim.scaled( 12, 12 ) ) );
}
