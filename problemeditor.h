#ifndef PROBLEMEDITOR_H
#define PROBLEMEDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfView>
#include <QPdfDocument>
#include <QPaintDevice>
#include <QPagedPaintDevice>
#include <QPainter>
#include <QPaintEvent>
#include "ctsettings.h"

namespace Ui {
class ProblemEditor;
}

class ProblemEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProblemEditor(QWidget *parent = nullptr);
    ~ProblemEditor();

    QMainWindow* conEditor=nullptr;
    Problem* problem=nullptr;
    QString probPath="";//problem path
    QPdfView* pdfview=nullptr;
    QPdfDocument* pdfdoc=nullptr;
    QPdfDocument* docnull=nullptr;
    void loadBasic();
    void closePDF();
    void loadPDF();
    void refresh();
private slots:
    void on_refreshbtn_clicked();

    void on_backbtn_clicked();

    void on_tlspinbox_valueChanged(int arg1);

    void on_mlspinbox_valueChanged(int arg1);

    void on_probtypecombo_activated(int index);

    void on_importpdfbtn_clicked();

    void on_exportpdfbtn_clicked();

    void on_loadpdfbtn_clicked();

    void on_closepdfbtn_clicked();

private:
    Ui::ProblemEditor *ui;
};

#endif // PROBLEMEDITOR_H
