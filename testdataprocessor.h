#ifndef TESTDATAPROCESSOR_H
#define TESTDATAPROCESSOR_H

#include <QMainWindow>
#include "dataconfigwidget.h"
#include "val_utils.h"
namespace Ui {
class TestDataProcessor;
}

class TestDataProcessor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestDataProcessor(QWidget *parent = nullptr);
    ~TestDataProcessor();
    Problem* problem=nullptr;
    QString probPath="";
    void show_subtsk_tree();
    void refresh_testdata_settings();
protected:
    void closeEvent(QCloseEvent *event);
public: signals:
    void ExitWin();
private slots:
    void on_CrSubtskBTN_clicked();

    void on_AddCaseBTN_clicked();

    void on_NCaseSpBOX_valueChanged(int arg1);

    void on_RemSubtskBTN_clicked();

    void on_RemCaseBTN_clicked();

    void on_DataConfTWID_currentChanged(int index);

    void on_RefreshBTN_clicked();

    void on_RunValBTN_clicked();

private:
    Ui::TestDataProcessor *ui;
};

#endif // TESTDATAPROCESSOR_H
