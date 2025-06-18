#ifndef TESTDATAPROCESSOR_H
#define TESTDATAPROCESSOR_H

#include <QMainWindow>
#include <QInputDialog>
#include "dataconfigwidget.h"
#include "val_utils.h"

class ValidationThread:public QObject {
    Q_OBJECT
public:
    explicit ValidationThread(QObject *parent = nullptr);
    ~ValidationThread();
public slots:
    void runValidation(Problem* problem,QString probPath);

signals:
    void ValidationResults(TResult result,QString file={},QString content={},QString log={});
};


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
    QThread* valThreadContainer;
    ValidationThread* valThread;
    void show_subtsk_tree();
    void refresh_testdata_settings();
    bool pending=false;
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void HandleResults(TResult result,QString file={},QString content={},QString log={});
public: signals:
    void startValidation(Problem* problem,QString probPath);
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
