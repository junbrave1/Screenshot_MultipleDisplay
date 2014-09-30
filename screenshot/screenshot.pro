QT += widgets
QMAKE_MAC_SDK = macosx10.9

HEADERS             = screenshot.h
SOURCES             = main.cpp \
                      screenshot.cpp

# install
target.path = /Users/President/Desktop\screenshot
INSTALLS += target
