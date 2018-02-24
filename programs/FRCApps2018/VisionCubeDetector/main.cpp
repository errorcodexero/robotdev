#include "Cube.h"
#include <iostream>


int main(int argc, char **argv) {
    std::string params_file(argv[1]);
    paramsInput params;
    params.readFile(params_file);
    //auto debug = params.getValue("CAMERA_EXPOSURE", -9001);
    std::string video_file(params.getString("FILENAME", ""));
    bool playVideo = true;
    char key;
    while (1) {
      cv::VideoCapture capture(video_file);
      while(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
        //we can loop the video by re-opening the capture every time the video reaches its last frame

        if (!capture.isOpened()) {
          std::cout << "ERROR ACQUIRING VIDEO FEED\n";
          getchar();
          return -1;
        }
        if (playVideo) {
          //check if the video has reach its last frame.
          //we add '-1' because we are reading two frames from the video at a time.
          //if this is not included, we get a memory error!
          cv::Mat frame(cv::Scalar(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
          //read first frame
          capture >> frame;
          //std::cout << frame.cols;
          //std::cout << frame.rows << std::endl;
          /*if (frame.empty()) {
            capture.release();
            capture = cv::VideoCapture(video_file);    
          }*/
            
          Cube cube(frame, params);
          //std::cout << cube.getPosition() << std::endl;
          cube.getPosition(Cube::detectionMode::CONTOURS);
          //cv::imshow("Cube", cube.showFrame());
          //cv::imshow("Camera", frame);
          //cv::imshow("Video", frame);
        }
        key = cv::waitKey(15);
        if (key == 'p') playVideo = !playVideo;
        
      }
      capture.release();
    }
	


  
    return 0;
}