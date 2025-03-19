#include "problemeditor.h"
#include "ui_problemeditor.h"

ProblemEditor::ProblemEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProblemEditor)
{
    ui->setupUi(this);

    pdfview=new QPdfView(ui->contentbox);
    pdfview->setPageMode(QPdfView::PageMode::MultiPage);
    pdfview->setZoomMode(QPdfView::ZoomMode::FitToWidth);
    ui->pdfarea->setWidget(pdfview);
    docnull=new QPdfDocument(ui->contentbox);
    pdfdoc=docnull;
    pdfview->setDocument(docnull);
    pdfview->hide();
}

ProblemEditor::~ProblemEditor()
{
    delete pdfview;
    if(pdfdoc!=docnull)
        delete pdfdoc;
    delete docnull;
    delete ui;
}

void ProblemEditor::loadBasic() {
    ui->probnametext->setText(problem->name);
    ui->probpathtext->setText(probPath);
    ui->tlspinbox->setValue(problem->time_limit_ms);
    ui->mlspinbox->setValue(problem->mem_limit_MiB);
    ui->probtypecombo->setCurrentIndex(int(problem->type));
}

void ProblemEditor::loadPDF() {
    if(pdfdoc==docnull)
        pdfdoc=new QPdfDocument(ui->contentbox);
    pdfdoc->load(probPath+probcontent);
    pdfview->setDocument(pdfdoc);
    pdfview->show();
}

void ProblemEditor::closePDF() {
    pdfview->setDocument(docnull);
    pdfview->hide();
    if(pdfdoc!=docnull) {
        pdfdoc->close();
        delete pdfdoc;
    }
    pdfdoc=docnull;
}

void ProblemEditor::refresh() {
    loadBasic();
    if(!pdfview->isHidden()) loadPDF();
    update();
}

void ProblemEditor::on_refreshbtn_clicked()
{
    refresh();
}


void ProblemEditor::on_backbtn_clicked()
{
    conEditor->show();
    this->destroy();
}


void ProblemEditor::on_tlspinbox_valueChanged(int arg1)
{
    problem->time_limit_ms=arg1;
}


void ProblemEditor::on_mlspinbox_valueChanged(int arg1)
{
    problem->mem_limit_MiB=arg1;
}


void ProblemEditor::on_probtypecombo_activated(int index)
{
    problem->type=Problem::ProblemType(index);
}


void ProblemEditor::on_importpdfbtn_clicked()
{
    QString pdfpath=QFileDialog::getOpenFileName(this, tr("Import from pdf"), probPath, tr("pdf files (*.pdf)"));
    if(pdfpath.isEmpty()) return;
    if (!FileOp::exists(pdfpath)) {
        QMessageBox::warning(NULL, "warning", tr("PDF file does not exist!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    closePDF();
    if(!FileOp::copy(pdfpath,probPath+probcontent,true)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while importing pdf!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    loadPDF();
    update();
}


void ProblemEditor::on_exportpdfbtn_clicked()
{
    if(pdfdoc==nullptr) return;
    QString pdfpath=QFileDialog::getSaveFileName(this, tr("Export to pdf"), probPath, tr("pdf files (*.pdf)"));
    if(pdfpath.isEmpty()) return;
    if(!FileOp::copy(probPath+probcontent,pdfpath,true)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while exporting pdf!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
}


void ProblemEditor::on_loadpdfbtn_clicked()
{
    loadPDF();
}


void ProblemEditor::on_closepdfbtn_clicked()
{
    closePDF();
}


void ProblemEditor::on_resfaddbtn_clicked()
{
    QString filename=ui->filenametext->text();
    QString filetype=ui->filetypetext->text();
}

