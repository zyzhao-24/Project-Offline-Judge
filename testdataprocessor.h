#ifndef TESTDATAPROCESSOR_H
#define TESTDATAPROCESSOR_H

#include <QMainWindow>

namespace Ui {
class TestDataProcessor;
}

class TestDataProcessor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestDataProcessor(QWidget *parent = nullptr);
    ~TestDataProcessor();

private:
    Ui::TestDataProcessor *ui;
};

#endif // TESTDATAPROCESSOR_H
