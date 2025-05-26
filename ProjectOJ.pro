QT       += core gui
QT       += pdf
QT       += pdfwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Qaesencryption/qaesencryption.cpp \
    contesteditor.cpp \
    dataconfigwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    problemeditor.cpp \
    testdataprocessor.cpp

HEADERS += \
    Qaesencryption/aesni/aesni-enc-cbc.h \
    Qaesencryption/aesni/aesni-enc-ecb.h \
    Qaesencryption/aesni/aesni-key-exp.h \
    Qaesencryption/aesni/aesni-key-init.h \
    Qaesencryption/qaesencryption.h \
    contesteditor.h \
    ctsettings.h \
    dataconfigwidget.h \
    judge_utils.h \
    mainwindow.h \
    problemeditor.h \
    procexelib.h \
    testdataprocessor.h \
    testlib/testlib.h \
    val_utils.h

FORMS += \
    contesteditor.ui \
    dataconfigwidget.ui \
    mainwindow.ui \
    problemeditor.ui \
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
