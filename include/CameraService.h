#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <opencv2/opencv.hpp>

using namespace cv;

class CameraService
{
    public:
        // Constructeur avec l'index de la caméra (0 par défaut)
        CameraService(int cameraIndex = 0);
        ~CameraService();

        bool isOpened() const;
        bool updateFrame();
        void release();
        Mat getFrame();
        void cameraProcess();

    private:
        VideoCapture cap;  // Gère l'accès à la caméra
        Mat frame;         // Stocke l'image capturée
        Mat symbolTemplate; // Template pour le symbole à détecter
};

#endif // CAMERASERVICE_H
