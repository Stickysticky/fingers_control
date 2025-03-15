#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <opencv2/opencv.hpp>

using namespace cv;

class CameraService
{
    public:
        // Constructeur avec l'index de la caméra (0 par défaut)
        CameraService(int cameraIndex = 0);
        virtual ~CameraService();

        bool isOpened() const;
        bool updateFrame();
        void release();
        Mat getFrame();
        void cameraProcess();
    private:
        VideoCapture cap;  // Gère l'accès à la caméra
        Mat frame;         // Stocke l'image capturée
};

#endif // CAMERASERVICE_H
