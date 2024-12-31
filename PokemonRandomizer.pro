QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -O3

# Include paths for thirdparty, SV_FLATBUFFERS, and images
INCLUDEPATH += $$PWD/thirdparty
INCLUDEPATH += $$PWD/SV_FLATBUFFERS
INCLUDEPATH += $$PWD/images
INCLUDEPATH += $$PWD/include

# Add files to DISTFILES for deployment
DISTFILES += $$files($$PWD/thirdparty/*, true) \
    SV_FLATBUFFERS/SV_ITEMS/dropitemdata_array_clean.json \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_array.bfbs \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_array.fbs \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_array_clean.json \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_lc_array.bfbs \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_lc_array_clean.json \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_su1_array.bfbs \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_su1_array_clean.json \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_su2_array.bfbs \
    SV_FLATBUFFERS/SV_ITEMS/hiddenItemDataTable_su2_array_clean.json \
    SV_FLATBUFFERS/SV_ITEMS/monohiroiItemData_array.bfbs \
    SV_FLATBUFFERS/SV_ITEMS/monohiroiItemData_array_clean.json \
    SV_FLATBUFFERS/SV_ITEMS/pokemon_items_dev.json \
    SV_FLATBUFFERS/SV_ITEMS/rummagingItemDataTable_array.bfbs \
    SV_FLATBUFFERS/SV_ITEMS/rummagingItemDataTable_array_clean.json
DISTFILES += $$files($$PWD/SV_FLATBUFFERS/*, true)
DISTFILES += $$files($$PWD/images/*, true)
DISTFILES += $$files($$PWD/include/*, true)

# Enable compatibility for building across platforms
win32 {
    CONFIG += windows
    CONFIG -= debug_and_release debug_and_release_target
    QMAKE_HOST.arch:equals(x86_64) {
        DEFINES += ARCH_64
        message("Building for 64-bit Windows")
    } else {
        DEFINES += ARCH_32
        message("Building for 32-bit Windows")
    }

    # Paths for source and destination directories

    THIRDPARTY_SRC = $$PWD/thirdparty
    IMAGES_SRC = $$PWD/images
    SV_FLATBUFFERS_SRC = $$PWD/SV_FLATBUFFERS
    INCLUDE_SRC = $$PWD/include

    # Destination directory adjusted for release/debug
    CONFIG(debug, debug|release) {
        BUILD_DIR = $$OUT_PWD
    } else {
        BUILD_DIR = $$OUT_PWD
    }

    THIRDPARTY_DEST = $$BUILD_DIR/thirdparty
    IMAGES_DEST = $$BUILD_DIR/images
    SV_FLATBUFFERS_DEST = $$BUILD_DIR/SV_FLATBUFFERS
    INCLUDE_DEST = $$BUILD_DIR/include


    SOURCE_DIR_WIN = $$replace(THIRDPARTY_SRC, /, \\)
    DEST_DIR_WIN = $$replace(THIRDPARTY_DEST, /, \\)
    SOURCE_IMAGE_WIN = $$replace(IMAGES_SRC, /, \\)
    DEST_IMAGE_WIN = $$replace(IMAGES_DEST, /, \\)
    SOURCE_FLAT_WIN = $$replace(SV_FLATBUFFERS_SRC, /, \\)
    DEST_FLAT_WIN = $$replace(SV_FLATBUFFERS_DEST, /, \\)
    SOURCE_INCLUDE_WIN = $$replace(INCLUDE_SRC, /, \\)
    DEST_INCLUDE_WIN = $$replace(INCLUDE_DEST, /, \\)

    copy_third.commands = \
        $$QMAKE_COPY_DIR \"$$SOURCE_DIR_WIN\" \"$$DEST_DIR_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_IMAGE_WIN\" \"$$DEST_IMAGE_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_FLAT_WIN\" \"$$DEST_FLAT_WIN\" && \
        $$QMAKE_COPY_DIR \"$$SOURCE_INCLUDE_WIN\" \"$$DEST_INCLUDE_WIN\"


    QMAKE_EXTRA_TARGETS += copy_third
    PRE_TARGETDEPS += copy_third
    CONFIG(debug, debug|release) {
        BUILD_DIR = $$OUT_PWD
    } else {
        DEPLOY_PATH = $$OUT_PWD
        DEPLOY_SCRIPT = $$PWD/scripts/deploy_windows.bat
        post_build.commands = $$DEPLOY_SCRIPT $$OUT_PWD/PokemonRandomizer.exe $$DEPLOY_PATH
        QMAKE_EXTRA_TARGETS += post_build
        QMAKE_POST_LINK += $(MAKE) -f Makefile post_build
    }



}

macx {
    CONFIG += macx
    contains(QMAKE_CXXFLAGS, -arch\\s*x86_64): DEFINES += MAC_INTEL
    contains(QMAKE_CXXFLAGS, -arch\\s*arm64): DEFINES += MAC_ARM

    # Paths for source and destination directories
    THIRDPARTY_SRC = $$PWD/thirdparty
    THIRDPARTY_DEST = $$OUT_PWD/thirdparty
    IMAGES_SRC = $$PWD/images
    IMAGES_DEST = $$OUT_PWD/images
    SV_FLATBUFFERS_SRC = $$PWD/SV_FLATBUFFERS
    SV_FLATBUFFERS_DEST = $$OUT_PWD/SV_FLATBUFFERS
    INCLUDE_SRC = $$PWD/include
    INCLUDE_DEST = $$OUT_PWD/include

    # Custom command to copy the include folder
    copy_include.commands = mkdir -p $$INCLUDE_DEST && cp -r $$INCLUDE_SRC/. $$INCLUDE_DEST
    QMAKE_EXTRA_TARGETS += copy_include

    # Custom command to copy the images folder
    copy_images.commands = mkdir -p $$IMAGES_DEST && cp -r $$IMAGES_SRC/. $$IMAGES_DEST
    QMAKE_EXTRA_TARGETS += copy_images

    # Custom command to copy the thirdparty folder
    copy_thirdparty.commands = mkdir -p $$THIRDPARTY_DEST && cp -r $$THIRDPARTY_SRC/. $$THIRDPARTY_DEST
    QMAKE_EXTRA_TARGETS += copy_thirdparty

    # Custom command to copy the SV_FLATBUFFERS folder
    copy_sv_flatbuffers.commands = mkdir -p $$SV_FLATBUFFERS_DEST && cp -r $$SV_FLATBUFFERS_SRC/. $$SV_FLATBUFFERS_DEST
    QMAKE_EXTRA_TARGETS += copy_sv_flatbuffers

    # Ensure the copy commands are executed before building
    PRE_TARGETDEPS += copy_images copy_thirdparty copy_sv_flatbuffers copy_include

    # Deployment for macOS using macdeployqt
    DEPLOY_PATH = $$OUT_PWD/deploy
    DEPLOY_SCRIPT = $$PWD/scripts/deploy_mac.sh
    post_build.commands = bash $$DEPLOY_SCRIPT $$OUT_PWD/PokemonRandomizer.app $$DEPLOY_PATH
    QMAKE_EXTRA_TARGETS += post_build
    QMAKE_POST_LINK += $(MAKE) -f Makefile post_build
}

unix:!macx {
    CONFIG += linux

    # Paths for source and destination directories
    THIRDPARTY_SRC = $$PWD/thirdparty
    THIRDPARTY_DEST = $$OUT_PWD/thirdparty
    IMAGES_SRC = $$PWD/images
    IMAGES_DEST = $$OUT_PWD/images
    SV_FLATBUFFERS_SRC = $$PWD/SV_FLATBUFFERS
    SV_FLATBUFFERS_DEST = $$OUT_PWD/SV_FLATBUFFERS
    INCLUDE_SRC = $$PWD/include
    INCLUDE_DEST = $$OUT_PWD/include

    # Custom command to copy the include folder
    copy_include.commands = mkdir -p $$INCLUDE_DEST && cp -r $$INCLUDE_SRC/. $$INCLUDE_DEST
    QMAKE_EXTRA_TARGETS += copy_include

    # Custom command to copy the images folder
    copy_images.commands = mkdir -p $$IMAGES_DEST && cp -r $$IMAGES_SRC/. $$IMAGES_DEST
    QMAKE_EXTRA_TARGETS += copy_images

    # Custom command to copy the thirdparty folder
    copy_thirdparty.commands = mkdir -p $$THIRDPARTY_DEST && cp -r $$THIRDPARTY_SRC/. $$THIRDPARTY_DEST
    QMAKE_EXTRA_TARGETS += copy_thirdparty

    # Custom command to copy the SV_FLATBUFFERS folder
    copy_sv_flatbuffers.commands = mkdir -p $$SV_FLATBUFFERS_DEST && cp -r $$SV_FLATBUFFERS_SRC/. $$SV_FLATBUFFERS_DEST
    QMAKE_EXTRA_TARGETS += copy_sv_flatbuffers

    # Ensure the copy commands are executed before building
    PRE_TARGETDEPS += copy_images copy_thirdparty copy_sv_flatbuffers

    contains(QMAKE_CXX, g++) {
        message("Using g++ on Linux")
        DEFINES += LINUX_GCC
    } else: contains(QMAKE_CXX, clang++) {
        message("Using clang++ on Linux")
        DEFINES += LINUX_CLANG
    }

    # Deployment for Linux using linuxdeployqt
    # Uncomment the next lines if you have Ubuntu 20.04 (Focal Fossa) with glibc 2.31
    # DEPLOY_PATH = $$OUT_PWD/deploy
    # DEPLOY_SCRIPT = $$PWD/scripts/deploy_linux.sh
    # post_build.commands = bash $$DEPLOY_SCRIPT $$OUT_PWD/release/PokemonRandomizer $$DEPLOY_PATH
    # QMAKE_EXTRA_TARGETS += post_build
    # QMAKE_POST_LINK += $(MAKE) -f Makefile post_build

}

# Subfolders visibility in the project sidebar
VPATH += \
    src \
    headers \
    sv_randomizer_code \
    sv_randomizer_headers \
    SV_FLATBUFFERS

SOURCES += \
    src/GameEditWindow.cpp \
    src/SVRandomizerWindow.cpp \
    src/VGCWindow.cpp \
    src/alternatewindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/sv_randomizer_code/sv_items.cpp \
    src/sv_randomizer_code/sv_raids.cpp \
    src/sv_randomizer_code/sv_shared_class.cpp \
    src/sv_randomizer_code/sv_starters_gifts.cpp \
    src/sv_randomizer_code/sv_stats.cpp \
    src/sv_randomizer_code/sv_trainers.cpp \
    src/sv_randomizer_code/sv_wild.cpp \
    src/sv_randomizer_code/svrandomizercode.cpp \

HEADERS += \
    headers/AlternateWindow.h \
    headers/GameEditWindow.h \
    headers/SVRandomizerWindow.h \
    headers/VGCWindow.h \
    headers/mainwindow.h \
    headers/sv_randomizer_headers/sv_items.h \
    headers/sv_randomizer_headers/sv_raids.h \
    headers/sv_randomizer_headers/sv_shared_class.h \
    headers/sv_randomizer_headers/sv_starters_gifts.h \
    headers/sv_randomizer_headers/sv_stats.h \
    headers/sv_randomizer_headers/sv_trainers.h \
    headers/sv_randomizer_headers/sv_wild_static.h \
    headers/sv_randomizer_headers/svrandomizercode.h

FORMS +=

# Deployment paths
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Handle platform-specific libraries and dependencies
macx {
    LIBS += -framework Cocoa
}

win32 {
    LIBS += -luser32 -lgdi32 -lcomdlg32
}

unix:!macx {
    LIBS += -lpthread
}

# Additional build configurations
DEFINES += QT_DEPRECATED_WARNINGS
