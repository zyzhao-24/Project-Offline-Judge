#include "judgingwidget.h"
#include "ui_judgingwidget.h"

JudgingWidget::JudgingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JudgingWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
}

JudgingWidget::~JudgingWidget()
{
    delete ui;
}

void JudgingWidget::closeEvent(QCloseEvent *event) {
    emit ExitWin();
}
