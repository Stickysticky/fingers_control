#include "CameraService.h"
#include <iostream>

using namespace std;
using namespace cv;

CameraService::CameraService(int cameraIndex) {
    cap.open(cameraIndex);
    if (!cap.isOpened()) {
        std::cerr << "❌ Error : Camera unavailable " << cameraIndex << endl;
    } else {
        std::cout << "✅ Camera " << cameraIndex << " succesfully opened" << endl;
    }
}

CameraService::~CameraService()
{
    release();
}

bool CameraService::isOpened() const {
    return cap.isOpened();
}


bool CameraService::getFrame(Mat& frame) {
    if (!cap.isOpened()) return false;
    cap >> frame;
    return !frame.empty();
}

void CameraService::release() {
    if (cap.isOpened()) {
        cap.release();
        cout << "📷 Caméra freed" << endl;
    }
}
