#include "HistoryField.h"

HistoryField::HistoryField( QWidget* parent /* = 0 */ ) : QComboBox( parent ) {
    setEditable( true );
    setDuplicatesEnabled( false );
    setAutoCompletion( true );
}

void HistoryField::activate() {
    int c = count();
    insertItem( c, currentText() );
    setCurrentIndex( c );  
}

void HistoryField::keyPressEvent( QKeyEvent* e ) {
    if( e->key() == Qt::Key_Return )
        emit returnPressed();
    QComboBox::keyPressEvent(e);
}
