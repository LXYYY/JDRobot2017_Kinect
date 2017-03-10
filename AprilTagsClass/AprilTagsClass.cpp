//
// Created by root on 2/26/17.
//

#include "AprilTagsClass.h"

AprilTagsClass::AprilTagsClass() :
        m_tagDetector(NULL),
        m_tagCodes(AprilTags::tagCodes16h5),

        m_draw(true),
        m_arduino(false),
        m_timing(false),

        m_width(640),
        m_height(480),
        m_tagSize(0.166),
        m_fx(600),
        m_fy(600),
        m_px(m_width / 2),
        m_py(m_height / 2) {

}

AprilTagsClass::~AprilTagsClass() {

}

void AprilTagsClass::setTagCodes(string s) {
    if (s == "16h5") {
        m_tagCodes = AprilTags::tagCodes16h5;
    } else if (s == "25h7") {
        m_tagCodes = AprilTags::tagCodes25h7;
    } else if (s == "25h9") {
        m_tagCodes = AprilTags::tagCodes25h9;
    } else if (s == "36h9") {
        m_tagCodes = AprilTags::tagCodes36h9;
    } else if (s == "36h11") {
        m_tagCodes = AprilTags::tagCodes36h11;
    } else {
        cout << "Invalid tag family specified" << endl;
        exit(1);
    }
}

bool AprilTagsClass::setup() {
    try {
        m_tagDetector = new AprilTags::TagDetector(m_tagCodes);
    }
    catch (...) {
        cout << "apriltags set failed" << endl;
        return false;
    }
    return true;
}

bool AprilTagsClass::processImage(Mat image) {
    Mat image_gray;
    tags.clear();
    cv::cvtColor(image, image_gray, CV_BGR2GRAY);
    vector<AprilTags::TagDetection> detections;
    detections = m_tagDetector->extractTags(image_gray);
    for(size_t i=0;i<detections.size();i++){
        if(detections.at(i).good)
            tags.push_back(detections.at(i));
    }
    return false;
}

void AprilTagsClass::drawTags(Mat &image) {
    for (int i = 0; i < tags.size(); i++) {
        // also highlight in the image
        tags[i].draw(image);
    }
}
