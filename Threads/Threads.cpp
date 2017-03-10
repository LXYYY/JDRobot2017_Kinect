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
    config.MaxDepth=1.f;
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
        cout<<x<<","<<y<<ends<< img->at<float>(y, x) << endl;
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

    //    depthMatUndistorted=rotatedImage(depthMatUndistorted);
    //    depthMat=rotatedImage(depthMat);
    //    rgbMat=rotatedImage(rgbMat);
    //    rgbd=rotatedImage(rgbd);
    //    rgbd2=rotatedImage(rgbd2);
    //    irMat=rotatedImage(irMat);

    flip(depthMatUndistorted,depthMatUndistorted,0);
    flip(depthMat,depthMat,0);
    flip(rgbMat,rgbMat,0);
    flip(rgbd,rgbd,0);
    flip(rgbd2,rgbd2,0);
    flip(irMat,irMat,0);
    return true;
}
int thresh=-1;
bool MyThread::showFrames(){
    boxes.clear();
    Mat rgbdCopy;
    rgbd.copyTo(rgbdCopy);
    if(!ifBackGoundSet){
//        aprilTags.processImage(rgbdCopy);
//        aprilTags.drawTags(rgbdCopy);
    }
    else {

        line(rgbdCopy,Point(groundPts.at(1).x,groundPts.at(1).y),
             Point(groundPts.at(0).x,groundPts.at(0).y),Scalar(0,0,255));
        line(rgbdCopy,Point(groundPts.at(2).x,groundPts.at(2).y),
             Point(groundPts.at(0).x,groundPts.at(0).y),Scalar(0,0,255));
        Point3f vZ=Point3f(R2G.col(2).at<double>(0),R2G.col(2).at<double>(1),R2G.col(2).at<double>(2));
        Point2f z;
        z.x=vZ.x*irParams.fx/vZ.z+irParams.cx;
        z.y=vZ.y*irParams.fy/vZ.z+irParams.cy;
        z=-z*100+Point2f(groundPts.at(0).x,groundPts.at(0).y);
        line(rgbdCopy,z, Point(groundPts.at(0).x,groundPts.at(0).y),Scalar(255,0,0));
        cv::imshow("registered", rgbdCopy);
        foreGround = (backGround - depthMatUndistorted);
        imshow("background", backGround / 1024.f);
        imshow("foregound", foreGround / 1024.f);

        Mat binaryMat;
        binaryMat=(foreGround/1000.f*255.f);
        binaryMat.convertTo(binaryMat,CV_8U);
        imshow("binary",binaryMat);

        vector<Point3f> pts2Draw;

        if(mode==SEARCH_BOX){
            threshold(binaryMat,binaryMat,10,255,THRESH_BINARY);
            imshow("thresh",binaryMat);

            vector<Vec4i> hierarchy;
            vector<vector<Point> > contours0;

            Mat mask;
            mask.create(foreGround.size(), CV_8UC1);
            pair<vector<Point3f>,int> tBoxPointsDepth;
            vector<Point3f> tBoxPoints3dG,tBoxPoints3dW;

            findContours(binaryMat, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
            for (int i = 0; i < contours0.size(); i++) {
                if (contourArea(contours0.at(i)) >= 5000) {

                    /////////make a mask//////////
                    mask.setTo(0);
                    drawContours(mask, contours0, i, Scalar(255), -1);
                    drawContours(mask, contours0, i, Scalar(0), 5);
                    /////////make a mask end//////////

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
                        if(tBoxPoints3dG.at(j).z>z){
                            z=tBoxPoints3dG.at(j).z;
                        }
                    }
                    RotatedRect boundingBox=minAreaRect(proj2ZPts);

                    boundingBox.points(tBox.pts);
                    tBox.size=boundingBox.size;
                    tBox.center=boundingBox.center;
                    tBox.color=tBoxPointsDepth.second;
                    tBox.z=z;

                    if(fabs((tBox.pts[0].x-tBox.pts[1].x)*(tBox.pts[0].x-tBox.pts[1].x)+(tBox.pts[0].y-tBox.pts[1].y)*(tBox.pts[0].y-tBox.pts[1].y))>
                            fabs((tBox.pts[1].x-tBox.pts[2].x)*(tBox.pts[1].x-tBox.pts[2].x)+(tBox.pts[1].y-tBox.pts[2].y)*(tBox.pts[1].y-tBox.pts[2].y))){
                        tBox.dir=(tBox.pts[0]-tBox.pts[1]).y<=0?(tBox.pts[0]-tBox.pts[1]):(tBox.pts[1]-tBox.pts[0]);
                    }
                    else {
                        tBox.dir=(tBox.pts[1]-tBox.pts[2]).y<=0?(tBox.pts[1]-tBox.pts[2]):(tBox.pts[2]-tBox.pts[1]);
                    }

                    boxes.push_back(tBox);
                    ///////push a new box end///////////
                }
            }

            /////////make a image to check//////////
            Mat check(Size(1000,1000),CV_8UC3);
            check.setTo(0);
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
                    line(check,boxes.at(i).pts[j]+Point2f(500,500),boxes.at(i).pts[(j+1)%4]+Point2f(500,500),color);
                }
                line(check,boxes.at(i).center+Point2f(500,500),boxes.at(i).center+Point2f(500,500)+boxes.at(i).dir,color);
                cout<<"box center:"<<boxes.at(i).center<<endl;
            }
            imshow("check",check);
            /////////make a image to check end//////////

            /////////send box centers to communication thread////////
            if(boxes.size()>0){
                int tgtId;
                float minX=10000,minY=10000;
                for(size_t i=0;i<boxes.size();i++){
                    if(fabs(boxes.at(i).center.x)<minX){
                        minX=fabs(boxes.at(i).center.x);
                        tgtId=i;
                    }
                }
                float point[3];
                point[0]=boxes.at(tgtId).center.x;
                point[1]=boxes.at(tgtId).center.y;
                point[2]=0;
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

                    if(fabs(ptG.x)<10)
                }
            }

//            bool detected[3];
//            for(size_t i=0;i<cases.size();i++){
//                if(cases.at(i).color==1
//                        ||cases.at(i).color==2
//                        ||cases.at(i).color==3){
//                    if(cases.at(i).dir.x!=Point2f(0,0))
//                        detected[cases.at(i).color-1]=true;
//                }
//            }
//            if(detected[0]&&detected[1]&&detected[2]){
//                float tPt[3]={0,0,0,0,0,0};
//                emit sendPoint(Communication::InitCasesEnd,tPt);
//            }


            //            threshold(binaryMat,binaryMat,thresh,255,THRESH_BINARY);
            //            imshow("thresh",binaryMat);
//            pair<vector<Point3f>,int> tBoxPointsDepth;
//            vector<Point3f> tBoxPoints3dG,tBoxPoints3dW;
//            Mat mask;
//            mask.create(depthMatUndistorted.size(),CV_8UC1);
//            mask.setTo(1);
//            tBoxPointsDepth=getBoxPointsDepth(depthMatUndistorted,rgbd,mask);
//            tBoxPoints3dW=getBoxPoints3d(tBoxPointsDepth.first);
//            tBoxPoints3dG=convertWorld2Ground(tBoxPoints3dW);

//            Mat proj(Size(700,700),CV_8UC1);
//            proj.setTo(0);
//            if(thresh>0){
//                for(size_t i=0;i<tBoxPoints3dG.size();i++){
//                    if(fabs(tBoxPoints3dG.at(i).z-(thresh))<50){
//                        pts2Draw.push_back(tBoxPoints3dG.at(i));
//                        circle(proj,Point(tBoxPoints3dG.at(i).x+proj.cols/2,-tBoxPoints3dG.at(i).y+proj.rows/2),3,Scalar(255),-1);
//                    }
//                }
//            }
//            else
//                pts2Draw=tBoxPoints3dG;

//            imshow("proj",proj);

        }

        /////////emit drawPoints signal/////////
        npts=pts2Draw.size();
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
    cv::imshow("rgb", rgbMat);
    cv::imshow("ir", irMat / 4096.0f);
    cv::imshow("depth", depthMat / 4096.0f);
    cv::imshow("undistorted", depthMatUndistorted / 1024.f);
    cv::imshow("registered", rgbdCopy);
    usleep(1000);
    return false;
}

void MyThread::init(libfreenect2::Freenect2Device *dev) {
    namedWindow("undistorted");
    setMouseCallback("undistorted", OnMouseAction, &depthMatUndistorted);
    createTrackbar("threash","undistorted", &thresh,300);

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
    vector<Point3f> points;
    points.clear();
    long r=0,g=0,b=0;
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
    //    cout<<"test getBoxPointsDepth"<<endl;
    //    cout<<(Mat)points<<endl;
    //    cout<<"test RGB:"<<b<<","<<g<<","<<r<<endl;
    return pair<vector<Point3f>,int>(points,0);
}

Point3f MyThread::getBoxPointsDepth(Mat depthMat, Point2f pts){
    Point3f rlt;
    rlt=(Point3f(pts.at(i).x,pts.at(i).y,depthMat.at<float>(pts)));
    return rlt;
}


bool MyThread::setBackGround(){
    //    pthread_mutex_lock(&mutex);
    depthMatUndistorted.copyTo(backGround);
    groundPts.clear();
    groundPts.push_back(Point3f(backGround.cols/2,backGround.rows/2,
                                backGround.at<float>(Point(backGround.cols/2,backGround.rows/2))));
    groundPts.push_back(Point3f(backGround.cols/2,backGround.rows/3,
                                backGround.at<float>(Point(backGround.cols/2,backGround.rows/3))));
    groundPts.push_back(Point3f(backGround.cols/2*3,backGround.rows/2,
                                backGround.at<float>(Point(backGround.cols/2*3,backGround.rows/2))));
    vector<Point3f> groundPts3d;
    groundPts3d=getBoxPoints3d(groundPts);

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

    if(aprilTags.tags.size()==1&&aprilTags.tags.at(0).id==0){
        Point3f center=Point3f(aprilTags.tags.at(0).cxy.first, aprilTags.tags.at(0).cxy.second,
                               depthMatUndistorted.at<float>(Point(aprilTags.tags.at(0).cxy.first, aprilTags.tags.at(0).cxy.second)));
        vector<Point3f> tpts;
        tpts.push_back(center);
        cout<<center<<endl;
        T2O=Mat_<double>(3,1);
        T2O.setTo(0);
        tpts=convertWorld2Ground(getBoxPoints3d(tpts));

        tMat.at<double>(0)=tpts.at(0).x;
        tMat.at<double>(1)=tpts.at(0).y;
        tMat.at<double>(2)=tpts.at(0).z;
        T2O=-tMat;

        cout<<"R2G"<<R2G<<endl<<"T2G"<<T2G<<"T2O"<<T2O<<endl;
        ifBackGoundSet=true;
    }
    ifBackGoundSet=true;

    return true;
}

vector<Point3f> MyThread::convertWorld2Ground(vector<Point3f> inputPts){
    vector<Point3f> tPts;
    for(size_t j=0;j<inputPts.size();j++){
        Mat_<double> tMat(3,1);
        tMat.at<double>(0)=inputPts.at(j).x;
        tMat.at<double>(1)=inputPts.at(j).y;
        tMat.at<double>(2)=inputPts.at(j).z;
        tMat=R2G.inv()*(tMat-T2G)/*+T2O*/;
        if(tMat.at<double>(2)>=0)
            tPts.push_back(Point3f(tMat.at<double>(0),tMat.at<double>(1),tMat.at<double>(2)));
    }
    return tPts;
    //        return inputPts;
}

Point3f MyThread::convertWorld2Ground(Point3f inputPts){
    Mat_<double> tMat(3,1);
    tMat.at<double>(0)=inputPts.x;
    tMat.at<double>(1)=inputPts.y;
    tMat.at<double>(2)=inputPts.z;
    tMat=R2G.inv()*(tMat-T2G)/*+T2O*/;
    return Point3f(tMat.at<double>(0),tMat.at<double>(1),tMat.at<double>(2));
}

Mat MyThread::rotatedImage(Mat& inputImage){
    rotationMat=getRotationMatrix2D(Point(inputImage.cols/2,inputImage.rows/2),180,1);
    Mat outputImage;
    warpAffine(inputImage,outputImage,rotationMat,inputImage.size());
    return outputImage;
}

