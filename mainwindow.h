#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createbtn_clicked();//create contest

    void on_editbtn_clicked();//modify contest settings

    void on_loadbtn_clicked();//start contest(student)

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
