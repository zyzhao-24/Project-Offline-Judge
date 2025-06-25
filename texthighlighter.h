#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class TextHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    TextHighlighter(QTextDocument *parent=nullptr,const QString& suffix={});
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

#endif // TEXTHIGHLIGHTER_H
