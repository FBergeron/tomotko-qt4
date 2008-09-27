#ifndef IMAGE_BOX_H
#define IMAGE_BOX_H

#include <iostream>
#include <QByteArray>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QResizeEvent>
#include <QSize>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>
#include "PixMap.h"

using namespace std;

class ImageBox : public QGroupBox {

    Q_OBJECT

public:

    ImageBox( const QString& title, const QString& hiddenLabel, const QString& hiddenTooltip, QWidget* parent = 0 );
    ~ImageBox();

    void setImage( const QString& path );
    bool isImageRevealed() const;
    bool containsValidImage() const;

    virtual QSize sizeHint() const;
    virtual void show();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

public slots:

    void revealImage();
    void hideImage();

private slots:

    void restartMovie() const;

private:

    void resizeImage();

    int                         imageWidth;
    int                         imageHeight;
    QByteArray                  imageFormat;

    QStackedWidget*             imageStack;
    QWidget*                    imageWrapper;
    QHBoxLayout*                imageWrapperLayout;
    QLabel*                     image;
    QPushButton*                imageButton;

    QVBoxLayout*                mainLayout;

};

#endif

