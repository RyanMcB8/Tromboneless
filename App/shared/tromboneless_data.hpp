/* Header file to pass functions which may have an effect on both the app and core functionality. */
#ifndef _TROMBONELESS_DATA_H_
#define _TROMBONELESS_DATA_H_

/* ============================== This is where the types are defined ============================== */

/*  Creation of an enum which corresponds to different shift keying
    initial positions which may adjusted by the user through the app. 
    These represent the different types of trombones which may be
    synthesised by calibrating the correct note and octave. */
typedef enum{
    SKOpt_EMPTY,                /** There was no chosen type of trombone by the user. */
    SKOpt_BASS,                 /** The chosen type of trombone by the user was the bass. */
    SKOpt_PICCOLO,              /** The chosen type of trombone by the user was the piccolo. */
    SKOpt_ALTO,                 /** The chosen type of trombone by the user was the alto. */
    SKOpt_TENOR,                /** The chosen type of trombone by the user was the tenor. */
    SKOpt_CONTRABASS,           /** The chosen type of trombone by the user was the contrabass. */
    SKOpt_SOPRANO,              /** The chosen type of trombone by the user was the soprano. */
} ShiftKeyingOptions_t;

/*  Creation of an enum which corresponds to the different
    embouchure options that the user may select to calibrate.
    This should be adjusted before release the have the correct
    names and number of options. */
typedef enum{
    EMBCR_EMPTY,                /** Option for a no chosen embouchure placement. */
    EMBCR_LowPlacement,         /** Option for a low embouchure placement. */
    EMBCR_MediumPlacement,      /** Option for a medium embouchure placement. */
    EMBCR_HighPlacement,        /** Option for a high embouchure placement. */
} EmbachureOptions_t;

/*  A struct which stores all the necessary parameters that may be
    changed on the app side effecting the core functionality. This
    struct should add a flag which is triggered whenever a parameter
    has been changed.*/
typedef struct{
    ShiftKeyingOptions_t shiftKeyingOption;         /** The chosen shiftkey option by the user corresponding to the type of trombone. */
    double nearDistance;                            /** The nearest distance the user wishes to have their hand from the sensor for pitch bend. */
    double farDistance;                             /** The furthest distance the user wishes to have their hand from the sensor for pitch bend. */

    double lowPressure;                             /** The lower pressure gain which the user has chosen for the mouthpiece corresponding to the peak input. */
    double highPressure;                            /** The upper pressure gain which the user has chosen for the mouthpiece corresponding to the peak input. */

    EmbachureOptions_t embouchureOption;            /**  The chosen embouchure option which the user wishes to calibrate.   */
    bool triggerEmbouchureCalibrate;                /**  High/true when the user has pressed the button to calibrate the Embouchure. */
} TrombonelessParameters_t;

/*  A struct to store any of the synthesier parameters that may be
    changed within the app. A data changed flag variable should be
    added to this struct to tell the core to read the new values. */
typedef struct{
    bool synthEnable;                                   /** The variable used to determine when the internal synth and equalizer is used. */
    float gains[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};   /** The chosen gains for the various frequency bands in the equalizer. */
} SynthesiserParameters_t;

/*  Creating an instance of the trombonelessParameters_t struct to 
    store the active parameter variables. */
extern TrombonelessParameters_t trombonelessParameters; /** The struct instance storing all the data used to interface between calibrating the Tromboneless from the App to the Core. */

/*  Creating an instance of the SynthethiserParameters_t struct to
    store the active parameter variables. */
extern SynthesiserParameters_t synthesiserParameters;   /** The struct instance storing all the data used to interface between the internal synthesiser and the App. */

#endif