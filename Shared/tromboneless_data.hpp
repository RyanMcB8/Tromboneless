/* Header file to pass functions which may have an effect on both the app and core functionality. */
#ifndef _TROMBONELESS_DATA_H_
#define _TROMBONELESS_DATA_H_

/* ============================== This is where the types are defined ============================== */

/** Creation of an enum which corresponds to different shift keying
    initial positions which may adjusted by the user through the app. 
    These represent the different types of trombones which may be
    synthesised by calibrating the correct note and octave. */
typedef enum{
    /** There was no chosen type of trombone by the user. */
    SKOpt_EMPTY,
    /** The chosen type of trombone by the user was the piccolo. */                 
    SKOpt_PICCOLO, 
    /** The chosen type of trombone by the user was the soprano. */     
    SKOpt_SOPRANO,              
    /** The chosen type of trombone by the user was the alto. */             
    SKOpt_ALTO,   
    /** The chosen type of trombone by the user was the tenor. */              
    SKOpt_TENOR,     
    /** The chosen type of trombone by the user was the bass. */                
    SKOpt_BASS,
    /** The chosen type of trombone by the user was the contrabass. */           
    SKOpt_CONTRABASS,      
} ShiftKeyingOptions_t;

/**  Creation of an enum which corresponds to the different
    embouchure options that the user may select to calibrate.
    This should be adjusted before release the have the correct
    names and number of options. */
typedef enum{
    /** Option for a no chosen embouchure placement. */
    EMBCR_EMPTY,       
    /** Option for a low embouchure placement. */          
    EMBCR_LowPlacement,
    /** Option for a medium embouchure placement. */        
    EMBCR_MediumPlacement, 
    /** Option for a high embouchure placement. */     
    EMBCR_HighPlacement,        
} EmbachureOptions_t;

/** @brief      A struct which stores all the necessary parameters that may be
 *              changed on the app side effecting the core functionality. This
 *              struct should add a flag which is triggered whenever a parameter
 *              has been changed.
 */
typedef struct{
    /** @brief  The chosen shiftkey option by the user corresponding to the type of trombone. */
    ShiftKeyingOptions_t shiftKeyingOption;

    /** @brief  The nearest distance the user wishes to have their hand from the sensor for pitch bend. */         
    double nearDistance;
    
    /** @brief  The furthest distance the user wishes to have their hand from the sensor for pitch bend. */                            
    double farDistance;   

    /** @brief  The lower pressure gain which the user has chosen for the mouthpiece corresponding to the peak input. */
    double lowPressure;
    /** @brief  The upper pressure gain which the user has chosen for the mouthpiece corresponding to the peak input. */                             
    double highPressure;                            
    /** @brief   The chosen embouchure option which the user wishes to calibrate.   */
    EmbachureOptions_t embouchureOption;           
    /** @brief   High/true when the user has pressed the button to calibrate the Embouchure. */ 
    bool triggerEmbouchureCalibrate;                
} TrombonelessParameters_t;

/** @brief      A struct to store any of the synthesier parameters that may be
 *              changed within the app. A data changed flag variable should be
 *              added to this struct to tell the core to read the new values.
 */
typedef struct{
    /** @brief The variable used to determine when the internal synth and equalizer is used. */
    bool synthEnable; 
    /** @brief The chosen gains for the various frequency bands in the equalizer. */                                 
    float gains[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  
} SynthesiserParameters_t;

/*  Creating an instance of the trombonelessParameters_t struct to 
    store the active parameter variables. */
extern TrombonelessParameters_t trombonelessParameters; /** The struct instance storing all the data used to interface between calibrating the Tromboneless from the App to the Core. */

/*  Creating an instance of the SynthethiserParameters_t struct to
    store the active parameter variables. */
extern SynthesiserParameters_t synthesiserParameters;   /** The struct instance storing all the data used to interface between the internal synthesiser and the App. */

#endif