#ifndef DIALOGMULTIPAGEEDITOR_H
#define DIALOGMULTIPAGEEDITOR_H

#include <QDialog>
#include <QList>
#include <QToolBar>
#include <QStringList>
#include "Modules/multipageconverter.h"

namespace Ui {
class DialogMultipageEditor;
}

class DialogMultipageEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMultipageEditor(QWidget *parent = 0);
    ~DialogMultipageEditor();
    void readFile(QString fileName);
    QStringList fileNames();

private slots:
    void on_actionCheck_triggered();
    void on_actionCheckAll_triggered();
    void on_actionUncheck_triggered();
    void on_actionUncheckAll_triggered();

private:
    Ui::DialogMultipageEditor *ui;
    QToolBar *toolBar;

    QString m_currentFileName;
    QStringList m_fileNames;

    void selector(bool select_all, bool action_select);
    void analyzeMultipageFile(QString fileName);

    void checkGsWinInstalled();

private slots:
    void acceptDialog();
    void on_actionHelp_triggered();
};

#endif // DIALOGMULTIPAGEEDITOR_H
