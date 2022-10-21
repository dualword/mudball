TEMPLATE = app
TARGET = ../../mudball-editor
DEPENDPATH += .
INCLUDEPATH += .

LIBS += -lIrrlicht

HEADERS += App.h \
           EElevator.h \
           EElevator2X.h \
           EGoal.h \
           Element.h \
           EMud.h \
           EPlatform.h \
           ERamp.h \
           ERampEnd.h \
           ERampFlat.h \
           ERampStart.h \
           EStart.h \
           ETube2.h \
           ETube2C.h \
           ETube2H.h \
           ETubeBig.h \
           ETubeBigCurve.h \
           ETubeH.h \
           EventReceiver.h \
           EWind.h \
           ../tinyXML/tinystr.h \
           ../tinyXML/tinyxml.h

SOURCES += App.cpp \
           EElevator.cpp \
           EElevator2X.cpp \
           EGoal.cpp \
           Element.cpp \
           EMud.cpp \
           EPlatform.cpp \
           ERamp.cpp \
           ERampEnd.cpp \
           ERampFlat.cpp \
           ERampStart.cpp \
           EStart.cpp \
           ETube2.cpp \
           ETube2C.cpp \
           ETube2H.cpp \
           ETubeBig.cpp \
           ETubeBigCurve.cpp \
           ETubeH.cpp \
           EWind.cpp \
           mainLevelEditor.cpp \
           ../tinyXML/tinystr.cpp \
           ../tinyXML/tinyxml.cpp \
           ../tinyXML/tinyxmlerror.cpp \
           ../tinyXML/tinyxmlparser.cpp

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui

