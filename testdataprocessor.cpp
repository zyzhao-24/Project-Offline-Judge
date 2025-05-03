#include "testdataprocessor.h"
#include "ui_testdataprocessor.h"

TestDataProcessor::TestDataProcessor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestDataProcessor)
{
    ui->setupUi(this);
}

TestDataProcessor::~TestDataProcessor()
{
    delete ui;
}
