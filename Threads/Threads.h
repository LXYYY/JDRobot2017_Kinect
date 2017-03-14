//
// Created by root on 3/2/17.
//

#ifndef JDROBOT_THREADS_H
#define JDROBOT_THREADS_H
#include "../Communicate/Communication.h"
#include <QThread>
#include <opencv2/opencv.hpp>
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
#include "AprilTagsClass/AprilTagsClass.h"
extern pthread_mutex_t mutex;

class MyThread : public QThread
{
    Q_OBJECT
public:
    enum ColorE{
        Green=1,
        Blue,
        Yellow
    };
    enum ModeE{
        SEARCH_BOX=0,
        SEARCH_CASE=1,
        PAUSE=2
    };
    int mode=SEARCH_BOX;
    AprilTagsClass aprilTags;
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
    struct BoxS{
        Point2f center;
        Size size;
        Point2f pts[4];
        float z;
        int color;
        Point2f dir;
        vector<Point3f> boxPoints3dW;
        vector<Point3f> boxPoints3dG;
        vector<Point3f> boxPointsDepth;
    };

    struct CaseS{
        int color;
        int ptId;   //0:left top 1:center 2:right top
        float centerAngle;
        float targetAngle[4];
        float targetX[4];
        float targetDir[4];
        Point3f center;
        Point2f dir=Point(0,0);
    };

    vector<struct CaseS> cases;
    vector<struct BoxS> boxes;
    int npts;
    vector<Point3f> groundPtsDepth;
    vector<Point3f> groundPts;
    Mat_<double> R2G,T2G,T2O;
    Mat rotationMat;
    bool rotationMatInited=false;
    bool ifBackGoundSet=false;
    bool ifOriginSet=false;
    bool stop ;
    explicit MyThread(QObject *parent = 0);
    void run();
    void kinect();
    bool getFrames(libfreenect2::Frame *rgb, libfreenect2::Frame *ir, libfreenect2::Frame *depth,
                                    libfreenect2::Frame *undistorted, libfreenect2::Frame *registered,
                                    libfreenect2::Frame *depth2rgb);

    bool showFrames();

    void init(libfreenect2::Freenect2Device *dev);

    vector<Point3f> getBoxPoints3d(vector<Point3f> inputPts) ;
    Point3f getBoxPoints3d(Point3f inputPts) ;

    pair<vector<Point3f>, int> getBoxPointsDepth(Mat depthMat, Mat rgb, Mat mask);
    Point3f getBoxPointsDepth(Mat depthMat, Point2f pts);


    vector<Point3f> convertWorld2Ground(vector<Point3f> inputPts);
    Point3f convertWorld2Ground(Point3f inputPts);

    Point3f convertGround2World(Point3f inputPts);

    Mat rotatedImage(Mat& inputImage);
signals:
    void drawPoints(float *points, int size);

    void sendPoint(unsigned char id,float* point);

    void pushRgbd(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void pushDepth(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void pushContours(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void pushProj(unsigned char* imageData,int cols,int rows,int bytesPerLine);

public slots:
    void changeMode();
    bool setBackGround();
    void setOrigin();
};
#endif //JDROBOT_THREADS_H
