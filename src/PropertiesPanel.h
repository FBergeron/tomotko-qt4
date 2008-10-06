#ifndef PROPERTIES_PANEL_H
#define PROPERTIES_PANEL_H 

//#include <qdialog.h>
#include <QHBoxLayout>
//#include <qheader.h>
#include <QLabel>
#include <QLineEdit>
//#include <qpixmap.h>
//#include <qpushbutton.h>
#include <QTextEdit>
#include <QTreeWidget>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
//#include "ContentListView.h"
#include "DigraphLineEdit.h"
#include "DigraphMultiLineEdit.h"
#include "Folder.h"
#include "Preferences.h"
#include "SmartListView.h"
#include "Vocabulary.h"

class PropertiesPanel : public QWidget/*QVBox*/ {

    Q_OBJECT
    
public:

    PropertiesPanel( const Preferences& prefs, QWidget* parent );
    ~PropertiesPanel(); 

    void setVocabulary( Vocabulary* vocab );
    void setFolder( Folder* folder );

    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();

public slots:

    void updateCounters();

private slots:
    
    void updateDescription();
    void updateAuthor( const QString& author );

private:

    void init();
    void updateModel();

    void addListeners();
    void removeListeners();

    QString getType() const;

    QVBoxLayout*            mainLayout;
    QWidget*                simplePropsPanel;
    QHBoxLayout*            simplePropsPanelLayout;

    QWidget*                simplePropsLabelsPanel;
    QVBoxLayout*            simplePropsLabelsPanelLayout;
    QWidget*                simplePropsFieldsPanel;
    QVBoxLayout*            simplePropsFieldsPanelLayout;

    QLabel*                 descriptionLabel;
    DigraphMultiLineEdit*   descriptionMultiLineEdit;

    QLabel*                 contentLabel;
    //QTreeWidget*            contentListView;//ContentListView*        contentListView;
    SmartListView*          contentListView;

    QLabel*                 authorLabel;
    DigraphLineEdit*        authorField;

    QLabel*                 creationDateLabel;
    QLabel*                 creationDateValueLabel;

    QLabel*                 modificationDateLabel;
    QLabel*                 modificationDateValueLabel;

    const Preferences&      prefs;
    Vocabulary*             editedVocab;
    Folder*                 editedFolder;

};

#endif
