#pragma once
#include <memory>
#include "libobsensor/ObSensor.hpp"

/*  Save frames as PNG files using a running index in the filename.
 *
 *  ─ rgb_0000.png      : 24-bit BGR (ready to view)
 *  ─ depth_raw_0000.png: original 16-bit depth values (millimetres)
 *  ─ depth_vis_0000.png: 8-bit JET colour-mapped preview
 */
void saveColor      (const std::shared_ptr<ob::ColorFrame>& colour, int idx);
void saveDepthRaw   (const std::shared_ptr<ob::DepthFrame>& depth,  int idx);
void saveDepthVis   (const std::shared_ptr<ob::DepthFrame>& depth,  int idx);
