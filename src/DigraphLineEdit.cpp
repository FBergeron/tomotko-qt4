#include "DigraphLineEdit.h"

DigraphLineEdit::DigraphLineEdit( QWidget* parent /* = 0 */ )
    : QLineEdit( parent ), digraphEnabled( false ) {
}

DigraphLineEdit::DigraphLineEdit( const QString& content, QWidget* parent /* = 0 */ )
    : QLineEdit( content, parent ), digraphEnabled( false ) {
}

DigraphLineEdit::~DigraphLineEdit() {
}

void DigraphLineEdit::keyPressEvent( QKeyEvent* event ) {
    if( digraphEnabled ) {
        // We consider only printable keys.  Control keys are processed normally.
        // I'm not sure if this test covers all the cases though.
        if( event->count() > 0 ) {
            if( event->key() == Qt::Key_Backspace ) {
                if( cursorPosition() > 0 ) {
                    if( buffer.isNull() ) {
                        QString charToDelete( text().mid( cursorPosition() - 1, 1 ) );
                        buffer = charToDelete;
                    }
                    else 
                        buffer = QString::null;
                }
            }
            else {
                // Shift key is required to input some digraphs so it's a special case.
                if( !buffer.isNull() && event->key() != Qt::Key_Shift ) {
                    buffer += event->text();
                    const QString newChar( Util::getDigraph( buffer ) );
                    if( newChar == QString::null )
                        buffer = QString::null;
                    else {
                        QKeyEvent* digraphEvent = new QKeyEvent( QEvent::KeyPress, 0, Qt::NoModifier, newChar, event->isAutoRepeat(), 0 );
                        QLineEdit::keyPressEvent( digraphEvent );
                        buffer = QString::null;
                        return;
                    }
                }
            }
        }
    }
    QLineEdit::keyPressEvent( event );
}

void DigraphLineEdit::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool DigraphLineEdit::isDigraphEnabled() const {
    return( digraphEnabled );
}
