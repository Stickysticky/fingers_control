#include "CameraService.h"
#include <iostream>
#include "MobileNetSSDService.h"

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
    MobileNetSSDService mobileNetService;

    while (true) {
        // Récupérer la frame
        if (!updateFrame()) {
            cerr << "⚠️ Warning: No frame captured!" << endl;
            break;
        }

        // Prétraitement de l'image
        Mat blob = blobFromImage(frame, 0.007843, Size(300, 300), Scalar(127.5, 127.5, 127.5), true, false);

        // Passer l'image dans le réseau
        mobileNetService.setInput(blob);
        Mat detections = mobileNetService.getNet().forward();

        // Parcourir les résultats
        Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());
        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);
            if (confidence > 0.5) {  // Seulement les objets détectés avec une bonne précision
                int classId = static_cast<int>(detectionMat.at<float>(i, 1));
                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                // Dessiner un rectangle autour de l'objet détecté
                rectangle(frame, Point(xLeftBottom, yLeftBottom), Point(xRightTop, yRightTop), Scalar(0, 255, 0), 2);

                // Ajouter le nom de l'objet détecté
                string label = MobileNetSSDService::CLASS_NAMES[classId] + ": " + to_string(confidence);
                putText(frame, label, Point(xLeftBottom, yLeftBottom - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            }
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
