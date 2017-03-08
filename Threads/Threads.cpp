//
// Created by root on 3/2/17.
//

#include "pthread.h"
#include "Threads.h"
#include "mainwindow.h"
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
    libfreenect2::Freenect2Device::Config config;
    config.EnableBilateralFilter=true;
    config.EnableEdgeAwareFilter=true;
    config.MaxDepth=1.5;
    config.MinDepth=0.f;
    dev->setConfiguration(config);

    /// [loop start]
    while (!protonect_shutdown) {
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




    kinect();
}

//void *CommunicateThread(void *arg) {
//    communicator.connect();
//    while (1){
//        for (size_t i = 0; i < communicator.PtsToSend.size(); i++) {
//            pthread_mutex_lock(&mutex);
//            communicator.sendFrame(communicator.PtsToSend.at(i).first,
//                                   communicator.PtsToSend.at(i).second.x,
//                                   communicator.PtsToSend.at(i).second.y,
//                                   communicator.PtsToSend.at(i).second.z);
//            pthread_mutex_unlock(&mutex);
//            cout<<communicator.PtsToSend.at(i).second.x<<"x"
//                <<communicator.PtsToSend.at(i).second.y<<"x"
//                <<communicator.PtsToSend.at(i).second.z<<endl;
//            usleep(10000);
//        }
////        communicator.sendFrame(Communication::PointID,99);
//    }
//}



//bool createThread() {
//    pthread_t id_1, id_2;
//    int ret;
////    ret = pthread_create(&id_1, NULL, kinect, NULL);
////    ret = pthread_create(&id_2, NULL, viewerThread, NULL);
//    return ret;
//}

bool pressed = false;

void OnMouseAction(int event, int x, int y, int flags, void *ustc) {
    Mat *img = (Mat *) ustc;
    if (event == CV_EVENT_LBUTTONDOWN) {
        pressed = true;
    } else if (event == CV_EVENT_LBUTTONUP && pressed) {
        cout << img->at<float>(y, x) << endl;
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

    depthMatUndistorted=rotatedImage(depthMatUndistorted);
    depthMat=rotatedImage(depthMat);
    rgbMat=rotatedImage(rgbMat);
    rgbd=rotatedImage(rgbd);
    rgbd2=rotatedImage(rgbd2);
    irMat=rotatedImage(irMat);

    return true;
}

bool MyThread::showFrames(){
    //    cout<<irParams.fx<<","<<irParams.fy<<endl;
    cv::resize(rgbMat, rgbMat, Size(), 0.5, 0.5);
    cv::resize(irMat, irMat, Size(), 0.5, 0.5);
    cv::resize(depthMat, depthMat, Size(), 0.5, 0.5);
    cv::imshow("rgb", rgbMat);
    cv::imshow("ir", irMat / 4096.0f);
    cv::imshow("depth", depthMat / 4096.0f);

    cv::imshow("undistorted", depthMatUndistorted / 1024.f);
    //    cv::imshow("depth2RGB", rgbd2 / 4096.0f);
    try {
        line(rgbd,groundPts.at(0).at(1).first,groundPts.at(0).at(0).first,Scalar(0,0,255));
        line(rgbd,groundPts.at(0).at(2).first,groundPts.at(0).at(0).first,Scalar(0,255,0));
        Point3f vZ=Point3f(R2G.col(2).at<double>(0),R2G.col(2).at<double>(1),R2G.col(2).at<double>(2));
        Point2f z;
        z.x=vZ.x*irParams.fx/vZ.z+irParams.cx;
        z.y=vZ.y*irParams.fy/vZ.z+irParams.cy;
        z=-z*100+groundPts.at(0).at(0).first;
        line(rgbd,z,groundPts.at(0).at(0).first,Scalar(255,0,0));
        cv::imshow("registered", rgbd);
        foreGround = (backGround - depthMatUndistorted);
        imshow("background", backGround / 1024.f);
        imshow("foregound", foreGround / 1024.f);

        Mat binaryMat;
        binaryMat=(foreGround/1000.f*255.f);
        binaryMat.convertTo(binaryMat,CV_8U);
        imshow("binary",binaryMat);
        threshold(binaryMat,binaryMat,30,255,THRESH_BINARY);
        imshow("thresh",binaryMat);

        vector<Vec4i> hierarchy;
        vector<vector<Point> > contours0;
        vector<vector<Point> > approx;
        vector<vector<Point> > objBoxPts;
        Mat mask;
        mask.create(foreGround.size(), CV_8UC1);

        mask.setTo(0);
        findContours(binaryMat, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        approx.resize((contours0.size()));
        for (int i = 0; i < contours0.size(); i++) {
            if (contourArea(contours0.at(i)) >= 10000) {
                vector<Point> hull;
                convexHull(contours0.at(i), hull, true);
                approxPolyDP(hull, approx.at(i), 10, true);
                drawContours(mask, contours0, i, Scalar(255), -1);
                drawContours(mask, contours0, i, Scalar(0), 3);
                //            objBoxPts.push_back(hull);
            }
        }

//        drawContours(mask, approx, -1, Scalar(255), -1);
//        mask.setTo(1);
        imshow("masK",mask);
        getBoxPointsDepth(depthMatUndistorted, mask,boxPointsDepth);

        //        Mat tMat;
        //        mask.copyTo(tMat);
        //        tMat.setTo(0);
        //        for(size_t i=0;i<boxPointsDepth.size();i++){
        //            for(size_t j=0;j<boxPointsDepth.at(i).size();j++){
        //                circle(tMat,boxPointsDepth.at(i).at(j).first,1,Scalar(255),-1);
        //            }
        //        }
        //        imshow("test",tMat);

        getBoxPoints3d(boxPointsDepth,boxPoints3dW);
        convertWorld2Ground(boxPoints3dW,boxPoints3dG);
        Mat proj2Z(Size(500,500),CV_8UC1);
        proj2Z.setTo(0);
//        boxPoints3dG=boxPoints3dW;
//        cout<<"boxPoint3dG"<<(Mat)boxPoints3dG.at(0)<<endl;
        int size=0;
        float points[npts*3];
        for (size_t i = 0; i < boxPoints3dG.size(); i++) {
            for(size_t j=0;j<boxPoints3dG.at(i).size();j++){
                points[(i+1)*j*3+0]=boxPoints3dG.at(i).at(j).x;
                points[(i+1)*j*3+1]=boxPoints3dG.at(i).at(j).y;
                points[(i+1)*j*3+2]=boxPoints3dG.at(i).at(j).z;
                circle(proj2Z,Point(boxPoints3dG.at(i).at(j).x+250,boxPoints3dG.at(i).at(j).y+250),3,Scalar(255),-1);
                //                if(points[(i+1)*j*3+0]==0&&points[(i+1)*j*3+1]==0){

                //                    cout<<"test:"<<points[(i+1)*j*3+0]<<","<<points[(i+1)*j*3+1]<<","<<points[(i+1)*j*3+2]<<endl
                //                       <<boxPoints3d.at(i).at(j)<<endl;
                //                }
                //                cout<<"Points:"<<points[(i+1)*j*3+0]<<","
                //                   <<points[(i+1)*j*3+1]<<","
                //                  <<points[(i+1)*j*3+2]<<endl;
                size++;
            }
        }

        vector<vector<Point>> contours;
        RotatedRect boundingBox;
        findContours(proj2Z, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < contours.size(); i++) {
            if (contourArea(contours.at(i)) >= 10000) {
                boundingBox=minAreaRect(contours.at(i));
            }
        }
        Point2f pts[4];
        boundingBox.points(pts);
        cvtColor(proj2Z,proj2Z,COLOR_GRAY2BGR);
        for(size_t i=0;i<4;i++){
            line(proj2Z,pts[i],pts[(i+1)%4],Scalar(0,0,255));
        }
        char width[20],height[20];
        sprintf(width,"%5.2f",boundingBox.size.width);
        sprintf(height,"%5.2f",boundingBox.size.height);
        putText(proj2Z,width,(pts[0]+pts[1])/2,FONT_HERSHEY_PLAIN,2,Scalar(0,0,255));
        putText(proj2Z,height,(pts[1]+pts[2])/2,FONT_HERSHEY_PLAIN,2,Scalar(0,0,255));
        imshow("proj2Z",proj2Z);
        //        cout<<"show"<<endl;
        //        float test1[3]={1,1,1};
        //        cout<<"size tp emit="<<npts<<endl;
        emit drawPoints(points,npts);
//        usleep(10000);
//        emit drawPoints(proj2Z,npts);

        //        cout<<"show1"<<endl;
    }
    catch (...) {
        perror("foreground");
    }
    //    char c = waitKey(1);
    usleep(1000);
    //    if (c == 'g')
    //        depthMatUndistorted.copyTo(backGround);
    //    if (c == 'q')
    //        return true;
    return false;
}

void MyThread::init(libfreenect2::Freenect2Device *dev) {
    namedWindow("undistorted");
    setMouseCallback("undistorted", OnMouseAction, &depthMatUndistorted);

    irParams = dev->getIrCameraParams();
}

bool MyThread::getBoxPoints3d(vector<vector<pair<Point2f,float>>> inputPts,vector<vector<Point3f>>& outputPts) {
    outputPts.clear();
    npts=0;
    //    boxPoints3d.resize(boxPointsDepth.size());
    for (size_t i = 0; i < inputPts.size(); i++) {
        vector<Point3f> points;
        points.clear();
        for (size_t j = 0; j < inputPts.at(i).size(); j++) {
            float x = (inputPts.at(i).at(j).first.x - irParams.cx) * inputPts.at(i).at(j).second /
                    irParams.fx;
            float y = (inputPts.at(i).at(j).first.y - irParams.cy) * inputPts.at(i).at(j).second /
                    irParams.fy;
            float z = inputPts.at(i).at(j).second;
            //            if(x==0&&y==0){

            //                cout<<"test:"<<x<<","<<y<<","<<z<<endl
            //                   <<boxPointsDepth.at(i).at(j).first<<","<<boxPointsDepth.at(i).at(j).second<<endl;
            //            }
            //            cout<<"z:"<<z<<endl;
            points.push_back(Point3f(x, y, z));
            npts++;
        }
        outputPts.push_back(points);
    }
    return true;
}

bool MyThread::getBoxPointsDepth(Mat depthMat, Mat mask,vector<vector<pair<Point2f,float>>>& outputPts) {
    outputPts.clear();
    vector<pair<Point2f, float>> points;
    for (size_t i = 0; i < depthMat.rows; i++) {
        uchar *maskData = mask.ptr<uchar>(i);
        //        double *depthData = depthMat.ptr<double>(i);
        for (size_t j = 0; j < depthMat.cols; j++) {
            if (maskData[j] > 0) {
                //                cout<<"depth"<<depthMat.at<float>(Point2f(j,i))<<"Points:"<<Point2f(j,i)<<endl;
                points.push_back(pair<Point2f, double>(Point2f(j, i), depthMat.at<float>(Point2f(j,i))));
            }
        }
    }
    outputPts.push_back(points);
    return false;
}

bool MyThread::setBackGround(){
    //    pthread_mutex_lock(&mutex);
    depthMatUndistorted.copyTo(backGround);
    groundPts.clear();
    vector<pair<Point2f,float>> tPts;
    tPts.push_back(pair<Point2f,float>(Point(backGround.cols/2,backGround.rows/2),
                                       backGround.at<float>(Point(backGround.cols/2,backGround.rows/2))));
    tPts.push_back(pair<Point2f,float>(Point(backGround.cols/2,backGround.rows/4),
                                       backGround.at<float>(Point(backGround.cols/2,backGround.rows/4))));
    tPts.push_back(pair<Point2f,float>(Point(backGround.cols/4*3,backGround.rows/2),
                                       backGround.at<float>(Point(backGround.cols/4*3,backGround.rows/2))));
    groundPts.push_back(tPts);
    vector<vector<Point3f>> groundPts3d;
    getBoxPoints3d(groundPts,groundPts3d);
    Mat_<double> ptsMat(3, 3);
    Mat_<double> deltaMat(3, 3);

    Mat_<double> tMat(3, 1);
    tMat.at<double>(0)=groundPts3d.at(0).at(2).x-groundPts3d.at(0).at(0).x;
    tMat.at<double>(1)=groundPts3d.at(0).at(2).y-groundPts3d.at(0).at(0).y;
    tMat.at<double>(2)=groundPts3d.at(0).at(2).z-groundPts3d.at(0).at(0).z;
//    cout<<"tMat1:"<<tMat<<endl;
    tMat/=norm(tMat);
    tMat.copyTo(ptsMat.col(0));

    tMat.at<double>(0)=groundPts3d.at(0).at(1).x-groundPts3d.at(0).at(0).x;
    tMat.at<double>(1)=groundPts3d.at(0).at(1).y-groundPts3d.at(0).at(0).y;
    tMat.at<double>(2)=groundPts3d.at(0).at(1).z-groundPts3d.at(0).at(0).z;
//    cout<<"tMat2:"<<tMat<<endl;
    tMat/=norm(tMat);
    tMat.copyTo(ptsMat.col(1));

    tMat=ptsMat.col(0).cross(ptsMat.col(1));
//    cout<<"tMat3:"<<tMat<<endl;
    tMat/=norm(tMat);
    tMat.copyTo(ptsMat.col(2));

    tMat.at<double>(0)=groundPts3d.at(0).at(0).x;
    tMat.at<double>(1)=groundPts3d.at(0).at(0).y;
    tMat.at<double>(2)=groundPts3d.at(0).at(0).z;
    tMat.copyTo(T2G);

    R2G=ptsMat;
//    cout<<"R2G:"<<endl<<R2G<<endl<<R2G.inv()<<"T2G"<<T2G<<endl;

//    Mat tmp1;
//    tmp1=R2G.col(0).mul(R2G.col(1)).mul(R2G.col(2));
//    cout<<"check:"<<tmp1<<endl;
//    getchar();
    return true;
}

bool MyThread::convertWorld2Ground(vector<vector<Point3f>> inputPts,vector<vector<Point3f>>& outputPts){
//    cout<<"test1"<<endl;
    outputPts.clear();
    vector<Point3f> tPts;
    for(size_t i=0;i<inputPts.size();i++){
        for(size_t j=0;j<inputPts.at(i).size();j++){
            Mat_<double> tMat(3,1);
            tMat.at<double>(0)=inputPts.at(i).at(j).x;
            tMat.at<double>(1)=inputPts.at(i).at(j).y;
            tMat.at<double>(2)=inputPts.at(i).at(j).z;
            tMat=R2G.inv()*(tMat-T2G);
            tPts.push_back(Point3f(tMat.at<double>(0),tMat.at<double>(1),tMat.at<double>(2)));
        }
    }
//    cout<<"test2"<<endl;
    outputPts.push_back(tPts);
//    cout<<"outputPts"<<(Mat)outputPts.at(0)<<endl;
    return true;
}

Mat MyThread::rotatedImage(Mat& inputImage){
    if(!rotationMatInited){
        rotationMat=getRotationMatrix2D(Point(inputImage.cols/2,inputImage.rows/2),180,1);
        rotationMatInited=true;
    }
    Mat outputImage;
    warpAffine(inputImage,outputImage,rotationMat,inputImage.size());
}

