//
// Created by root on 3/5/17.
//

#ifndef JDROBOTKINECT_OPENCVPROCESSER_H
#define JDROBOTKINECT_OPENCVPROCESSER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <libfreenect2/libfreenect2.hpp>
#include <openni2/OpenNI.h>
#include <QtCore/qobject.h>

using namespace cv;
using namespace std;

class cvProcesser :public QObject{
Q_OBJECT
public:
    libfreenect2::Freenect2Device::IrCameraParams irParams;
    libfreenect2::Freenect2Device::ColorCameraParams colorParams;
    Mat rgbMat;
    Mat irMat;
    Mat depthMat;
    Mat depthMatUndistorted;
    Mat rgbd;
    Mat rgbd2;
    Mat backGround;
    Mat foreGround;
    vector<vector<Point3f>> boxPoints3d;
    vector<vector<pair<Point2f,float>>> boxPointsDepth;
    int npts;
    cvProcesser();

    void init(libfreenect2::Freenect2Device *dev);

    bool getFrames(libfreenect2::Frame *rgb, libfreenect2::Frame *ir, libfreenect2::Frame *depth,
                   libfreenect2::Frame *undistorted, libfreenect2::Frame *registered,
                   libfreenect2::Frame *depth2rgb);
    bool setBackGround();

    bool getBoxPointsDepth(Mat depthMat, Mat mask);

    bool getBoxPoints3d();

//    virtual ~cvProcesser();

    bool showFrames();

    signals:
    void drawPoints(float*,size_t);
};


#endif //JDROBOTKINECT_OPENCVPROCESSER_H
