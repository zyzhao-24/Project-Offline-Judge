#include "problemeditor.h"
#include "ui_problemeditor.h"
#include <QStandardItem>
ProblemEditor::ProblemEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProblemEditor)
{
    ui->setupUi(this);

    QListView* categoryview = dynamic_cast<QListView*>(ui->categorycombo->view());
    if (categoryview != nullptr) {
        categoryview->setRowHidden(3, true);
    }
    QStandardItemModel* categorymodel = dynamic_cast<QStandardItemModel*>(ui->categorycombo->model());
    if (categorymodel != nullptr) {
        QStandardItem* item = categorymodel->item(3);
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    QListView* filetypeview = dynamic_cast<QListView*>(ui->filetypecombo->view());
    if (filetypeview != nullptr) {
        filetypeview->setRowHidden(3, true);
    }
    QStandardItemModel* filetypemodel = dynamic_cast<QStandardItemModel*>(ui->filetypecombo->model());
    if (filetypemodel != nullptr) {
        QStandardItem* item = filetypemodel->item(3);
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }

    pdfview=new QPdfView(ui->contentbox);
    pdfview->setPageMode(QPdfView::PageMode::MultiPage);
    pdfview->setZoomMode(QPdfView::ZoomMode::FitToWidth);
    ui->pdfarea->setWidget(pdfview);
    docnull=new QPdfDocument(ui->contentbox);
    pdfdoc=docnull;
    pdfview->setDocument(docnull);
    pdfview->hide();

    ui->srchlayout->addWidget(&cplsrc[0]);
    cplsrc[0].show();
    cplsrc_cnt=1;
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

void ProblemEditor::loadUtils() {
    QVector<Problem::Utility> utilsvec(problem->utils.values());
    for(QVector<Problem::Utility>::Iterator it=utilsvec.begin();it!=utilsvec.end();it++) {
        if(it->filetype==Problem::Utility::FileType::snippet) {
            problem->utils.remove(it->filename);
            continue;
        }
        if(it->category==Problem::Utility::FileCategory::builtin) {
            if(!FileOp::exists(probPath+it->filename)) {
                QMessageBox::warning(NULL, "warning", tr("Builtin file ")+it->filename+tr(" does not exist!"), QMessageBox::Yes, QMessageBox::Yes);
            }
            continue;
        }
        if(it->category==Problem::Utility::FileCategory::resource||
            (it->category==Problem::Utility::FileCategory::testdata&&it->filetype!=Problem::Utility::FileType::plain) ) {
            if(!FileOp::exists(probPath+it->filename)) {
                problem->utils.remove(it->filename);
                continue;
            }
        }
        if(it->filetype==Problem::Utility::FileType::templ) {
            if(!FileOp::exists(probPath+it->filename+".tpl")) {
                problem->utils.remove(it->filename);
                continue;
            }
        }
    }
    for(QVector<Problem::Utility>::Iterator it=utilsvec.begin();it!=utilsvec.end();it++)
        if(it->filetype==Problem::Utility::FileType::templ){
            QFile tplfile(probPath+it->filename+".tpl");
            if(!tplfile.open(QIODevice::ReadOnly)) {
                problem->utils.remove(it->filename) ;
                continue;
            }
            QTextStream str(&tplfile);
            QString tplcontent=str.readAll();
            tplfile.close();
            if(!Codetpl::is_valid(tplcontent)) {
                problem->utils.remove(it->filename);
                continue;
            }
            QVector<QString> snippetnames=Codetpl::get_snippets(tplcontent);
            for(int index=0;index<snippetnames.size();index++)
                problem->utils[snippetnames[index]]={snippetnames[index],Problem::Utility::FileCategory::submission,Problem::Utility::FileType::snippet};
    }
    QStringList utilslist(problem->utils.keys());
    ui->jutilswid->clear();
    ui->jutilswid->addItems(utilslist);
    ui->filenametext->clear();
    update_cplsrc_option();
}
void ProblemEditor::update_cplsrc_option() {
    QStringList utilslist(problem->utils.keys());
    bool precompile=ui->phaserbtn->isChecked();
    for(QStringList::Iterator it=utilslist.begin();it!=utilslist.end();it++) {
        Problem::Utility util=problem->utils[*it];
        if(util.filetype==Problem::Utility::FileType::snippet||util.filetype==Problem::Utility::FileType::plain)
        {
            it=utilslist.erase(it)-1;
            continue;
        }
        if(!precompile) {
            if(util.category==Problem::Utility::FileCategory::submission)
            {
                it=utilslist.erase(it)-1;
                continue;
            }
        }
    }
    for(int index=0;index<max_cplsrc;index++) {
        cplsrc[index].clear();
        cplsrc[index].clearEditText();
        cplsrc[index].addItems(utilslist);
    }
}
void ProblemEditor::refresh() {
    loadBasic();
    if(!pdfview->isHidden()) loadPDF();
    loadUtils();
    update();
}

void ProblemEditor::on_refreshbtn_clicked()
{
    refresh();
}


void ProblemEditor::on_backbtn_clicked()
{
    refresh();
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

void ProblemEditor::set_cplsrc_cnt(int new_cnt) {
    if(new_cnt<1||new_cnt>max_cplsrc) return;
    if(new_cnt>cplsrc_cnt)
        for(;cplsrc_cnt<new_cnt;cplsrc_cnt++) {
        ui->srchlayout->addWidget(&cplsrc[cplsrc_cnt]);
        cplsrc[cplsrc_cnt].show();
    }
    else for(;cplsrc_cnt>new_cnt;cplsrc_cnt--) {
        cplsrc[cplsrc_cnt--].clearEditText();
        ui->srchlayout->removeWidget(&cplsrc[cplsrc_cnt-1]);
        cplsrc[cplsrc_cnt-1].hide();
    }
    return;
}

void ProblemEditor::on_srcaddbtn_clicked()
{
    if(cplsrc_cnt>=max_cplsrc) return;
    ui->srchlayout->addWidget(&cplsrc[cplsrc_cnt]);
    cplsrc[cplsrc_cnt].show();
    cplsrc_cnt++;
}

void ProblemEditor::on_srcrembtn_clicked()
{
    if(cplsrc_cnt<=1) return;
    cplsrc_cnt--;
    cplsrc[cplsrc_cnt].clearEditText();
    ui->srchlayout->removeWidget(&cplsrc[cplsrc_cnt]);
    cplsrc[cplsrc_cnt].hide();
}

void ProblemEditor::on_jutilswid_itemClicked(QListWidgetItem *item)
{
    QString name=item->text();
    if(!problem->utils.contains(name)) {
        ui->jutilswid->clearSelection();
        loadUtils();
        return;
    }
    Problem::Utility util=problem->utils[name];


    ui->filenametext->setText(util.filename);
    ui->categorycombo->setCurrentIndex((int) util.category);
    ui->filetypecombo->setCurrentIndex((int) util.filetype);
    return;
}

void ProblemEditor::on_jutilsaddbtn_clicked()
{
    QString name=ui->filenametext->text();
    Problem::Utility::FileCategory category=Problem::Utility::FileCategory(ui->categorycombo->currentIndex());
    Problem::Utility::FileType type=Problem::Utility::FileType(ui->filetypecombo->currentIndex());
    // process template file name
    if(type==Problem::Utility::FileType::templ) {
        if(!name.endsWith(".tpl")) {
            QMessageBox::warning(NULL, "warning", tr("Invalid template file name!(template file name should be the name of original code+\".tpl\"),e.g. example.cpp.tpl"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        name.erase(name.end()-4,name.end());
    }
    // validate filename
    if(problem->utils.contains(name)) {
        QMessageBox::warning(NULL, "warning", tr("File already exists!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(!StrVal::isValidFileName(name)) {
        QMessageBox::warning(NULL, "warning", tr("Invalid file name!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(category==Problem::Utility::FileCategory::builtin || type==Problem::Utility::FileType::snippet) {// invalid type/category filter
        QMessageBox::warning(NULL, "warning", tr("Invalid file type/category!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(category!=Problem::Utility::FileCategory::submission&&type==Problem::Utility::FileType::templ) {
        QMessageBox::warning(NULL, "warning", tr("Template file only allowed for submission category!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(category==Problem::Utility::FileCategory::resource||
        (category==Problem::Utility::FileCategory::testdata&&type==Problem::Utility::FileType::code)||
        type==Problem::Utility::FileType::templ) {
        // file importing needed
        QString fpath=QFileDialog::getOpenFileName(this, tr("Import from file"), probPath, tr("all files (*.*)"));

        if(type==Problem::Utility::FileType::templ) {
            if(!FileOp::copy(fpath,probPath+name+".tpl",true)) {
                QMessageBox::warning(NULL, "warning", tr("Failed while importing file!"), QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
            // template file parsing
            QFile tplfile(probPath+name+".tpl");
            if(!tplfile.open(QIODevice::ReadOnly)) {
                QMessageBox::warning(NULL, "warning", tr("Failed while reading template file!"), QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
            QTextStream str(&tplfile);
            QString tplcontent=str.readAll();
            tplfile.close();
            if(!Codetpl::is_valid(tplcontent)) {
                QMessageBox::warning(NULL, "warning", tr("Invalid template file!"), QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
            QVector<QString> snippetnames=Codetpl::get_snippets(tplcontent);
            for(int index=0;index<snippetnames.size();index++) if(problem->utils.contains(snippetnames[index])) {
                if(problem->utils[snippetnames[index]].filetype!=Problem::Utility::FileType::snippet) {
                    QMessageBox::warning(NULL, "warning", tr("Invalid snippet file!"), QMessageBox::Yes, QMessageBox::Yes);
                    return;
                }
            }
            problem->utils[name]={name,category,type};
            for(int index=0;index<snippetnames.size();index++)
                problem->utils[snippetnames[index]]={snippetnames[index],Problem::Utility::FileCategory::submission,Problem::Utility::FileType::snippet};
        } else {
            if(!FileOp::copy(fpath,probPath+name,true)) {
                QMessageBox::warning(NULL, "warning", tr("Failed while importing file!"), QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
            problem->utils[name]={name,category,type};
        }
    } else {
        problem->utils[name]={name,category,type};
    }
    loadUtils();
    return;
}


void ProblemEditor::on_jutilsrembtn_clicked()
{
    if(ui->jutilswid->selectedItems().isEmpty()) return;

    QString name=ui->jutilswid->currentItem()->text();
    // validate filename
    if(!problem->utils.contains(name)) {
        loadUtils();
        return;
    }

    // invalid type/category filter
    if(problem->utils[name].category==Problem::Utility::FileCategory::builtin||
        problem->utils[name].filetype==Problem::Utility::FileType::snippet) {
        QMessageBox::warning(NULL, "warning", tr("Invalid file type/category!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    Problem::Utility::FileCategory category=problem->utils[name].category;
    Problem::Utility::FileType type=problem->utils[name].filetype;

    if(category==Problem::Utility::FileCategory::resource||
        (category==Problem::Utility::FileCategory::testdata&&type!=Problem::Utility::FileType::plain) )
        FileOp::remove(probPath+name);
    if(type==Problem::Utility::FileType::templ)
        FileOp::remove(probPath+name+".tpl");
    problem->utils.remove(name);

    loadUtils();
    return;
}

void ProblemEditor::on_phaserbtn_clicked()
{
    update_cplsrc_option();
}


void ProblemEditor::on_phaserbtn_2_clicked()
{
    update_cplsrc_option();
}

