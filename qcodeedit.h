#ifndef QCODEEDIT_H
#define QCODEEDIT_H

#include <QSci/qsciscintilla.h>
#include <QSci/qscilexercpp.h>
#include <QSci/qscilexerpython.h>

class QCodeEdit : public QsciScintilla
{
    Q_OBJECT
public:
    QCodeEdit(QWidget* parent=nullptr,const QString& suffix="");
    QCodeEdit(const QString& content,QWidget* parent=nullptr,const QString& suffix="");
    ~QCodeEdit();

    void setupVSCodeLightTheme();
    void setCppLang();
    void setPythonLang();
    void setPlainText();
protected:
    QsciLexer* lexer=nullptr;
};

#endif // QCODEEDIT_H
