//
// Created by root on 3/2/17.
//

#include "pthread.h"
#include "Threads.h"
#include "mainwindow.h"
#include "Communicate/Communication.h"
pthread_mutex_t mutex;

#include<QDebug>


/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2011 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

/** @file Protonect.cpp Main application file. */

#include <iostream>
#include <cstdlib>
#include <signal.h>



/// [headers]
#ifdef EXAMPLES_WITH_OPENGL_SUPPORT

#endif


bool protonect_shutdown = false; ///< Whether the running application should shut down.

void sigint_handler(int s) {
    protonect_shutdown = true;
}

bool protonect_paused = false;
libfreenect2::Freenect2Device *devtopause;

//Doing non-trivial things in signal handler is bad. If you want to pause,
//do it in another thread.
//Though libusb operations are generally thread safe, I cannot guarantee
//everything above is thread safe when calling start()/stop() while
//waitForNewFrame().
void sigusr1_handler(int s) {
    if (devtopause == 0)
        return;
    /// [pause]
    if (protonect_paused)
        devtopause->start();
    else
        devtopause->stop();
    protonect_paused = !protonect_paused;
    /// [pause]
}

//The following demostrates how to create a custom logger
/// [logger]
#include <fstream>
#include <cstdlib>

class MyFileLogger : public libfreenect2::Logger {
private:
    std::ofstream logfile_;
public:
    MyFileLogger(const char *filename) {
        if (filename)
            logfile_.open(filename);
        level_ = Debug;
    }

    bool good() {
        return logfile_.is_open() && logfile_.good();
    }

    virtual void log(Level level, const std::string &message) {
        logfile_ << "[" << libfreenect2::Logger::level2str(level) << "] " << message << std::endl;
    }
};
/// [logger]

/// [main]
/**
 * Main application entry point.
 *
 * Accepted argumemnts:
 * - cpu Perform depth processing with the CPU.
 * - gl  Perform depth processing with OpenGL.
 * - cl  Perform depth processing with OpenCL.
 * - <number> Serial number of the device to open.
 * - -noviewer Disable viewer window.
 */

void MyThread:: kinect()
/// [main]
{
    //    viewer::init(argc, argv);

    //    std::string program_path(argv[0]);
    //    std::cerr << "Version: " << LIBFREENECT2_VERSION << std::endl;
    //    std::cerr << "Environment variables: LOGFILE=<protonect.log>" << std::endl;
    //    std::cerr << "Usage: " << program_path << " [-gpu=<id>] [gl | cl | clkde | cuda | cudakde | cpu] [<device serial>]"
    //              << std::endl;
    //    std::cerr << "        [-noviewer] [-norgb | -nodepth] [-help] [-version]" << std::endl;
    //    std::cerr << "        [-frames <number of frames to process>]" << std::endl;
    //    std::cerr << "To pause and unpause: pkill -USR1 Protonect" << std::endl;
    //    size_t executable_name_idx = program_path.rfind("Protonect");

    //    std::string binpath = "/";

    //    if (executable_name_idx != std::string::npos) {
    //        binpath = program_path.substr(0, executable_name_idx);
    //    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    // avoid flooing the very slow Windows console with debug messages
    libfreenect2::setGlobalLogger(libfreenect2::createConsoleLogger(libfreenect2::Logger::Info));
#else
    // create a console logger with debug level (default is console logger with info level)
    /// [logging]
    libfreenect2::setGlobalLogger(libfreenect2::createConsoleLogger(libfreenect2::Logger::Debug));
    /// [logging]
#endif
    /// [file logging]
    MyFileLogger *filelogger = new MyFileLogger(getenv("LOGFILE"));
    if (filelogger->good())
        libfreenect2::setGlobalLogger(filelogger);
    else
        delete filelogger;
    /// [file logging]

    /// [context]
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = 0;
    libfreenect2::PacketPipeline *pipeline = 0;
    /// [context]



    std::string serial = "";

    bool viewer_enabled = true;
    bool enable_rgb = true;
    bool enable_depth = true;
    int deviceId = -1;

    //    if (!pipeline)
    /// [pipeline]
    //        pipeline = new libfreenect2::CpuPacketPipeline();
    /// [pipeline]
    //    for (int argI = 1; argI < argc; ++argI) {
    //        const std::string arg(argv[argI]);

    //        if (arg == "-help" || arg == "--help" || arg == "-h" || arg == "-v" || arg == "--version" ||
    //            arg == "-version") {
    //            // Just let the initial lines display at the beginning of main
    //            return 0;
    //        } else if (arg.find("-gpu=") == 0) {
    //            if (pipeline) {
    //                std::cerr << "-gpu must be specified before pipeline argument" << std::endl;
    //                return -1;
    //            }
    //            deviceId = atoi(argv[argI] + 5);
    //        } else if (arg == "cpu") {

    //        } else if (arg == "gl") {
    //#ifdef LIBFREENECT2_WITH_OPENGL_SUPPORT
    //            if (!pipeline)
    //                pipeline = new libfreenect2::OpenGLPacketPipeline();
    //#else
    //            std::cout << "OpenGL pipeline is not supported!" << std::endl;
    //#endif
    //        } else if (arg == "cl") {
    //#ifdef LIBFREENECT2_WITH_OPENCL_SUPPORT
    //            if (!pipeline)
    //                pipeline = new libfreenect2::OpenCLPacketPipeline(deviceId);
    //#else
    //            std::cout << "OpenCL pipeline is not supported!" << std::endl;
    //#endif
    //        } else if (arg == "clkde") {
    //#ifdef LIBFREENECT2_WITH_OPENCL_SUPPORT
    //            if (!pipeline)
    //                pipeline = new libfreenect2::OpenCLKdePacketPipeline(deviceId);
    //#else
    //            std::cout << "OpenCL pipeline is not supported!" << std::endl;
    //#endif
    //        } else if (arg == "cuda") {
    //#ifdef LIBFREENECT2_WITH_CUDA_SUPPORT
    //            if(!pipeline)
    //              pipeline = new libfreenect2::CudaPacketPipeline(deviceId);
    //#else
    //            std::cout << "CUDA pipeline is not supported!" << std::endl;
    //#endif
    //        } else if (arg == "cudakde") {
    //#ifdef LIBFREENECT2_WITH_CUDA_SUPPORT
    //            if(!pipeline)
    //              pipeline = new libfreenect2::CudaKdePacketPipeline(deviceId);
    //#else
    //            std::cout << "CUDA pipeline is not supported!" << std::endl;
    //#endif
    //        } else if (arg.find_first_not_of("0123456789") ==
    //                   std::string::npos) //check if parameter could be a serial number
    //        {
    //            serial = arg;
    //        } else if (arg == "-noviewer" || arg == "--noviewer") {
    //            viewer_enabled = false;
    //        } else if (arg == "-norgb" || arg == "--norgb") {
    //            enable_rgb = false;
    //        } else if (arg == "-nodepth" || arg == "--nodepth") {
    //            enable_depth = false;
    //        } else if (arg == "-frames") {
    //            ++argI;
    //            framemax = strtol(argv[argI], NULL, 0);
    //            if (framemax == 0) {
    //                std::cerr << "invalid frame count '" << argv[argI] << "'" << std::endl;
    //                return -1;
    //            }
    //        } else {
    //            std::cout << "Unknown argument: " << arg << std::endl;
    //        }
    //    }

    if (!enable_rgb && !enable_depth) {
        std::cerr << "Disabling both streams is not allowed!" << std::endl;
        return;
    }

    /// [discovery]
    if (freenect2.enumerateDevices() == 0) {
        std::cout << "no device connected!" << std::endl;
        return;
    }

    if (serial == "") {
        serial = freenect2.getDefaultDeviceSerialNumber();
    }
    /// [discovery]

    if (pipeline) {
        /// [open]
        dev = freenect2.openDevice(serial, pipeline);
        /// [open]
    } else {
        dev = freenect2.openDevice(serial);
    }

    if (dev == 0) {
        std::cout << "failure opening device!" << std::endl;
    }

    devtopause = dev;

    signal(SIGINT, sigint_handler);
#ifdef SIGUSR1
    signal(SIGUSR1, sigusr1_handler);
#endif
    protonect_shutdown = false;

    /// [listeners]
    int types = 0;
    if (enable_rgb)
        types |= libfreenect2::Frame::Color;
    if (enable_depth)
        types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
    libfreenect2::SyncMultiFrameListener listener(types);
    libfreenect2::FrameMap frames;

    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);
    /// [listeners]

    /// [start]
    if (enable_rgb && enable_depth) {
        if (!dev->start())
            return;
    } else {
        if (!dev->startStreams(enable_rgb, enable_depth))
            return;
    }

    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    /// [start]

    /// [registration setup]
    libfreenect2::Registration *registration = new libfreenect2::Registration(dev->getIrCameraParams(),
                                                                              dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
    /// [registration setup]

    size_t framecount = 0;
    //#ifdef EXAMPLES_WITH_OPENGL_SUPPORT
    //    Viewer viewer;
    //    if (viewer_enabled)
    //        viewer.initialize();
    //#else
    //    viewer_enabled = false;
    //#endif


    init(dev);
    aprilTags.setup();
    libfreenect2::Freenect2Device::Config config;
    config.EnableBilateralFilter=true;
    config.EnableEdgeAwareFilter=true;
    config.MaxDepth=0.9f;
    config.MinDepth=0.f;
    dev->setConfiguration(config);

    /// [loop start]
    while (!shutdown) {
        if (!listener.waitForNewFrame(frames, 10 * 1000)) // 10 sconds
        {
            std::cout << "timeout!" << std::endl;
            return;
        }
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];



        /// [loop start]

        if (enable_rgb && enable_depth) {
            /// [registration]
            registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);
            /// [registration]
        }

        framecount++;
        //        cout << rgb->height << "x" << rgb->width << "format:" << rgb->format << endl;
        getFrames(rgb, ir, depth,&undistorted,&registered,&depth2rgb);
        protonect_shutdown = protonect_shutdown || showFrames();

        //    if (!viewer_enabled)
        //    {
        //      if (framecount % 100 == 0)
        //        std::cout << "The viewer is turned off. Received " << framecount << " frames. Ctrl-C to stop." << std::endl;
        //      listener.release(frames);
        //      continue;
        //    }

        //#ifdef EXAMPLES_WITH_OPENGL_SUPPORT
        //    if (enable_rgb)
        //    {
        //      viewer.addFrame("RGB", rgb);
        //    }
        //    if (enable_depth)
        //    {
        //      viewer.addFrame("ir", ir);
        //      viewer.addFrame("depth", depth);
        //    }
        //    if (enable_rgb && enable_depth)
        //    {
        //      viewer.addFrame("registered", &registered);
        //    }
        //
        //    protonect_shutdown = protonect_shutdown || viewer.render();
        //#endif

        /// [loop end]
        listener.release(frames);
        /** libfreenect2::this_thread::sleep_for(libfreenect2::chrono::milliseconds(100)); */
    }
    /// [loop end]

    // TODO: restarting ir stream doesn't work!
    // TODO: bad things will happen, if frame listeners are freed before dev->stop() :(
    /// [stop]
    dev->stop();
    dev->close();
    /// [stop]

    delete registration;

    return;
}




MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    stop = false;
}
void MyThread::run()
{
    cout<<"running"<<endl;



    try{
        kinect();
    }
    catch(...){
        perror("kinect");
    }
}

bool pressed = false;

void OnMouseAction(int event, int x, int y, int flags, void *ustc) {
    Mat *img = (Mat *) ustc;
    if (event == CV_EVENT_LBUTTONDOWN) {
        pressed = true;
    } else if (event == CV_EVENT_LBUTTONUP && pressed) {
        cout/*<<x<<","<<y<<*//*ends*/<< img->at<Vec4b>(y, x) << endl;
    }
}

bool MyThread::getFrames(libfreenect2::Frame *rgb, libfreenect2::Frame *ir, libfreenect2::Frame *depth,
                         libfreenect2::Frame *undistorted, libfreenect2::Frame *registered,
                         libfreenect2::Frame *depth2rgb) {
    cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbMat);
    cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irMat);
    cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthMat);
    cv::Mat(registered->height, registered->width, CV_8UC4, registered->data).copyTo(rgbd);
    cv::Mat(undistorted->height, undistorted->width, CV_32FC1, undistorted->data).copyTo(depthMatUndistorted);
    cv::Mat(depth2rgb->height, depth2rgb->width, CV_32FC1, depth2rgb->data).copyTo(rgbd2);

    flip(depthMatUndistorted,depthMatUndistorted,0);
    flip(depthMat,depthMat,0);
    flip(rgbMat,rgbMat,0);
    flip(rgbd,rgbd,0);
    flip(rgbd2,rgbd2,0);
    flip(irMat,irMat,0);
    return true;
}
int thresh=-1;

void splitColor(Mat& rgbd,Mat mask){
//    cout<<"rgbd.channels"<<rgbd.channels()<<endl;
    for (size_t i = 0; i < rgbd.rows; i++) {
        uchar *maskData = mask.ptr<uchar>(i);
        uchar *rgbdData=rgbd.ptr<uchar>(i);
        for (size_t j = 0; j < rgbd.cols; j++) {
            if (maskData[j]>0) {
                float b=rgbdData[(j)*4+0];
                float g=rgbdData[(j)*4+1];
                float r=rgbdData[(j)*4+2];
//                cout<<b<<","<<g<<","<<"r"<<r<<endl;
                if((g-b)/b>0.1&&(g-r)/r>0.1){
                    rgbdData[(j)*4+0]=100;
                    rgbdData[(j)*4+1]=100;
                    rgbdData[(j)*4+2]=100;
                  }
                else if((b-g)/g>0.1&&(b-r)/r>0.1){
                    rgbdData[(j)*4+0]=255;
                    rgbdData[(j)*4+1]=255;
                    rgbdData[(j)*4+2]=255;
                }
                else {
                    rgbdData[(j)*4+0]=170;
                    rgbdData[(j)*4+1]=170;
                    rgbdData[(j)*4+2]=170;
                }
            }
        }
    }
}

int thresh1=236,thresh2=400;
bool MyThread::showFrames(){
    try{
//        cout<<"mode:"<<mode<<endl;
        Mat check(Size(1500,1500),CV_8UC3);
        check.setTo(0);
        boxes.clear();
        Mat rgbdCopy;
        rgbd.copyTo(rgbdCopy);
//        imshow("undistorted",rgbd);
        cout<<"test:"<<ifBackGroundSet<<","<<ifOriginSet<<endl;
        if(ifBackGroundSet&&!ifOriginSet){
            cout<<"ifbackgoundset"<<ifBackGroundSet<<endl;
            cout<<"size:"<<groundPtsDepth.size()<<endl;
            aprilTags.processImage(rgbdCopy);
            aprilTags.drawTags(rgbdCopy);
            //        cout<<"test"<<endl;
        }
        if(ifBackGroundSet&&ifOriginSet){
//            cout<<"fuck1"<<endl;
            Mat binaryMat;
            vector<Point3f> pts2Draw;

            try{
                circle(rgbdCopy,Point(groundPtsDepth.at(0).x,groundPtsDepth.at(0).y),
                       3,Scalar(0,0,255),-1);
                circle(rgbdCopy,Point(groundPtsDepth.at(1).x,groundPtsDepth.at(1).y),
                       3,Scalar(0,0,255),-1);
//                circle(rgbdCopy,Point(groundPtsDepth.at(2).x,groundPtsDepth.at(2).y),
//                       3,Scalar(0,0,255),-1);
//                cout<<"R2G"<<R2G<<"T2G"<<T2G<<"T2O"<<T2O<<endl;
            for(int i=0;i<3;i++){
                Point3f vZ=Point3f(R2G.col(i).at<double>(0),R2G.col(i).at<double>(1),R2G.col(i).at<double>(2));
                Point2f z;
                z.x=vZ.x*irParams.fx/vZ.z+irParams.cx;
                z.y=vZ.y*irParams.fy/vZ.z+irParams.cy;
                z=-z*100+Point2f(groundPtsDepth.at(1).x,groundPtsDepth.at(1).y);
                line(rgbdCopy,z, Point(groundPtsDepth.at(1).x,groundPtsDepth.at(1).y),Scalar(255,0,0));
            }
//            cv::imshow("registered", rgbdCopy);
            foreGround = (backGround - depthMatUndistorted);
////            imshow("background", backGround / 1024.f);
////            imshow("foregound", foreGround / 1024.f);

            binaryMat=(foreGround/1000.f*255.f);
            binaryMat.convertTo(binaryMat,CV_8U);
            GaussianBlur(binaryMat,binaryMat,Size(3,3),0);
////            imshow("binary",binaryMat);

            }
            catch(...){
                perror("part1");
            }
            if(mode==SEARCH_BOX){
                Mat mask;
                mask.create(foreGround.size(), CV_8UC1);
                pair<vector<Point3f>,int> tBoxPointsDepth;
                vector<Point3f> tBoxPoints3dG,tBoxPoints3dW;
                vector<vector<Point> > contours0;
                try{
//                cout<<"fuck2"<<endl;

                //            cout<<"testing"<<endl;
                threshold(binaryMat,binaryMat,10,255,THRESH_BINARY);

                Mat canny,rgbdGray;
                Mat bMat,gMat,rMat;

                rgbd.copyTo(rgbdGray,binaryMat);
                GaussianBlur(rgbdGray,rgbdGray,Size(5,5),0);
//                splitColor(rgbdGray,binaryMat);
                cvtColor(rgbdGray,rgbdGray,COLOR_BGR2GRAY);
//                imshow("rgbdGray",rgbdGray);
                Canny(rgbdGray,canny,thresh1,thresh2,3);
//                imshow("canny",canny);
                vector<Vec4i> hierarchy;
                vector<vector<Point> > edges;
                findContours(canny,edges,RETR_TREE,CHAIN_APPROX_SIMPLE);
                drawContours(binaryMat,edges,-1,Scalar(0),3);
//                imshow("test",binaryMat);
//                usleep(10000);


;

                findContours(binaryMat, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
                cvtColor(binaryMat,binaryMat,COLOR_GRAY2BGR);
                RNG rng;
                drawContours(binaryMat,contours0,-1,Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)));
                emit pushContours(binaryMat.data,binaryMat.cols,binaryMat.rows,binaryMat.cols*binaryMat.channels());
                }
                catch(...){
                    perror("part2");
                }
                try{
//                cout<<"contours.size():"<<contours0.size()<<endl;
                for (int i = 0; i < contours0.size(); i++) {
                    if (contours0.at(i).size()>4&&contourArea(contours0.at(i)) >= 2000) {
                        vector<int> convex;
                        vector<Point> convexP;
                        vector<Vec4i> convexity;
                        convexHull(contours0.at(i),convex,false,false);
                        convexHull(contours0.at(i),convexP);
                        vector<vector<Point> > t;
                        t.push_back(convexP);
                        drawContours(binaryMat,t,-1,Scalar(0,255,0));
//                        cout<<"isContourConvex:"<<isContourConvex(contours0.at(i))<<endl;
                        if(!isContourConvex(contours0.at(i))){
                            convexityDefects(contours0.at(i),convex,convexity);
                            int idMax=-1;
                            float max=0;
                            for(size_t j=0;j<convexity.size();j++){
                                if(convexity.at(j)[3]/256.f>max){
                                    max=convexity.at(j)[3]/256.f;
                                    idMax=j;
                                }
                            }
//                            cout<<"test convexity:"<<max<<","<<scdMax<<endl;

                            if(max>10){
                                continue;
                            }

                        }
                        /////////make a mask//////////
                        mask.setTo(0);
                        drawContours(mask, contours0, i, Scalar(255), -1);
//                        drawContours(mask, contours0, i, Scalar(0), 5);
//                        /////////make a mask end//////////

                        ////////get points and color///////
                        tBoxPointsDepth=getBoxPointsDepth(depthMatUndistorted,rgbd, mask);
                        tBoxPoints3dW=getBoxPoints3d(tBoxPointsDepth.first);
                        tBoxPoints3dG=convertWorld2Ground(tBoxPoints3dW);
                        //                tBoxPoints3dG=tBoxPoints3dW;
                        ////////get points and color end///////


                        ///////push a new box///////////
                        struct BoxS tBox;
                        tBox.boxPointsDepth=tBoxPointsDepth.first;
                        tBox.boxPoints3dW=tBoxPoints3dW;
                        tBox.boxPoints3dG=tBoxPoints3dG;

                        vector<Point2f> proj2ZPts;
                        proj2ZPts.clear();
                        float z=0;
                        for(int j=0;j<tBoxPoints3dG.size();j++){

                            //////push back points to draw///////////
                            pts2Draw.push_back(tBoxPoints3dG.at(j));
                            //////push back points to draw end///////////

                            proj2ZPts.push_back(Point2f
                                                (tBoxPoints3dG.at(j).x,
                                                 tBoxPoints3dG.at(j).y));
                            //                    cout<<"check z:"<<tBoxPoints3dG.at(j).z<<endl;

                            z+=tBoxPoints3dG.at(i).z;
                        }
                        z/=tBoxPoints3dG.size();
                        if(proj2ZPts.size()<4) continue;
                        RotatedRect boundingBox=minAreaRect(proj2ZPts);

                        boundingBox.points(tBox.pts);
                        tBox.size=boundingBox.size;
                        tBox.center=boundingBox.center;
                        tBox.color=tBoxPointsDepth.second;
                        tBox.z=z;
                        cout<<"size:"<<tBox.size.width<<","<<tBox.size.height<<endl<<"area:"<<tBox.size.area()<<","<<z<<endl;

                        bool areaCheck=true;
                        switch(tBoxPointsDepth.second){
                        case Green:
                            if(fabs(tBox.size.area()-(180*150))<3000)
                                tBox.status=STATUS_LARGESIDE;
                            else if(fabs(tBox.size.area()-(180*70))<3000)
                                tBox.status=STATUS_MEDIUMSIDE;
                            else if(fabs(tBox.size.area()-(150*70))<3000)
                                tBox.status=STATUS_SMALLSIDE;
                            else{
//                                areaCheck=false;
                            }
                            break;
                        case Blue:
                            if(((180*150)-tBox.size.area())<3000)
                                tBox.status=STATUS_LARGESIDE;
                            else if(((180*70)-tBox.size.area())<3000)
                                tBox.status=STATUS_MEDIUMSIDE;
                            else if(fabs(tBox.size.area()-(150*70))<3000)
                                tBox.status=STATUS_SMALLSIDE;
                            else{
//                                areaCheck=false;
                            }
                            break;
                        case Yellow:
                            if(((350*50)-tBox.size.area())<3000)
                                tBox.status=STATUS_MEDIUMSIDE;
                            else{
//                                areaCheck=false;
                            }
                            break;
                        }

                        if(fabs((tBox.pts[0].x-tBox.pts[1].x)*(tBox.pts[0].x-tBox.pts[1].x)+(tBox.pts[0].y-tBox.pts[1].y)*(tBox.pts[0].y-tBox.pts[1].y))>
                                fabs((tBox.pts[1].x-tBox.pts[2].x)*(tBox.pts[1].x-tBox.pts[2].x)+(tBox.pts[1].y-tBox.pts[2].y)*(tBox.pts[1].y-tBox.pts[2].y))){
                            tBox.dir=(tBox.pts[0]-tBox.pts[1]).y<=0?(tBox.pts[0]-tBox.pts[1]):(tBox.pts[1]-tBox.pts[0]);
                        }
                        else {
                            tBox.dir=(tBox.pts[1]-tBox.pts[2]).y<=0?(tBox.pts[1]-tBox.pts[2]):(tBox.pts[2]-tBox.pts[1]);
                        }
                        if(areaCheck)
                            boxes.push_back(tBox);
                        ///////push a new box end///////////
                    }
                }
                }
                catch(...){
                    perror("part3");
                }
//                cout<<"fuck3"<<endl;
//                imshow("thresh",binaryMat);

                ///////make a image to check//////////

                for(int i=0;i<boxes.size();i++){
                    Scalar color;
                    switch(boxes.at(i).color){
                    case Green:
                        color=Scalar(0,255,0);
                        break;
                    case Blue:
                        color=Scalar(255,0,0);
                        break;
                    case Yellow:
                        color=Scalar(0,255,255);
                        break;
                    }
                    for(int j=0;j<4;j++){
                        line(check,boxes.at(i).pts[j]+Point2f(check.cols/2,check.rows/2),boxes.at(i).pts[(j+1)%4]+Point2f(check.cols/2,check.rows/2),color);
                    }
                    line(check,boxes.at(i).center+Point2f(check.cols/2,check.rows/2),boxes.at(i).center+Point2f(check.cols/2,check.rows/2)+boxes.at(i).dir,color);
//                    cout<<"box center:"<<boxes.at(i).center<<endl;
//                    cout<<boxes.at(i).center<<","<<boxes.at(i).color<<endl;
                }
                resize(check,check,Size(400,400));
                cvtColor(check,check,COLOR_BGR2RGB);
                emit pushDepth(check.data,check.cols,check.rows,check.cols*check.channels());
                ///////make a image to check end//////////

                ///////send box centers to communication thread////////
                if(boxes.size()>0){
                    int tgtId;
                    float minX=1000000,minY=1000000;
                    for(size_t i=0;i<boxes.size();i++){
                        if(fabs(boxes.at(i).center.x)<minX){
                            minX=fabs(boxes.at(i).center.x);
                            tgtId=i;
                        }
                    }
                    float *point=new float[6];
                    point[0]=boxes.at(tgtId).center.x;
                    point[1]=boxes.at(tgtId).center.y;
                    point[2]=boxes.at(tgtId).z;
                    cout<<boxes.at(tgtId).center<<","<<boxes.at(tgtId).z<<endl;
                    cout<<"size:"<<boxes.at(tgtId).size<<endl;
                    point[3]=boxes.at(tgtId).dir.x;
                    point[4]=boxes.at(tgtId).dir.y;
                    point[5]=boxes.at(tgtId).color;
                    emit sendPoint(Communication::TgtBox,point);
                }
                /////////send box centers to communication thread end////////
            }
            else if(mode==SEARCH_CASE){
                aprilTags.processImage(rgbd);
                aprilTags.drawTags(rgbdCopy);

                vector<Point2f> tCasePts;
                vector<Point3f> tCasePtsDepth;
                for(size_t i=0;i<aprilTags.tags.size();i++){

                    if(aprilTags.tags.at(i).id==1
                            ||aprilTags.tags.at(i).id==2
                            ||aprilTags.tags.at(i).id==3){
                        Point3f pt=getBoxPointsDepth(depthMatUndistorted,Point2f(aprilTags.tags.at(i).cxy.first,aprilTags.tags.at(i).cxy.second));
                        Point3f ptW=getBoxPoints3d(pt);
                        Point3f ptG=convertWorld2Ground(ptW);

                        if(fabs(ptG.x)<10){
                            CaseS tCase;
                            Point3f p2=getBoxPointsDepth(depthMatUndistorted,Point2f(aprilTags.tags.at(i).p[2].first,aprilTags.tags.at(i).p[2].second));
                            Point3f p2W=getBoxPoints3d(p2);
                            Point3f p2G=convertWorld2Ground(p2W);

                            Point3f p3=getBoxPointsDepth(depthMatUndistorted,Point2f(aprilTags.tags.at(i).p[3].first,aprilTags.tags.at(i).p[3].second));
                            Point3f p3W=getBoxPoints3d(p3);
                            Point3f p3G=convertWorld2Ground(p3W);

                            if(p3G.z>50)
                                continue;

                            Point3f dir=p2G.cross(p3G);
                            cout<<"dir:"<<dir<<endl;

                            tCase.color=aprilTags.tags.at(i).id;
                            tCase.center=Point3f(ptG.x,ptG.y,280);


                            //////get a angle/////////

                            cases.at(aprilTags.tags.at(i).id)=tCase;

                        }
                        cout<<"center:"<<ptG<<endl;
                    }
                }

                bool allCaseDetected;
                if(cases.at(0).color==1&&
                        cases.at(1).color==2&&
                        cases.at(2).color==3){
                    allCaseDetected=true;
                }
                if(allCaseDetected){
                    ////////////calc yellow case///////////
                    float offsetAngle=0;
                    float l1=cases.at(Blue).center.x+20;
                    float l2=20;
                    offsetAngle=atan(l2/l1);
                    cases.at(Yellow).targetAngle[0]=cases.at(Blue).centerAngle;
                    float offsetDir=0;
                    offsetDir=atan(l2/(cases.at(Blue).center.x));
                    cases.at(Yellow).targetDir[0]=offsetDir;
                    cases.at(Yellow).targetX[0]=cases.at(Yellow).center.x;

                    ////////////calc blue case///////////
                    cases.at(Blue).targetAngle[0]=cases.at(Blue).centerAngle;
                    cases.at(Blue).targetAngle[1]=cases.at(Blue).centerAngle;
                    cases.at(Blue).targetDir[0]=cases.at(Blue).centerAngle;
                    cases.at(Blue).targetDir[1]=cases.at(Blue).centerAngle;
                    cases.at(Blue).targetX[0]=cases.at(Blue).center.x+10;
                    cases.at(Blue).targetX[1]=cases.at(Blue).center.x-10;

                    ////////////calc green case//////////
                    l1=cases.at(Blue).center.x+10;
                    l2=15;
                    offsetAngle=atan(l2/l1);
                    cases.at(Green).targetAngle[0]=cases.at(Blue).centerAngle+offsetAngle;
                    cases.at(Green).targetX[0]=(float)sqrt(l1*l1+l2*l2);

                    l2=25;
                    offsetAngle=atan(l2/l1);
                    cases.at(Green).targetAngle[1]=cases.at(Blue).centerAngle+offsetAngle;
                    cases.at(Green).targetX[1]=(float)sqrt(l1*l1+l2*l2);

                    l1=cases.at(Blue).center.x-10;
                    l2=25;
                    offsetAngle=atan(l2/l1);
                    cases.at(Green).targetAngle[2]=cases.at(Blue).centerAngle+offsetAngle;
                    cases.at(Green).targetX[2]=(float)sqrt(l1*l1+l2*l2);

                    l2=15;
                    offsetAngle=atan(l2/l1);
                    cases.at(Green).targetAngle[3]=cases.at(Blue).centerAngle+offsetAngle;
                    cases.at(Green).targetX[3]=(float)sqrt(l1*l1+l2*l2);

                    /////////////end clac cases//////////
                    mode=PAUSE;
                }
            }

            /////////emit drawPoints signal/////////
            npts=pts2Draw.size();
//            cout<<"pts2Draw.size"<<pts2Draw.size()<<endl;
            float points[npts*3];
            for(int i=0;i<npts;i++){
                points[i*3+0]=pts2Draw.at(i).x;
                points[i*3+1]=pts2Draw.at(i).y;
                points[i*3+2]=pts2Draw.at(i).z;
                //            cout<<"test drawPoints:"<<points[i*3+0]<<","<<points[i*3+1]<<","<<points[i*3+2]<<endl;
            }
            emit drawPoints(points,npts);
            //        cout<<"pts2Draw.size="<<pts2Draw.size()<<endl;
            //        cout<<pts2Draw.at(100)<<endl;
            /////////emit drawPoints signal end/////////
        }




//        cvtColor(depthMatUndistorted,depthMatUndistorted,COLOR_GRAY2BGR);
//        cout<<"size:"<<depthMatUndistorted.cols<<","<<depthMatUndistorted.rows<<endl;
//        cout<<"rgbd:"<<rgbdCopy.cols<<","<<rgbdCopy.rows<<endl;
//        cout<<"drawing"<<endl;
        Mat tDepth;
        tDepth=(depthMatUndistorted/1000.f*255.f);
        tDepth.convertTo(tDepth,CV_8U);
//        cout<<"rgbdCopy:"<<rgbdCopy.rows<<","<<rgbdCopy.cols<<endl;\
//        cout<<"depth:"<<tDepth.rows<<","<<tDepth.cols<<endl;
        emit pushRgbd(rgbdCopy.data,rgbdCopy.cols,rgbdCopy.rows,rgbdCopy.cols*rgbdCopy.channels());
//        emit pushDepth(tDepth.data,tDepth.cols,tDepth.rows,tDepth.cols*tDepth.channels());

//        cv::imshow("rgb", rgbMat);
//        cv::imshow("ir", irMat / 4096.0f);
//        cv::imshow("depth", depthMat / 4096.0f);
//        cv::imshow("undistorted", depthMatUndistorted / 1024.f);
//        cv::imshow("registered", rgbdCopy);
////        usleep(1000);
//        char c=waitKey(1);
//        if(c=='g') setBackGround();
//        usleep(10000);
        return false;
    }
    catch(...){
        perror("showFrames");
    }
}

void MyThread::init(libfreenect2::Freenect2Device *dev) {
//    namedWindow("undistorted");
//    setMouseCallback("undistorted", OnMouseAction, &rgbd);
//    createTrackbar("thresh1","undistorted", &thresh1,700);
//    createTrackbar("thresh2","undistorted", &thresh2,700);
//    groundPts.reserve(3);
    cases.reserve(3);
    T2O=Mat_<double>(3,1)<<0,0,0;
    irParams = dev->getIrCameraParams();
}

vector<Point3f> MyThread::getBoxPoints3d(vector<Point3f> inputPts) {
    vector<Point3f> outputPts;
    //    boxPoints3d.resize(boxPointsDepth.size());
    vector<Point3f> points;
    for (size_t j = 0; j < inputPts.size(); j++) {
        float x = (inputPts.at(j).x - irParams.cx) * inputPts.at(j).z /
                irParams.fx;
        float y = (inputPts.at(j).y - irParams.cy) * inputPts.at(j).z /
                irParams.fy;
        float z = inputPts.at(j).z;
        //            if(x==0&&y==0){

        //                cout<<"test:"<<x<<","<<y<<","<<z<<endl
        //                   <<boxPointsDepth.at(i).at(j).first<<","<<boxPointsDepth.at(i).at(j).second<<endl;
        //            }
        //            cout<<"z:"<<z<<endl;
        points.push_back(Point3f(x, y, z));
        //        cout<<"test getBoxPoints3d:"<<x<<","<<y<<","<<z<<endl;
    }
    return points;
}

Point3f MyThread::getBoxPoints3d(Point3f inputPts){
    float x = (inputPts.x - irParams.cx) * inputPts.z /
            irParams.fx;
    float y = (inputPts.y - irParams.cy) * inputPts.z /
            irParams.fy;
    float z = inputPts.z;
    return Point3f(x,y,z);
}

pair<vector<Point3f>, int> MyThread::getBoxPointsDepth(Mat depthMat,Mat rgb, Mat mask) {
    try{
        vector<Point3f> points;
        points.clear();
        double r=0,g=0,b=0;
        for (size_t i = 0; i < depthMat.rows; i++) {
            uchar *maskData = mask.ptr<uchar>(i);
            uchar *rgbData=rgb.ptr<uchar>(i);
            float *depthData=depthMat.ptr<float>(i);
            //        double *depthData = depthMat.ptr<double>(i);
            for (size_t j = 0; j < depthMat.cols; j++) {
                if (maskData[j] > 0&&depthData[j]>0) {
                    //                cout<<"depth"<<depthMat.at<float>(Point2f(j,i))<<"Points:"<<Point2f(j,i)<<endl;
                    points.push_back(Point3f(j, i,depthData[j]));
                    uchar tG,tB,tR;
                    tB=rgbData[j*4+0];
                    tG=rgbData[j*4+1];
                    tR=rgbData[j*4+2];
                    b+=tB;
                    g+=tG;
                    r+=tR;
                    //                cout<<tB<<","<<tG<<","<<tR<<endl;
                }
            }
        }
        int color;
        if((g-b)/b>0.2&&(g-r)/r>0.2)
            color=1;
        else if((b-g)/g>0.2&&(b-r)/r>0.2)
            color=2;
        else color=3;
        //    cout<<"test getBoxPointsDepth"<<endl;
        //    cout<<(Mat)points<<endl;
//            cout<<"test RGB:"<<b<<","<<g<<","<<r<<","<<color<<endl;
        return pair<vector<Point3f>,int>(points,color);
    }
    catch(...){
        perror("getBoxPointsDepth");
    }
}

Point3f MyThread::getBoxPointsDepth(Mat depthMat, Point2f pts){
    Point3f rlt;
    rlt=(Point3f(pts.x,pts.y,depthMat.at<float>(pts)));
    return rlt;
}


bool MyThread::setBackGround(){
    //    try{
    //        //    pthread_mutex_lock(&mutex);
//    if(!ifBackGoundSet){
        depthMatUndistorted.copyTo(backGround);
//        imwrite("backGround.",backGround);
        vector<Point3f> tGroundPoints;
        tGroundPoints.clear();
        tGroundPoints.push_back(Point3f(backGround.cols/2,backGround.rows/2,
                                    backGround.at<float>(Point(backGround.cols/2,backGround.rows/2))));
        tGroundPoints.push_back(Point3f(backGround.cols/2,backGround.rows/3,
                                    backGround.at<float>(Point(backGround.cols/2,backGround.rows/3))));
        tGroundPoints.push_back(Point3f(backGround.cols/2*3,backGround.rows/2,
                                    backGround.at<float>(Point(backGround.cols/2*3,backGround.rows/2))));
        vector<Point3f> groundPts3d;
        groundPts3d=getBoxPoints3d(tGroundPoints);

        Mat_<double> ptsMat(3, 3);
        Mat_<double> tMat(3, 1);
        tMat.at<double>(0)=groundPts3d.at(2).x-groundPts3d.at(0).x;
        tMat.at<double>(1)=groundPts3d.at(2).y-groundPts3d.at(0).y;
        tMat.at<double>(2)=groundPts3d.at(2).z-groundPts3d.at(0).z;
        tMat/=norm(tMat);
        tMat.copyTo(ptsMat.col(0));

        tMat.at<double>(0)=groundPts3d.at(1).x-groundPts3d.at(0).x;
        tMat.at<double>(1)=groundPts3d.at(1).y-groundPts3d.at(0).y;
        tMat.at<double>(2)=groundPts3d.at(1).z-groundPts3d.at(0).z;
        tMat/=norm(tMat);
        tMat.copyTo(ptsMat.col(1));

        tMat=ptsMat.col(0).cross(ptsMat.col(1));
        tMat/=norm(tMat);
        tMat.copyTo(ptsMat.col(2));

        tMat.at<double>(0)=groundPts3d.at(0).x;
        tMat.at<double>(1)=groundPts3d.at(0).y;
        tMat.at<double>(2)=groundPts3d.at(0).z;
        tMat.copyTo(T2G);

        R2G=ptsMat;
//        FileStorage fs("params.xml", FileStorage::WRITE);
//        fs<<"backGround"<<backGround;
//        fs.release();
        ifBackGroundSet=true;
//    }
//    else if(!ifOriginSet){
//        try{
//            for(size_t i=0;i<aprilTags.tags.size();i++){
//                if(aprilTags.tags.at(i).id==0){
//                    Point3f center=Point3f(aprilTags.tags.at(i).cxy.first, aprilTags.tags.at(i).cxy.second,
//                                           depthMatUndistorted.at<float>(Point(aprilTags.tags.at(i).cxy.first, aprilTags.tags.at(i).cxy.second)));
//                    vector<Point3f> tpts;
//                    tpts.push_back(center);
//                    cout<<center<<endl;
//                    T2O=Mat_<double>(3,1);
//                    T2O.setTo(0);
//                    tpts=convertWorld2Ground(getBoxPoints3d(tpts));
//                    cout<<"tpts:"<<(Mat)tpts<<endl;
//                    Mat_<double> tMat(3,1);
//                    tMat.at<double>(0)=tpts.at(0).x;
//                    tMat.at<double>(1)=tpts.at(0).y;
//                    tMat.at<double>(2)=tpts.at(0).z;
//                    T2O=-tMat;

//                    cout<<"R2G"<<R2G<<endl<<"T2G"<<T2G<<"T2O"<<T2O<<endl;
//                }
//                ifOriginSet=true;
//                break;
//            }

//        }
//        catch(...){
//            cout<<"set origin"<<endl;
//            perror("set origin");
//        }
//    }
    return true;
}

vector<Point3f> MyThread::convertWorld2Ground(vector<Point3f> inputPts){
    try{
        vector<Point3f> tPts;
        for(size_t j=0;j<inputPts.size();j++){
            Mat_<double> tMat(3,1);
            tMat.at<double>(0)=inputPts.at(j).x;
            tMat.at<double>(1)=inputPts.at(j).y;
            tMat.at<double>(2)=inputPts.at(j).z;
            tMat=R2G.inv()*(tMat-T2G)+T2O;
//            cout<<T2O<<endl;
            if(tMat.at<double>(2)>=0)
                tPts.push_back(Point3f(tMat.at<double>(0),tMat.at<double>(1),tMat.at<double>(2)));
        }
        return tPts;
    }
    catch(...){
        perror("convertWorld2Ground");
    }

    //        return inputPts;
}

Point3f MyThread::convertWorld2Ground(Point3f inputPts){
    Mat_<double> tMat(3,1);
    tMat.at<double>(0)=inputPts.x;
    tMat.at<double>(1)=inputPts.y;
    tMat.at<double>(2)=inputPts.z;
    tMat=R2G.inv()*(tMat-T2G)+T2O;
    return Point3f(tMat.at<double>(0),tMat.at<double>(1),tMat.at<double>(2));
}

Point3f MyThread::convertGround2World(Point3f inputPts){
    Mat_<double> tMat(3,1);
    tMat.at<double>(0)=inputPts.x;
    tMat.at<double>(1)=inputPts.y;
    tMat.at<double>(2)=inputPts.z;
    tMat=R2G*(tMat-T2O)+T2G;
    return Point3f(tMat.at<double>(0),tMat.at<double>(1),tMat.at<double>(2));
}

Mat MyThread::rotatedImage(Mat& inputImage){
    rotationMat=getRotationMatrix2D(Point(inputImage.cols/2,inputImage.rows/2),180,1);
    Mat outputImage;
    warpAffine(inputImage,outputImage,rotationMat,inputImage.size());
    return outputImage;
}

void MyThread::changeMode(){
    int tmode=(mode+1)%2;
    mode=tmode;
}

void MyThread::setOrigin(){
    for(size_t i=0;i<aprilTags.tags.size();i++){
        if(aprilTags.tags.at(i).id==0){
            Point3f center=Point3f(aprilTags.tags.at(i).cxy.first, aprilTags.tags.at(i).cxy.second,
                                   depthMatUndistorted.at<float>(Point(aprilTags.tags.at(i).cxy.first, aprilTags.tags.at(i).cxy.second)));

//            T2O=Mat_<double>(3,1);
//            T2O.setTo(0);
            groundPtsDepth.push_back(center);
            center=getBoxPoints3d(center);
            groundPts.push_back(center);
        }
    }
    if(groundPts.size()==2){
//        vector<Point3f> groundPts3d;
//        groundPts3d=getBoxPoints3d(groundPts);


        Point3f tPt;
        tPt=convertWorld2Ground(groundPts.at(0));
        cout<<tPt<<endl;
        tPt.z=0;
        tPt=convertGround2World(tPt);
        groundPtsDepth.push_back(tPt);
        getBoxPoints3d(tPt);
        groundPts.push_back(tPt);
        cout<<tPt<<endl;

        Mat_<double> ptsMat(3, 3);
        Mat_<double> tMat(3, 1);
        tMat.at<double>(0)=groundPts.at(0).x-groundPts.at(1).x;
        tMat.at<double>(1)=groundPts.at(0).y-groundPts.at(1).y;
        tMat.at<double>(2)=groundPts.at(0).z-groundPts.at(1).z;
        cout<<"y"<<tMat<<endl;
        tMat/=norm(tMat);
        tMat.copyTo(ptsMat.col(1));

        tMat.at<double>(0)=groundPts.at(0).x-groundPts.at(2).x;
        tMat.at<double>(1)=groundPts.at(0).y-groundPts.at(2).y;
        tMat.at<double>(2)=groundPts.at(0).z-groundPts.at(2).z;
        tMat/=norm(tMat);
        cout<<"z"<<tMat<<endl;
        tMat.copyTo(ptsMat.col(2));

        tMat=ptsMat.col(1).cross(ptsMat.col(2));
        cout<<"x"<<tMat<<endl;
        tMat/=norm(tMat);
        tMat.copyTo(ptsMat.col(0));

        tMat.at<double>(0)=groundPts.at(2).x;
        tMat.at<double>(1)=groundPts.at(2).y;
        tMat.at<double>(2)=groundPts.at(2).z;
        tMat.copyTo(T2G);

        R2G=ptsMat;

        T2O.at<double>(0)=0;
        T2O.at<double>(1)=0;
        T2O.at<double>(2)=0;

        FileStorage fs("params.xml", FileStorage::WRITE);
        fs<<"R2G"<<R2G;
        fs<<"T2G"<<T2G;
        fs<<"groundPtsDepth"<<groundPtsDepth;
        fs.release();
        ifOriginSet=true;
    }
}

void MyThread::shutDownKinect(){
    shutdown=true;
}

void MyThread::readParam(){
    FileStorage fs("params.xml", FileStorage::READ);
    fs["R2G"]>>R2G;
    fs["T2G"]>>T2G;
    fs["groundPtsDepth"]>>groundPtsDepth;
    fs.release();
    T2O.at<double>(0)=0;
    T2O.at<double>(1)=0;
    T2O.at<double>(2)=0;
    cout<<"read Param:"<<R2G<<endl<<T2G<<endl;
//    backGround=imread("backGround.jpg");
//    ifBackGroundSet=true;
    ifOriginSet=true;
}
