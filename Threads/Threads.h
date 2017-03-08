//
// Created by root on 3/2/17.
//

#ifndef JDROBOT_THREADS_H
#define JDROBOT_THREADS_H
#include "../Communicate/Communication.h"
#include "../openGLShow/glViewer.h"
#include <QThread>
#include <opencv2/opencv.hpp>
#include "opencvProc/cvProcesser.h"

#include "Threads/Threads.h"

/// [headers]
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>

#include "../Communicate/Communication.h"
#include <GL/gl.h>
#include "Threads/Threads.h"
extern Communication communicator;
bool createThread();
void kinect();
extern pthread_mutex_t mutex;

class MyThread : public QThread
{
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
    vector<vector<Point3f>> boxPoints3dW,boxPoints3dG;
    vector<vector<pair<Point2f,float>>> boxPointsDepth;
    int npts;
    vector<vector<pair<Point2f,float>>> groundPts;
    Mat R2G,T2G;
    Mat rotationMat;
    bool rotationMatInited=false;

    bool stop ;
    explicit MyThread(QObject *parent = 0);
    void run();
    void kinect();
    bool getFrames(libfreenect2::Frame *rgb, libfreenect2::Frame *ir, libfreenect2::Frame *depth,
                                    libfreenect2::Frame *undistorted, libfreenect2::Frame *registered,
                                    libfreenect2::Frame *depth2rgb);

    bool showFrames();

    void init(libfreenect2::Freenect2Device *dev);

    bool getBoxPoints3d(vector<vector<pair<Point2f,float>>> inputPts, vector<vector<Point3f> > &outputPts) ;

    bool getBoxPointsDepth(Mat depthMat, Mat mask, vector<vector<pair<Point2f,float>>>& outputPts);

    bool setBackGround();

    bool convertWorld2Ground(vector<vector<Point3f>> inputPts, vector<vector<Point3f> > &outputPts);

    Mat rotatedImage(Mat& inputImage);
signals:
    void drawPoints(float *points, int size);

    void test(float*,int);

public slots:

};
#endif //JDROBOT_THREADS_H
