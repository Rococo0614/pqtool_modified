#ifndef COLOR_BLOCK_DETECT_H
#define COLOR_BLOCK_DETECT_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <fstream>
#include <memory>
#include <vector>
#include "utils.hpp"

using namespace std;

struct ColorBlock {
    int id = -1;            // 色块的序号
    double centerX = 0.0;   // 色块中心的X坐标
    double centerY = 0.0;   // 色块中心的Y坐标
    double width = 0.0;     // 色块宽度
    double height = 0.0;    // 色块高度
    // 默认构造函数
    ColorBlock() : id(-1), centerX(0.0), centerY(0.0), width(0.0), height(0.0) {}
    // 带参数的构造函数
    ColorBlock(int id, double centerX, double centerY, double width, double height)
        : id(id), centerX(centerX), centerY(centerY), width(width), height(height) {}
};


vector<ColorBlock> ColorBlocksDetect(const string& filename, int lowThr, int highThr, int width, int height, string bayerid, bool wdr_only_le=false);

vector<ColorBlock> ColorBlocksDetectV2(const string& filename, int width, int height, utils::RAW_BAYER_ID bayerid,
                                       bool wdr_only_le=false, utils::BLC_ATTR_S *blc_attr=nullptr, utils::WBG_ATTR_S *wbg_attr=nullptr);

#endif
