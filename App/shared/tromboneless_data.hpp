/* Header file to pass functions which may have an effect on both the app and core functionality. */
#ifndef _TROMBONELESS_DATA_H_
#define _TROMBONELESS_DATA_H_

/* ============================== This is where the types are defined ============================== */

/*  Creation of an enum which corresponds to different shift keying
    initial positions which may adjusted by the user through the app. 
    These represent the different types of trombones which may be
    synthesised by calibrating the correct note and octave. */
typedef enum{
    SKOpt_EMPTY,
    SKOpt_BASS,
    SKOpt_PICCOLO,
    SKOpt_ALTO,
    SKOpt_TENOR,
    SKOpt_CONTRABASS,
    SKOpt_SOPRANO,
} ShiftKeyingOptions_t;

/*  Creation of an enum which corresponds to the different
    embouchure options that the user may select to calibrate.
    This should be adjusted before release the have the correct
    names and number of options. */
typedef enum{
    EMBCR_EMPTY,
    EMBCR_LowPlacement,
    EMBCR_MediumPlacement,
    EMBCR_HighPlacement,
} EmbachureOptions_t;

/*  A struct which stores all the necessary parameters that may be
    changed on the app side effecting the core functionality. This
    struct should add a flag which is triggered whenever a parameter
    has been changed.*/
typedef struct{
    ShiftKeyingOptions_t shiftKeyingOption;
    double nearDistance;
    double farDistance;

    double lowPressure;
    double highPressure;

    EmbachureOptions_t embouchureOption;
    bool triggerEmbouchureCalibrate;
} TrombonelessParameters_t;

/*  A struct to store any of the synthesier parameters that may be
    changed within the app. A data changed flag variable should be
    added to this struct to tell the core to read the new values. */
typedef struct{
    bool synthEnable;
    float gains[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
} SynthesiserParameters_t;

/*  Creating an instance of the trombonelessParameters_t struct to 
    store the active parameter variables. */
extern TrombonelessParameters_t trombonelessParameters;

/*  Creating an instance of the SynthethiserParameters_t struct to
    store the active parameter variables. */
extern SynthesiserParameters_t synthesiserParameters;

#endif