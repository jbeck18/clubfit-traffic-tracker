#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "image_processor.hpp"

// Flag to specify if display should be used.
bool USE_DISPLAY = false;

int read() {
    std::cout << "Configuring camera settings..." << std::endl;

    // Load config file
    Config config = Config::get_instance();

    // Configure capture settings
    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, config.get_camera_resolution_width());
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.get_camera_resolution_height());

    // Check if capture is properly set up
    if (!cap.isOpened()) {
        std::cout << "!!! Failed to open file: " << "camera" << std::endl;
        return -1;
    }

    // Main loop
    cv::Mat frame;
    ImageProcessor processor;

    std::cout << "Establishing background... This can take a minute!" << std::endl;
    int frames_captured = 0;
    for(;;) {
        // Attempt to read in frame
        if(!cap.read(frame)) {
            continue;
        }

        if(frames_captured <= 50) {
            frames_captured++;
            if(frames_captured == 50) {
                std::cout << "Ready!\n" << std::endl;
            } else {
                processor.setup(frame);
                continue;
            }
        }

        // Process frame
        cv::Mat3b result = processor.process(frame);
        
        if(USE_DISPLAY) {
            // Display result
            cv::imshow("Object Detection", result);

            // Exit program on `ESC`
            // Show each frame for 30ms
            char key = cv::waitKey(30);
            if (key == 27) // ESC
            break;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {

    // Parse arguments
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            std::string arg = std::string(argv[i]);
            if(arg.compare("--display") == 0) {
                USE_DISPLAY = true;
            } else if(arg.compare("--no-display") == 0) {
                USE_DISPLAY = false;
            }
        }
    }

    // Read from camera and process
    read();

    return 0;
}