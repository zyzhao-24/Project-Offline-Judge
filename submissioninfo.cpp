#include "submissioninfo.h"
#include "ui_submissioninfo.h"
#include <QPlainTextEdit>
SubmissionHighlighter::SubmissionHighlighter(QTextDocument *parent,const QString& suffix):QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    if(suffix=="c"||suffix=="h"||suffix=="cpp"||suffix=="hpp")
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                        << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                        << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                        << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                        << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                        << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                        << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                        << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                        << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                        << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"
                        << "\\breturn\\b" << "\\bif\\b" << "\\belse\\b"
                        << "\\bfor\\b" << "\\bwhile\\b" << "\\bdo\\b"
                        << "\\bswitch\\b" << "\\bcase\\b" << "\\bbreak\\b"
                        << "\\bcontinue\\b" << "\\bgoto\\b" << "\\bdefault\\b"
                        << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b"
                        << "\\bnew\\b" << "\\bdelete\\b" << "\\bthis\\b"
                        << "\\btrue\\b" << "\\bfalse\\b" << "\\bnullptr\\b"
                        << "\\bauto\\b" << "\\busing\\b";

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);
        for(const QString &pattern: keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        quotationFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(QStringLiteral("\"[^\"]*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);
        rule.pattern = QRegularExpression(QStringLiteral("\'[^\']*\'"));
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::red);
        rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        macrosFormat.setForeground(Qt::red);
        rule.pattern = QRegularExpression(QStringLiteral("#(\\\n|[^\n])*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::red);
        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
    }
    else if(suffix=="py")
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\bFalse\\b" << "\\bNone\\b" << "\\bTrue\\b"
                        << "\\band\\b" << "\\bas\\b" << "\\bassert\\b"
                        << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
                        << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b"
                        << "\\belse\\b" << "\\bexcept\\b" << "\\bfinally\\b"
                        << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b"
                        << "\\bif\\b" << "\\bimport\\b" << "\\bin\\b"
                        << "\\bis\\b" << "\\blambda\\b" << "\\bnonlocal\\b"
                        << "\\bnot\\b" << "\\bor\\b" << "\\bpass\\b"
                        << "\\braise\\b" << "\\breturn\\b" << "\\btry\\b"
                        << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);
        for(const QString &pattern: keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        quotationFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(QStringLiteral("\"[^\"]*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);
        rule.pattern = QRegularExpression(QStringLiteral("\'[^\']*\'"));
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::red);
        rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        macrosFormat.setForeground(Qt::red);
        rule.pattern = QRegularExpression(QStringLiteral("@[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::red);
        commentStartExpression = QRegularExpression(QStringLiteral("\"\"\""));
        commentEndExpression = QRegularExpression(QStringLiteral("\"\"\""));
    }
}
void SubmissionHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);
    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

SubmissionInfo::SubmissionInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubmissionInfo)
{
    ui->setupUi(this);
}

SubmissionInfo::~SubmissionInfo()
{
    while(!highlighters.isEmpty()) {
        SubmissionHighlighter* highlighter=highlighters.back();
        highlighters.pop_back();
        delete highlighter;
    }
    delete ui;
}

void SubmissionInfo::reset() {
    while(!highlighters.isEmpty()) {
        SubmissionHighlighter* highlighter=highlighters.back();
        highlighters.pop_back();
        delete highlighter;
    }
    ui->tabWidget->clear();
    ui->tableWidget->setRowCount(0);
}
void SubmissionInfo::addcode(const QString& filename,const QString& filecontent) {
    QWidget* codewid=new QWidget(this);
    codewid->setFixedSize(ui->tabWidget->size()-QSize({5,30}));
    codewid->show();
    ui->tabWidget->addTab(codewid,filename);
    QPlainTextEdit* codetext=new QPlainTextEdit(filecontent,codewid);
    codetext->setFixedSize(codewid->size());
    codetext->setReadOnly(true);
    codetext->setFont(QFont("Consolas", 10));
    QFontMetrics metrics(codetext->font());
    codetext->setTabStopDistance(4 * metrics.averageCharWidth());
    if(filename.contains(".")) highlighters.push_back(new SubmissionHighlighter(codetext->document(),filename.split(".").back()));
    codetext->show();
}
void SubmissionInfo::addcase(int caseid,TResult verdict,double score) {
    int row=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(caseid)));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(DescriptionStr(verdict)));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(score,'f',2)));

    ui->tableWidget->item(row,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,1)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,1)->setForeground(QBrush(DescriptionCol(verdict)));
    ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter);
}
