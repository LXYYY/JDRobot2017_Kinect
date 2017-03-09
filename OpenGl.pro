#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T18:47:41
#
#-------------------------------------------------

QT       += core gui opengl serialport network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = OpenGl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oglwidget.cpp \
    cameraview.cpp \
    dialog.cpp \
    clientclass.cpp \
    globject.cpp \
    Communicate/SocketClass/SocketClass.cpp \
    Communicate/Communication.cpp \
    Threads/Threads.cpp \
    AprilTagsClass/AprilTagsClass.cpp \


HEADERS  += mainwindow.h \
    oglwidget.h \
    cameraview.h \
    dialog.h \
    clientclass.h \
    globject.h \
    Communicate/SocketClass/SocketClass.h \
    Communicate/Communication.h \
    Threads/Threads.h
    AprilTagsClass/AprilTagsClass.h


FORMS    += mainwindow.ui \
    dialog.ui

LIBS+=-L/usr/lib/x86_64-linux-gnu -lglut -lGLU\
      -L/usr/local/lib\
        -lapriltags\
        -lopencv_calib3d\
        -lopencv_core\
        -lopencv_features2d\
        -lopencv_flann\
        -lopencv_highgui\
        -lopencv_imgcodecs\
        -lopencv_imgproc\
        -lopencv_ml\
        -lopencv_objdetect\
        -lopencv_videostab\
        -lopencv_video\
        -lopencv_videoio\
        -lopencv_superres\
        -lopencv_stitching\
        -lopencv_shape\
        -lopencv_photo\
        -lopencv_xphoto\
        -lopencv_xfeatures2d\
        -lopencv_ximgproc\
        -lopencv_xobjdetect\
        -lpthread\
        -L/root/freenect2/lib\
        -lfreenect2\



#LIBS+=

INCLUDEPATH+=/usr/include/\
        /root/freenect2/include/\
        /home/jdrobot/apriltags/build/include\
        /usr/include/eigen3

DISTFILES += \
    depends/debs/i965-va-driver_1.7.0-1_amd64.deb \
    depends/debs/libglfw3-dev_3.1.2-3_amd64.deb \
    depends/debs/libglfw3_3.1.2-3_amd64.deb \
    depends/debs/libusb-1.0-0-dev_1.0.20-1_amd64.deb \
    depends/debs/libusb-1.0-0_1.0.20-1_amd64.deb \
    depends/debs/libva-dev_1.7.0-1_amd64.deb \
    depends/debs/libva-drm1_1.7.0-1_amd64.deb \
    depends/debs/libva-egl1_1.7.0-1_amd64.deb \
    depends/debs/libva-glx1_1.7.0-1_amd64.deb \
    depends/debs/libva-tpi1_1.7.0-1_amd64.deb \
    depends/debs/libva-wayland1_1.7.0-1_amd64.deb \
    depends/debs/libva-x11-1_1.7.0-1_amd64.deb \
    depends/debs/libva1_1.7.0-1_amd64.deb \
    depends/debs/ocl-icd-libopencl1_2.2.8-1_amd64.deb \
    depends/debs/ocl-icd-opencl-dev_2.2.8-1_amd64.deb \
    depends/debs/vainfo_1.7.0-1_amd64.deb \
    depends/download_debs_trusty.sh \
    depends/install_glfw.sh \
    depends/install_libusb.sh \
    depends/install_ubuntu.sh \
    depends/INSTALL-windows.txt \
    depends/install_libusb_vs2013.cmd \
    depends/install_libusb_vs2015.cmd \
    depends/LICENSES.txt \
    depends/make_release_msvc.cmd



