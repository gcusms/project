#include "Tools/RM_DataExchange/RM_DataExchange.h"

RM_DataExchange::RM_DataExchange(const std::string& filename)
{
    cv::FileStorage setting_fs(filename, cv::FileStorage::READ);
    read(setting_fs);
    setting_fs.release();
    this->file = filename;
    //显示参数文件是否读取正确
    // cout<<"file:"<<this->file<<endl;
}

RM_DataExchange::~RM_DataExchange() {}

void RM_DataExchange::readData()
{
    cv::FileStorage read_fs(this->file, cv::FileStorage::READ);
    read(read_fs);
    read_fs.release();
}

void RM_DataExchange::writeData()
{
    cv::FileStorage write_fs(this->file, cv::FileStorage::WRITE);
    write(write_fs);
    write_fs.release();
}

void RM_DataExchange::read(const cv::FileStorage& fs)
{
    fs["IS_OPEN"] >> this->setting_cfg.IS_OPEN;

    if (this->setting_cfg.IS_OPEN == 0) {
        std::cout << "----- 文件导入失败 -----" << std::endl;
        return;
    }
    std::cout << "----- 文件导入成功 -----" << std::endl;

    this->updatePNPConfig(fs);

    this->updateSettingInfo(fs);

    this->updateCameraConfig(fs);

    this->updateArmorConfig(fs);

    this->updateBuffConfig(fs);

    this->updateMessengerConfig(fs);

    this->updateSerialConfig(fs);
    
    this->getvideowrite(fs);
    // TODO:for debug image
    // TODO:for flag
    // TODO:for armor value
    // TODO:for buff value
    // TODO:check
    this->check();
}

void RM_DataExchange::write(cv::FileStorage& fs) const
{
    fs << "COUT_FPS" << this->setting_cfg.COUT_FPS;
    // TODO:for debug image
    // TODO:for flag
    // TODO:for armor value
    // TODO:for buff value
}

/**
 * @brief 更新pnp数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updatePNPConfig(const cv::FileStorage& fs)
{
    // pnp 这两个改成pnp
    fs["CAMERA_PARAM_FILE"] >> this->pnp_cfg.CAMERA_PARAM_FILE;
    fs["SHOW_PNP_INFORMATION"] >> this->pnp_cfg.SHOW_PNP_INFORMATION;
    fs["CAMERA_PTZ_X"] >> this->pnp_cfg.CAMERA_PTZ_X;
    fs["CAMERA_PTZ_Y"] >> this->pnp_cfg.CAMERA_PTZ_Y;
    fs["CAMERA_PTZ_Z"] >> this->pnp_cfg.CAMERA_PTZ_Z;
    fs["PTZ_BARREL_X"] >> this->pnp_cfg.PTZ_BARREL_X;
    fs["PTZ_BARREL_Y"] >> this->pnp_cfg.PTZ_BARREL_Y;
    fs["PTZ_BARREL_Z"] >> this->pnp_cfg.PTZ_BARREL_Z;

    fs["BIG_ARMOR_SIZE_W"] >> this->pnp_cfg.BIG_ARMOR_SIZE_W;
    fs["BIG_ARMOR_SIZE_H"] >> this->pnp_cfg.BIG_ARMOR_SIZE_H;
    fs["SMALL_ARMOR_SIZE_W"] >> this->pnp_cfg.SMALL_ARMOR_SIZE_W;
    fs["SMALL_ARMOR_SIZE_H"] >> this->pnp_cfg.SMALL_ARMOR_SIZE_H;
    fs["BUFF_ARMOR_SIZE_W"] >> this->pnp_cfg.BUFF_ARMOR_SIZE_W;
    fs["BUFF_ARMOR_SIZE_H"] >> this->pnp_cfg.BUFF_ARMOR_SIZE_H;

    fs["BUFF_GROUND_H"] >> this->pnp_cfg.BUFF_GROUND_H;
    fs["ROBOT_H"] >> this->pnp_cfg.ROBOT_H;
    fs["UPLAND_H"] >> this->pnp_cfg.UPLAND_H;
    fs["BUFF_ROBOT_Z"] >> this->pnp_cfg.BUFF_ROBOT_Z;

    fs["OFFSET_ARMOR_PITCH"] >> this->pnp_cfg.OFFSET_ARMOR_PITCH;
    fs["OFFSET_ARMOR_YAW"] >> this->pnp_cfg.OFFSET_ARMOR_YAW;
    fs["OFFSET_PITCH_SYMBOL"] >> this->pnp_cfg.OFFSET_PITCH_SYMBOL;
    fs["OFFSET_YAW_SYMBOL"] >> this->pnp_cfg.OFFSET_YAW_SYMBOL;

    std::cout << "PNP 初始化参数 读取成功" << std::endl;
}

/**
 * @brief 更新控制开关数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateSettingInfo(const cv::FileStorage& fs)
{
    fs["COUT_FPS"] >> this->setting_cfg.COUT_FPS;

    fs["CAMERA_OR_VEDIO"] >> this->setting_cfg.CAMERA_OR_VEDIO;
    fs["USB_CAPTURE"] >> this->setting_cfg.USB_CAPTURE;
    fs["AVI_CAPTURE"] >> this->setting_cfg.AVI_CAPTURE;

    fs["MY_COLOR"] >> this->setting_cfg.MY_COLOR;
    fs["MY_MODE"] >> this->setting_cfg.MY_MODE;
    fs["MY_ROBOT_ID"] >> this->setting_cfg.MY_ROBOT_ID;
    fs["BULLET_VOLACITY"] >> this->setting_cfg.BULLET_VOLACITY;

    fs["IS_SERIAL_OPEN"] >> this->setting_cfg.IS_SERIAL_OPEN;
    fs["SHOW_SERIAL_INFORMATION"] >> this->setting_cfg.SHOW_SERIAL_INFORMATION;
    fs["SHOW_OUTPUT_IMG"] >> this->setting_cfg.SHOW_OUTPUT_IMG;
    fs["IS_PARAM_ADJUSTMENT"] >> this->setting_cfg.IS_PARAM_ADJUSTMENT;
    fs["ANALYZE_EACH_FRAME"] >> this->setting_cfg.ANALYZE_EACH_FRAME;
    fs["SHOW_DEBUG_INFORMATION"] >> this->setting_cfg.SHOW_DEBUG_INFORMATION;

    std::cout << "控制开关参数 读取成功" << std::endl;
}

/**
 * @brief 更新相机数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateCameraConfig(const cv::FileStorage& fs)
{
    fs["ISOPEN_INDUSTRY_CAPTURE"] >> this->camera_cfg.ISOPEN_INDUSTRY_CAPTURE;
    fs["CAMERA_EXPOSURETIME"] >> this->camera_cfg.CAMERA_EXPOSURETIME;
    fs["CAMERA_RESOLUTION_COLS"] >> this->camera_cfg.CAMERA_RESOLUTION_COLS;
    fs["CAMERA_RESOLUTION_ROWS"] >> this->camera_cfg.CAMERA_RESOLUTION_ROWS;

    this->camera_cfg.CAMERA_RESOLUTION_COLS_FOV =
        (1280 - this->camera_cfg.CAMERA_RESOLUTION_COLS) * 0.5;
    this->camera_cfg.CAMERA_RESOLUTION_ROWS_FOV =
        (1024 - this->camera_cfg.CAMERA_RESOLUTION_ROWS) * 0.5;

    fs["CAMERA_BLUE_GAIN"] >> this->camera_cfg.CAMERA_BLUE_GAIN;
    fs["CAMERA_GREEN_GAIN"] >> this->camera_cfg.CAMERA_GREEN_GAIN;
    fs["CAMERA_RED_GAIN"] >> this->camera_cfg.CAMERA_RED_GAIN;
    fs["CAMERA_GAMMA"] >> this->camera_cfg.CAMERA_GAMMA;
    fs["CAMERA_CONTRAST"] >> this->camera_cfg.CAMERA_CONTRAST;
    fs["CAMERA_SATURATION"] >> this->camera_cfg.CAMERA_SATURATION;
    fs["CAMERA_CONFIG"] >> this->camera_cfg.CAMERA_CONFIG;
    // fs["IS_PARAM_ADJUSTMENT"] >> this->camera_config.IS_PARAM_ADJUSTMENT;

    std::cout << "相机初始化参数 读取成功" << std::endl;
}

/**
 * @brief 更新自瞄数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateArmorConfig(const cv::FileStorage& fs)
{
    // control
    fs["IS_NUMBER_PREDICT_OPEN"] >> this->arrmor_cfg.IS_NUMBER_PREDICT_OPEN;
    fs["ENABLE_ROI_RECT"] >> this->arrmor_cfg.ENABLE_ROI_RECT;
    fs["DRAW_LIGHT"] >> this->arrmor_cfg.DRAW_LIGHT;
    fs["DRAW_ARMOR_RECT"] >> this->arrmor_cfg.DRAW_ARMOR_RECT;
    fs["IS_PARAM_ADJUSTMENT"] >> this->arrmor_cfg.IS_PARAM_ADJUSTMENT;
    fs["SHOW_BIN_IMG"] >> this->arrmor_cfg.SHOW_BIN_IMG;
    fs["PROCESSING_MODE"] >> this->arrmor_cfg.PROCESSING_MODE;

    // param
    fs["CAMERA_RESOLUTION_COLS"] >> this->arrmor_cfg.CAMERA_RESOLUTION_COLS;
    fs["CAMERA_RESOLUTION_ROWS"] >> this->arrmor_cfg.CAMERA_RESOLUTION_ROWS;
    // BGR
    fs["RED_ARMOR_GRAY_TH"] >> this->arrmor_cfg.RED_ARMOR_GRAY_TH;
    fs["RED_ARMOR_COLOR_TH"] >> this->arrmor_cfg.RED_ARMOR_COLOR_TH;
    fs["BLUE_ARMOR_GRAY_TH"] >> this->arrmor_cfg.BLUE_ARMOR_GRAY_TH;
    fs["BLUE_ARMOR_COLOR_TH"] >> this->arrmor_cfg.BLUE_ARMOR_COLOR_TH;

    // HSV-red
    fs["H_RED_MAX"] >> this->arrmor_cfg.H_RED_MAX;
    fs["H_RED_MIN"] >> this->arrmor_cfg.H_RED_MIN;
    fs["S_RED_MAX"] >> this->arrmor_cfg.S_RED_MAX;
    fs["S_RED_MIN"] >> this->arrmor_cfg.S_RED_MIN;
    fs["V_RED_MAX"] >> this->arrmor_cfg.V_RED_MAX;
    fs["V_RED_MIN"] >> this->arrmor_cfg.V_RED_MIN;
    // HSV-blue
    fs["H_BLUE_MAX"] >> this->arrmor_cfg.H_BLUE_MAX;
    fs["H_BLUE_MIN"] >> this->arrmor_cfg.H_BLUE_MIN;
    fs["S_BLUE_MAX"] >> this->arrmor_cfg.S_BLUE_MAX;
    fs["S_BLUE_MIN"] >> this->arrmor_cfg.S_BLUE_MIN;
    fs["V_BLUE_MAX"] >> this->arrmor_cfg.V_BLUE_MAX;
    fs["V_BLUE_MIN"] >> this->arrmor_cfg.V_BLUE_MIN;

    // LAB-red
    fs["L_RED_MAX"] >> this->arrmor_cfg.L_RED_MAX;
    fs["L_RED_MIN"] >> this->arrmor_cfg.L_RED_MIN;
    fs["A_RED_MAX"] >> this->arrmor_cfg.A_RED_MAX;
    fs["A_RED_MIN"] >> this->arrmor_cfg.A_RED_MIN;
    fs["B_RED_MAX"] >> this->arrmor_cfg.B_RED_MAX;
    fs["B_RED_MIN"] >> this->arrmor_cfg.B_RED_MIN;
    // LAB-blue
    fs["L_BLUE_MAX"] >> this->arrmor_cfg.L_BLUE_MAX;
    fs["L_BLUE_MIN"] >> this->arrmor_cfg.L_BLUE_MIN;
    fs["A_BLUE_MAX"] >> this->arrmor_cfg.A_BLUE_MAX;
    fs["A_BLUE_MIN"] >> this->arrmor_cfg.A_BLUE_MIN;
    fs["B_BLUE_MAX"] >> this->arrmor_cfg.B_BLUE_MAX;
    fs["B_BLUE_MIN"] >> this->arrmor_cfg.B_BLUE_MIN;

    // 灯条筛选条件
    fs["RATIO_H_W_MAX"] >> this->arrmor_cfg.light_cfg.RATIO_H_W_MAX;
    fs["RATIO_H_W_MIN"] >> this->arrmor_cfg.light_cfg.RATIO_H_W_MIN;
    fs["LIGHT_AREA"] >> this->arrmor_cfg.light_cfg.LIGHT_AREA;
    fs["LIGHT_LENGTH_MIN"] >> this->arrmor_cfg.light_cfg.LIGHT_LENGTH_MIN;
    fs["LIGHT_LENGTH_MAX"] >> this->arrmor_cfg.light_cfg.LIGHT_LENGTH_MAX;
    fs["LIGHT_HEIGHT"] >> this->arrmor_cfg.light_cfg.LIGHT_HEIGHT;

    // 装甲板筛选条件
    fs["DIFF_LIGHT_Y"] >> this->arrmor_cfg.candidate_armor_cfg.DIFF_LIGHT_Y;
    fs["DISTANCE_MULTIPLE_TH"] >> this->arrmor_cfg.candidate_armor_cfg.DISTANCE_MULTIPLE_TH;
    fs["DISTANCE_MIN_TH"] >> this->arrmor_cfg.candidate_armor_cfg.DISTANCE_MIN_TH;
    fs["DIFF_ANGLE_TH_SAME"] >> this->arrmor_cfg.candidate_armor_cfg.DIFF_ANGLE_TH_SAME;
    fs["DIFF_ANGLE_TH_WB"] >> this->arrmor_cfg.candidate_armor_cfg.DIFF_ANGLE_TH_WB;
    fs["DIFF_ANGLE_TH_NB"] >> this->arrmor_cfg.candidate_armor_cfg.DIFF_ANGLE_TH_NB;
    fs["DIFF_ANGLE_TH_ZS"] >> this->arrmor_cfg.candidate_armor_cfg.DIFF_ANGLE_TH_ZS;
    fs["DIFF_ANGLE_TH_YS"] >> this->arrmor_cfg.candidate_armor_cfg.DIFF_ANGLE_TH_YS;
    fs["LIGHT_SIZE_RATIO_MAX"] >> this->arrmor_cfg.candidate_armor_cfg.LIGHT_SIZE_RATIO_MAX;
    fs["LIGHT_SIZE_RATIO_MIN"] >> this->arrmor_cfg.candidate_armor_cfg.LIGHT_SIZE_RATIO_MIN;
    fs["ARMOR_TYPE_TH"] >> this->arrmor_cfg.candidate_armor_cfg.ARMOR_TYPE_TH;
    fs["MATCHING_TH"] >> this->arrmor_cfg.candidate_armor_cfg.MATCHING_TH;

    std::cout << "自瞄初始化参数 读取成功" << std::endl;
}

/**
 * @brief 更新能量机关数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateBuffConfig(const cv::FileStorage& fs)
{
    // 设置能量机关打击模型

    std::cout << "能量机关初始化参数 读取成功" << std::endl;
}

/**
 * @brief 更新信使数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateMessengerConfig(const cv::FileStorage& fs)
{
    fs["SHOW_SERIAL_INFORMATION"] >> this->messneger_cfg.SHOW_SERIAL_INFORMATION;
    std::cout << "信使初始化参数 读取成功" << std::endl;
}

/**
 * @brief 更新串口数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateSerialConfig(const cv::FileStorage& fs)
{
    fs["SHOW_SERIAL_INFORMATION"] >> this->setting_cfg.SHOW_SERIAL_INFORMATION;
    std::cout << "串口初始化参数 读取成功" << std::endl;
}

/**
 * @brief 更新ROI数据
 * @param  fs               文件对象
 */
void RM_DataExchange::updateRoiConfig(const cv::FileStorage& fs){

    fs["FIRST_LOSE_CNT_MAX"] >> this->roi_cfg.FIRST_LOSE_CNT_MAX;
    fs["SECOND_LOSE_CNT_MAX"] >> this->roi_cfg.SECOND_LOSE_CNT_MAX;
    std::cout << "ROI初始化参数 读取成功" << std::endl;

}



/**
 * @brief 添加路径
 * @param Image_cols; //列
 * @param Image_rows; // 行
 * @param USING_RECORD; //是否打开预测
 * @param RECORD_PATH;     //选择路径 
*/
void RM_DataExchange::getvideowrite(const cv::FileStorage& fs)
{
    fs["CAMERA_RESOLUTION_COLS"] >> this->videorecord_param_cfg.Image_cols; //列
    fs["CAMERA_RESOLUTION_ROWS"] >> this->videorecord_param_cfg.Image_rows; // 行
    fs["USING_RECORD"] >> this->videorecord_param_cfg.USING_RECORD; //是否打开预测
    fs["RECORD_PATH"]>>this->videorecord_param_cfg.RECORD_PATH;     //选择路径
}


/************************/
/**
 * @brief 检查并设置默认参数，控制开关关联性
 */
void RM_DataExchange::check()
{
    // 根据条件检查参数是否有读取错误

    // 根据控制开关选择模式
    // 相机或视频模式
    if (this->setting_cfg.CAMERA_OR_VEDIO == 1) {
        this->camera_cfg.ISOPEN_INDUSTRY_CAPTURE = 1;
    }

    // Debug of Release
    this->camera_cfg.IS_PARAM_ADJUSTMENT = this->setting_cfg.IS_PARAM_ADJUSTMENT;
    this->arrmor_cfg.IS_PARAM_ADJUSTMENT = this->setting_cfg.IS_PARAM_ADJUSTMENT;

    if (this->setting_cfg.IS_PARAM_ADJUSTMENT == 0) {
        // 设置
        this->resetSettingInfo();
        // 自瞄
        this->resetArmorConfig();
        // 能量机关
        this->resetBUffConfig();
        // 相机
        this->resetCameraConfig();
        // pnp
        this->resetPNPConfig();
        //串口
        this->resetSerialConfig();
        std::cout << "进入发布模式" << std::endl;
    }
    else {
        std::cout << "进入调试模式" << std::endl;
    }

    // TODO：参数设置
}

void RM_DataExchange::resetSettingInfo()
{
    // 这里的写法比较愚蠢，先用着
    // 初始化默认参数（不调试的参数）
    unique_ptr<Setting_Cfg> default_setting = make_unique<Setting_Cfg>();
    // 图像开关 src、dst、bin、滑动条
    this->setting_cfg.MY_MODE                 = default_setting->MY_MODE;
    this->setting_cfg.MY_COLOR                = default_setting->MY_COLOR;
    this->setting_cfg.IS_SERIAL_OPEN          = default_setting->IS_SERIAL_OPEN;
    this->setting_cfg.SHOW_SERIAL_INFORMATION = default_setting->SHOW_SERIAL_INFORMATION;
    this->setting_cfg.SHOW_DEBUG_INFORMATION  = default_setting->SHOW_DEBUG_INFORMATION;
    this->setting_cfg.SHOW_OUTPUT_IMG         = default_setting->SHOW_OUTPUT_IMG;
    this->setting_cfg.ANALYZE_EACH_FRAME      = default_setting->ANALYZE_EACH_FRAME;
}

void RM_DataExchange::resetCameraConfig()
{
    // 初始化默认参数（不调试的参数）
}

void RM_DataExchange::resetArmorConfig()
{
    unique_ptr<Armor_Cfg> default_armor = make_unique<Armor_Cfg>();

    // control
    this->arrmor_cfg.PROCESSING_MODE = default_armor->PROCESSING_MODE;
    this->arrmor_cfg.SHOW_BIN_IMG    = default_armor->SHOW_BIN_IMG;

    // param
}

void RM_DataExchange::resetPNPConfig()
{
    // 初始化默认参数（不调试的参数）
}

void RM_DataExchange::resetBUffConfig()
{
    // 初始化默认参数（不调试的参数）
}

void RM_DataExchange::resetSerialConfig()
{
    // 初始化默认参数（不调试的参数）
}