#include "MobileNetSSDService.h"
#include <fstream>
#include <stdexcept>

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

const string MODEL_CONFIGURATION_PATH = "/home/olivier/Documents/dev/c++/fingers_control/assets/MobileNetSSD/deploy.prototxt";
const string MODEL_WEIGTHS_PATH = "/home/olivier/Documents/dev/c++/fingers_control/assets/MobileNetSSD/mobilenet_iter_73000.caffemodel";

const vector<string> MobileNetSSDService::CLASS_NAMES = {
    "background", "aeroplane", "bicycle", "bird", "boat",
    "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
    "dog", "horse", "motorbike", "person", "pottedplant",
    "sheep", "sofa", "train", "tvmonitor"
};


MobileNetSSDService::MobileNetSSDService()
{
    if (!fileExists(MODEL_CONFIGURATION_PATH) || !fileExists(MODEL_WEIGTHS_PATH)) {
        throw runtime_error("❌ Erreur : Fichiers du modèle introuvables dans assets/ !");
    }

    net = readNetFromCaffe(MODEL_CONFIGURATION_PATH, MODEL_WEIGTHS_PATH);
    if (net.empty()) {
        throw runtime_error("❌ Erreur : Impossible de charger le modèle MobileNet SSD !");
    }
    cout << "✅ Modèle chargé avec succès !" << endl;
}

Net MobileNetSSDService::getNet(){
    return net;
}

MobileNetSSDService::~MobileNetSSDService()
{}

void MobileNetSSDService::setInput(InputArray blob, const String& name,double scalefactor, const Scalar& mean){
    net.setInput(blob, name,scalefactor,mean);
}
