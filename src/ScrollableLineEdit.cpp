#include "ScrollableLineEdit.h"

ScrollableLineEdit::ScrollableLineEdit( QWidget* parent /* = 0 */ )
    : QLineEdit( parent ) {
}

ScrollableLineEdit::~ScrollableLineEdit() {
}

void ScrollableLineEdit::scrollLeft() {
    setCursorPosition( cursorPositionAt( QPoint( 0, 0 ) ) - 1 );
}

void ScrollableLineEdit::scrollRight() {
    setCursorPosition( cursorPositionAt( QPoint( size().width(), 0 ) ) + 1 );
}
