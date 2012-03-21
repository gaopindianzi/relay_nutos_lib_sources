
TEMPLATE = app
TARGET += 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += qt thread
CONFIG -= flat

win32:DEFINES += _CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_WARNINGS

DEFINES += NUTCONF_VERSION_STR=\\\"3.0.0\\\"

# Input
SOURCES +=	\
			builder.cpp \
			dirtraverser.cpp \
			finddialog.cpp \
			main.cpp \
			mainwindow.cpp \
			nutcomponent.c \
			nutcomponentdelegate.cpp \
			nutcomponentmodel.cpp \
			nutcomponentdetailsmodel.cpp \
			nutcomponentmodel_p.cpp \
			settings.cpp \
			settingsdialog.cpp \
			systeminfo.cpp
			
HEADERS +=	\
			builder.h \
			dirtraverser.h \
			finddialog.h \
			mainwindow.h \
			nutcomponent.h \
			nutcomponentdelegate.h \
			nutcomponentmodel.h \
			nutcomponentdetailsmodel.h \
			nutcomponentmodel_p.h \
			settings.h \
			settingsdialog.h \
			systeminfo.h

FORMS +=	\
			mainwindow.ui \
			settingsdialog.ui \
			finddialog.ui

include(lua/lua.pri)

RESOURCES += qnutconf.qrc

RC_FILE = qnutconf.rc

VERSION = 3.0.0
QMAKE_TARGET_COMPANY = Ethernut Project
QMAKE_TARGET_DESCRIPTION = Ethernut Configurator
QMAKE_TARGET_COPYRIGHT = All rights reserved by authors
