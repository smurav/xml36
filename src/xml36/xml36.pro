#-------------------------------------------------
#
# Project created by QtCreator 2012-02-22T12:57:52
#
#-------------------------------------------------

QT       += core gui xml

TARGET = xml36
TEMPLATE = app

# Относительный путь рабочего каталога репозитория
OUTPUT = ../..

# Относительный путь каталога для хранения временных файлов
TEMP = $${OUTPUT}/tmp/xml36

# Настройки путей для сборки отладочных и релизных версий
debug {
  DEFINES += _DEBUG
  DESTDIR = $${OUTPUT}/debug
  OBJECTS_DIR = $${TEMP}/debug
} else {
  DESTDIR = $${OUTPUT}/release
  OBJECTS_DIR = $${TEMP}/release
}

win32 {
} else {
  INCLUDEPATH += /usr/include/libxml2\
                 /usr/include
  LIBS +=	-lxml2
}

RCC_DIR = ./res

RESOURCES = ./res/resource.qrc

# Путь для сохранения временных файлов Qt
MOC_DIR = $${OBJECTS_DIR}

SOURCES += main.cpp\
        mainwindow.cpp \
    addnodedialog.cpp

HEADERS  += mainwindow.h \
    addnodedialog.h

FORMS    += mainwindow.ui \
    addnodedialog.ui
