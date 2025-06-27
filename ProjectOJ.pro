QT       += core gui
QT       += pdf
QT       += pdfwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Qaesencryption/qaesencryption.cpp \
    codeeditorwidget.cpp \
    contesteditor.cpp \
    dataconfigwidget.cpp \
    judgepanel.cpp \
    judgesetting.cpp \
    judgingwidget.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    problemeditor.cpp \
    procexecinfowidget.cpp \
    studenteditor.cpp \
    submissioninfo.cpp \
    submitide.cpp \
    testdataprocessor.cpp \
    texthighlighter.cpp

HEADERS += \
    Qaesencryption/aesni/aesni-enc-cbc.h \
    Qaesencryption/aesni/aesni-enc-ecb.h \
    Qaesencryption/aesni/aesni-key-exp.h \
    Qaesencryption/aesni/aesni-key-init.h \
    Qaesencryption/qaesencryption.h \
    codeeditorwidget.h \
    contesteditor.h \
    ctsettings.h \
    dataconfigwidget.h \
    judge_utils.h \
    judgepanel.h \
    judgesetting.h \
    judgingwidget.h \
    login.h \
    mainwindow.h \
    problemeditor.h \
    procexecinfowidget.h \
    studenteditor.h \
    submissioninfo.h \
    submitide.h \
    procexelib.h \
    testdataprocessor.h \
    testlib/testlib.h \
    texthighlighter.h \
    val_utils.h

FORMS += \
    codeeditorwidget.ui \
    contesteditor.ui \
    dataconfigwidget.ui \
    judgepanel.ui \
    judgesetting.ui \
    judgingwidget.ui \
    login.ui \
    mainwindow.ui \
    problemeditor.ui \
    procexecinfowidget.ui \
    studenteditor.ui \
    submissioninfo.ui \
    submitide.ui \
    testdataprocessor.ui

TRANSLATIONS += \
    ProjectOJ_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    favicon.ico

RC_ICONS = favicon.ico

CONFIG +=debug_and_release
CONFIG(debug,debug|release){
    testlib.path = $${OUT_PWD}\\debug
}else{
    testlib.path = $${OUT_PWD}\\release
}
testlib.files =  $${PWD}\\testlib\\testlib.h

INSTALLS += \
    testlib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QScintilla/lib/ -lqscintilla2_qt6
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QScintilla/lib/ -lqscintilla2_qt6d

INCLUDEPATH += $$PWD/QScintilla/include
DEPENDPATH += $$PWD/QScintilla/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/QScintilla/lib/libqscintilla2_qt6.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/QScintilla/lib/libqscintilla2_qt6d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/QScintilla/lib/qscintilla2_qt6.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/QScintilla/lib/qscintilla2_qt6d.lib
