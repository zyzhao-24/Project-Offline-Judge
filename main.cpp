#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ctsettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ProjectOJ_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QString str1="code fill <snippet filename=\"snippet_name\"> code fill";
    QMap<QString,QString> fill;
    fill["snippet_name1"]="fcode";
    qDebug()<<Codetpl::get_snippets(str1)<<Codetpl::fill_in(str1,fill)<<Qt::endl;
    MainWindow w;
    w.show();
    return a.exec();
}
