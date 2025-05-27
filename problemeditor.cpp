#include "problemeditor.h"
#include "ui_problemeditor.h"

ProblemEditor::ProblemEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProblemEditor)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

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
                problem->validationSuccess=false;
                ui->validationbtn_2->setChecked(true);
                ui->validationbtn->setChecked(false);
                continue;
            }
        }
        if(it->filetype==Problem::Utility::FileType::templ) {
            if(!FileOp::exists(probPath+it->filename+".tpl")) {
                problem->utils.remove(it->filename);
                problem->validationSuccess=false;
                ui->validationbtn_2->setChecked(true);
                ui->validationbtn->setChecked(false);
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
                problem->validationSuccess=false;
                ui->validationbtn_2->setChecked(true);
                ui->validationbtn->setChecked(false);
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

void ProblemEditor::loadCplSettings() {
    QVector<Problem::CompileSetting> cplsetvec(problem->cpl_settings.values());
    for(QVector<Problem::CompileSetting>::Iterator it=cplsetvec.begin();it!=cplsetvec.end();it++) {
        bool valid=true;
        if(it->inputs.size()>max_cplsrc) valid=false;
        foreach(QString input,it->inputs) {
            if(it->inputs.count(input)>1) {
                valid=false;
                break;
            }
            if(!problem->utils.contains(input)) {
                valid=false;
                break;
            }
            Problem::Utility::FileCategory category=problem->utils[input].category;
            Problem::Utility::FileType filetype=problem->utils[input].filetype;
            if(filetype==Problem::Utility::FileType::plain||filetype==Problem::Utility::FileType::snippet) {
                valid=false;
                break;
            }
            if(it->precompile) if(category==Problem::Utility::FileCategory::submission) {
                valid=false;
                break;
            }
        }
        if(!valid) {
            problem->cpl_settings.remove(it->output);
            problem->validationSuccess=false;
            ui->validationbtn_2->setChecked(true);
            ui->validationbtn->setChecked(false);
            continue;
        }
    }
    cplsetvec=problem->cpl_settings.values();
    ui->cplsettabwid->setRowCount(cplsetvec.size());
    for(int index=0;index<cplsetvec.size();index++) {
        QString stage=cplsetvec[index].precompile?tr("precompile"):tr("in judge");
        QTableWidgetItem *phaseItem = new QTableWidgetItem(stage);
        QTableWidgetItem *compilerItem = new QTableWidgetItem(cplsetvec[index].compiler);
        QTableWidgetItem *sourceItem = new QTableWidgetItem(cplsetvec[index].inputs.join(" "));
        QTableWidgetItem *outputItem = new QTableWidgetItem(cplsetvec[index].output);
        QTableWidgetItem *paramsItem = new QTableWidgetItem(cplsetvec[index].params);
        ui->cplsettabwid->setItem(index,0,phaseItem);
        ui->cplsettabwid->setItem(index,1,compilerItem);
        ui->cplsettabwid->setItem(index,2,sourceItem);
        ui->cplsettabwid->setItem(index,3,outputItem);
        ui->cplsettabwid->setItem(index,4,paramsItem);
    }
    ui->cplsettabwid->clearSelection();
    ui->outfnametext->clear();
    set_cplsrc_cnt(1);
    cplsrc[0].clearEditText();
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
        if(precompile) {
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
    loadCplSettings();
    if(problem->validationSuccess) {
        ui->validationbtn->setChecked(true);
        ui->validationbtn_2->setChecked(false);
    } else {
        ui->validationbtn_2->setChecked(true);
        ui->validationbtn->setChecked(false);
    }
    update();
}

void ProblemEditor::on_refreshbtn_clicked()
{
    refresh();
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
    update_cplsrc_option();
    if(new_cnt<1||new_cnt>max_cplsrc) return;
    if(new_cnt>cplsrc_cnt)
        for(;cplsrc_cnt<new_cnt;cplsrc_cnt++) {
        ui->srchlayout->addWidget(&cplsrc[cplsrc_cnt]);
        cplsrc[cplsrc_cnt].show();
    }
    else for(;cplsrc_cnt>new_cnt;cplsrc_cnt--) {
        cplsrc[cplsrc_cnt-1].clearEditText();
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
            QMessageBox::warning(NULL, "warning", tr("Invalid template file name!(template file name should be the name of original code+\".tpl\", e.g. example.cpp.tpl)"), QMessageBox::Yes, QMessageBox::Yes);
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
    problem->validationSuccess=false;
    ui->validationbtn_2->setChecked(true);
    ui->validationbtn->setChecked(false);
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
    problem->validationSuccess=false;
    ui->validationbtn_2->setChecked(true);
    ui->validationbtn->setChecked(false);
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


void ProblemEditor::on_cplsetaddbtn_clicked()
{
    Problem::CompileSetting cplset={
        ui->phaserbtn->isChecked(),
        ui->cplcmdbox->currentText(),
        {},
        ui->outfnametext->text(),
        ui->paramtext->text()
    };
    if(!StrVal::isValidFileName(cplset.output)) {
        QMessageBox::warning(NULL, "warning", tr("Invalid file name!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(problem->cpl_settings.contains(cplset.output)) {
        QMessageBox::warning(NULL, "warning", tr("File already exists!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    for(int index=0;index<cplsrc_cnt;index++) {
        QString input=cplsrc[index].currentText();
        if(!problem->utils.contains(input)) {
            QMessageBox::warning(NULL, "warning", tr("Input file does not exist!"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        Problem::Utility::FileCategory category=problem->utils[input].category;
        Problem::Utility::FileType filetype=problem->utils[input].filetype;
        if(filetype==Problem::Utility::FileType::plain||filetype==Problem::Utility::FileType::snippet) {
            QMessageBox::warning(NULL, "warning", tr("Invalid file category!"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        if(cplset.precompile) if(category==Problem::Utility::FileCategory::submission) {
            QMessageBox::warning(NULL, "warning", tr("File in submission category not allowed in precompile stage!"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        cplset.inputs.append(input);
        if(cplset.inputs.count(input)>1) {
            QMessageBox::warning(NULL, "warning", tr("Input file duplicated!"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    problem->cpl_settings[cplset.output]=cplset;
    loadCplSettings();
    problem->validationSuccess=false;
    ui->validationbtn_2->setChecked(true);
    ui->validationbtn->setChecked(false);
}


void ProblemEditor::on_cplsetrembtn_clicked()
{
    if(ui->cplsettabwid->selectedItems().isEmpty()) return;
    int row_index=ui->cplsettabwid->currentRow();
    QString output=ui->cplsettabwid->item(row_index,3)->text();
    if(!problem->cpl_settings.contains(output)) {
        loadCplSettings();
        return;
    }
    problem->cpl_settings.remove(output);
    loadCplSettings();
    problem->validationSuccess=false;
    ui->validationbtn_2->setChecked(true);
    ui->validationbtn->setChecked(false);
}

void ProblemEditor::on_cplsettabwid_itemClicked(QTableWidgetItem *item)
{
    int row_index=item->row();
    QString output=ui->cplsettabwid->item(row_index,3)->text();
    if(!problem->cpl_settings.contains(output)) {
        loadCplSettings();
        return;
    }
    Problem::CompileSetting cplset=problem->cpl_settings[output];
    if(cplset.inputs.size()>max_cplsrc) {
        loadCplSettings();
        return;
    }
    if(cplset.precompile) {
        ui->phaserbtn->setChecked(true);
        ui->phaserbtn_2->setChecked(false);
    }
    else {
        ui->phaserbtn_2->setChecked(true);
        ui->phaserbtn->setChecked(false);
    }
    update_cplsrc_option();
    ui->outfnametext->setText(cplset.output);
    ui->cplcmdbox->setCurrentText(cplset.compiler);
    ui->paramtext->setText(cplset.params);
    set_cplsrc_cnt(cplset.inputs.size());
    for(int index=0;index<cplset.inputs.size();index++) {
        cplsrc[index].setCurrentText(cplset.inputs[index]);
    }
}


void ProblemEditor::on_TestDataBTN_clicked()
{
    tdataproc.problem=problem;
    tdataproc.probPath=probPath;
    tdataproc.show();
    tdataproc.show_subtsk_tree();
    tdataproc.refresh_testdata_settings();
    this->hide();
    connect(&tdataproc,SIGNAL(ExitWin()),this,SLOT(show()));
}

void ProblemEditor::closeEvent(QCloseEvent *event) {
    emit ExitWin();
}
void ProblemEditor::showEvent(QShowEvent *event) {
    refresh();
}

void ProblemEditor::on_validationbtn_clicked()
{
    problem->validationSuccess=true;
}


void ProblemEditor::on_validationbtn_2_clicked()
{
    problem->validationSuccess=false;
}

