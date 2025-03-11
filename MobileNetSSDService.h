#ifndef MOBILENETSSDSERVICE_H
#define MOBILENETSSDSERVICE_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

class MobileNetSSDService
{
    public:
        MobileNetSSDService();
        virtual ~MobileNetSSDService();
        Net getNet();
        void setInput(InputArray blob, const String& name = "", double scalefactor = 1.0, const Scalar& mean = Scalar());
        static const vector<string> CLASS_NAMES;

    private:
        Net net; //modèle MobileNet SSD
};

#endif // MOBILENETSSDSERVICE_H
