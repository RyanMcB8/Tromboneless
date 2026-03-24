/* Header file to pass functions which may have an effect on both the app and core functionality. */
#ifndef _TROMBONELESS_DATA_H_
#define _TROMBONELESS_DATA_H_

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

/* A struct which stores all the necessary parameters that may be changed on the app side effecting the core functionality. */
typedef struct{
    ShiftKeyingOptions_t shiftKeyingOption;
    double nearDistance;
    double farDistance;

    double lowPressure;
    double highPressure;
} TrombonelessParameters_t;

/* A struct to store any of the synthesier parameters that may be changed within the app. */
typedef struct{
    bool synthEnable;
    float gains[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
} SynthesiserParameters_t;

/* Creating an instance of the trombonelessParameters_t struct to store the active parameter variables.*/
extern TrombonelessParameters_t trombonelessParameters;

/* Creating an instance of the SynthethiserParameters_t struct to store the active parameter variables.*/
extern SynthesiserParameters_t synthesiserParameters;

#endif