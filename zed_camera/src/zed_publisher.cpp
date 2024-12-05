#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"

#include "videocapture.hpp"
#include "ocv_display.hpp"

using namespace std::chrono_literals;

class ZEDPublisher : public rclcpp::Node {
public:
    ZEDPublisher() : Node("zed_publisher") {
        publisher_ = this->create_publisher<sensor_msgs::msg::Image>("camera/image_raw", 10);
        timer_ = this->create_wall_timer(10ms, std::bind(&ZEDPublisher::publish_message, this));

        // Initialize the camera
        sl_oc::video::VideoParams params;
        params.res = sl_oc::video::RESOLUTION::HD2K;
        params.fps = sl_oc::video::FPS::FPS_60;

        cap_0_ = std::make_unique<sl_oc::video::VideoCapture>(params);
        if (!cap_0_->initializeVideo()) {
            std::cerr << "Cannot open camera video capture" << std::endl;
            std::cerr << "See verbosity level for more details." << std::endl;
            return;
        }

        std::cout << "Connected to camera sn: " << cap_0_->getSerialNumber() << "[" << cap_0_->getDeviceName() << "]" << std::endl;
    }

private:
    void publish_message() {
        // Retrieve the last frame
        const sl_oc::video::Frame frame = cap_0_->getLastFrame();

        // Check if the frame is valid and print details
        if (frame.data != nullptr) {
            RCLCPP_INFO(this->get_logger(), "Received frame with dimensions: %d x %d", frame.width, frame.height);

            // Conversion from YUV 4:2:2 to BGR for visualization
            cv::Mat frameYUV = cv::Mat(frame.height, frame.width, CV_8UC2, frame.data);
            cv::Mat frameBGR;
            cv::cvtColor(frameYUV, frameBGR, cv::COLOR_YUV2BGR_YUYV);
           

            int half_width = frameBGR.cols / 2;
            cv::Mat frameHalf = frameBGR(cv::Rect(half_width, 0, frameBGR.cols - half_width, frameBGR.rows));


            // Convert OpenCV Mat to ROS Image message
            cv_bridge::CvImage cv_image;
            cv_image.header.stamp = this->get_clock()->now(); 
            cv_image.header.frame_id = "camera_frame";        
            cv_image.encoding = sensor_msgs::image_encodings::BGR8; // Encoding type
            cv_image.image = frameHalf;

            // Publish the image 
            publisher_->publish(*cv_image.toImageMsg());  
            
            // For debugging
            //cv::imshow("Stream RGB", frameBGR);  // Display the image in an OpenCV window
            //cv::waitKey(1);  // Wait for 1 ms to display the frame and process window events
        } else {
            RCLCPP_WARN(this->get_logger(), "No frame received!");
        }
    }

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::unique_ptr<sl_oc::video::VideoCapture> cap_0_;   
    sl_oc::video::VideoParams params_;                    
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ZEDPublisher>());
    rclcpp::shutdown();
    return 0;
}
