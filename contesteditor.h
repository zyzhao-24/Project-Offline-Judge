#ifndef CONTESTEDITOR_H
#define CONTESTEDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "ctsettings.h"
#include "judgepanel.h"

namespace Ui {
class ContestEditor;
}

class ContestEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ContestEditor(QWidget *parent = nullptr);
    ~ContestEditor();

    QMainWindow* father=nullptr;//will be set to father mainwindow
    QString ctPath="";//contest path

    Contest contest;

    void loadPath();
    void setName();
    void loadName();
    void setTime();
    void loadTime();
    void setPa();
    void loadPa();
    void loadProb();

    void refresh();
    void save();
    void savestu();

private slots:
    //refresh()
    void on_refreshbtn_clicked();

    //save()
    void on_savebtn_clicked();

    //save() and close window, return to homepage
    void on_returnbtn_clicked();

    //setName()
    void on_setnamebtn_clicked();

    //disable/enable start time input when "disable" checkbox toggles
    void on_dstchkbox_checkStateChanged(const Qt::CheckState &arg1);
    //disable/enable end time input when "disable" checkbox toggles
    void on_dedchkbox_checkStateChanged(const Qt::CheckState &arg1);

    //setTime()
    void on_settimebtn_clicked();

    //add a row to participant Table Widget
    void on_addbtn_clicked();

    //remove selected row from participant Table Widget
    void on_rembtn_clicked();

    //import to participant Table Widget from csv
    void on_impbtn_clicked();

    //clear data in participant Table Widget
    void on_clearbtn_clicked();

    //export data in participant Table Widget to csv
    void on_expbtn_clicked();

    //setPa()
    void on_setbtn_clicked();

    //add problem to probmap
    void on_addprobbtn_clicked();

    void on_renprobbtn_clicked();

    void on_remprobbtn_clicked();

    void on_problistwid_currentTextChanged(const QString &currentText);

    void on_confprobbtn_clicked();

    void on_gradebtn_clicked();

    void on_stusavebtn_clicked();

private:
    Ui::ContestEditor *ui;
};

#endif // CONTESTEDITOR_H
