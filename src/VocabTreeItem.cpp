#include "VocabTreeItem.h"
#include "icons/closedVocab.xpm"
#include "icons/openedVocab.xpm"

VocabTreeItem::VocabTreeItem( QTreeWidgetItem* parent, Vocabulary* vocab, PropertiesPanel* propsPanel )
    : TreeItem( parent ), vocab( vocab ), propsPanel( propsPanel ) {
    setCheckState( 0, vocab->isMarkedForStudy() ? Qt::Checked : Qt::Unchecked );
    setText( 0, vocab->getTitle() );
}

VocabTreeItem::~VocabTreeItem() {
}

void VocabTreeItem::setPropertiesPanel( PropertiesPanel* propsPanel ) {
    this->propsPanel = propsPanel;
}

void VocabTreeItem::setOpen( bool isOpened ) {
    setExpanded( isOpened );
    setIcon( 0, QIcon( QPixmap( isOpened ? ZPIXMAP( openedVocab_xpm ) : ZPIXMAP( closedVocab_xpm ) ) ) );
}

void VocabTreeItem::setOn( bool isOn ) {
    vocab->setMarkedForStudy( isOn );
}

void VocabTreeItem::setSelected( bool isSelected ) {
    QTreeWidgetItem::setSelected( isSelected );
    if( !isSelected )
        setOpen( false );
}

Vocabulary* VocabTreeItem::getVocabulary() {
    return( vocab );
}

void VocabTreeItem::update() {
    if( propsPanel )
        propsPanel->updateCounters();
}
