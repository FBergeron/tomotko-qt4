#ifndef SEQUENCE_DIALOG_H
#define SEQUENCE_DIALOG_H 

#include <QDialog>
#include <QStack>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>
#include "Preferences.h"
#include "Sequence.h"

class SequenceDialog : public QDialog {

    Q_OBJECT
    
public:

    SequenceDialog( Preferences* prefs, QWidget* parent );
    ~SequenceDialog(); 

    Sequence getSequence() const;

public slots:

protected slots:

    virtual void accept();

private slots:

    virtual bool eventFilter( QObject* obj, QEvent* evt );

    void addSequenceItem( const QString& itemStr );
    void addSequenceMark();
    void addGroupMark();
    void removeLastToken();

private:

    void init();
    void setUnionButtonsEnabled( bool isEnabled );
    bool isItemUsed( const Sequence::Item& item ) const;
    bool areItemsLeft() const;
    void setItemButtonsEnabled( bool isEnabled );
    //void trace() const;

    Preferences*                prefs;

    Sequence                    sequence;

    QStack<Sequence::Item>      items; 
    Sequence::ItemList          currGroup;

    QVBoxLayout*                mainLayout;

    QWidget*                    sequencePanel;
    QHBoxLayout*                sequencePanelLayout;

    QGroupBox*                  quizPanel;
    QVBoxLayout*                quizPanelLayout;

    QWidget*                    quizTopPanel;
    QHBoxLayout*                quizTopPanelLayout;
    QWidget*                    quizTopLeftPanel;
    QVBoxLayout*                quizTopLeftPanelLayout;
    QGroupBox*                  quizImagePanel;
    QVBoxLayout*                quizImagePanelLayout;
    QPushButton*                quizImageButton;
    
    QGroupBox*                  quizFirstLangPanel;
    QHBoxLayout*                quizFirstLangPanelLayout;
    QWidget*                    quizFirstLangPanelWrapper;
    QHBoxLayout*                quizFirstLangPanelWrapperLayout;
    QLabel*                     quizFirstLangLabel;
    QPushButton*                quizFirstLangTermButton;

    QGroupBox*                  quizTestLangPanel;
    QHBoxLayout*                quizTestLangPanelLayout;
    QWidget*                    quizTestLangPanelWrapper;
    QHBoxLayout*                quizTestLangPanelWrapperLayout;
    QWidget*                    quizTestLangLabelsPanel;
    QVBoxLayout*                quizTestLangLabelsPanelLayout;
    QWidget*                    quizTestLangButtonsPanel;
    QVBoxLayout*                quizTestLangButtonsPanelLayout;

    QLabel*                     quizTestLangAltLabel;
    QLabel*                     quizTestLangTermLabel;
    QPushButton*                quizTestLangAltButton;
    QPushButton*                quizTestLangTermButton;

    QWidget*                    quizCommentBox;
    QVBoxLayout*                quizCommentBoxLayout;
    QLabel*                     quizCommentLabel;
    QPushButton*                quizCommentButton;

    QWidget*                    sequencePanelButtons;
    QVBoxLayout*                sequencePanelButtonsLayout;
    QPushButton*                addSequenceMarkButton;
    QPushButton*                addGroupMarkButton;
    QPushButton*                removeLastMarkButton;

    QWidget*                    sequenceLinePanel;
    QHBoxLayout*                sequenceLinePanelLayout;
    QLabel*                     sequenceLineLabel;
    QLineEdit*                  sequenceLineLineEdit;

    QWidget*                    bottomButtonsPanel;
    QHBoxLayout*                bottomButtonsPanelLayout;
    QPushButton*                acceptButton;
    QPushButton*                cancelButton;

};

#endif
