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
#include "submitide.h"

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

    QString ctPath;
    Contest* contest;
    QString curProb;
    QMap<QString,JudgeInfo> judgeinfo;
    QString id,pwd;

    QPdfView* pdfview=nullptr;
    QPdfDocument* pdfdoc=nullptr;

    SubmitIDE IDE;

    void refresh();
    void set_name();
    void set_time();
    void load_prob();
    void view();

private slots:

    void on_submitbtn_clicked();

    void on_returnbtn_clicked();

    void on_savebtn_clicked();

    void on_Problist_itemClicked(QListWidgetItem *item);
private:
    Ui::StudentEditor *ui;
};

#endif // STUDENTEDITOR_H
