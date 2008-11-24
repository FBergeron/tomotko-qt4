#include "ScrollableMultiLineEdit.h"

ScrollableMultiLineEdit::ScrollableMultiLineEdit( QWidget* parent /* = 0 */ )
    : QTextEdit( parent ) {
}

ScrollableMultiLineEdit::~ScrollableMultiLineEdit() {
}

void ScrollableMultiLineEdit::scrollUp() {
    moveCursor( QTextCursor::Up );
}

void ScrollableMultiLineEdit::scrollDown() {
    moveCursor( QTextCursor::Down );
}

void ScrollableMultiLineEdit::scrollPageUp() {
    moveCursor( QTextCursor::PreviousBlock );
}

void ScrollableMultiLineEdit::scrollPageDown() {
    moveCursor( QTextCursor::NextBlock );
}
