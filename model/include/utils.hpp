#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <QString>
#include <QVariant>
#include <QPixmap>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "global_data.hpp"
#include <opencv2/opencv.hpp>

using namespace std;

const QString LDC_FOLDER = "ldc_bin/";
#undef ALIGN
#define ALIGN(x, a)      (((x) + ((a)-1)) & ~((a)-1))
#define DEFAULT_ALIGN                64
#define PI (3.1415926)

typedef struct _SIZE_S {
    unsigned int u32Width;
    unsigned int u32Height;
} SIZE_S;

typedef struct _LDC_ATTR_S {
    bool bAspect; /* RW;Whether aspect ration  is keep */
    int s32XRatio; /* RW; Range: [0, 100], field angle ration of  horizontal,valid when bAspect=0.*/
    int s32YRatio; /* RW; Range: [0, 100], field angle ration of  vertical,valid when bAspect=0.*/
    int s32XYRatio; /* RW; Range: [0, 100], field angle ration of  all,valid when bAspect=1.*/
    int s32CenterXOffset;
    int s32CenterYOffset;
    int s32DistortionRatio;
} LDC_ATTR_S;

typedef struct _MODULE_LDC_ATTR_S {
    bool vi_enable;
    bool vpss_enable;
    LDC_ATTR_S  vi_ldc_attr;
    LDC_ATTR_S  vpss_ldc_attr;
} MODULE_LDC_ATTR_S;

typedef enum _ROTATION_E {
    ROTATION_0 = 0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270,
    ROTATION_MAX
} ROTATION_E;

typedef enum _LDC_MODULE_E {
    LDC_VI = 0,
    LDC_VPSS,
    LDC_MAX
} LDC_MODULE_E;

class MyInt
{
public:
    explicit MyInt(const int &value_) : value(value_) {}

    int value;

    bool operator < (const MyInt& other) const {
        return value < other.value;
    }
};

typedef QVariant (*GenGammaCall)(double, double, ItemValueType);
extern QMap<QString, GenGammaCall> gamma_call_list;

namespace utils {
    #define PATH_SEPARATOR "/"
    #define BRACKETING_EV_NUM 11
    //#if defined(WIN32) || defined(_WIN32)
    //#define PATH_SEPARATOR "\\"
    //#else
    //#define PATH_SEPARATOR "/"
    //#endif

    const QString HEX_PREFIX = "0x";

    typedef enum {
        BAYER_ID_BEGIN = 0,
        BAYER_ID_BG = BAYER_ID_BEGIN,
        BAYER_ID_GB,
        BAYER_ID_GR,
        BAYER_ID_RG,
        BAYER_ID_RGB,
        BAYER_ID_YUV,
    } RAW_BAYER_ID;

    typedef enum {
        COLOR_R,
        COLOR_G,
        COLOR_B
    } COLOR_RGB;

    enum class RAW_BITS {
        BITS_8 = 8,
        BITS_10 = 10,
        BITS_12 = 12,
        BITS_16 = 16,
    };

    enum class RAW_PACK_TYPE {
        RAW_UNPACKED,
        RAW_PACKED,
        RAW_DPCM6,
    };

    enum class IMAGE_TYPE {
        IMAGE_RAW,
        IMAGE_YUV,
        IMAGE_YUV420,
        IMAGE_NV21,
        IMAGE_NV12,
        IMAGE_JPG,
        IMAGE_BMP,
        IMAGE_PPM,
        IMAGE_UNKNOWN
    };

    typedef enum {
        IR_NO = 0,
        IR_BB,
        IR_RR,
        IR_MAX
    } IR_POSITION;

    typedef struct _BLC_ATTR_S {
        unsigned int offset_r;
        unsigned int offset_g;
        unsigned int offset_b;
    } BLC_ATTR_S;

    typedef struct _WBG_ATTR_S {
        unsigned int wb_gain_r;
        unsigned int wb_gain_g;
        unsigned int wb_gain_b;
    } WBG_ATTR_S;

    typedef struct _SIMPLE_PIPE_ENABLE_ATTR_S {
        bool pipe_enable;
        bool blc_enable;
        bool wbg_enable;
        bool rgbgamma_enable;
    } SIMPLE_PIPE_ENABLE_ATTR_S;

    template <class T, class F>
    inline string mapToString(map<T, F> &&map, string &&delimiter = "\n") {
        string str = "";
        for (auto map_item : map) {
            str += map_item.first + "," + map_item.second + delimiter;
        }

        return str;
    }

    inline vector<string> stringToVector(string &&str, string &&delimiter = ",") {
        vector<string> v;
        size_t pos = 0;

        while((pos = str.find(delimiter)) != string::npos) {
            v.emplace_back(str.substr(0, pos));
            str.erase(0, pos + delimiter.length());
        }

        if (!str.empty()) {
            v.emplace_back(str);
        }

        return v;
    }

    inline QString regAddressToNumString(QString address)
    {
        return QString::number(address.mid(2).toInt());
    }

    template <class T>
    inline QString listToString(QList<T> list, QString &&delimiter = ",")
    {
        QString str = "";
        for (int i = 0; i < list.size(); i++) {
            str += QString::number(list[i]);
            if (i != (list.size() - 1)) {
                str += delimiter;
            }
        }

        return str;
    }

    template <>
    inline QString listToString<QString>(QList<QString> list, QString &&delimiter)
    {
        QString str = "";
        for (int i = 0; i < list.size(); i++) {
            str += list[i];
            if (i != (list.size() - 1)) {
                str += delimiter;
            }
        }

        return str;
    }

    template <>
    inline QString listToString<QVariant>(QList<QVariant> vec, QString &&delimiter)
    {
        QString str = "";
        for (int i = 0; i < vec.size(); i++) {
            str += vec[i].toString();
            if (i != (vec.size() - 1)) {
                str += delimiter;
            }
        }

        return str;
    }

    template <class T>
    inline QString vectorToString(QVector<T> vec, QString &&delimiter = ",")
    {
        QString str = "";
        for (int i = 0; i < vec.size(); i++) {
            str += vec[i];
            if (i != (vec.size() - 1)) {
                str += delimiter;
            }
        }

        return str;
    }

    template <>
    inline QString vectorToString<QVariant>(QVector<QVariant> vec, QString &&delimiter)
    {
        QString str = "";
        for (int i = 0; i < vec.size(); i++) {
            str += vec[i].toString();
            if (i != (vec.size() - 1)) {
                str += delimiter;
            }
        }

        return str;
    }

    template <class T>
    inline QList<T> stringToList(QString str, QString &&delimiter = ",")
    {
        QList<T> list;

        QStringList str_list = str.split(delimiter);
        for (auto value : str_list) {
            list << value;
        }

        return list;
    }

    template <>
    inline QList<int> stringToList<int>(QString str, QString &&delimiter)
    {
        QList<int> list;

        QStringList str_list = str.split(delimiter);
        for (auto value : str_list) {
            list << value.toInt();
        }

        return list;
    }

    template <>
    inline QList<int64_t> stringToList<int64_t>(QString str, QString &&delimiter)
    {
        QList<int64_t> list;

        QStringList str_list = str.split(delimiter);
        for (auto value : str_list) {
            list << value.toLongLong();
        }

        return list;
    }

    inline QVariantList stringToVariantList(QString str, QString &&delimiter = ",")
    {
        QVariantList list;

        QStringList str_list = str.split(delimiter);
        for (auto value : str_list) {
            list.append(value.toInt());
        }

        return list;
    }

    inline int compareVersion(QString version_src, QString version_target)
    {
        /*
         * version_src > version_target, return 1
         * version_src = version_target, return 0
         * version_src < version_target, return -1
        */

        int result = -1;
        QStringList version_src_list = version_src.split('.');
        QStringList version_target_list = version_target.split('.');

        if (version_src_list.length() != version_target_list.length()) {
            qDebug("version str format not equal");
            return result;
        }

        for (int i = 0; i < version_src_list.length(); i++) {
            if (version_src_list.at(i).toInt() > version_target_list.at(i).toInt()) {
                result = 1;
                break;
            } else if (version_src_list.at(i).toInt() < version_target_list.at(i).toInt()) {
                result = -1;
                break;
            } else {
                result = 0;
            }
        }

        return result;
    }

    inline QString toHex(unsigned int value, int padding = 0)
    {
        return QString(HEX_PREFIX + "%1").arg(value, padding, 16, QChar('0'));
    }

    inline QJsonObject jsonObjectFromString(const QString& in)
    {
        QJsonObject obj;
        QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

        // check validity of the document
        if (!doc.isNull()) {
            if (doc.isObject()) {
                obj = doc.object();
            } else {
                qDebug("Document is not an object\n");
            }
        } else {
            qDebug("Invalid JSON...\n");
        }

        return obj;
    }

    inline QJsonArray jsonArrayFromString(const QString& in)
    {
        QJsonArray arr;
        QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

        // check validity of the document
        if (!doc.isNull()) {
            if (doc.isArray()) {
                arr = doc.array();
            } else {
                qDebug("Document is not an object\n");
            }
        } else {
            qDebug("Invalid JSON...\n");
        }

        return arr;
    }

    inline QString stringFromObject(const QJsonObject &obj)
    {
        return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    }

    inline QRgb yuv2rgb(uint8_t y, uint8_t u, uint8_t v)
    {
        int r = qRound(y + 1.403 * (v - 128));
        int g = qRound(y - 0.343 * (u - 128) - 0.714 * (v - 128));
        int b = qRound(y + 1.770 * (u - 128));

        r = qBound(0, r, 255);
        g = qBound(0, g, 255);
        b = qBound(0, b, 255);

        return qRgb(r, g, b);
    }

    inline QImage YUV21toRGBImage(unsigned char *yuv_image, int img_width, int img_height)
    {
        /*only use for yuv nv21 format image convert.*/
        QImage rgb_image(img_width, img_height, QImage::Format_RGB32);
        if (!yuv_image) return rgb_image;

        //yuv420 to yuv 444,later to rgb
        unsigned char *y_in = yuv_image;
        unsigned char *uv_in = yuv_image + img_width * img_height;
        unsigned char *y_out = static_cast<unsigned char*>(malloc(img_width * img_height));
        unsigned char *u_out = static_cast<unsigned char*>(malloc(img_width * img_height));
        unsigned char *v_out = static_cast<unsigned char*>(malloc(img_width * img_height));

        for (int y_idx = 0; y_idx < img_width * img_height; ++y_idx) {
            y_out[y_idx] = y_in[y_idx];
        }

        for (int h = 0; h < img_height; h++) {
            for (int w = 0; w < img_width; w++) {
                int idx = h * img_width + w;
                u_out[idx] = uv_in[(h/2) * img_width + (w/2)*2+1];
                v_out[idx] = uv_in[(h/2) * img_width + (w/2)*2];
                rgb_image.setPixel(w, h, utils::yuv2rgb(y_out[idx], u_out[idx], v_out[idx]));
            }
        }

        if (y_out) {
            free(y_out);
        }
        if (u_out) {
            free(u_out);
        }
        if (v_out) {
            free(v_out);
        }

        return rgb_image;
    }

    inline QImage convertYUV2RGBImage(QByteArray &data, YUV_HEADER *header)
    {
        int i =0, j =0, u_start = 0, v_start = 0;
        uint8_t y = 0, u = 0, v = 0, tmp = 0;

        QImage rgb_image(header->width, header->height, QImage::Format_RGB32);
        if (data.isEmpty()) return rgb_image;

        if (header->pixelFormat == PIXEL_FORMAT_NV21 || header->pixelFormat == PIXEL_FORMAT_NV12) {
            u_start = header->stride[0] * header->height;
            v_start = header->stride[0] * header->height;
            for (i = 0; i < header->height; i++) {
                for (j = 0; j < header->width; j++) {
                    y = static_cast<uint8_t>(data[j + i * header->stride[0]]);
                    v = static_cast<uint8_t>(data[u_start + (2 * (j/2) + (i/2) * header->stride[1])]);
                    u = static_cast<uint8_t>(data[v_start + (2 * (j/2) + 1 + (i/2) * header->stride[1])]);
                    if (header->pixelFormat == PIXEL_FORMAT_NV12) {
                        tmp = u;
                        u = v;
                        v = tmp;
                    }
                    rgb_image.setPixel(j, i, utils::yuv2rgb(y, u, v));
                }
            }
        } else if (header->pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
            u_start = header->stride[0] * header->height;
            v_start = u_start + header->stride[1] * header->height / 2;
            for (i = 0; i < header->height; i++) {
                for (j = 0; j < header->width; j++) {
                    y = static_cast<uint8_t>(data[j + i * header->stride[0]]);
                    u = static_cast<uint8_t>(data[u_start + ((j/2) + (i/2) * header->stride[1])]);
                    v = static_cast<uint8_t>(data[v_start + ((j/2) + (i/2) * header->stride[2])]);
                    rgb_image.setPixel(j, i, utils::yuv2rgb(y, u, v));
                }
            }
        } else {
            qDebug("YUV pixel format is not supported!");
        }

        return rgb_image;
    }

    template <typename T>
    inline QJsonArray arrayPtrToJsonArray(T array_ptr, int size)
    {
        QJsonArray array;
        if (array_ptr == nullptr) return array;

        for (int i = 0; i < size; i++) {
            array.append(array_ptr[i]);
        }

        return array;
    }

    inline void cubic_intp_1D (qreal y0, qreal y1, qreal y2, qreal y3, qreal x0, qreal x1, qreal x2, qreal x3, qreal x_intp, qreal *y_intp)
    {
         qreal m2 = (y3 - y1) / (x3 - x1);
         qreal x23 = x2 + (x2 - x1);
         qreal y23 = y3 - m2 * (x3 - x23);

         qreal m1 = (y2 - y0) / (x2 - x0);
         qreal x01 = x1 - (x2 - x1);
         qreal y01 = y0 - m1 * (x0 - x01);

         qreal normalize = 1.0 / (x2 - x1);
         qreal x_intp_normal = (x_intp - x1) * normalize;
         qreal c0 = y1;
         qreal c1 = 0.5 * (y2 - y01);
         qreal c2 = 0.5 * (2 * y01 - 5 * y1 + 4 * y2 - y23);
         qreal c3 = 0.5 * (y23 - 3 * y2 + 3 * y1 - y01);

         *y_intp = c0 + x_intp_normal * (c1 + x_intp_normal * (c2 + x_intp_normal * c3));
    }

    template<typename T>
    inline bool is_positive(const T &x) {
        return (x > 0) ? true : false;
    }

    bool chckxy(const std::vector<int> &x_in, const std::vector<int> &y_in);
    std::vector<int> interp1_pchip(const std::vector<int> &x_in, const std::vector<int> &y_in);

    // functions define
    QString getBayerName(int bayer_id);
    QString getPackTypeName(RAW_PACK_TYPE raw_pack_type);
    QString getBitsName(RAW_BITS bits);
    QString getIrPositionName(int index);
    void saveJson(const QString &file_path, const QJsonObject &json);
    void saveAllParametersJson(const QString &file_path, const QJsonObject &json);
    void saveLscJson(const QString &file_path, const QJsonObject &json, bool is_export_rlsc=false);
    int loadJson(const QString &file_path, QJsonObject &json, QString &error_msg, QString extract_file = "pqtool_definition.json");
    void saveLog(const QString &file_path, QString &file_name, const QString &log, const QString &dateTime_str, QString type_str, int mode);

    QJsonArray pointListToJsonArray(QList<QPointF> points);
    QList<QPointF> pointListFromJsonArray(QJsonArray points_array);

    QList<qreal> interpolation(QList<qreal> values, int interpolation_number);
    QList<QVariant> jsonArrayToList(QJsonArray array);
    void stringToInt(const QString string_s, float *data);

    std::vector<unique_ptr<int []>> readMultiRawIntData(QString rawfilename, int width, int height);
    unique_ptr<int []> readRawIntData(QString rawfilename, int width, int height, bool wdr_only_le = false);
    int readYuvIntData(QString filename, int *image, int size);
    unsigned char *rawIntData2RGBData(int *raw_data, int width, int height, RAW_BAYER_ID bayer, RAW_BITS bits = RAW_BITS::BITS_12, bool packed = false);
    unsigned int getRawSize(unsigned width, unsigned height, RAW_BITS bits, RAW_PACK_TYPE raw_pack_type);
    unsigned char *readRawImage(QString rawfilename, unsigned int width, unsigned int height, RAW_BAYER_ID bayer, RAW_BITS bits = RAW_BITS::BITS_12,
                                RAW_PACK_TYPE raw_pack_type = RAW_PACK_TYPE::RAW_UNPACKED, bool wdr_only_le = false);
    int readRawFile(unsigned char *praw, unsigned int size, const char *filename);
    void raw_packed2unpacked(const unsigned char *packed, unsigned short *unpacked, unsigned int width, unsigned int height);
    void raw_unpacked2packed(const unsigned short *unpacked, unsigned char *packed, unsigned int width, unsigned int height);
    void fill_channels(unsigned char *praw, unsigned char *dst, unsigned int width, unsigned int height, RAW_BAYER_ID bayer, RAW_BITS bits = RAW_BITS::BITS_12);
    void raw_simple_pipe(unsigned char *praw, unsigned char *dst, unsigned int width, unsigned int height, RAW_BAYER_ID bayer, RAW_BITS bits,
        BLC_ATTR_S *blc_attr, WBG_ATTR_S *wbg_attr, SIMPLE_PIPE_ENABLE_ATTR_S *simple_pipe_enable);
    unsigned int fromU32MaskValue(unsigned int original_value, QString bitmask);
    unsigned int toU32MaskValue(unsigned int original_value, QString bitmask);
    int write_raw(const char *file_path,int width, int height, int *im_info);
    QImage convertRaw2RGBImage(QByteArray data, unsigned int width, unsigned int height, RAW_BAYER_ID bayer, RAW_BITS bits);
    QImage convertRaw2RGBImage(QByteArray data, unsigned int width, unsigned int height, RAW_BAYER_ID bayer, RAW_BITS bits,
            RAW_PACK_TYPE raw_pack_type, BLC_ATTR_S *blc_attr, WBG_ATTR_S *wbg_attr, SIMPLE_PIPE_ENABLE_ATTR_S *simle_pipe_enable);
    unsigned char *convertAnyRaw2Raw16(const unsigned char *praw_data, unsigned int width, unsigned height,
        RAW_BITS bits, RAW_PACK_TYPE raw_pack_type);
    bool isTileMode(RAW_INFO rawInfo, RAW_TYPE rawType, int *tileIdx);
    QByteArray cropUnpackRaw(QByteArray src, RAW_HEADER &header, bool tileMode, int tileIdx);

    // DPCM
    void dpcm_tx(const ushort *idata, ushort *odata, int width, int height, int mode);
    void dpcm_rx(const ushort *idata, ushort *odata, int width, int height, int mode);
    void RgbToLab(double red, double green, double blue, double* l, double* a, double* b);
    void LabToRgb(double l, double a, double b, double *red, double *green, double *blue);

    // Tool JSON Converter
    int createPagesInfo(QJsonObject &tool_json, QJsonArray &pages_json, QString &error_msg);
    int rearrangeLayout(QJsonObject &tool_json, QJsonArray &pages_json, QString &error_msg);
    void saveAllParametersJsonJsonrpc(const QString &file_path, const QJsonObject &json);
    void saveTemplateWithData(QFile &file, QJsonObject &template_obj, const QJsonObject &data_obj);
    QString getNoiseLayoutMemberName(int index);
    QJsonObject convertNoiseProfileParamFormat(QJsonObject params_obj);
    QJsonArray convertNoiseProfileParamFormat(QJsonArray json_array);
    QJsonObject revertNoiseProfileParamFormat(QJsonObject params_obj);
    void deleteListSameMember(QList<QString> &list);
    bool isMatchCCMSumTitleWithPrefix(QString member);
    bool isMatchCCMSumTitle(QString member);
    bool isZipFile(const QString &file_path, bool bExtract, QString &extract_path);
    int validateValue(int v, int min, int max);
    double validateDoubleValue(double v, double min, double max);
    QStringList getDirSpecifyTypeFileNameList(QString path,  QStringList type_list);
    bool removeSpecifyDirFile(QString dir_path, QStringList file_name_list);
    QImage cvMat2QImage(const cv::Mat &mat);
    QList<QImage> spliceGrayImage(QList<QImage> &image_list);
    void saveLblcJson(const QString &file_path, const QJsonObject &json);
    void jpg2ppm(QString srcFile, QString dstFile);
    QList<QImage> RGB2Grayscale(QList<QImage> img_list);
    void findFiles(QString dirPath, QString typStr, QStringList &files, bool findAll = false);
    QJsonObject remapParamsFromRead(QString pqt_struct_id, QJsonObject &params_obj);
    void parseParams(QString &prefix_id, const QJsonObject &params_obj, QJsonObject &mapping_obj, QJsonObject &out);
    void remapParamsToWrite(QString pqt_struct_id, QStringList &path_split, QJsonObject &params_obj, QJsonValue &value);
    void createParamJsonrpc(QStringList &path_split, QJsonObject &params_obj, QJsonValue &value, QJsonObject &mapping_obj);
}

// gdc of LDC
void mesh_gen_get_size(SIZE_S in_size, SIZE_S out_size, unsigned int *mesh_1st_size, unsigned int *mesh_2nd_size);
int mesh_gen_ldc(SIZE_S in_size, SIZE_S out_size, const LDC_ATTR_S *pstLDCAttr, uint64_t mesh_phy_addr, void *mesh_vir_addr, ROTATION_E rot);

// Caculate for Color Error
typedef struct {
    vector <double> patches;
    double mean;
    double max;
} COLOR_ERROR_METHOD;
typedef struct {
    COLOR_ERROR_METHOD dCab;
    COLOR_ERROR_METHOD dEab;
    COLOR_ERROR_METHOD dC94;
    COLOR_ERROR_METHOD dE94;
} COLOR_ERROR;
COLOR_ERROR colorError(int img[], double ref_lab[], int width, int num_of_roi, int roi_width, int roi_height, int roi_pointxy[][2]);

#endif // UTILS_H
