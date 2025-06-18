#ifndef IDEGROUPWIDGET_H
#define IDEGROUPWIDGET_H

#include <QWidget>
#include "ctsettings.h"
namespace Ui {
class IDEGroupWidget;
}

class IDEGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IDEGroupWidget(QWidget *parent = nullptr);
    ~IDEGroupWidget();
    QString getText();

private:
    Ui::IDEGroupWidget *ui;
};

#endif // IDEGROUPWIDGET_H
