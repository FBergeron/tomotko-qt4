#include "HistoryField.h"

HistoryField::HistoryField( QWidget* parent /* = 0 */ ) : QComboBox( parent ), digraphEnabled( false ) {
    setEditable( true );
    setDuplicatesEnabled( false );
    setAutoCompletion( true );
}

void HistoryField::activate() {
    int c = count();
    insertItem( c, currentText() );
    setCurrentIndex( c );  
}

void HistoryField::keyPressEvent( QKeyEvent* event ) {
    if( digraphEnabled ) {
        // We consider only printable keys.  Control keys are processed normally.
        // I'm not sure if this test covers all the cases though.
        if( event->count() > 0 ) {
            if( event->key() == Qt::Key_Backspace ) {
                if( lineEdit()->cursorPosition() > 0 ) {
                    if( buffer.isNull() ) {
                        QString charToDelete( lineEdit()->text().mid( lineEdit()->cursorPosition() - 1, 1 ) );
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
                        QComboBox::keyPressEvent( digraphEvent );
                        buffer = QString::null;
                        return;
                    }
                }
            }
        }
    }
    QComboBox::keyPressEvent( event );
}

void HistoryField::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool HistoryField::isDigraphEnabled() const {
    return( digraphEnabled );
}
