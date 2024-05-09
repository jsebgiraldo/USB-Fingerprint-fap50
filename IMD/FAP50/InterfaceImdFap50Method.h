#ifndef INTERFACEIMDFAP50METHOD_H
#define INTERFACEIMDFAP50METHOD_H

#include <cstddef>
typedef struct {
    double x, y;
} ImdPoint2d;

typedef struct {
    ImdPoint2d pa, pb, pc, pd, cntr;
    double angle;
} ImdDiamond;
enum E_GUI_SHOW_MODE {
    GUI_SHOW_MODE_NONE,
    GUI_SHOW_MODE_CAPTURE,	//scan fingerprints immediately
    GUI_SHOW_MODE_ROLL,
    GUI_SHOW_MODE_FLAT,
};
enum E_FINGER_POSITION {
    FINTER_POSITION_UNKNOW_FINGER = 0,

    FINGER_POSITION_RIGHT_THUMB,
    FINGER_POSITION_RIGHT_INDEX,
    FINGER_POSITION_RIGHT_MIDDLE,
    FINGER_POSITION_RIGHT_RING,
    FINGER_POSITION_RIGHT_LITTLE,
    FINGER_POSITION_LEFT_THUMB,
    FINGER_POSITION_LEFT_INDEX,
    FINGER_POSITION_LEFT_MIDDLE,
    FINGER_POSITION_LEFT_RING,
    FINGER_POSITION_LEFT_LITTLE,

    FINGER_POSITION_RIGHT_FOUR = 13,
    FINGER_POSITION_LEFT_FOUR,
    FINGER_POSITION_BOTH_THUMBS,
    FINGER_POSITION_ANY_FINGER,
    FINGER_POSITION_SIZE,
};

enum NFIQ_VERSION {
    NFIQ_V1,
    NFIQ_V2,
    NFIQ_VER_SIZE,
};

typedef struct {
    E_FINGER_POSITION finger_position;
    int png_compress_ratio;		/** The PNG compression ratio is 0~9. (default:9) */
    int jp2_quality;			/** The Jpeg2000 quality level is 0~1000. (default:750)
                                 Zero is loseless compression. */
    float wsq_bit_rate;			/** The bit rate is set as follows:
                                 2.25 yields around 5:1 compression
                                 0.75 yields around 15:1 compression. (default) */
    char* wsq_comment_text;		/** The WSQ comment default is NULL. */

    NFIQ_VERSION nfiq_ver;			//default:NFIQ_V1
    int nfiq_score_minimum_acceptable;	//1~5, default:3
    int nfiq2_score_minimum_acceptable;	//0~100, default:35
    int speech_language;				//0:EN, 1:CH 2:user's define
    int speech_volume;				//0:small, 1:medium, 2:large
    int life_check_en;

    int image_width, image_height;
    int image_bit_per_pix, image_pix_per_inch;
    std::byte fap50_lib_ver[3];
    std::byte opencv_lib_ver[3];		//OpenCV LIB version
    std::byte nbis_lib_ver[3];		      //NBIS LIB version
    std::byte nfiq2_lib_ver[3];		//NFIQ2 LIB version
    std::byte guid[16];
    int chip_id;
    std::byte fw_ver[3];				//[1]Major, [0]Minor, [2]Type


} SystemProperty;

#define SCORE_ARRAY_SIZE 4
struct ImageProperty {
    E_GUI_SHOW_MODE mode;
    E_FINGER_POSITION pos;
    bool this_scan;

    std::byte* img;
    long width, height;
    int score_array[SCORE_ARRAY_SIZE];
    int score_size;
};

/** iMD result ENUM */
enum IMD_RESULT {
    /** If IMD_RLT_SUCCESS is returned, it means success.If not, it may be a hint
        or an error.*/
    IMD_RLT_SUCCESS = 0x00,

    IMD_RLT_PROMPT = 0x100,
    /** This prompt is triggered when a scan is started.In GUI_SHOW_MODE_CAPTURE,
        there will be no prompt. */
    IMD_RLT_SCAN_THREAD_START,
    //The device is busy scanning images.
    IMD_RLT_SCAN_THREAD_IS_BUSY,
    /** After starting to scan, if no finger is placed for more than 30 seconds,
        the device will stop scanning.*/
    IMD_RLT_SCAN_THREAD_IDLE_TIMEOUT,
    //The prompt is triggered when the device stops.
    IMD_RLT_SCAN_THREAD_END,

    IMD_RLT_FAIL = 0x200,
    //This API does not have any implementation.
    IMD_RLT_NO_IMPLEMENTATION,
    IMD_RLT_NULL_POINTER,
    IMD_RLT_FLASH_WRITE_FAIL,
    //There is a problem with the input parameters of the API.
    IMD_RLT_PARAM_WRONG,
    //No USB devices were found.

    IMD_RLT_CANT_FIND_ANY_DEVICE,
    //Device reset failed.
    IMD_RLT_RESET_DEVICE_FAIL,
    //The device's CHIP ID is incorrect.
    IMD_RLT_CHIP_ID_FAIL,
    //Failed to adjust image background.
    IMD_RLT_CLAI_TIMEOUT,
    //Exception occurred while reading image from USB.
    IMD_RLT_USB_READ_IMAGE_EXCEPTION,
    IMD_RLT_USB_READ_IMAGE_TIMEOUT,
    //No images available.
    IMD_RLT_NO_AVAILABLE_IMAGE,
    //Hardware's DCMI is too fast
    IMD_RLT_DCMI_TOO_FAST,
    //Hardware's DCMI is stuck.
    IMD_RLT_DCMI_IS_STUCK,
    //Scanned images are not yet complete.
    IMD_RLT_NO_ANY_SCAN_DONE,
    //Scanning images cannot be stopped.
    IMD_RLT_STOP_SCAN_TIMEOUT,
    //No fingers were detected.
    IMD_RLT_NO_ANY_FINGER,

    IMD_RLT_NEED_TO_TAKE_FINGER_OFF,

    IMD_RLT_FINGER_TOO_TOP = 0x300,
    IMD_RLT_FINGER_NUM_OVER_FOUR,
    IMD_RLT_PUT_WRONG_HAND,
    IMD_RLT_POOR_NFIQ_QUALITY,
    IMD_RLT_POOR_QUALITY_AND_WRONG_HAND,
    IMD_RLT_POOR_QUALITY_AND_CANTACT_IRON,
    IMD_RLT_POOR_CONTACT_IRON,

    IMD_RLT_OPEN_FILE_FAIL,
    IMD_RLT_INVALID_BIN_FILE,
};

typedef struct {
    /** It is up to the user to decide the fingerprint reading mode. */
    E_GUI_SHOW_MODE show_mode;
    E_FINGER_POSITION finger_position;

    /** Status of roll : If init, the flag is_roll_init will be TRUE.If finished,
        the flag is_roll_done will be TRUE. The value line is the current guideline.*/
    bool is_roll_init, is_roll_done;

    /** Status of flat : The flag is_roll_init will be TRUE if is in init.
        If finished, the flag is_flat_done will be TRUE. */
    bool is_flat_init, is_flat_done;

    int finger_num;		//The "finger_num" is the number of finger presses.

    bool is_finger_on;	//If any finger is touching, the flag "is_finger_on" is TRUE.

    /** This "contours" is a container for fingerprint outline coordinates, which records the
        coordinates of the rectangle. Use this to extract images for each finger separately.
        (The type void* is vector<ImdDiamond>*) */
    void* contours;

    float frame_rate;	//The "frame_rate" is image frame rate. Unit:fps (frame per second).
    std::byte* img;			//The FAP50 image buffer. (1600*1000)

    IMD_RESULT result;

} ImageStatus;


extern "C" {
__declspec(dllimport) IMD_RESULT __stdcall device_reset();
__declspec(dllimport) SystemProperty __stdcall get_system_property();
}



#endif // INTERFACEIMDFAP50METHOD_H
