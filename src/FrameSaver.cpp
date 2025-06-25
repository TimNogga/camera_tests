#include "/home/timnogga/camera_tests/include/FrameSaver.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <sstream>
#include <iostream>

static std::string fn(const char* tag, int idx) {
    std::ostringstream ss;
    ss << tag << '_' << std::setw(4) << std::setfill('0') << idx << ".png";
    return ss.str();
}

// ────────────────────────────────────────────────────────────────────────────
//  COLOUR  (MJPG or RGB888) → BGR PNG
// ────────────────────────────────────────────────────────────────────────────
void saveColor(const std::shared_ptr<ob::ColorFrame>& f, int idx)
{
    cv::Mat img;

    if (f->format() == OB_FORMAT_MJPG) {
        cv::Mat raw(1, f->dataSize(), CV_8UC1,
                    const_cast<void*>(f->data()));
        img = cv::imdecode(raw, cv::IMREAD_COLOR);
    } else {                                     // RGB888 to BGR
        img = cv::Mat(f->height(), f->width(), CV_8UC3,
                      const_cast<void*>(f->data())).clone();
        cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
    }
    cv::imwrite(fn("rgb", idx), img,
                {cv::IMWRITE_PNG_COMPRESSION, 0});
    std::cout << "Saved " << fn("rgb", idx) << '\n';
}

// ────────────────────────────────────────────────────────────────────────────
//  DEPTH – RAW 16-bit values (for processing)
// ────────────────────────────────────────────────────────────────────────────
void saveDepthRaw(const std::shared_ptr<ob::DepthFrame>& f, int idx)
{
    cv::Mat depth(f->height(), f->width(), CV_16UC1,
                  const_cast<void*>(f->data()));
    cv::imwrite(fn("depth_raw", idx), depth,
                {cv::IMWRITE_PNG_COMPRESSION, 0});
    std::cout << "Saved " << fn("depth_raw", idx) << '\n';
}

// ────────────────────────────────────────────────────────────────────────────
//  DEPTH – 8-bit visually scaled + JET colour-map
// ────────────────────────────────────────────────────────────────────────────
void saveDepthVis(const std::shared_ptr<ob::DepthFrame>& f, int idx)
{
    cv::Mat raw16(f->height(), f->width(), CV_16UC1,
                  const_cast<void*>(f->data()));

    double maxVal;
    cv::minMaxLoc(raw16, nullptr, &maxVal);
    if (maxVal == 0) maxVal = 1;                   // avoid div-by-zero

    cv::Mat norm8;
    raw16.convertTo(norm8, CV_8U, 255.0 / maxVal); // 16-bit → 8-bit

    cv::applyColorMap(norm8, norm8, cv::COLORMAP_JET);
    cv::imwrite(fn("depth_vis", idx), norm8,
                {cv::IMWRITE_PNG_COMPRESSION, 0});
    std::cout << "Saved " << fn("depth_vis", idx) << '\n';
}
