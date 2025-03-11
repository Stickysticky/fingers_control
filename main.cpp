#include "CameraService.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <opencv2/dnn.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

/*
int main() {

   CameraService camera;
   camera.cameraProcess();
   return 0;
}*/


bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Chargement des fichiers du modèle MobileNet SSD
const string modelConfiguration = "assets/deploy.prototxt";
const string modelWeights = "assets/mobilenet_iter_73000.caffemodel";


// Liste des classes que MobileNet SSD peut détecter
vector<string> classNames = {"background", "aeroplane", "bicycle", "bird", "boat",
                             "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
                             "dog", "horse", "motorbike", "person", "pottedplant",
                             "sheep", "sofa", "train", "tvmonitor"};

int main() {
cout<<filesystem::current_path()<<endl;
if (!fileExists(modelConfiguration) || !fileExists(modelWeights)) {
    cerr << "❌ Erreur : Fichiers du modèle introuvables dans assets/ !" << endl;
    return -1;
}


    // Charger le modèle MobileNet SSD
    Net net = readNetFromCaffe(modelConfiguration, modelWeights);
    if (net.empty()) {
        cerr << "❌ Erreur : Impossible de charger le modèle MobileNet SSD !" << endl;
        return -1;
    }
    cout << "✅ Modèle chargé avec succès !" << endl;

    // Ouvrir la webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "❌ Erreur : Impossible d'ouvrir la webcam !" << endl;
        return -1;
    }

    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Prétraitement de l'image
        Mat blob = blobFromImage(frame, 0.007843, Size(300, 300), Scalar(127.5, 127.5, 127.5), true, false);

        // Passer l'image dans le réseau
        net.setInput(blob);
        Mat detections = net.forward();

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
                string label = classNames[classId] + ": " + to_string(confidence);
                putText(frame, label, Point(xLeftBottom, yLeftBottom - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            }
        }

        // Affichage de la vidéo avec détection en temps réel
        imshow("MobileNet SSD - Object Detection", frame);

        // Quitter si l'utilisateur appuie sur 'q' ou ferme la fenêtre
        char key = (char)waitKey(1);
        if (key == 'q' || key == 27) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
