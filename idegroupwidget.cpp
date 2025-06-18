#include "idegroupwidget.h"
#include "ui_idegroupwidget.h"

IDEGroupWidget::IDEGroupWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IDEGroupWidget)
{
    ui->setupUi(this);
}

IDEGroupWidget::~IDEGroupWidget()
{
    delete ui;
}

QString IDEGroupWidget::getText(){
    return ui->plainTextEdit->toPlainText();
}
