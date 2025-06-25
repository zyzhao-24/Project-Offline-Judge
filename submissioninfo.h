#ifndef SUBMISSIONINFO_H
#define SUBMISSIONINFO_H

#include <QWidget>

#include "ctsettings.h"
#include "texthighlighter.h"

namespace Ui {
class SubmissionInfo;
}

class SubmissionInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SubmissionInfo(QWidget *parent = nullptr);
    ~SubmissionInfo();
    void reset();
    void addcode(const QString& filename,const QString& filecontent);
    void addcase(int caseid,TResult verdict,double score);
private:
    Ui::SubmissionInfo *ui;
    QList<TextHighlighter*> highlighters;
};

#endif // SUBMISSIONINFO_H
