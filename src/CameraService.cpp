#include "CameraService.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

CameraService::CameraService(int cameraIndex)
    : cap(cameraIndex)  // Initialisation directe de cap
{
    if (!cap.isOpened()) {
        cerr << "❌ Error: Camera unavailable " << cameraIndex << endl;
    } else {
        cout << "✅ Camera " << cameraIndex << " successfully opened" << endl;
    }

     symbolTemplate = imread("images/pokeball.png", IMREAD_GRAYSCALE);  // Charger le modèle
     if (symbolTemplate.empty()) {
        cerr << "❌ Error: Could not load image pokeball.png" << endl;
        return;
    }

}

CameraService::~CameraService()
{
    release();
}

bool CameraService::isOpened() const {
    return cap.isOpened();
}

bool CameraService::updateFrame() {
    if (!cap.isOpened()) return false;
    cap >> frame;
    return !frame.empty();
}

void CameraService::release() {
    if (cap.isOpened()) {
        cap.release();
        cout << "📷 Camera freed" << endl;
    }
}

Mat CameraService::getFrame() {
    updateFrame();  // Correction de la faute de frappe
    return frame;
}



/*
void CameraService::cameraProcess(){
   namedWindow("Fingers Control");//Declaring the video to show the video//

   while (true){ //Taking an everlasting loop to show the video//
      cap >> frame;
      if (frame.empty()){ //Breaking the loop if no video frame is detected//
         break;
      }
      imshow("Fingers Control", frame);//Showing the video//
      char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
      if (c == 27){ //If 'Esc' is entered break the loop//
         break;
      }

      if (cv::getWindowProperty("Fingers Control", WND_PROP_VISIBLE) < 1) {
            break;  // La fenêtre est fermée
      }
   }

   release();//Releasing the buffer memory//
   destroyAllWindows();
}*/

void CameraService::cameraProcess() {
    namedWindow("Fingers Control");

    while (true) {
        // Récupérer la frame
        if (!updateFrame()) {
            cerr << "⚠️ Warning: No frame captured!" << endl;
            break;
        }

        // Convertir l'image en niveau de gris
        Mat grayFrame;
        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        // Appliquer le template matching pour détecter le symbole
        Mat result;
        matchTemplate(grayFrame, symbolTemplate, result, TM_CCOEFF_NORMED);

        // Seuil de détection (plus c'est proche de 1, plus c'est une correspondance parfaite)
        double minVal, maxVal;
        Point minLoc, maxLoc;
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
        cout<<maxVal;

        // Si la correspondance est assez forte (au-dessus d'un seuil), dessiner un rectangle
        if (maxVal > 0.2) {
            Rect symbolRect(maxLoc.x, maxLoc.y, symbolTemplate.cols, symbolTemplate.rows);
            rectangle(frame, symbolRect, Scalar(0, 255, 0), 2);  // Dessiner un rectangle vert
        }

        // Afficher la vidéo avec les rectangles autour des symboles détectés
        imshow("Fingers Control", frame);

        // Condition pour quitter avec la touche 'Esc'
        char c = (char)waitKey(25);
        if (c == 27) {
            break;
        }

        if (cv::getWindowProperty("Fingers Control", cv::WND_PROP_VISIBLE) < 1) {
            break;  // La fenêtre est fermée
        }
    }

    release();
    destroyAllWindows();
}
