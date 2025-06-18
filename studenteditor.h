#ifndef STUDENTEDITOR_H
#define STUDENTEDITOR_H

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
#include "ctsettings.h"
#include "packer.h"

namespace Ui {
class StudentEditor;
}

class StudentEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentEditor(QWidget *parent = nullptr);
    ~StudentEditor();

    QMainWindow * father = nullptr;

    QString maindir;
    QString dir;
    Contest contest;//Validation and read contest info
    Problem problem  = Problem("");//Currently
    Submission submission;//store submission info
    bool access = false;

    QJsonObject Pack;
    QString probPath="";//problem path
    QPdfView* pdfview=nullptr;
    QPdfDocument* pdfdoc=nullptr;
    QPdfDocument* docnull=nullptr;
    void set_time();
    void load_prob();
    void view();

private slots:
    void on_loginbtn_clicked();

    void on_submitbtn_clicked();

    void on_loadcontestbtn_clicked();



    void on_returnbtn_clicked();

    void on_viewbtn_clicked();

    void on_savebtn_clicked();
private:
    Ui::StudentEditor *ui;
};

#endif // STUDENTEDITOR_H
