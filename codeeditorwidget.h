#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPushButton>

namespace Ui {
class CodeEditorWidget;
}
class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent=nullptr);
    void SetLanguage(const QString &language);
protected:
    void highlightBlock(const QString& text) override;
private:
    struct highlightRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<highlightRule> hlRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
};
class CodeEditorWidget : public QWidget
{
    Q_OBJECT
private:
    QTextEdit *CodeEdit;
    Highlighter *highlighter;
    QString save_path;
public:
    ~CodeEditorWidget();
    CodeEditorWidget(const QString& filename,QWidget* parent=nullptr);
private slots:
    void on_SaveBTN_clicked();
private:
    Ui::CodeEditorWidget *ui;
};

#endif // CODEEDITORWIDGET_H
