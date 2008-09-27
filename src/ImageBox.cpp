#include "ImageBox.h"
#include "icons/void.xpm"

ImageBox::ImageBox( const QString& title, const QString& hiddenLabel, const QString& hiddenTooltip, QWidget *parent /* = 0 */ )
    : QGroupBox( title, parent ), imageWidth( -1 ), imageHeight( -1 ) {
    imageStack = new QStackedWidget();
    imageStack->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    imageWrapper = new QWidget();
    imageWrapperLayout = new QHBoxLayout();
    imageWrapperLayout->setContentsMargins( 0, 0, 0, 0 );
    imageWrapper->setLayout( imageWrapperLayout );
    image = new QLabel();
    imageWrapperLayout->addStretch();
    imageWrapperLayout->addWidget( image );
    imageWrapperLayout->addStretch();
    image->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding ) );
    //image->setAlignment( Qt::AlignCenter );
    //image->setScaledContents( true );

    imageButton = new QPushButton( hiddenLabel );
    imageButton->installEventFilter( this );
    imageButton->setToolTip( hiddenTooltip );
    connect( imageButton, SIGNAL( clicked() ), this, SLOT( revealImage() ) );  
    imageStack->addWidget( imageWrapper );
    imageStack->addWidget( imageButton );

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget( imageStack );
    mainLayout->activate();
    setLayout( mainLayout );
}

ImageBox::~ImageBox() {
}

QSize ImageBox::sizeHint() const {
    //int width = ( imageWidth == -1 ? 200 : height() * imageWidth / imageHeight );
    //cerr << "sz=" << width << " x " << height() << " img sz=" << imageWidth << " x " << imageHeight << endl;
    //return( QSize( width, QGroupBox::sizeHint().height() ) );
    return( QGroupBox::sizeHint() );
}

void ImageBox::show() {
    QGroupBox::show();
    restartMovie();
}

bool ImageBox::isImageRevealed() const {
    return( imageStack->currentWidget() == imageWrapper );
}

bool ImageBox::containsValidImage() const {
    // If imageWidth could not be properly initialized in setImage(),
    // we consider that the imageBox is not initialized. 
    return( imageWidth != -1 );
}

void ImageBox::revealImage() {
    imageStack->setCurrentIndex( 0 );
    restartMovie();
}

void ImageBox::hideImage() {
    imageStack->setCurrentIndex( 1 );
}

void ImageBox::restartMovie() const {
    QMovie* movie = image->movie();
    if( movie )
        movie->start();
}

void ImageBox::setImage( const QString& path ) {
    cerr << "setImage" << endl;
    imageWidth = imageHeight = -1;
    if( !path.isNull() ) {
        QFileInfo info( path );
        if( info.exists() ) {
            imageFormat = QImageReader::imageFormat( path );
            if( imageFormat == "gif" || imageFormat == "png" ) {
                const QPixmap& pixmap( path );
                imageWidth = pixmap.width();
                imageHeight = pixmap.height();
                if( imageFormat == "gif" ) {
                    QMovie* movie = new QMovie( path );
                    image->setMovie( movie );
                    movie->setScaledSize( image->size() );
                    movie->start();
                }
                else if( imageFormat == "png" ) {
                    image->setScaledContents( true );
                    image->setPixmap( pixmap );
                }
                //show();
                //setFixedWidth( sizeHint().width() );
                resizeImage();
            }
        }
    }
    
    if( imageWidth == -1 ) {
        image->setPixmap( ZPIXMAP( void_xpm ) );
        hide();
    }
    cerr << "setImage END" << endl;
}

void ImageBox::resizeEvent( QResizeEvent* evt ) {
    QGroupBox::resizeEvent( evt );
    //if( evt->size().width() != sizeHint().width() )
    //    setFixedWidth( sizeHint().width() );
}

void ImageBox::resizeImage() {
    if( imageFormat == "gif" ) {
        // Nothing to do for this format.  The default behavior works fine.
    }
    else if( imageFormat == "png" ) {
        int proportionalWidth = image->height() * image->pixmap()->width() / image->pixmap()->height();
        image->setFixedWidth( proportionalWidth );
    cerr << "image sz=" << image->width() << " x " << image->height() << " pixmap sz=" << image->pixmap()->width() << " x " << image->pixmap()->height() << endl;
    }
    updateGeometry();
}

