#ifndef SUBMISSIONINFO_H
#define SUBMISSIONINFO_H

#include <QWidget>
#include <QSyntaxHighlighter>

#include "ctsettings.h"

namespace Ui {
class SubmissionInfo;
}

class SubmissionHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    SubmissionHighlighter(QTextDocument *parent=nullptr,const QString& suffix={});
protected:
    void highlightBlock(const QString& text) override;
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat macrosFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

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
    QList<SubmissionHighlighter*> highlighters;
};

#endif // SUBMISSIONINFO_H
