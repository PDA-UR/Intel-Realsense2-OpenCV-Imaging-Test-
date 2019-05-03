#include <iostream>
#include <cstdio>

#include <librealsense2/rs.hpp>

#include <opencv2/opencv.hpp>

# define WIDTH_RGB 1920
# define HEIGHT_RGB 1080
# define WIDTH_DEPTH 640
# define HEIGHT_DEPTH 480

/*
 * READ FIRST!
 *
 * following two functions retrieved from https://github.com/IntelRealSense/librealsense/blob/master/wrappers/opencv/cv-helpers.hpp
 * it appears that custom rs2::configs interfere with reliable camera output
 * it is highly recommened to not use custom configs as of now as using the default works best and reliably
 * additionally, USB3.0 connection is required otherwise errors like "could not resolve" may appear frequently
 *
 * the functions frame_to_mat and depth_frame_to_meters can be optimized for performance in the future
 * */

cv::Mat frame_to_mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        cvtColor(r, r, COLOR_RGB2BGR);
        return r;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8)
    {
        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }

    throw std::runtime_error("Frame format is not supported yet!");
}


cv::Mat depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f)
{
    using namespace cv;
    using namespace rs2;

    Mat dm = frame_to_mat(f);
    dm.convertTo(dm, CV_64F);
    auto depth_scale = pipe.get_active_profile()
            .get_device()
            .first<depth_sensor>()
            .get_depth_scale();
    dm = dm * depth_scale;
    return dm;
}


int main(int argc, char * argv[]) try
{
    rs2::colorizer color_map;
    rs2::pipeline pipe;

    rs2::config cfg;


    cfg.enable_stream(RS2_STREAM_COLOR, WIDTH_RGB, HEIGHT_RGB, RS2_FORMAT_RGB8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, RS2_FORMAT_Z16, 30);

    pipe.start(cfg);

    while(true)
    {
        rs2::frameset frames = pipe.wait_for_frames();

        rs2::frame depth = frames.get_depth_frame().apply_filter(color_map);

        cv::Mat imagec = frame_to_mat(frames.get_color_frame());
        cv::Mat imaged = frame_to_mat(depth);

        cv::imshow("Display Image RGB", imagec);
        cv::imshow("Display Image Depth", imaged);

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
