#ifndef DATACONFIGWIDGET_H
#define DATACONFIGWIDGET_H

#include <QWidget>
#include "ctsettings.h"
namespace Ui {
class DataConfigWidget;
}

class DataConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataConfigWidget(QWidget *parent = nullptr);
    ~DataConfigWidget();
    Problem* problem=nullptr;
    Testdata::TDSetting *setting=nullptr;

    void refresh();
private slots:
    void on_ValidatorCBOX_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButton_clicked();

private:
    Ui::DataConfigWidget *ui;
};

#endif // DATACONFIGWIDGET_H
