# ZED Open-Capture ROS2
ROS2 Package that creates a ROS node that publishes Raw Images coming from the ZED Open Capture API to a topic. This was tested on a ROS Humble environment.

# Pre-requesites
1. Run:
    ```bash
    sudo apt update
    sudo apt install ros-humble-vision-opencv ros-humble-image-transport python3-opencv
    sudo apt install ros-humble-cv-bridge
    sudo apt install -y libhidapi-dev`
    ```
3. Follow the install steps (and pre-requesites) of the [ZED Open Capture API](https://github.com/stereolabs/zed-open-capture)

# Warning
This will not be supported or maintained, only getting updates according to any bug that may affect our work.
