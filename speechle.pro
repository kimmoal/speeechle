QT += quick texttospeech multimedia
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    speechrecognition.cpp \
    translate.cpp

HEADERS += \
    speechrecognition.h \
    translate.h

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        /home/kalm/Prebuilt-OpenSSL-Android/Prebuilt/armv7-shared/lib/libcrypto.so \
        /home/kalm/Prebuilt-OpenSSL-Android/Prebuilt/armv7-shared/lib/libssl.so
}


contains(ANDROID_TARGET_ARCH, x86) {
    ANDROID_EXTRA_LIBS = \
        /home/kalm/Prebuilt-OpenSSL-Android/Prebuilt/x86-shared/lib/libcrypto.so \
        /home/kalm/Prebuilt-OpenSSL-Android/Prebuilt/x86-shared/lib/libssl.so
}

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
