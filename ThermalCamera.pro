QT += core gui
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TEMPLATE = app
TARGET = ThermalCamera

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    frames.cpp \
    main.cpp \
    mainwindow.cpp \
    process_frame.cpp

HEADERS += \
    frames.h \
    mainwindow.h \
    process_frame.h

FORMS += \
    mainwindow.ui


win32 {
    message("Win32 enabled")

    RC_ICONS += $$PWD/icons/matrix.ico
    LIBS += -L$$PWD/opencv4/x64/vc16/lib -lopencv_world480
    INCLUDEPATH += $$PWD/opencv4/include
}

unix:!macx:!ios:!android {
    message("linux enabled")

    INCLUDEPATH += /usr/lib
    INCLUDEPATH += /usr/local/lib
    INCLUDEPATH += /usr/local/include/opencv4
    INCLUDEPATH += /usr/include/opencv4

    LIBS += -lopencv_core -lopencv_dnn -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -lopencv_video -lopencv_videoio
}

macos {
    message("macx enabled")    

    QMAKE_INFO_PLIST = ./macos/Info.plist
    QMAKE_ASSET_CATALOGS = $$PWD/macos/Assets.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"
    INCLUDEPATH += /opt/homebrew/Cellar/opencv/4.8.1_4/include/opencv4
    LIBS += -L/opt/homebrew/Cellar/opencv/4.8.1_4/lib -lopencv_core -lopencv_dnn -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -lopencv_video -lopencv_videoio
 }

ios {
    message("ios enabled")
    QMAKE_INFO_PLIST = ./ios/Info.plist
    QMAKE_ASSET_CATALOGS = $$PWD/ios/Assets.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"

    INCLUDEPATH += $$(HOME)/opencv2.framework/include
#    add opencv from xcode, Build Phases -> Link Binary With Libraries
#    LIBS += -F$$(HOME)/opencv2.framework -framework opencv2
}

android {
    INCLUDEPATH += $$PWD/OpenCV-android-sdk/sdk/native/jni/include
    DEPENDPATH += $$PWD/OpenCV-android-sdk/sdk/native/jni/include
    ANDROID_ABIS = armeabi-v7a

    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        LIBS += -L$$PWD/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a -lopencv_java4
        LIBS += \
            -L$$PWD/OpenCV-android-sdk/sdk/native/staticlibs/armeabi-v7a \
            -lopencv_ml\
            -lopencv_objdetect\
            -lopencv_photo\
            -lopencv_stitching\
            -lopencv_video\
            -lopencv_calib3d\
            -lopencv_features2d\
            -lopencv_highgui\
            -lopencv_flann\
            -lopencv_videoio\
            -lopencv_imgcodecs\
            -lopencv_imgproc\
            -lopencv_core
        ANDROID_EXTRA_LIBS = $$PWD/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java4.so
    }

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    # Add Java sources explicitly
    ANDROID_JAVA_SOURCES.path = /src/org/tbiliyor/thermal/camera
    ANDROID_JAVA_SOURCES.files = $$files($$PWD/android/src/org/tbiliyor/thermal/camera/*.java)
}

#sudo apt install libopencv-dev python3-opencv
#brew install opencv

RESOURCES += \
    resources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-xhdpi/icon.png \
    android/res/drawable-xxhdpi/icon.png \
    android/res/drawable-xxxhdpi/icon.png \
    android/res/values/libs.xml \
    android/src/org/tbiliyor/thermal/camera/MainActivity.java \
    ios/Assets.xcassets/AppIcon.appiconset/Contents.json \
    ios/Assets.xcassets/AppIcon.appiconset/_/1024.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/114.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/120.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/128.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/16.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/180.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/256.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/29.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/32.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/40.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/512.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/57.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/58.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/60.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/64.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/80.png \
    ios/Assets.xcassets/AppIcon.appiconset/_/87.png
