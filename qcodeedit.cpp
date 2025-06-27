#include "qcodeedit.h"
#include <QPalette>
#include <QStyleFactory>
#include <QApplication>

QCodeEdit::QCodeEdit(QWidget* parent, const QString& suffix)
    : QsciScintilla(parent), lexer(nullptr)
{
    // 设置VSCode风格的浅色主题
    setupVSCodeLightTheme();

    // 原有代码保持不变
    setUtf8(true);
    setMarginLineNumbers(1, true);
    setMarginWidth(1, 50);
    setAutoIndent(true);
    setIndentationGuides(true);
    setTabWidth(4);

    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    setMatchedBraceBackgroundColor(QColor(206, 214, 244)); // 浅蓝色匹配括号背景
    setUnmatchedBraceForegroundColor(QColor(201, 44, 44)); // 红色未匹配括号

    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(232, 240, 254)); // 浅蓝色当前行背景

    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionThreshold(2);
    setAutoCompletionShowSingle(false);

    QFont font("Consolas", 10);
    setFont(font);

    if(suffix=="c"||suffix=="h"||suffix=="cpp"||suffix=="hpp")
        setCppLang();
    else if (suffix == "py")
        setPythonLang();
    else setPlainText();
}

QCodeEdit::QCodeEdit(const QString& content, QWidget* parent, const QString& suffix)
    : QsciScintilla(parent), lexer(nullptr)
{
    // 设置VSCode风格的浅色主题
    setupVSCodeLightTheme();

    // 原有代码保持不变
    setUtf8(true);
    setMarginLineNumbers(0, true);
    setMarginWidth(0, 50);
    setAutoIndent(true);
    setIndentationGuides(true);
    setTabWidth(4);

    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    setMatchedBraceBackgroundColor(QColor(206, 214, 244)); // 浅蓝色匹配括号背景
    setUnmatchedBraceForegroundColor(QColor(201, 44, 44)); // 红色未匹配括号

    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(232, 240, 254)); // 浅蓝色当前行背景

    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionThreshold(2);
    setAutoCompletionShowSingle(false);

    QFont font("Consolas", 10);
    setFont(font);

    if(suffix=="c"||suffix=="h"||suffix=="cpp"||suffix=="hpp")
        setCppLang();
    else if (suffix == "py")
        setPythonLang();
    else setPlainText();

    setText(content);
}

QCodeEdit::~QCodeEdit() {
    if(lexer && lexer!=nullptr) {
        delete lexer;
    }
}

void QCodeEdit::setupVSCodeLightTheme()
{
    // VSCode浅色主题颜色方案
    QColor bgColor(255, 255, 255);       // 纯白背景
    QColor textColor(30, 30, 30);        // 深灰色文本
    QColor lineNumBg(245, 245, 245);     // 浅灰色行号背景
    QColor lineNumColor(128, 128, 128);  // 灰色行号文本
    QColor caretColor(9, 106, 217);      // 蓝色光标
    QColor selectionColor(173, 214, 255);// 浅蓝色选中区域
    QColor indentGuideColor(232, 232, 232); // 浅灰色缩进参考线

    // 应用基本颜色设置
    setPaper(bgColor);
    setColor(textColor);
    setCaretForegroundColor(caretColor);
    setSelectionBackgroundColor(selectionColor);

    // 行号区域
    setMarginsBackgroundColor(lineNumBg);
    setMarginsForegroundColor(lineNumColor);

    // 折叠区域
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setFoldMarginColors(lineNumBg, QColor(220, 220, 220));

    // 缩进参考线
    setIndentationGuidesForegroundColor(indentGuideColor);
    setIndentationGuidesBackgroundColor(bgColor);

    // 滚动条样式
    setScrollWidth(1);
    setScrollWidthTracking(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Base, bgColor);
    pal.setColor(QPalette::Text, textColor);
    setPalette(pal);

    // 自动补全样式
    setAutoCompletionFillupsEnabled(true);
    setAutoCompletionFillups(".:;!?{}()[]<>+-*/%&|^~=@");
}

void QCodeEdit::setCppLang()
{
    setIndentationsUseTabs(true);
    if(lexer && lexer!=nullptr) {
        delete lexer;
    }
    lexer = new QsciLexerCPP(this);
    setLexer(lexer);

    // VSCode Light+主题的C++语法高亮颜色
    lexer->setColor(QColor(0, 0, 255), QsciLexerCPP::Keyword);        // 关键字 - 蓝色
    lexer->setColor(QColor(43, 145, 175), QsciLexerCPP::GlobalClass); // 类名 - 青色
    lexer->setColor(QColor(106, 153, 85), QsciLexerCPP::Comment);     // 注释 - 绿色
    lexer->setColor(QColor(106, 153, 85), QsciLexerCPP::CommentLine); // 行注释 - 绿色
    lexer->setColor(QColor(9, 134, 88), QsciLexerCPP::Number);        // 数字 - 深绿色
    lexer->setColor(QColor(163, 21, 21), QsciLexerCPP::PreProcessor); // 预处理器 - 红色
    lexer->setColor(QColor(163, 21, 21), QsciLexerCPP::DoubleQuotedString);   // 字符串 - 红色
    lexer->setColor(QColor(163, 21, 21), QsciLexerCPP::SingleQuotedString);   // 字符串 - 红色
    lexer->setColor(QColor(30, 30, 30), QsciLexerCPP::Default);       // 默认文本 - 深灰色
    lexer->setColor(QColor(0, 0, 0), QsciLexerCPP::Operator);         // 操作符 - 黑色

    // 设置字体样式
    lexer->setFont(QFont("Consolas", 10));
    lexer->setPaper(QColor(255, 255, 255)); // 白色背景

    // 设置注释字体为斜体
    lexer->setFont(QFont("Consolas", 10, -1, true), QsciLexerCPP::Comment);
    lexer->setFont(QFont("Consolas", 10, -1, true), QsciLexerCPP::CommentLine);
}

void QCodeEdit::setPythonLang()
{
    setIndentationsUseTabs(false);
    if(lexer && lexer!=nullptr) {
        delete lexer;
    }
    lexer = new QsciLexerPython(this);
    setLexer(lexer);

    // VSCode Light+主题的Python语法高亮颜色
    lexer->setColor(QColor(0, 0, 255), QsciLexerPython::Keyword);      // 关键字 - 蓝色
    lexer->setColor(QColor(43, 145, 175), QsciLexerPython::ClassName); // 类名 - 青色
    lexer->setColor(QColor(0, 122, 204), QsciLexerPython::FunctionMethodName);   // 函数 - VSCode蓝色
    lexer->setColor(QColor(106, 153, 85), QsciLexerPython::Comment);   // 注释 - 绿色
    lexer->setColor(QColor(9, 134, 88), QsciLexerPython::Number);      // 数字 - 深绿色
    lexer->setColor(QColor(163, 21, 21), QsciLexerPython::Decorator);  // 装饰器 - 红色
    lexer->setColor(QColor(30, 30, 30), QsciLexerPython::Default);     // 默认文本 - 深灰色
    lexer->setColor(QColor(0, 0, 0), QsciLexerPython::Operator);       // 操作符 - 黑色
    lexer->setColor(QColor(163, 21, 21), QsciLexerPython::SingleQuotedString); // 字符串 - 红色
    lexer->setColor(QColor(163, 21, 21), QsciLexerPython::DoubleQuotedString); // 字符串 - 红色
    lexer->setColor(QColor(106, 153, 85), QsciLexerPython::TripleSingleQuotedString); // 多行字符串 - 绿色
    lexer->setColor(QColor(106, 153, 85), QsciLexerPython::TripleDoubleQuotedString); // 多行字符串 - 绿色

    // 设置字体样式
    lexer->setFont(QFont("Consolas", 10));
    lexer->setPaper(QColor(255, 255, 255)); // 白色背景

    // 设置注释字体为斜体
    lexer->setFont(QFont("Consolas", 10, -1, true), QsciLexerPython::Comment);
    lexer->setFont(QFont("Consolas", 10, -1, true), QsciLexerPython::TripleSingleQuotedString);
    lexer->setFont(QFont("Consolas", 10, -1, true), QsciLexerPython::TripleDoubleQuotedString);
}

void QCodeEdit::setPlainText()
{
    setIndentationsUseTabs(true);
    if(lexer && lexer!=nullptr) {
        delete lexer;
    }

    // 默认浅色主题设置
    QColor bgColor(255, 255, 255); // 白色背景
    QColor textColor(30, 30, 30);  // 深灰色文本

    setPaper(bgColor);
    setColor(textColor);
}
