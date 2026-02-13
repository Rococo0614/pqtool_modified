#ifndef _ICAMCALIB_H_
#define _ICAMCALIB_H_

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

//#define CAM_CALIB_EXPORTS
#ifdef CAM_CALIB_EXPORTS
#define CAM_CALIB_API __declspec(dllexport)
#else
#define CAM_CALIB_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

class IMonoRectify
{
public:
	virtual int monoCapture(int devID, cv::Size resolution = cv::Size(2560, 720)) = 0;
	virtual int monoCalib(cv::Mat* images, int imagesNum, const char* imageExt_c, const char* boardType_c, const cv::Size boardSize, double alpha, bool isFisheye = false, int flag = 0, bool verbose = false) = 0;
    virtual int monoUndistort(cv::Mat image, cv::Mat& undistortImage, bool drawLines = false) = 0;
    virtual int monoSaveCameraParams(const char* fileName_c, const char* binFilePath_c) = 0;
    virtual int monoReadCameraParams(const char* fileName_c) = 0;
};

extern "C" CAM_CALIB_API IMonoRectify * GetMonoRectify();

class IStereoRectify
{
public:
    virtual int stereoCapture(int devID, cv::Size resolution, const char* imageExt_c, const char* leftImageSavePath_c, const char* rightImageSavePath_c, const char* stereoImageSavePath_c) = 0;
    virtual int stereoCalib(cv::Mat* leftImages, cv::Mat* rightImages, int imagesNum, const char* imageExt_c, const char* boardType_c, cv::Size boardSize, float cornerStep, cv::Mat leftCameraMatrix, cv::Mat leftDistCoeffs, cv::Mat rightCameraMatrix, cv::Mat rightDistCoeffs, double alpha, int flag1, int flag2, bool isFisheye = false, bool verbose = false) = 0;
    virtual int stereoUndistort(cv::Mat stereoLeftImage, cv::Mat stereoRightImage, cv::Mat& stereoLeftRectiImage, cv::Mat& stereoRightRectiImage) = 0;
    virtual int stereoSaveCameraParams(const char* fileName_c, const char* leftBinFileDir_c, const char* rightBinFileDir_c) = 0;
    virtual int stereoReadCameraParams(const char* fileName_c) = 0;
};

extern "C" CAM_CALIB_API IStereoRectify * GetStereoRectify();
extern "C" CAM_CALIB_API int generate_meshInfo(const char* binFilePath_c, float homo_mtx[32]);

#ifdef __cplusplus
}
#endif

#endif
