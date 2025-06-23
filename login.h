#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfView>
#include <QPdfDocument>
#include <QPaintDevice>
#include <QPagedPaintDevice>
#include <QPainter>
#include <QPaintEvent>
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QWidget>
#include "ctsettings.h"
#include "packer.h"
#include <QDateTime>
#include "studenteditor.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    QWidget* father;
    Contest contest;
    QDateTime time;
    QString dir;

private slots:
    void on_loginbtn_clicked();
private:
    Ui::login *ui;
};

#endif // LOGIN_H
