#include "ScrollableMultiLineEdit.h"

ScrollableMultiLineEdit::ScrollableMultiLineEdit( QWidget* parent /* = 0 */ )
    : QTextEdit( parent ) {
}

ScrollableMultiLineEdit::~ScrollableMultiLineEdit() {
}

void ScrollableMultiLineEdit::scrollUp() {
    QTextCursor cursor( cursorForPosition( QPoint( 0, 0 ) ) );
    setTextCursor( cursor );
    moveCursor( QTextCursor::Up );
}

void ScrollableMultiLineEdit::scrollDown() {
    QTextCursor cursor( cursorForPosition( QPoint( 0, size().height() ) ) );
    setTextCursor( cursor );
    moveCursor( QTextCursor::Down );
}

void ScrollableMultiLineEdit::scrollPageUp() {
    int delta = size().height() - 40; // Not perfect but good enough.
    QTextCursor cursor( cursorForPosition( QPoint( 0, -delta  ) ) );
    setTextCursor( cursor );
}

void ScrollableMultiLineEdit::scrollPageDown() {
    int delta = size().height() - 40; // Not perfect but good enough.
    QTextCursor cursor( cursorForPosition( QPoint( 0, size().height() + delta ) ) );
    setTextCursor( cursor );
}
