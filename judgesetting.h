#ifndef JUDGESETTING_H
#define JUDGESETTING_H

#include <QWidget>
#include <QListWidgetItem>
#include "ctsettings.h"
#include "procexecinfowidget.h"
namespace Ui {
class JudgeSetting;
}

class JudgeSetting : public QWidget
{
    Q_OBJECT

public:
    explicit JudgeSetting(QWidget *parent = nullptr);
    ~JudgeSetting();
    Problem* problem=nullptr;

    void refresh();
public: signals:
    void ExitWin();
private slots:
    void on_folderaddBTN_clicked();

    void on_folderremBTN_clicked();

    void on_folderLWid_itemClicked(QListWidgetItem *item);

    void on_claddBTN_clicked();

    void on_clremBTN_clicked();

    void on_clLWid_itemClicked(QListWidgetItem *item);

    void on_pipecntSpBox_valueChanged(int arg1);

    void on_exeaddBTN_clicked();

    void on_exeremBTN_clicked();

    void on_chkaddBTN_clicked();

    void on_chkremBTN_clicked();

    void on_checkerLWid_itemClicked(QListWidgetItem *item);

    void closeEvent(QCloseEvent* event);
private:
    Ui::JudgeSetting *ui;
};


#endif // JUDGESETTING_H
