#-------------------------------------------------
#
# Project created by QtCreator 2019-05-15T19:34:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AtisLauncher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    model/basemodel.cpp \
    model/itemmodel.cpp \
    controller/profilecontroller.cpp \
    widgets/Forms/sectionform.cpp \
    widgets/Forms/setupform.cpp \
    widgets/Forms/itemform.cpp \
    widgets/Forms/profileform.cpp \
    widgets/Dialogs/newprofiledialog.cpp \
    widgets/Dialogs/taskdialog.cpp \
    widgets/Dialogs/taskcreationform.cpp \
    Utils/utilities.cpp \
    widgets/Forms/runtimeform.cpp \
    widgets/Tools/editablelabel.cpp \
    process/processmanager.cpp \
    widgets/Logs/logpageform.cpp \
    widgets/Stacked/basestackedpage.cpp \
    widgets/Forms/baseform.cpp

HEADERS += \
        mainwindow.h \
    model/basemodel.h \
    model/itemmodel.h \
    controller/profilecontroller.h \
    widgets/Forms/sectionform.h \
    widgets/Forms/setupform.h \
    widgets/Forms/itemform.h \
    widgets/Forms/profileform.h \
    widgets/Dialogs/newprofiledialog.h \
    widgets/Dialogs/taskdialog.h \
    widgets/Dialogs/taskcreationform.h \
    Utils/utilities.h \
    widgets/Forms/runtimeform.h \
    widgets/Tools/editablelabel.h \
    process/processmanager.h \
    widgets/Logs/logpageform.h \
    widgets/Stacked/basestackedpage.h \
    widgets/Forms/baseform.h

FORMS += \
        mainwindow.ui \
    widgets/Forms/sectionform.ui \
    widgets/Forms/setupform.ui \
    widgets/Forms/itemform.ui \
    widgets/Forms/profileform.ui \
    widgets/Dialogs/newprofiledialog.ui \
    widgets/Dialogs/taskdialog.ui \
    widgets/Dialogs/taskcreationform.ui \
    widgets/Forms/runtimeform.ui \
    widgets/Tools/editablelabel.ui \
    widgets/Logs/logpageform.ui

DISTFILES +=

RESOURCES += \
    resources/mrl_resources.qrc \
    qdarkstyle/style.qrc \
    resources/material_design_rc/resources.qrc

unix:!macx: LIBS += -L$$PWD/libs/ -lcomponents

INCLUDEPATH += $$PWD/libs/qt_material_headers
DEPENDPATH += $$PWD/libs/qt_material_headers

unix:!macx: PRE_TARGETDEPS += $$PWD/libs/libcomponents.a
