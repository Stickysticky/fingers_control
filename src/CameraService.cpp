#include "CameraService.h"
#include <iostream>

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

VideoCapture CameraService::getCap (){
    return cap;
}

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
}
