#include "CameraService.h"
#include <iostream>
#include "MobileNetSSDService.h"
#include <vector>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;


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



void CameraService::cameraProcess() {
    namedWindow("Fingers Control");
    MobileNetSSDService mobileNetService;

    while (true) {
        // Récupérer la frame
        if (!updateFrame()) {
            cerr << "⚠️ Warning: No frame captured!" << endl;
            break;
        }

        cv::flip(frame, frame, 1);

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


/*
void CameraService::cameraProcess() {
    namedWindow("Fingers Control");

    // Chargement du modèle YOLO
    String yoloCfg = "/home/olivier/Documents/dev/IA/darknet/cfg/yolov3-tiny.cfg";  // Le fichier de configuration YOLO
    String yoloWeights = "/home/olivier/Documents/dev/IA/darknet/weights/yolov3-tiny.weights";  // Le fichier de poids YOLO

    Net net = readNetFromDarknet(yoloCfg, yoloWeights);
    if (net.empty()) {
        cerr << "⚠️ Error loading YOLO model!" << endl;
        return;
    }

    while (true) {
        // Récupérer la frame
        if (!updateFrame()) {
            cerr << "⚠️ Warning: No frame captured!" << endl;
            break;
        }

        cv::flip(frame, frame, 1);

        // Prétraitement de l'image pour YOLO
        Mat blob = blobFromImage(frame, 0.00392, Size(416, 416), Scalar(0, 0, 0), true, false);
        net.setInput(blob);

        // Passer l'image dans le réseau
        vector<Mat> outs;
        net.forward(outs, net.getUnconnectedOutLayersNames());

        // Parcourir les résultats
        for (size_t i = 0; i < outs.size(); i++) {
            Mat detectionMat = outs[i];
            for (int j = 0; j < detectionMat.rows; j++) {
                float* data = (float*)detectionMat.data + j * 85;  // 85 = 4 (box) + 1 (class) + 80 (scores)
                float confidence = data[4];
                if (confidence > 0.5) {  // Seulement les objets détectés avec une bonne précision
                    // Récupérer la classe avec la plus grande probabilité
                    int classId = -1;
                    float maxProb = -1;
                    for (int k = 5; k < 85; k++) {
                        if (data[k] > maxProb) {
                            maxProb = data[k];
                            classId = k - 5;
                        }
                    }

                    // Si une classe a été détectée
                    if (classId >= 0 && classId < MobileNetSSDService::CLASS_NAMES.size()) {
                        int xLeftBottom = static_cast<int>(data[0] * frame.cols);
                        int yLeftBottom = static_cast<int>(data[1] * frame.rows);
                        int xRightTop = static_cast<int>(data[2] * frame.cols);
                        int yRightTop = static_cast<int>(data[3] * frame.rows);

                        // Dessiner un rectangle autour de l'objet détecté
                        rectangle(frame, Point(xLeftBottom, yLeftBottom), Point(xRightTop, yRightTop), Scalar(0, 255, 0), 2);

                        // Ajouter le nom de l'objet détecté
                        string label = MobileNetSSDService::CLASS_NAMES[classId] + ": " + to_string(confidence);
                        putText(frame, label, Point(xLeftBottom, yLeftBottom - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
                    }
                }
            }
        }

        // Afficher la vidéo avec les rectangles autour des objets détectés
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
*/
