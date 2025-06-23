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
    packer.cpp \
    problemeditor.cpp \
    idegroupwidget.cpp \
    idehighlighter.cpp \
    procexecinfowidget.cpp \
    studenteditor.cpp \
    submitide.cpp \
    testdataprocessor.cpp

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
    idegroupwidget.h \
    idehighlighter.h \
    packer.h \
    procexecinfowidget.h \
    studenteditor.h \
    submitide.h \
    procexelib.h \
    testdataprocessor.h \
    testlib/testlib.h \
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
    submitide.ui \
    idegroupwidget.ui \
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
