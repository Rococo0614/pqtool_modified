#ifndef _OTP_PDAF_CALI_H_
#define _OTP_PDAF_CALI_H_

#include <sstream>
#include <vector>
#define PDAF_CALIBRATION_DLL_EXPORTS

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef PDAF_CALIBRATION_DLL_EXPORTS
#define ISP_CALIBRATION_API __declspec(dllexport)
#else
#define ISP_CALIBRATION_API __declspec(dllimport)
#endif

// BLC API
#define BLC_BAYER_MAX_VALUE	4096

static enum CHIP_ARCH {
	CV183X = 0,
	CV182X,
	CHIP_MAX,
} CHIP;

typedef int(*FNUC_BLC_CALC)(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int* blc_rr_offset, int* blc_gr_offset, int* blc_gb_offset, int* blc_bb_offset, float* blc_rr_gain, float* blc_gr_gain, float* blc_gb_gain, float* blc_bb_gain,
    int IC_TYPE
    );
typedef int(*FNUC_LOCALBLC_CALC)(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
		int* blc_rr_offset, int* blc_gr_offset, int* blc_gb_offset, int* blc_bb_offset, float* blc_rr_gain, float* blc_gr_gain, float* blc_gb_gain, float* blc_bb_gain,
		int block_size, int IC_TYPE
		);
ISP_CALIBRATION_API int cali_blc_calc(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int* blc_rr_offset, int* blc_gr_offset, int* blc_gb_offset, int* blc_bb_offset, float* blc_rr_gain, float* blc_gr_gain, float* blc_gb_gain, float* blc_bb_gain,
    int IC_TYPE
    );

ISP_CALIBRATION_API int cali_blc_calc_by_mean(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int* blc_rr_offset, int* blc_gr_offset, int* blc_gb_offset, int* blc_bb_offset, float* blc_rr_gain, float* blc_gr_gain, float* blc_gb_gain, float* blc_bb_gain,
    int IC_TYPE
    );
ISP_CALIBRATION_API int cali_blc_calc_by_median(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int* blc_rr_offset, int* blc_gr_offset, int* blc_gb_offset, int* blc_bb_offset, float* blc_rr_gain, float* blc_gr_gain, float* blc_gb_gain, float* blc_bb_gain,
    int IC_TYPE
    );
ISP_CALIBRATION_API int cali_blc_calc_by_localblc(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
		int* blc_rr_offset, int* blc_gr_offset, int* blc_gb_offset, int* blc_bb_offset, float* blc_rr_gain, float* blc_gr_gain, float* blc_gb_gain, float* blc_bb_gain,
		int block_size, int IC_TYPE
	);
typedef int(*FNUC_BLC_GET_HISTOGRAM)(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int* blc_rr_hist, int* blc_gr_hist, int* blc_gb_hist, int* blc_bb_hist,
    int IC_TYPE
    );
ISP_CALIBRATION_API int cali_blc_get_histogram(int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int* blc_rr_hist, int* blc_gr_hist, int* blc_gb_hist, int* blc_bb_hist,
    int IC_TYPE
    );
typedef int(*FNUC_BLC_VERIFY)(int **RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset, float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain, int **blc_raw_image);
ISP_CALIBRATION_API int cali_blc_verify(int **RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset, float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain, int **blc_raw_image);

// LSC API
	typedef int(*FNUC_LSC_CALC)(int **RAW, int IMAGE_W, int IMAGE_H,
		int ORDER, int NumKnotX, int NumKnotY, int calib_flag, int fisheye_flag,
		int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
		int *center_x, int *center_y, int *radius, int *norm,
		int *lsc_r_gain, int *lsc_g_gain, int *lsc_b_gain, int *lsc_radius_gain);
	ISP_CALIBRATION_API int cali_lsc_calc(int **RAW, int IMAGE_W, int IMAGE_H,
		int ORDER, int NumKnotX, int NumKnotY, int calib_flag, int fisheye_flag,
		int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
		int *center_x, int *center_y, int *radius, int *norm,
		int *lsc_r_gain, int *lsc_g_gain, int *lsc_b_gain, int *lsc_radius_gain);

	typedef int(*FNUC_LSC_VERIFY)(
		int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
		int NumKnotX, int NumKnotY, int fisheye_flag,
		int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
		float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
		int center_x, int center_y, int radius, int norm,
		int *lsc_r_gain, int *lsc_g_gain, int *lsc_b_gain,
		int *lsc_radius_gain, int **lsc_raw_image, int **rlsc_raw_image);
	ISP_CALIBRATION_API int cali_lsc_verify(
		int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
		int NumKnotX, int NumKnotY, int fisheye_flag,
		int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
		float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
		int center_x, int center_y, int radius, int norm,
		int *lsc_r_gain, int *lsc_g_gain, int *lsc_b_gain,
		int *lsc_radius_gain, int **lsc_raw_image, int **rlsc_raw_image);
	ISP_CALIBRATION_API int cali_lsc_verify_v2(
		int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
		int NumKnotX, int NumKnotY, int fisheye_flag,
		int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
		float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
		int center_x, int center_y, int radius, int norm,
		int *lsc_r_gain, int *lsc_g_gain, int *lsc_b_gain,
		int *lsc_radius_gain, int **lsc_raw_image, int **rlsc_raw_image);

// AWB API
typedef int(*FNUC_AWB_CALC)(
    int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int NumKnotX, int NumKnotY, int* lsc_r_gain, int* lsc_g_gain, int* lsc_b_gain,
    int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
    float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
    unsigned short* wb_r_gain, unsigned short* wb_g_gain, unsigned short* wb_b_gain);
ISP_CALIBRATION_API int cali_awb_calc(
    int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int NumKnotX, int NumKnotY, int* lsc_r_gain, int* lsc_g_gain, int* lsc_b_gain,
    int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
    float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
    unsigned short* wb_r_gain, unsigned short* wb_g_gain, unsigned short* wb_b_gain);

typedef int(*FNUC_AWB_VERIFY)(
    int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int NumKnotX, int NumKnotY, int* lsc_r_gain, int* lsc_g_gain, int* lsc_b_gain,
    int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
    float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
    unsigned short r_gain_ratio, unsigned short g_gain_ratio, unsigned short b_gain_ratio,
    int** wb_raw_image);
ISP_CALIBRATION_API int cali_awb_verify(
    int** RAW, int IMAGE_W, int IMAGE_H, int ORDER,
    int NumKnotX, int NumKnotY, int* lsc_r_gain, int* lsc_g_gain, int* lsc_b_gain,
    int blc_rr_offset, int blc_gr_offset, int blc_gb_offset, int blc_bb_offset,
    float blc_rr_gain, float blc_gr_gain, float blc_gb_gain, float blc_bb_gain,
    unsigned short r_gain_ratio, unsigned short g_gain_ratio, unsigned short b_gain_ratio,
    int** wb_raw_image);

typedef int(*FNUC_AWB_BUILD_CURVE)(
	float GoldenR, float GoldenB,float *pRgain, float *pBgain,
	float *pTempK, unsigned char *pWgt, unsigned char Amount, int *outParaK);
ISP_CALIBRATION_API int cali_awb_BuildCurve(float GoldenR, float GoldenB,float *pRgain, float *pBgain,
	float *pTempK, unsigned char *pWgt, unsigned char Amount, int *outParaK);

typedef int(*FNUC_AWB_GET_CURVE)(float *curveY,float *rgain, float *bgain);
ISP_CALIBRATION_API int cali_awb_getCurve(float *curveY,float *rgain, float *bgain);

typedef int(*FNUC_AWB_SET_ROI)(int x, int y, int w, int h);
ISP_CALIBRATION_API int cali_awb_setROI(int x, int y, int w, int h);

    // Noise Profile
typedef int (*FNUC_ISP_NP_FLOAT_CALC)(
    vector< vector<int*> > raw_roi_list,
    int roi_width, int roi_height,
    int num_of_roi,
    int num_of_images,
    int bayer_id,
    unsigned int blc_offset_b, unsigned int blc_offset_gb,
    unsigned int blc_offset_gr, unsigned int blc_offset_r,
    float& CaliSlopeB, float& CaliInterceptB,
    float& CaliSlopeGb, float& CaliInterceptGb,
    float& CaliSlopeGr, float& CaliInterceptGr,
    float& CaliSlopeR, float& CaliInterceptR
    );
ISP_CALIBRATION_API int cali_isp_np_float_calc(
    vector< vector<int*> > &raw_roi_list,
    int roi_width, int roi_height,
    int num_of_roi,
    int num_of_images,
    int bayer_id,
    unsigned int blc_offset_b, unsigned int blc_offset_gb,
    unsigned int blc_offset_gr, unsigned int blc_offset_r,
    float& CaliSlopeB, float& CaliInterceptB,
    float& CaliSlopeGb, float& CaliInterceptGb,
    float& CaliSlopeGr, float& CaliInterceptGr,
    float& CaliSlopeR, float& CaliInterceptR
);
typedef int (*FNUC_ISP_NP_FLOAT_CALC_AINR)(
	vector< vector<int*> > raw_roi_list,
	int roi_width, int roi_height,
	int num_of_roi,
	int num_of_images,
	int bayer_id,
	unsigned int blc_offset_b, unsigned int blc_offset_gb,
	unsigned int blc_offset_gr, unsigned int blc_offset_r,
	float& CaliSlopeMax, float& CaliInterceptMax
	);
ISP_CALIBRATION_API int cali_isp_np_float_calc_ainr(
	vector< vector<int*> >& raw_roi_list,
	int roi_width, int roi_height,
	int num_of_roi,
	int num_of_images,
	int bayer_id,
	unsigned int blc_offset_b, unsigned int blc_offset_gb,
	unsigned int blc_offset_gr, unsigned int blc_offset_r,
	float& CaliSlopeMax, float& CaliInterceptMax
);


typedef int (*FNUC_ISP_NP_FIX_CALC)(
    vector< vector<int*> > raw_roi_list,
    int roi_width, int roi_height,
    int num_of_roi,
    int num_of_images,
    int bayer_id,
    string str_nbit,
    long long int* CaliSlopeB, long long int* CaliInterceptB,
    long long int* CaliSlopeGb, long long int* CaliInterceptGb,
    long long int* CaliSlopeGr, long long int* CaliInterceptGr,
    long long int* CaliSlopeR, long long int* CaliInterceptR
    );
ISP_CALIBRATION_API int cali_isp_np_fix_calc(
    vector< vector<int*> > raw_roi_list,
    int roi_width, int roi_height,
    int num_of_roi,
    int num_of_images,
    int bayer_id,
    string str_nbit,
    long long int* CaliSlopeB, long long int* CaliInterceptB,
    long long int* CaliSlopeGb, long long int* CaliInterceptGb,
    long long int* CaliSlopeGr, long long int* CaliInterceptGr,
    long long int* CaliSlopeR, long long int* CaliInterceptR
);


typedef int (*FNUC_ISP_NP_LR_CALC)(float* input_x, float* desired_y, int ndata, float& slope, float& intercept);
ISP_CALIBRATION_API int cali_isp_np_lr_calc(float* input_x, float* desired_y, int ndata, float& slope, float& intercept);


typedef int (*FNUC_ISP_NP_FIX_TO_FIX_CALC)(
    int slope,
    int intercept,
    int np_luma_thr0,
    int* np_slope,
    int* np_low_offset,
    const string str_nbit_slope_in,
    const string str_nbit_slope,
    const string str_nbit_low_offset
    );
ISP_CALIBRATION_API int cali_isp_np_fix_to_fix_calc(
    int slope,
    int intercept,
    int np_luma_thr0,
    int* np_slope,
    int* np_low_offset,
    const string str_nbit_slope_in,
    const string str_nbit_slope,
    const string str_nbit_low_offset
);

typedef int (*FNUC_ISP_NP_FLOAT_TO_FIX_CALC)(
    float slope,
    float intercept,
    int np_luma_thr0,
    int* np_slope,
    int* np_low_offset,
    const char sign_slope,
    const int nbit_slope1,
    const int nbit_slope2,
    const char sign_low_offset,
    const int nbit_low_offset1,
    const int nbit_low_offset2);
ISP_CALIBRATION_API int cali_isp_np_float_to_fix_calc(
    float slope,
    float intercept,
    int np_luma_thr0,
    int* np_slope,
    int* np_low_offset,
    const char sign_slope,
    const int nbit_slope1,
    const int nbit_slope2,
    const char sign_low_offset,
    const int nbit_low_offset1,
    const int nbit_low_offset2);

typedef int (*FNUC_ISP_NP_FLOAT_TO_FIX_CALC_V2)(
    float slope,
    float intercept,
    int np_luma_thr0,
    int* np_slope,
    int* np_low_offset,
    const string str_nbit_slope,
    const string str_nbit_low_offset);
ISP_CALIBRATION_API int cali_isp_np_float_to_fix_calc_v2(
    float slope,
    float intercept,
    int np_luma_thr0,
    int* np_slope,
    int* np_low_offset,
    const string str_nbit_slope,
    const string str_nbit_low_offset);

typedef int (*FNUC_ISP_ROI_MEAN_STD_CALC)(vector< vector<int*> > raw_roi_list, int roi_width, int roi_height, int num_of_roi, int num_of_channels, int num_of_images, float** roi_mu, float** roi_std);
ISP_CALIBRATION_API int cali_isp_roi_mean_std_calc(
    vector< vector<int*> > raw_roi_list,
    int roi_width,
    int roi_height,
    int num_of_roi,
    int num_of_channels,
    int num_of_images,
    float** roi_mu,
    float** roi_std);

typedef int (*FNUC_ISP_ROI_MEAN_VAR_CALC)(vector< vector<int*> > raw_roi_list, int roi_width, int roi_height, int num_of_roi, int num_of_channels, int num_of_images, float** roi_mu, float** roi_std);
ISP_CALIBRATION_API int cali_isp_roi_mean_var_calc(
	vector< vector<int*> > raw_roi_list,
	int roi_width,
	int roi_height,
	int num_of_roi,
	int num_of_channels,
	int num_of_images,
	float** roi_mu,
	float** roi_std);

typedef int (*FNUC_ISP_YNR_NP_FIX_CALC)(
    int* ns_luma_th,
    int* ns_offset_th,
    int* ns_slope,
    const int num_of_slopes,
    const string str_sign,
    const string str_nbit_slope);
ISP_CALIBRATION_API int cali_isp_ynr_np_fix_calc(
    int* ns_luma_th,
    int* ns_offset_th,
    int* ns_slope,
    const int num_of_slopes,
    const string str_sign,
    const string str_nbit_slope
);


typedef int (*FNUC_ISP_DIFF_IMG_CALC)(int* img_raw, int img_width, int img_height, int swin_width, int swin_height, string str_nbit, int* img_diff);
ISP_CALIBRATION_API int cali_isp_diff_img_calc(int* img_raw, int img_width, int img_height, int swin_width, int swin_height, string str_nbit, int* img_diff);


typedef int (*FNUC_ISP_STD_IMG_CALC)(int* img_raw, int img_width, int img_height, int swin_width, int swin_height, string str_nbit, int* img_std);
ISP_CALIBRATION_API int cali_isp_std_img_calc(int* img_raw, int img_width, int img_height, int swin_width, int swin_height, string str_nbit, int* img_std);


    // CCM cali

#define MATRIX_NUM       (7)
#define CCM_MATRIX_SIZE  (9)
#define PATCH_NUM        (24)
#define DBG_CALI_CCM

typedef enum LSC_TYPE {
    TABLE,
    RADIUS,
    TABLE_RADIUS,
    RADIUS_TABLE,
} LSC_TYPE;

typedef struct Point_S
{
    int x, y;
} ISP_Point_S;

typedef struct ROI_S
{
    int roi_width;
    int roi_height;
    vector<ISP_Point_S> Points;
} ISP_ROI_S;

typedef struct BLC_S {
    int blc_bb_offset;
    int blc_gb_offset;
    int blc_gr_offset;
    int blc_rr_offset;
    float blc_bb_gain;
    float blc_gb_gain;
    float blc_gr_gain;
    float blc_rr_gain;
} ISP_BLC_S;

typedef struct LSC_S {
    LSC_TYPE lsc_type;
    int NumKnotX;
    int NumKnotY;
    int lsc_r_gain[1369];
    int lsc_g_gain[1369];
    int lsc_b_gain[1369];
    int lsc_radius_gain[128];
    int center_x;
    int center_y;
} ISP_LSC_S;

typedef struct WB_S {
    unsigned short wb_rgain;
    unsigned short wb_ggain;
    unsigned short wb_bgain;
} ISP_WB_S;

typedef struct CCM_S {
    short int ccm[9];
} ISP_CCM_S;

typedef struct GAMMA_S {
    unsigned int gamma_lut256[256];
    unsigned int gamma_lut[4096];
} ISP_GAMMA_S;

typedef struct PARAM_S {
    float sat_strength;
    float patch_weight_list[PATCH_NUM];
} ISP_PARAM_S;

typedef struct CCM_PIPE_S {
    ISP_BLC_S blc;
    ISP_LSC_S lsc;
    ISP_WB_S wb;
    ISP_CCM_S ccm;
    ISP_GAMMA_S gamma;
    ISP_PARAM_S param;
} ISP_CCM_PIPE_S;



/********************************************************************************************************************
                                                Calibration - CCM
********************************************************************************************************************/
// Calibration Pipeline - CCM
typedef int (*FNUC_ISP_CCM_PIPE_CALC)(
    int* img_src,
    int* img_dst,
    int* img_dst_gamma_off,
    int img_width, int img_height,
    int bayer_id,
    ISP_BLC_S& blc,
    ISP_LSC_S& lsc,
    ISP_WB_S& wb,
    ISP_GAMMA_S& gamma,
    ISP_PARAM_S& param,
    ISP_ROI_S& cali_ccm_roi,
    float CCMM_Result[CCM_MATRIX_SIZE],
    int iCCM_Result[CCM_MATRIX_SIZE]
    );
ISP_CALIBRATION_API int cali_isp_ccm_pipe_calc(
    int* img_src,
    int* img_dst,
    int* img_dst_gamma_off,
    int img_width, int img_height,
    int bayer_id,
    ISP_BLC_S& blc,
    ISP_LSC_S& lsc,
    ISP_WB_S& wb,
    ISP_GAMMA_S& gamma,
    ISP_PARAM_S& param,
    ISP_ROI_S& cali_ccm_roi,
    float CCM_Result[CCM_MATRIX_SIZE],
    int iCCM_Result[CCM_MATRIX_SIZE]
);

// Calibration Pipeline - CCM V2
typedef int (*FNUC_ISP_CCM_PIPE_CALC_V2)(
    int* img_src,
    double *ref_lab,
    int* img_dst,
    int* img_dst_gamma_off,
    int img_width, int img_height,
    int bayer_id,
    ISP_BLC_S& blc,
    ISP_LSC_S& lsc,
    ISP_WB_S& wb,
    ISP_GAMMA_S& gamma,
    ISP_PARAM_S& param,
    ISP_ROI_S& cali_ccm_roi,
    float CCM_Result[CCM_MATRIX_SIZE],
    int iCCM_Result[CCM_MATRIX_SIZE]
);
ISP_CALIBRATION_API int cali_isp_ccm_pipe_calc_v2(
    int* img_src,
    double *ref_lab,
    int* img_dst,
    int* img_dst_gamma_off,
    int img_width, int img_height,
    int bayer_id,
    ISP_BLC_S& blc,
    ISP_LSC_S& lsc,
    ISP_WB_S& wb,
    ISP_GAMMA_S& gamma,
    ISP_PARAM_S& param,
    ISP_ROI_S& cali_ccm_roi,
    float CCM_Result[CCM_MATRIX_SIZE],
    int iCCM_Result[CCM_MATRIX_SIZE],
    int point,
    bool bYuv_calibrate
);


/********************************************************************************************************************
                                                Calibration - CLUT
********************************************************************************************************************/
#define CLUT_RGB_MAX_COLOR_CELL			(256)

struct coordinate {
	double	x;
	double	y;
	double	z;
	bool	valid;
	int		create_idx;
};

struct ROI_INFO {
	int		str_x;
	int		str_y;
	int		win_width;
	int		win_height;
};

struct point_interpolation_coef {
	int		gray_point_std_div_distance;	// gray pair select condition
	int		init_max_int_distance;			// interpolation max distance
	float	min_to_max_ratio;				// interpolation min distance
	int		max_add_points;
};

// Calibration Pipeline - CLUT (183x HSV)
typedef int (*FNUC_ISP_CLUT_PIPE_CALC)(coordinate* src_rgb, coordinate* dst_rgb, int* hsv_lut_final_h, int* hsv_lut_final_s, int* hsv_lut_final_v);
ISP_CALIBRATION_API int cali_clut_calc(coordinate* src_rgb, coordinate* dst_rgb, int* hsv_lut_final_h, int* hsv_lut_final_s, int* hsv_lut_final_v);

// Calibration Pipeline - CLUT (182x RGB)
typedef int (*FNUC_ISP_CLUT_PIPE_CALC_RGB)(coordinate *src_rgb, coordinate *dst_rgb, int rgb_color_count, int *rgb_lut_final_r, int *rgb_lut_final_g, int *rgb_lut_final_b);
ISP_CALIBRATION_API int cali_clut_calc_rgb(coordinate *src_rgb, coordinate *dst_rgb, int rgb_color_count, int *rgb_lut_final_r, int *rgb_lut_final_g, int *rgb_lut_final_b);

// Calibration Pipeline - ROI Average Color
typedef int (*FNUC_ISP_CLUT_ROI_CALC)(unsigned char* img, int img_width, int img_height, coordinate* roi_avg_rgb, ROI_INFO* roi_list, int roi_cnt);
ISP_CALIBRATION_API int cali_roi_rgb_cal(unsigned char* img, int img_width, int img_height, coordinate* roi_avg_rgb, ROI_INFO* roi_list, int roi_cnt);

// Calibration Pipeline - ROI Color Adjustment
typedef int (*FNUC_ISP_CLUT_ROI_ADJUST)(coordinate* src_rgb, coordinate* dst_rgb, coordinate* proj_rgb, int rgb_color_count, int mode);
ISP_CALIBRATION_API int cali_roi_rgb_adjust(coordinate* src_rgb, coordinate* dst_rgb, coordinate* proj_rgb, int rgb_color_count, int mode);

// Calibration Pipeline - ROI Gray Interpolation
typedef int (*FNUC_ISP_CLUT_ROI_GRAY_INTERPOLATION)(coordinate* src_rgb, coordinate* dst_rgb, int rgb_color_count, point_interpolation_coef* coef, int* rgb_final_count);
ISP_CALIBRATION_API int cali_roi_rgb_interpolation(coordinate* src_rgb, coordinate* dst_rgb, int rgb_color_count, point_interpolation_coef* coef, int* rgb_final_count);

#ifdef __cplusplus
}
#endif

#endif
