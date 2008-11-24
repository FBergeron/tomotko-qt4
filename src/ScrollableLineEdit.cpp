#include "ScrollableLineEdit.h"

ScrollableLineEdit::ScrollableLineEdit( QWidget* parent /* = 0 */ )
    : QLineEdit( parent ) {
}

ScrollableLineEdit::~ScrollableLineEdit() {
}

void ScrollableLineEdit::scrollLeft() {
    cursorWordBackward( false );
}

void ScrollableLineEdit::scrollRight() {
    cursorWordForward( false );
}
