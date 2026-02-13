#include "color_block_detect.hpp"
#include <limits>
#include <QDebug>
#include <QtCore>

#define Debug 0

static int readRawFile(unsigned char* praw, unsigned int size, const char* filename);
static unique_ptr<ushort[]> readRawIntData(const string& rawfilename, int width, int height);
static void demosaicAndResize(const cv::Mat& bayerImage, cv::Mat& outputImage, string flag);
static void getEdges(cv::Mat& image, cv::Mat& edges, int lowThr, int highThr);
static double intersectionArea(const cv::Rect& r1, const cv::Rect& r2) ;
static int findblockPosition(cv::Rect& square, int w, int h, int x, int y, vector<vector<int>>& position, vector<ColorBlock>& colorblock);
static cv::Rect computeMinEnclosingSquare(const vector<cv::Point>& contour);
static vector<ColorBlock> FindColorBlockPosition(const string& filename, const vector<vector<cv::Point>>& contours, int w, int h, vector<cv::Point>& approx,  cv::Mat& image);
static void detectSquares(const cv::Mat& image, const vector<cv::Point>& approx, vector<cv::Rect>& squares);
static void calculatePositions(vector<vector<int>>& position, vector<ColorBlock>& knownBlocks);
static bool checkDetectBlock(vector<vector<int>>& position, vector<ColorBlock>& knownBlocks);


/*
代码逻辑流程如下：
1.读取raw图；int readRawFile(unsigned char* praw, unsigned int size, const char* filename)
2.转为智能指针：unique_ptr<ushort[]> readRawIntData(const string& rawfilename, int width, int height)
3.去马赛克，并做简单的白平衡，最后再做resize：void demosaicAndResize(const Mat& bayerImage, Mat& outputImage, String flag, double rgain, double bgain)
4.Canny提取图像轮廓：void getContours(Mat& image, Mat& edges)
5.检测轮廓的数量是否符合24，同时计算出轮廓位置，区分24色卡的色块位置：void findcolorblockPosition(const string& filename, const vector<vector<Point>>& contours, vector<Vec4i> hierarchy, int w, int h, vector<Point>& approx,  Mat& image)
6.推算未检测出的轮廓的位置
7.结束：findcolorblockPosition(const string& filename, const vector<vector<Point>>& contours, vector<Vec4i> hierarchy, int w, int h, vector<Point>& approx,  Mat& image)
*/

// 读取RAW图
static int readRawFile(unsigned char* praw, unsigned int size, const char* filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        qWarning() << "File doesn't exist" << endl;
        return -1;
    }

    file.read(reinterpret_cast<char*>(praw), size);
    if (file.gcount() != static_cast<streamsize>(size)) {
        qWarning() << "Size of file is abnormal!" << endl;
    }

    file.close();
    return 0;
}

// 根据文件名宽高将文件存入数组中

static unique_ptr<ushort[]> readRawIntData(const string& rawfilename, int width, int height)
{

    unsigned int size = static_cast<unsigned int>(width) * static_cast<unsigned int>(height) * sizeof(short);
    unique_ptr<ushort[]> image(new ushort[width * height]);
    unique_ptr<unsigned char[]> praw_data(new unsigned char[size]);

    if (readRawFile(praw_data.get(), size, rawfilename.c_str()) != 0) {
        qWarning() << "The file is abnormal!" << endl;
        return nullptr;
    }

    int idx = 0;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            unsigned short b1 = praw_data[idx];
            unsigned short b2 = praw_data[idx + 1];
            idx += 2;
            image[r * width + c] = static_cast<ushort>(b2 << 8 | b1);
        }
    }
    return image;
}

static void demosaicAndResize(const cv::Mat& bayerImage, cv::Mat& outputImage, string flag)
{
    int rows = bayerImage.rows;
    int cols = bayerImage.cols;

    cv::Mat tempOutputImage(rows / 2, cols / 2, CV_16UC3);

    for (int y = 0; y < rows; y += 2) {
        for (int x = 0; x < cols; x += 2) {
            ushort R, G, B;
            if( flag == "RGGB"){
                R = bayerImage.at<ushort>(y, x);
                B = bayerImage.at<ushort>(y + 1, x + 1);
                ushort G1 = bayerImage.at<ushort>(y, x + 1);
                ushort G2 = bayerImage.at<ushort>(y + 1, x);
                G = static_cast<ushort>((G1 + G2) / 2);
            }else if (flag == "BGGR"){
                B = bayerImage.at<ushort>(y, x);
                R = bayerImage.at<ushort>(y + 1, x + 1);
                ushort G1 = bayerImage.at<ushort>(y, x + 1);
                ushort G2 = bayerImage.at<ushort>(y + 1, x);
                G = static_cast<ushort>((G1 + G2) / 2);
            }else if (flag == "GRBG"){
                R = bayerImage.at<ushort>(y, x + 1);
                B = bayerImage.at<ushort>(y + 1, x);
                ushort G1 = bayerImage.at<ushort>(y, x);
                ushort G2 = bayerImage.at<ushort>(y + 1, x + 1);
                G = static_cast<ushort>((G1 + G2) / 2);
            }else{
                B = bayerImage.at<ushort>(y, x + 1);
                R = bayerImage.at<ushort>(y + 1, x);
                ushort G1 = bayerImage.at<ushort>(y, x);
                ushort G2 = bayerImage.at<ushort>(y + 1, x + 1);
                G = static_cast<ushort>((G1 + G2) / 2);
            }
            tempOutputImage.at<cv::Vec3s>(y / 2, x / 2) = cv::Vec3s(B, G, R);
        }
    }

    resize(tempOutputImage, outputImage, cv::Size(cols, rows), 0, 0, cv::INTER_LINEAR);
}

static void getEdges(cv::Mat& image, cv::Mat& edges, int lowThr, int highThr)
{
    if (image.empty())
    {
        qWarning() << "Could not open or find the image" << endl;
        return ;
    }

    cv::Mat imageclone = image.clone();
    if (imageclone.depth() != CV_8U) {
        imageclone.convertTo(imageclone, CV_8U);
    }

    cv::Mat gray;
    cvtColor(imageclone, gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    Canny(gray, edges, lowThr, highThr, 3);

    cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    dilate(edges, edges, kernel);

    return;
}

// 计算两个矩形的重叠面积，用于极大值抑制，去除重复的方框边缘
static double intersectionArea(const cv::Rect& r1, const cv::Rect& r2)
{
   int x_overlap = max(0, min(r1.x + r1.width, r2.x + r2.width) - max(r1.x, r2.x));
   int y_overlap = max(0, min(r1.y + r1.height, r2.y + r2.height) - max(r1.y, r2.y));
   return x_overlap * y_overlap;
}
// 需要判断检测出来的轮廓属于哪一个色块，同时判断色块是否检出

static int findblockPosition(cv::Rect& square, int w, int h, int x, int y, vector<vector<int>>& position, vector<ColorBlock>& colorblock)
{
    if (square.width < w / 12 || square.height < h / 8) {
        return -1;
    }
    // 计算色块中心点
    int center_w = square.x + square.width / 2;
    int center_h = square.y + square.height / 2;

    // 计算列宽和行高
    const double col_w = static_cast<double>(w) / 6.0;
    const double row_h = static_cast<double>(h) / 4.0;

    // 初始化行和列索引
    int col = -1, row = -1;
    // 计算列位置
    for (int i = 0; i < 6; ++i) {
        if ((x + i * col_w) <= center_w && center_w < (x + (i + 1) * col_w)) {
            col = i;
            break;
        }
    }

    // 计算行位置
    for (int i = 0; i < 4; ++i) {
        if ((y + i * row_h) <= center_h && center_h < (y + (i + 1) * row_h)) {
            row = i;
            break;
        }
    }
    if (row < 0 || col < 0) {
        return -1;
    }

    int number = row * 6 + col + 1;
    colorblock[number - 1].id = number;
    colorblock[number - 1].centerX = center_w;
    colorblock[number - 1].centerY = center_h;
    colorblock[number - 1].width = square.width;
    colorblock[number - 1].height = square.height;

    // 更新位置数组
    position[row][col] = 1 ;

    return number;
}

static cv::Rect computeMinEnclosingSquare(const vector<cv::Point>& contour)
{
    // 计算轮廓的最小外接矩形
    cv::Rect rect = boundingRect(contour);

    // 获取矩形的宽度和高度
    int width = rect.width;
    int height = rect.height;

    // 计算需要扩展的尺寸
    int delta = abs(width - height);
    int deltaW = height > width ? delta : 0;
    int deltaH = width > height ? delta : 0;

    // 调整矩形以形成正方形
    int newX = rect.x - deltaW / 2;
    int newY = rect.y - deltaH / 2;
    int newWidth = max(width, height);
    int newHeight = max(width, height);

    // 创建新的正方形矩形
    return cv::Rect(newX, newY, newWidth, newHeight);
}

static vector<ColorBlock> FindColorBlockPosition(const string& filename, const vector<vector<cv::Point>>& contours, int w, int h, vector<cv::Point>& approx,  cv::Mat& image)
{
    double maxarea = 0;
    vector<cv::Point> approx_rectange;

    for(size_t i = 0; i < contours.size(); i++) {
        vector<cv::Point> contour = contours[i];
        cv::Rect arearect = boundingRect(contour);
        double area = arearect.width * arearect.height;
        // 对最大矩形框的约束条件，其应该落在画面1/4~3/4左右的地方
        if (!((arearect.x + arearect.width / 2) < w * 0.75) &&
                ((arearect.x + arearect.width / 2) > w * 0.25) &&
                ((arearect.y + arearect.height / 2) < h * 0.75) &&
                ((arearect.y + arearect.height / 2) > h * 0.25)) {
            qDebug() << "no center" << endl;
            continue;
        }
        // 色卡大矩形的面积应该有限制
        // 导致矩形面积找不到,有一些边缘没有封闭，导致无法计算
        if ((double)(w*h*0.9)> area && area > (double)(w*h*0.1)) {
            double epsilon = 0.02 * arcLength(contour, true);
            approxPolyDP(contour, approx_rectange, epsilon, true);
            if(approx_rectange.size() >=4 ) {
                // 存储矩形轮廓
                //保证最大轮廓
                if (area > maxarea) {
                    maxarea = area;
                    approx = contour;
                }
            }
        }else{
            continue;
        }
    }

    if (maxarea == 0) {
        qWarning() <<"error ,cant find color block!";
        qWarning() << QString::fromStdString(filename);
        return vector<ColorBlock>();
    }

    vector<vector<cv::Point>> contours_approx;
    contours_approx.push_back(approx);

     cv::Mat imagerec = image.clone();

    double epsilon = 0.02 * arcLength(approx, true);
    approxPolyDP(approx, approx_rectange, epsilon, true);


    // 获取大色卡的位置
    int width = 0, height = 0, x = 0, y = 0;
    // 解决大轮廓不闭合的问题
    if (approx_rectange.size() >= 4) {
        cv::Rect largestRect = boundingRect(approx_rectange);

        width = largestRect.width;
        height = largestRect.height;
        // correct height
        if    (width * 3 / 5 > height) {
            qWarning() << "width / height: " << (double)width / height << ", not meet the need!";
            height = width * 3 / 4;
            // update maxarea
            maxarea = height * width;
        }

        x = largestRect.x;
        y = largestRect.y;
        qDebug() << approx_rectange.size() << endl;
    } else {
        qWarning() <<"error, size is not 4 color block"<< approx_rectange.size() <<endl;
        qWarning() << QString::fromStdString(filename);
        return vector<ColorBlock>();
    }

    // 根据新边界的位置，查找新边界中的正方体的位置
    // 遍历所有的轮廓,正方体的查找原则：首先:面积>矩形面积/48 边长>=3
    vector<vector<int>> position(4, vector<int>(6, -1));
    vector<ColorBlock> colorblock(24);

    for (uint i = 0; i < contours.size(); i++)
    {
        if (contourArea(contours[i]) > (maxarea / 64) && contourArea(contours[i])<(maxarea / 24)){
            //contours_approx.push_back(contours[i]);
            // 直接在这里判断正方体的id吗？是的
            // 计算中心点
            // 计算最小外接正方体
            cv::Rect square = computeMinEnclosingSquare(contours[i]);
            cv::Point center(square.x + square.width / 2, square.y + square.height / 2);
            findblockPosition(square, width, height, x, y, position, colorblock);
         }
    }

    // 最后一步操作，对没有识别到的矩形框做简单的插值
    if (checkDetectBlock(position, colorblock) == false) {
        return vector<ColorBlock>();
    }

    calculatePositions(position, colorblock);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            if (position[i][j] != 1) { // 位置未知
                 return vector<ColorBlock>();
            }
        }
    }
    // 使用最小框保证检测效果
    int roi_h = h;
    int roi_w = w;
    for (int i = 0; i < 24; i++) {
        if (colorblock[i].width == 0 || colorblock[i].height == 0) {
            qWarning() << "color block: " << i << "widht: " << colorblock[i].width <<
                "height: " << colorblock[i].height << ", is incorrect!";
            return vector<ColorBlock>();
        }
        if (colorblock[i].width < roi_w && colorblock[i].width > 0) {
            roi_w = colorblock[i].width;
        }
        if (colorblock[i].height < roi_h && colorblock[i].height > 0) {
            roi_h = colorblock[i].height;
        }
    }

    // final roi_h and roi_w
    roi_h = (roi_h >> 2 << 1);
    roi_w = (roi_w >> 2 << 1);

    cv::Mat image_end = image.clone();
    for (int i = 0; i < 24; i++) {
        int x1 = colorblock[i].centerX - roi_w / 2;
        int y1 = colorblock[i].centerY - roi_h  / 2;
        int x2 = colorblock[i].centerX + roi_w / 2;
        int y2 = colorblock[i].centerY + roi_h  / 2;

        rectangle(image_end, cv::Point(x1,y1),cv::Point(x2,y2), cv::Scalar(0, 255, 0), 2, cv::LINE_8);
        putText(image_end, to_string(colorblock[i].id), cv::Point(colorblock[i].centerX, colorblock[i].centerY),
            cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);

    }
    string edgefilename = filename;
    edgefilename.replace(filename.size() - 4, 4, "-block.png");
    imwrite(edgefilename, image_end);

    return colorblock;
}

static void detectSquares(const cv::Mat& image, const vector<cv::Point>& approx, vector<cv::Rect>& squares)
{
    // 创建掩膜
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    fillConvexPoly(mask, approx, cv::Scalar(255));

    // 在色卡掩膜中查找轮廓
    cv::Mat maskedImage;
    image.copyTo(maskedImage, mask);
    imwrite("./result_image2/mask2.png", maskedImage); // 保存掩膜图像

    // 边缘检测
    cv::Mat mask_edges = cv::Mat(maskedImage.size(), CV_8UC1);
    getEdges(maskedImage, mask_edges, 0, 0);
    imwrite("./result_image2/mask2edges.png", mask_edges); // 保存边缘图像

    vector<vector<cv::Point>> contours;
    findContours(mask_edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
    qDebug() << "contours.size()" << contours.size() << endl;
    for (const auto& contour : contours) {
        // 判断轮廓
        vector<cv::Point> approx;
        double epsilon = 0.01 * arcLength(contour, true);
        approxPolyDP(contour, approx, epsilon, true);

        if (approx.size() == 4) {
            double area = contourArea(contour);
            cv::Rect rect = boundingRect(contour);
            double aspectRatio = (double)rect.width / rect.height;

             // 判断是否为正方形（可根据需要调整阈值）
            if (aspectRatio > 0.75 && aspectRatio < 1.2 && area > 100) {
                squares.push_back(rect);
            }
        }
    }
    // 绘制清除后的轮廓
    qDebug() << "square.size()" << squares.size() << endl;

}


static bool checkDetectBlock(vector<vector<int>>& position, vector<ColorBlock>& knownBlocks)
{
    int rows = position.size();
    int cols = position[0].size();

    bool check_h;
    bool check_v;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            check_h = check_v = false;
            if (position[i][j] != 1) {
                // this algo too strict
                //if (i > 0 && position[i-1][j] == 1) {
                //    check_h = true;
                //}
                //if (i < rows - 1 && position[i+1][j] == 1) {
                //    check_h = true;
                //}
                //if (j > 0 && position[i][j-1] == 1) {
                //    check_v = true;
                //}
                //if (j < cols - 1 && position[i][j+1] == 1) {
                //    check_v = true;
                //}
                for (int r = 0; r < rows; ++r) {
                    if (position[r][j] == 1) {
                        check_h = true;
                        break;
                    }
                }
                for (int c = 0; c < cols; ++c) {
                    if (position[i][c] == 1) {
                        check_v = true;
                        break;
                    }
                }
                if (check_h == false || check_v == false) {
                    qWarning() << "color block: " << i << ", can not find the surrounding bocks!";
                    return false;
                }
            }
        }
    }

    return true;
}

// 在findblockPosition如果发现检测出的框小于24，或者多检测出来，需要做的事情就是计算未知色块的中心坐标与坐标的宽高
static void calculatePositions( vector< vector<int>>& position,  vector<ColorBlock>& knownBlocks)
{
    int rows = position.size();
    int cols = position[0].size();
    qDebug() << "cols" << cols  << "rows" << rows << endl;


    // 遍历每个位置
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (position[i][j] != 1) { // 位置未知
                // 查找最近的已知方块（列方向）
                // 先找右边
                if (j < cols - 1) {
                    for (int k = j+1; k < cols; k++) {
                        if (position[i][k] == 1) {
                            knownBlocks[i * 6 + j].centerY = knownBlocks[i * 6 + k].centerY;
                            knownBlocks[i * 6 + j].height = knownBlocks[i * 6 + k].height;
                            break;
                        }
                        else {
                            continue;
                        }
                    }
                }
                // 右边没有元素或者右边没有找到，从左边开始找
                if (j >= cols - 1 || knownBlocks[i * 6 + j].height == 0) {
                    for (int k = j-1; k >= 0; k--) {
                        if (position[i][k] == 1) {
                            knownBlocks[i * 6 + j].centerY = knownBlocks[i * 6 + k].centerY;
                            knownBlocks[i * 6 + j].height = knownBlocks[i * 6 + k].height;
                            break;
                        }
                    }
                }
                //
                // 查找最近的已知方块（行方向）
                // 下面开始
                if (i < rows - 1) {
                    for (int k = i+1; k < rows; k++) {
                        if (position[k][j] == 1) {
                            knownBlocks[i * 6 + j].centerX = knownBlocks[k * 6 + j].centerX;
                            knownBlocks[i * 6 + j].width = knownBlocks[k * 6 + j].width;
                            break;
                        }
                    }
                }
                // 下边没有元素或者下边没有找到，从上边开始找
                if (i > rows - 1 || knownBlocks[i * 6 + j].width == 0) {
                    for (int k = i - 1; k >= 0; k--) {
                        if (position[k][j] == 1) {
                            knownBlocks[i * 6 + j].centerX = knownBlocks[k * 6 + j].centerX;
                            knownBlocks[i * 6 + j].width = knownBlocks[k * 6 + j].width;
                            break;
                        }
                    }
                }

                if (knownBlocks[i * 6 + j].width != 0 && knownBlocks[i * 6 + j].height != 0) {
                    position[i][j] = 1; // 标记为已知
                    knownBlocks[i*6+j].id = i * 6 + j + 1;

                }
            }
        }
    }

    return;
 }

/*
const char* filename：raw图文件名
lowThreshold = 10：小于这个值不被当作轮廓，默认为10
highThreshold = 40：大于这个值视作轮廓，默认为40
bayerid:Bayerid格式
输出24色块的中心坐标以及宽高, vector<ColorBlock>& knownBlocks 包括了最后24色块的中心坐标和宽高；根据需要选择是否需要
*/
vector<ColorBlock> ColorBlocksDetect(const string& filename, int lowThr, int highThr,int width, int height, string bayerid, bool wdr_only_le)
{
    unique_ptr<ushort[]> raw_data = readRawIntData(filename, width, height);
    int v = 1;

    if (wdr_only_le) {
        v = 2;
        width /= v;
    }

    cv::Mat image = cv::Mat(height, width, CV_16UC1);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image.at<ushort>(i, j) = raw_data[i * width * v + j];
        }
    }

    cv::Mat demosaic_image = cv::Mat(height, width, CV_16UC3);
    demosaicAndResize(image, demosaic_image, bayerid);

    cv::Mat demoasci = cv::Mat(height, width, CV_8UC3);
    demosaic_image.convertTo(demoasci, CV_8UC3, 255.0 / 4095.0);

    cv::Mat edges = cv::Mat(image.rows, image.cols, CV_8UC1);
    getEdges(demoasci, edges, lowThr, highThr);

    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::Mat imageContours = cv::Mat::zeros(edges.size(), CV_8UC1);

    vector<cv::Rect> boundingBoxes;
    findContours(edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

    for (uint i = 0; i < contours.size(); i++)
    {
        drawContours(imageContours, contours, i, cv::Scalar(255), 1, 8, hierarchy);
    }

    string edgefilename = filename;
    edgefilename.replace(filename.size() - 4, 4, "-edge.png");
    imwrite(edgefilename, imageContours);

    vector<cv::Point> approx;
    vector<ColorBlock> colorblock24 = FindColorBlockPosition(filename, contours, width, height, approx, demoasci);

    return colorblock24;
}

/*
 * detect algo v2: applyy the hori and verti dirction's y means to split the color block
 */
#define CARD_ROWS 4
#define CARD_COLS 6
#define DET_RAW_BITS 12

static int get_raw_ymeans(string filename, unsigned int width, unsigned int height,
                          vector<unsigned int>& h_means_vec, vector<unsigned int>& v_means_vec,
                          utils::RAW_BAYER_ID bayer, bool wdr_only_le, utils::BLC_ATTR_S *blc_attr,
                          utils::WBG_ATTR_S *wbg_attr, cv::Mat& demosaic_image)
{
    unsigned int raw_size = getRawSize(width, height,
                      utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
    unsigned char *praw_data = reinterpret_cast<unsigned char *>(malloc(raw_size));
    unsigned char *pdata;

    if (utils::readRawFile(praw_data, raw_size, filename.c_str()) < 0) {
        free(praw_data);
        return -1;
    }

    int v = 1;

    if (wdr_only_le) {
        int w = raw_size / height;
        v = 2;
        width /= v;
        pdata = reinterpret_cast<unsigned char *>(malloc(raw_size / v));
        for (unsigned int i = 0; i < height; i++) {
            memcpy(pdata + w / v * i, praw_data + w * i, w / v);
        }
    } else {
        pdata = praw_data;
    }

    unsigned char *unpacked_raw_data = convertAnyRaw2Raw16(pdata, width, height,
            utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);

    if (unpacked_raw_data == nullptr) {
        free(praw_data);
        return -1;
    }

    unsigned short *s = reinterpret_cast<unsigned short *>(unpacked_raw_data);
    unsigned short *dst_y = reinterpret_cast<unsigned short *>(malloc(width * height * sizeof(unsigned short)));

    if (!dst_y) {
        qWarning() << "can not allow the memory, size: " << width * height * sizeof(unsigned short) << endl;
        return -1;
    }

    unsigned short *p_dst_y = dst_y;

    long long unsigned int tempR, tempG, tempB;

    utils::COLOR_RGB e_color[2][2];
    if (bayer == utils::BAYER_ID_RG) {
        e_color[0][0] = utils::COLOR_R;
        e_color[0][1] = utils::COLOR_G;
        e_color[1][0] = utils::COLOR_G;
        e_color[1][1] = utils::COLOR_B;
    } else if (bayer == utils::BAYER_ID_GR) {
        e_color[0][0] = utils::COLOR_G;
        e_color[0][1] = utils::COLOR_R;
        e_color[1][0] = utils::COLOR_B;
        e_color[1][1] = utils::COLOR_G;
    } else if (bayer == utils::BAYER_ID_BG) {
        e_color[0][0] = utils::COLOR_B;
        e_color[0][1] = utils::COLOR_G;
        e_color[1][0] = utils::COLOR_G;
        e_color[1][1] = utils::COLOR_R;
    } else {
        e_color[0][0] = utils::COLOR_G;
        e_color[0][1] = utils::COLOR_B;
        e_color[1][0] = utils::COLOR_R;
        e_color[1][1] = utils::COLOR_G;
    }

    unsigned int pixel_max_val = (1 << DET_RAW_BITS) - 1;

    unsigned int blc_offset_r;
    unsigned int blc_offset_g;
    unsigned int blc_offset_b;

    if (blc_attr != nullptr) {
        blc_offset_r = blc_attr->offset_r;
        blc_offset_g = blc_attr->offset_g;
        blc_offset_b = blc_attr->offset_b;
    } else {
        blc_offset_r = 0;
        blc_offset_g = 0;
        blc_offset_b = 0;
    }

    unsigned int blc_gain_r = (pixel_max_val << 12) / (pixel_max_val - blc_offset_r);
    unsigned int blc_gain_g = (pixel_max_val << 12) / (pixel_max_val - blc_offset_g);
    unsigned int blc_gain_b = (pixel_max_val << 12) / (pixel_max_val - blc_offset_b);

    unsigned int wb_gain_r;
    unsigned int wb_gain_g;
    unsigned int wb_gain_b;
    if (wbg_attr != nullptr) {
        wb_gain_r = wbg_attr->wb_gain_r;
        wb_gain_g = wbg_attr->wb_gain_g;
        wb_gain_b = wbg_attr->wb_gain_b;
    } else {
        wb_gain_r =
        wb_gain_g =
        wb_gain_b = 1024;
    }

    for (unsigned int j = 0; j < height; j++) {
        for (unsigned int i = 0; i < width; i++) {
            utils::COLOR_RGB color_id = e_color[j%2][i%2];

            tempR = tempG = tempB = 0;

            if((i > 0) && (i < (width - 1)) && (j > 0) && (j < (height - 1))) {
                switch (color_id) {
                case utils::COLOR_G: {
                    tempG = *s;
                    if(e_color[j%2][(i+1)%2] == utils::COLOR_B) {
                        tempB = (*(s - 1) + *(s + 1)) / 2;
                        tempR = (*(s - width) + *(s + width)) / 2;
                    } else {
                        tempR = (*(s - 1) + *(s + 1)) / 2;
                        tempB = (*(s - width) + *(s + width)) / 2;
                    }
                    break;
                }
                case utils::COLOR_R: {
                    tempR = *s;
                    tempG = (*(s - 1) + *(s + 1) + *(s - width) + *(s + width)) / 4;
                    tempB = (*(s - width - 1) + *(s - width + 1) + *(s + width - 1) + *(s + width + 1)) / 4;
                    break;
                }
                case utils::COLOR_B: {
                    tempR = (*(s - width - 1) + *(s - width + 1) + *(s + width - 1) + *(s + width + 1)) / 4;
                    tempG = (*(s - 1) + *(s + 1) + *(s - width) + *(s + width)) / 4;
                    tempB = *s;
                    break;
                }
                default:
                    break;
                }
            }

            tempR = (tempR > blc_offset_r) ? (tempR - blc_offset_r) : 0;
            tempG = (tempG > blc_offset_g) ? (tempG - blc_offset_g) : 0;
            tempB = (tempB > blc_offset_b) ? (tempB - blc_offset_b) : 0;

            tempR = ((tempR * blc_gain_r * wb_gain_r) >> 22);
            tempG = ((tempG * blc_gain_g * wb_gain_g) >> 22);
            tempB = ((tempB * blc_gain_b * wb_gain_b) >> 22);

            tempR = MIN(tempR, pixel_max_val);
            tempG = MIN(tempG, pixel_max_val);
            tempB = MIN(tempB, pixel_max_val);

            *p_dst_y = 0.299 * tempR + 0.587 * tempG + 0.114 * tempB;
            demosaic_image.at<cv::Vec3w>(j, i)[0] = tempB;
            demosaic_image.at<cv::Vec3w>(j, i)[1] = tempG;
            demosaic_image.at<cv::Vec3w>(j, i)[2] = tempR;

            p_dst_y++;
            s++;
        }
    }

    unsigned int *h_means = (unsigned int *)malloc(width * sizeof(unsigned int));
    unsigned int *v_means = (unsigned int *)malloc(height * sizeof(unsigned int));

    for (unsigned int h = 0; h < height; h++) {
        unsigned int sum = 0;
        for (unsigned int w = 0; w < width; ++w) {
            sum += dst_y[h * width + w];
        }
        v_means[h] = sum / width;
    }

    for (unsigned int w = 0; w < width; ++w) {
        unsigned int sum = 0;
        for (unsigned int h = 0; h < height; h++) {
            sum += dst_y[h * width + w];
        }
        h_means[w] = sum / height;
    }


    h_means_vec.assign(h_means, h_means + width);
    v_means_vec.assign(v_means, v_means + height);


    free(unpacked_raw_data);
    free(dst_y);
    free(h_means);
    free(v_means);

    return 0;
}

static int get_24_blocks_v2(vector<unsigned int>& row_means, vector<unsigned int>& col_means, vector<ColorBlock>& color_block)
{
    vector<pair<int, unsigned int>> row_win_min_idx_val;
    vector<pair<int, unsigned int>> col_win_min_idx_val;

    unsigned int rows = col_means.size();
    unsigned int cols = row_means.size();

    // TODO: opt
    unsigned int row_win_size = rows / (2 * CARD_ROWS * 2);
    unsigned int col_win_size = rows / (2 * CARD_COLS * 2);

    size_t i;
    unsigned int min_val;
    int min_idx;
    for (i = 0; i < row_means.size(); i += row_win_size) {
        min_val = (1 << DET_RAW_BITS);
        min_idx = i;
        for (size_t j = i; j < i + row_win_size && j < row_means.size(); j++) {
            if (row_means[j] < min_val && row_means[j] > 0) {
                min_val = row_means[j];
                min_idx = j;
            }
        }
        row_win_min_idx_val.emplace_back(min_idx, min_val);
    }

    if (i - row_win_size > row_means.size() - row_win_size) {
        min_val = (1 << DET_RAW_BITS);
        min_idx = i - row_win_size;
        for (size_t j = i - row_win_size; j < row_means.size(); j++) {
            if (row_means[j] < min_val && row_means[j] > 0) {
                min_val = row_means[j];
                min_idx = j;
            }
        }
        row_win_min_idx_val.emplace_back(min_idx, min_val);
    }

    for (i = 0; i < col_means.size(); i += col_win_size) {
        min_val = (1 << DET_RAW_BITS);
        min_idx = i;
        for (size_t j = i; j < i + col_win_size && j < col_means.size(); j++) {
            if (col_means[j] < min_val && col_means[j] > 0) {
                min_val = col_means[j];
                min_idx = j;
            }
        }
        col_win_min_idx_val.emplace_back(min_idx, min_val);
    }

    if (i - col_win_size > col_means.size() - col_win_size) {
        min_val = (1 << DET_RAW_BITS);
        min_idx = i - col_win_size;
        for (size_t j = i - col_win_size; j < col_means.size(); j++) {
            if (col_means[j] < min_val && col_means[j] > 0) {
                min_val = col_means[j];
                min_idx = j;
            }
        }
        col_win_min_idx_val.emplace_back(min_idx, min_val);
    }

    vector<pair<int, unsigned int>> row_win_min_idx_val_m;
    vector<pair<int, unsigned int>> col_win_min_idx_val_m;

    int idx_0, idx_1;

    for (size_t i = 0; i < row_win_min_idx_val.size() - 1; i++) {
        idx_0 = row_win_min_idx_val[i].first;
        idx_1 = row_win_min_idx_val[i + 1].first;

        if (idx_1 - idx_0 <= row_win_size / 3) {
            if (row_win_min_idx_val[i].second < row_win_min_idx_val[i + 1].second) {
                row_win_min_idx_val_m.emplace_back(row_win_min_idx_val[i]);
            } else {
                row_win_min_idx_val_m.emplace_back(row_win_min_idx_val[i + 1]);
            }
            i++;
        } else {
            row_win_min_idx_val_m.emplace_back(row_win_min_idx_val[i]);
            if (i == row_win_min_idx_val.size() - 2) {
                row_win_min_idx_val_m.emplace_back(row_win_min_idx_val[i + 1]);
            }
        }
    }

    for (size_t i = 0; i < col_win_min_idx_val.size() - 1; i++) {
        idx_0 = col_win_min_idx_val[i].first;
        idx_1 = col_win_min_idx_val[i + 1].first;

        if (idx_1 - idx_0 <= col_win_size / 3) {
            if (col_win_min_idx_val[i].second < col_win_min_idx_val[i + 1].second) {
                col_win_min_idx_val_m.emplace_back(col_win_min_idx_val[i]);
            } else {
                col_win_min_idx_val_m.emplace_back(col_win_min_idx_val[i + 1]);
            }
            i++;
        } else {
            col_win_min_idx_val_m.emplace_back(col_win_min_idx_val[i]);
            if (i == col_win_min_idx_val.size() - 2) {
                col_win_min_idx_val_m.emplace_back(col_win_min_idx_val[i + 1]);
            }
        }
    }

    vector<int> y_min_idx;
    vector<int> x_min_idx;
    unsigned int cur_val;

    for (size_t i = 0; i < row_win_min_idx_val_m.size(); ++i) {
        if (i == 0 || i == row_win_min_idx_val_m.size() - 1) {
            continue;
        }
        cur_val = row_win_min_idx_val_m[i].second;
        if (cur_val < row_win_min_idx_val_m[i - 1].second &&
                cur_val <= row_win_min_idx_val_m[i + 1].second) {
            x_min_idx.emplace_back(row_win_min_idx_val_m[i].first);
        }
    }

    for (size_t i = 0; i < col_win_min_idx_val_m.size(); ++i) {
        if (i == 0 || i == col_win_min_idx_val_m.size() - 1) {
            continue;
        }
        cur_val = col_win_min_idx_val_m[i].second;
        if (cur_val < col_win_min_idx_val_m[i - 1].second &&
                cur_val <= col_win_min_idx_val_m[i + 1].second) {
            y_min_idx.emplace_back(col_win_min_idx_val_m[i].first);
        }
    }

    // filter
    if (x_min_idx.size() < CARD_COLS + 1) {
        qWarning() << "error: " << x_min_idx.size() << endl;
        return -1;
    } else if (x_min_idx.size() > CARD_COLS + 1) {
        vector<pair<int, int>> idx_val;

        for (int val : x_min_idx) {
            idx_val.emplace_back(row_means[val], val);
        }

        sort(idx_val.begin(), idx_val.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.first < b.first;
         });

        vector<int> filtered_x_min_idx;
        for (int i = 0; i < CARD_COLS + 1 && i < idx_val.size(); ++i) {
            filtered_x_min_idx.push_back(idx_val[i].second);
        }
        sort(filtered_x_min_idx.begin(), filtered_x_min_idx.end());

        x_min_idx = filtered_x_min_idx;
    }

    if (y_min_idx.size() < CARD_ROWS + 1) {
        qWarning() << "error: " << y_min_idx.size() << endl;
        return -1;
    } else if (y_min_idx.size() > CARD_ROWS + 1) {
        vector<pair<int, int>> idx_val;

        for (int val : y_min_idx) {
            idx_val.emplace_back(col_means[val], val);
        }

        sort(idx_val.begin(), idx_val.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.first < b.first;
         });

        vector<int> filtered_y_min_idx;
        for (int i = 0; i < CARD_ROWS + 1 && i < idx_val.size(); ++i) {
            filtered_y_min_idx.push_back(idx_val[i].second);
        }
        sort(filtered_y_min_idx.begin(), filtered_y_min_idx.end());

        y_min_idx = filtered_y_min_idx;
    }
    // get card bounding
    vector<pair<int, int>> row_bound;
    vector<pair<int, int>> col_bound;
    double bound_rate_thres = 0.9;

    for (int row = 0; row < CARD_ROWS; ++row) {
        int idx0 = y_min_idx[row];
        int idx1 = y_min_idx[row + 1];
        // TODO: opt
        int m_idx = (idx0 + idx1) / 2;

        unsigned int m_val = col_means[m_idx] * bound_rate_thres;

        while (idx0 < m_idx) {
            if (col_means[idx0] >= m_val) {
                break;
            }
            idx0++;
        }

        while (idx1 > m_idx) {
            if (col_means[idx1] >= m_val) {
                break;
            }
            idx1--;
        }

        if ((idx1 - idx0) > row_win_size) {
            row_bound.emplace_back(idx0, idx1);
        } else {
            row_bound.emplace_back(y_min_idx[row], y_min_idx[row + 1]);
        }
    }

    for (int col = 0; col < CARD_COLS; ++col) {
        int idx0 = x_min_idx[col];
        int idx1 = x_min_idx[col + 1];
        // TODO: opt
        int m_idx = (idx0 + idx1) / 2;

        unsigned int m_val = row_means[m_idx] * bound_rate_thres;

        while (idx0 < m_idx) {
            if (row_means[idx0] >= m_val) {
                break;
            }
            idx0++;
        }

        while (idx1 > m_idx) {
            if (row_means[idx1] >= m_val) {
                break;
            }
            idx1--;
        }

        if ((idx1 - idx0) > col_win_size) {
            col_bound.emplace_back(idx0, idx1);
        } else {
            col_bound.emplace_back(x_min_idx[col], x_min_idx[col + 1]);
        }
    }

    // calculate block
    const double w_h_ratio_thres = 0.08;

    ColorBlock cb;
    int x, y, width, height;
    for (int row = 0; row < CARD_ROWS; ++row) {
        y = row_bound[row].first;
        height = row_bound[row].second - row_bound[row].first;
        for (int col = 0; col < CARD_COLS; ++col) {
            int tmp_y = y;
            int offset = 0;
            x = col_bound[col].first;
            width = col_bound[col].second - col_bound[col].first;
            // corret
            if (height > width) {
                if ((height - width) > w_h_ratio_thres * width) {
                    offset = height - width * (1 + w_h_ratio_thres);
                    height = (1 + w_h_ratio_thres) * width;
                    y = y - offset / 2;
                }
            } else {
                if ((width - height) > w_h_ratio_thres * height) {
                    offset = width - height * (1 + w_h_ratio_thres);
                    width = (1 + w_h_ratio_thres) * height;
                    x = x - offset / 2;
                }
            }

            cb.id = row * CARD_COLS + col + 1;
            cb.centerX = x + width / 2;
            cb.centerY = tmp_y + height / 2;

            cb.width = width;
            cb.height = height;

            color_block.push_back(cb);
        }
    }
    return 0;
}

vector<ColorBlock> ColorBlocksDetectV2(const string& filename, int width, int height, utils::RAW_BAYER_ID bayerid,
        bool wdr_only_le, utils::BLC_ATTR_S *blc_attr, utils::WBG_ATTR_S *wbg_attr)
{
    int v = 1;

    if (wdr_only_le) {
        v = 2;
        width /= v;
    }

    cv::Mat demosaic_image = cv::Mat(height, width, CV_16UC3);
    cv::Mat demosaic = cv::Mat(height, width, CV_8UC3);

    vector<ColorBlock> color_block;
    vector<unsigned int> h_means;
    vector<unsigned int> v_means;

    if (get_raw_ymeans(filename, width * v, height, h_means, v_means, bayerid,
                                    wdr_only_le, blc_attr, wbg_attr, demosaic_image) != 0) {
        return color_block;
    }

    demosaic_image.convertTo(demosaic, CV_8UC3, 255.0 / 4095.0);

    if (get_24_blocks_v2(h_means, v_means, color_block) != 0) {
        color_block.clear();
        return color_block;
    }
    // check and adjust the color block
    double width_min = width;
    double height_min = height;

    for (auto cb : color_block) {
        if (cb.width < width_min) {
            width_min = cb.width;
        }
        if (cb.height < height_min) {
            height_min = cb.height;
        }
    }

    // draw
    int i_width_min = width_min;
    int i_height_min = height_min;

    i_width_min = (i_width_min >> 2 << 1);
    i_height_min = (i_height_min >> 2 << 1);

    cv::Mat image_end = demosaic.clone();

    for (auto cb : color_block) {
        int x1, y1, x2, y2;
        x1 = cb.centerX - i_width_min / 2;
        y1 = cb.centerY - i_height_min / 2;
        x2 = cb.centerX + i_width_min / 2;
        y2 = cb.centerY + i_height_min / 2;

        x1 = x1 < 0 ? 0 : x1;
        y1 = y1 < 0 ? 0 : y1;
        x2 = x2 > width ? width : x2;
        y2 = y2 > width ? width : y2;

        rectangle(image_end, cv::Point(x1,y1),cv::Point(x2,y2), cv::Scalar(0, 255, 0), 2, cv::LINE_8);
        putText(image_end, to_string(cb.id), cv::Point(cb.centerX, cb.centerY),
        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
    }

    string edgefilename = filename;
    edgefilename.replace(filename.size() - 4, 4, "-v2_block.png");
    imwrite(edgefilename, image_end);

    return color_block;
}
