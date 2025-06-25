#include "submissioninfo.h"
#include "ui_submissioninfo.h"
#include <QPlainTextEdit>

SubmissionInfo::SubmissionInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubmissionInfo)
{
    ui->setupUi(this);
}

SubmissionInfo::~SubmissionInfo()
{
    while(!highlighters.isEmpty()) {
        TextHighlighter* highlighter=highlighters.back();
        highlighters.pop_back();
        delete highlighter;
    }
    delete ui;
}

void SubmissionInfo::reset() {
    while(!highlighters.isEmpty()) {
        TextHighlighter* highlighter=highlighters.back();
        highlighters.pop_back();
        delete highlighter;
    }
    ui->tabWidget->clear();
    ui->tableWidget->setRowCount(0);
}
void SubmissionInfo::addcode(const QString& filename,const QString& filecontent) {
    QWidget* codewid=new QWidget(this);
    codewid->setFixedSize(ui->tabWidget->size()-QSize({5,30}));
    codewid->show();
    ui->tabWidget->addTab(codewid,filename);
    QPlainTextEdit* codetext=new QPlainTextEdit(filecontent,codewid);
    codetext->setFixedSize(codewid->size());
    codetext->setReadOnly(true);
    codetext->setFont(QFont("Consolas", 10));
    QFontMetrics metrics(codetext->font());
    codetext->setTabStopDistance(4 * metrics.averageCharWidth());
    if(filename.contains(".")) highlighters.push_back(new TextHighlighter(codetext->document(),filename.split(".").back()));
    codetext->show();
}
void SubmissionInfo::addcase(int caseid,TResult verdict,double score) {
    int row=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(caseid)));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(DescriptionStr(verdict)));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(score,'f',2)));

    ui->tableWidget->item(row,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,1)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,1)->setForeground(QBrush(DescriptionCol(verdict)));
    ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter);
}
