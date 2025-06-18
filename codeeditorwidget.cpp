#include "codeeditorwidget.h"
#include "ui_codeeditorwidget.h"

Highlighter::Highlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
{
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
}

void Highlighter::SetLanguage(const QString& language)
{
    hlRules.clear();
    if(language=="C/C++")
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
                        << "\\btrue\\b" << "\\bfalse\\b" << "\\bnullptr\\b";
        foreach (const QString &pattern, keywordPatterns) {
            highlightRule rule;
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            hlRules.append(rule);
        }
        singleLineCommentFormat.setForeground(Qt::darkGreen);
        highlightRule singleLineCommentRule;
        singleLineCommentRule.pattern = QRegularExpression("//[^\n]*");
        singleLineCommentRule.format = singleLineCommentFormat;
        hlRules.append(singleLineCommentRule);
    }
    else if(language=="Python")
    {
        keywordFormat.setForeground(Qt::darkMagenta);
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
        foreach (const QString &pattern, keywordPatterns) {
            highlightRule rule;
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            hlRules.append(rule);
        }
        singleLineCommentFormat.setForeground(Qt::darkGreen);
        highlightRule pyComment;
        pyComment.pattern = QRegularExpression("#[^\n]*");
        pyComment.format = singleLineCommentFormat;
        hlRules.append(pyComment);
    }
}
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const highlightRule &rule, hlRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

CodeEditorWidget::CodeEditorWidget(const QString& filename,QWidget *parent)
    : QWidget(parent)
    , save_path(filename),ui(new Ui::CodeEditorWidget)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->setupUi(this);
    CodeEdit = new QTextEdit(this);
    CodeEdit->setFont(QFont("Consolas", 12));
    CodeEdit->setTabStopDistance(40);
    layout->addWidget(CodeEdit);
    QPushButton *saveButton = new QPushButton("保存", this);
    QPushButton *cppBtn = new QPushButton("C++", this);
    QPushButton *pythonBtn = new QPushButton("Python", this);
    QHBoxLayout *langLayout = new QHBoxLayout();
    connect(saveButton, &QPushButton::clicked, this, &CodeEditorWidget::on_SaveBTN_clicked);
    layout->addWidget(saveButton);
    connect(cppBtn, &QPushButton::clicked, this, [this]() {
        highlighter->SetLanguage("C/C++");
    });
    connect(pythonBtn, &QPushButton::clicked, this, [this]() {
        highlighter->SetLanguage("Python");
    });
    langLayout->addWidget(cppBtn);
    langLayout->addWidget(pythonBtn);
    setLayout(layout);
    layout->addLayout(langLayout);
    highlighter = new Highlighter(CodeEdit->document());
}

CodeEditorWidget::~CodeEditorWidget()
{
    delete ui;
}

void CodeEditorWidget::on_SaveBTN_clicked()
{
    QFile file(save_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法保存文件:\n" + file.errorString());
        return;
    }
    QTextStream out(&file);
    out << CodeEdit->toPlainText();
    file.close();
    QMessageBox::information(this, "成功", "文件已保存！");
    return;
}

