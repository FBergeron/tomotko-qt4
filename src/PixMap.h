#ifndef ZPIXMAP_H
#define ZPIXMAP_H

#include <QApplication>
#include <QDesktopWidget>
#include <QImage>
#include <QPixmap>

#define ZPIXMAP( x ) ( small_display_p() ? small_pixmap( ( x ) ) : QPixmap( ( x ) ) )
#define ZSMALLDISPLAY small_display_p()

extern int small_display_cache;
extern int small_display_p();
extern QPixmap small_pixmap( const char* xpm[] );

#endif
