QT += quick

SOURCES += \
        filemanager.cpp \
        main.cpp \
        syntaxhighlighter.cpp \
        textdocument.cpp \
        theme.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    filemanager.h \
    syntaxhighlighter.h \
    textdocument.h \
    theme.h
