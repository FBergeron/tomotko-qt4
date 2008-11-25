#include "DigraphMultiLineEdit.h"

DigraphMultiLineEdit::DigraphMultiLineEdit( QWidget* parent /* = 0 */ )
    : QTextEdit( parent ), digraphEnabled( false ) {
}

DigraphMultiLineEdit::~DigraphMultiLineEdit() {
}

void DigraphMultiLineEdit::keyPressEvent( QKeyEvent* event ) {
    if( digraphEnabled ) {
        // We consider only printable keys.  Control keys are processed normally.
        // I'm not sure if this test covers all the cases though.
        if( event->count() > 0 ) {
            if( event->key() == Qt::Key_Backspace ) {
                int col = textCursor().position();
                if( col > 0 ) {
                    if( buffer.isNull() ) {
                        QString charToDelete( toPlainText().mid( col - 1, 1 ) );
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
                        QTextEdit::keyPressEvent( digraphEvent );
                        buffer = QString::null;
                        return;
                    }
                }
            }
        }
    }
    QTextEdit::keyPressEvent( event );
}

void DigraphMultiLineEdit::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool DigraphMultiLineEdit::isDigraphEnabled() const {
    return( digraphEnabled );
}
