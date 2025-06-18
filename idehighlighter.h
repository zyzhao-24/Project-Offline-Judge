#ifndef IDEHIGHLIGHTER_H
#define IDEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
class IDEHighlighter:public QSyntaxHighlighter
{
public:
    IDEHighlighter(QTextDocument* document);
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingrules;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    QTextCharFormat multiLineCommentFormat;
};

#endif // IDEHIGHLIGHTER_H
