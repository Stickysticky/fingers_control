#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <opencv2/opencv.hpp>

using namespace cv;

class CameraService
{
    public:
        CameraService(int cameraIndex = 0);
        ~CameraService();

        bool isOpened() const;
        bool getFrame(cv::Mat& frame);
        void release();

    private:
        VideoCapture cap;
};

#endif // CAMERASERVICE_H
