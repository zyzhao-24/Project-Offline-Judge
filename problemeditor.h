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
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include "ctsettings.h"

const int max_cplsrc=5;

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

    QComboBox cplsrc[max_cplsrc];//src file name input widget,to be modified to combo box
    int cplsrc_cnt=0;
    void set_cplsrc_cnt(int new_cnt);
    void update_cplsrc_option();

    void loadBasic();
    void closePDF();
    void loadPDF();
    void loadUtils();
    void loadCplSettings();
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

    void on_srcaddbtn_clicked();

    void on_srcrembtn_clicked();

    void on_jutilswid_itemClicked(QListWidgetItem *item);

    void on_jutilsaddbtn_clicked();

    void on_jutilsrembtn_clicked();



    void on_phaserbtn_clicked();

    void on_phaserbtn_2_clicked();

    void on_cplsetaddbtn_clicked();

    void on_cplsetrembtn_clicked();

    void on_cplsettabwid_itemClicked(QTableWidgetItem *item);

private:
    Ui::ProblemEditor *ui;
};

#endif // PROBLEMEDITOR_H
