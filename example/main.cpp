#include <iostream>
#include <cstdio>

#include <librealsense2/rs.hpp>

#include <opencv2/opencv.hpp>

int main(int argc, char * argv[]) try
{
    rs2::pipeline pipe;
    rs2::config cfg;

    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Y8, 30);

    pipe.start();

    while(true)
    {
        rs2::frameset frames = pipe.wait_for_frames();

        rs2::frame depth_frame = frames.get_depth_frame();
        rs2::frame color_frame = frames.get_color_frame();

        cv::Mat depth(cv::Size(640, 480), CV_8UC1, (void*) depth_frame.get_data(), cv::Mat::AUTO_STEP);
        cv::Mat color(cv::Size(640, 480), CV_8UC3, (void*) color_frame.get_data(), cv::Mat::AUTO_STEP);

        cv::cvtColor(color, color, cv::COLOR_BGR2RGBA);

        cv::imshow("Display Image Depth Combined", depth);
        cv::imshow("Display Image RGB", color);


        if((char) cv::waitKey(25) == 27)
            break;
    }

    pipe.stop();

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
