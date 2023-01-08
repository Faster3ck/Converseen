#ifndef DIALOGSHOWUPDATEMSG_H
#define DIALOGSHOWUPDATEMSG_H

#include <QDialog>

namespace Ui {
class DialogShowUpdateMsg;
}

class DialogShowUpdateMsg : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowUpdateMsg(QWidget *parent = 0, const QString &caption = 0, const QString &message = 0);
    ~DialogShowUpdateMsg();

private slots:
    void on_pushOk_clicked();
    void on_checkBoxDisable_stateChanged(int arg1);

    void on_pushCancel_clicked();

private:
    Ui::DialogShowUpdateMsg *ui;

    void setAutoUpdates(int arg1);
};

#endif // DIALOGSHOWUPDATEMSG_H
