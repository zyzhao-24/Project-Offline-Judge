#ifndef PROCEXECINFOWIDGET_H
#define PROCEXECINFOWIDGET_H

#include <QWidget>
#include "ctsettings.h"
namespace Ui {
class ProcExecInfoWidget;
}

class ProcExecInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProcExecInfoWidget(QWidget *parent = nullptr);
    ~ProcExecInfoWidget();
    JudgeProcess::process* exec_info=nullptr;
    Problem* problem=nullptr;
    void refresh();
private slots:
    void on_procnameTXT_editingFinished();

    void on_interactchkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_paramsTXT_editingFinished();

    void on_IntercomboBox_currentTextChanged(const QString &arg1);

    void on_refreshBTN_clicked();

private:
    Ui::ProcExecInfoWidget *ui;
};

#endif // PROCEXECINFOWIDGET_H
