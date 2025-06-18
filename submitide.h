#ifndef SUBMITIDE_H
#define SUBMITIDE_H


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
#include "idehighlighter.h"
#include "idegroupwidget.h"
#include "judgingwidget.h"

namespace Ui {
class SubmitIDE;
}

class SubmitIDE : public QWidget
{
    Q_OBJECT


public:
    explicit SubmitIDE(QWidget *parent = nullptr, Problem prob = Problem());
    ~SubmitIDE();
    QMainWindow * father = nullptr;
    IDEHighlighter* highlighter;
    Submission* submission;
    Problem problem;
    JudgingWidget judge;
    QString maindir;
    void submit();

private slots:
    void on_returnbtn_clicked();

    void on_testbtn_clicked();

    void on_submitbtn_clicked();

    void on_initbtn_clicked();


private:
    Ui::SubmitIDE *ui;
};

#endif // SUBMITIDE_H
