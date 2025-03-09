#include "CameraService.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    /*CameraService camera;  // Création du service caméra

    if (!camera.isOpened()) {
        std::cerr << "❌ Impossible d'initialiser la caméra !" << std::endl;
        return -1;
    }

    cv::Mat frame; //classe principale de opencv pour stocker et manipuler des images
    namedWindow("Video Player");
    while (true) {
        if (!camera.getFrame(frame)) {
            std::cerr << "⚠️ Erreur : Impossible de capturer une image" << std::endl;
            break;
        }

        cv::imshow("Flux caméra", frame);

        // Quitter avec la touche 'q'
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

   std::cin.get();

    return 0;

    */

    Mat myImage;//Declaring a matrix to load the frames//
   namedWindow("Video Player");//Declaring the video to show the video//
   VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
   if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
      cout << "No video stream detected" << endl;
      system("pause");
      return-1;
   }
   while (true){ //Taking an everlasting loop to show the video//
      cap >> myImage;
      if (myImage.empty()){ //Breaking the loop if no video frame is detected//
         break;
      }
      imshow("Video Player", myImage);//Showing the video//
      char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
      if (c == 27){ //If 'Esc' is entered break the loop//
         break;
      }
   }
   cap.release();//Releasing the buffer memory//
   return 0;
}
