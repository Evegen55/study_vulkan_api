TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    util_init.cpp \
    util.cpp

HEADERS += \
    util_init.h \
    util.h

unix:INCLUDEPATH += /home/evgen/Progs/vulkan/1.1.82.1/x86_64/include #headers

unix:LIBS += /home/evgen/Progs/vulkan/1.1.82.1/x86_64/lib/libvulkan.so \ #dynamic linking
