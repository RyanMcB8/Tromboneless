/* Header file to pass functions which may have an effect on both the app and core functionality. */

#pragma once

/* ============================== This is where the types are defined ============================== */

/* Creation of an enum which corresponds to different shift keying
    initial positions which may adjusted by the user through the app. */
typedef enum{
    SKOpt_NoOption,
    SKOpt_MiddleF4,
    SKOpt_BSharp4,
    SKOpt_D5,
    SKOpt_F5,
    SKOpt_ASharp4,
} ShiftKeyingOptions_t;


/** @brief This function is used to select the shift keying used on the slider.
 *  @param shiftKey The key of which was chosen
 *  @note This function should be used to call upon the classes which hold the corresponding data which is being used. 
 */
void shiftKeyingUpdate(void);

/** @brief This function is used to change the maximum and minimum distances for the slider to work at.
 *  @param shiftKey The key of which was chosen
 *  @note This function should be used to call upon the classes which hold the corresponding data which is being used. 
 */
void distanceCalibrationUpdate(void);

/** @brief A function which can be used to change the maximum and minimum relative pressures of being read from the pressure sensor.  */
void pressureCalibrationUpdate(void);