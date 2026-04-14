/** @file           unitTestMacros.hpp
 *  @author         @RyanMcB8
 *  @brief          This file defines a set of macros which may be called upon
 *                  to test various different tests for the Tromboneless App.
 */

 /* Adding the necessary libraries and headers.  */
 #include <stdio.h>
 #include <iostream>
 #include <juce_core/juce_core.h>
 #include <juce_gui_basics/juce_gui_basics.h>



/** @brief                  Set/get test for float types.
 *  @param  object          The object which is being iterated/referenced.
 *  @param  setter          The name of the method which is used to set the
 *                          new value of the private variable.
 *  @param  getter          The name of the method which is used to get the
 *                          current value of the private variable.
 *  @param  newValue        The value which the variable should be set to.
 *  @param  className       The name of the class which the object is an
 *                          instance of.
 *  @param  result          The name of the variable used to give a pass/fail
 *                          result at the end of the test.
 */  
#define SET_GET_FLOAT(object, setter, getter, newValue, className, result)      \
{                                                                   \
    float original = object.getter();                               \
    if ((float)newValue == original) newValue += 0.1;                 \
    object.setter((float)newValue);                                        \
    float updated = object.getter();                                \
    if (updated != (float)newValue || updated == original)                 \
    {                                                               \
        std::cerr << "[FAIL]    " #setter "/" #getter " in "        \
        #className "failed\n";                                          \
        result &= false;                                             \
    }                                                               \
    else{                                                           \
        result &= true;                                            \
    }                                                               \
}

/*  Set/get test for juce::Colour types. */
#define SET_GET_COLOUR(object, setter, getter, className, result)   \
{                                                                   \
    juce::Colour original = object.getter();                        \
    juce::Colour newValue = juce::Colours::red;                     \
    if (original == juce::Colours::red) newValue = juce::Colours::blue;\
    object.setter(newValue);                                        \
    juce::Colour updated = object.getter();                         \
    if (updated != newValue || updated == original)                 \
    {                                                               \
        std::cerr << "[FAIL]    " #setter "/" #getter " in "        \
        #className "failed\n";                                      \
        result &= false;                                            \
    }                                                               \
    else{                                                           \
        result &= true;                                             \
    }                                                               \
}

/*  Set/get test for int type. */
#define SET_GET_INT(object, setter, getter, newValue, className, result)\
{                                                                   \
    int original = object.getter();                                 \
    if ((int)newValue == original) newValue += 1;                 \
    object.setter(newValue);                                        \
    int updated = object.getter();                                  \
    if (updated != newValue || updated == original)                 \
    {                                                               \
        std::cerr << "[FAIL]    " #setter "/" #getter " in "        \
        #className "failed\n";                                      \
        result &= false;                                            \
    }                                                               \
    else{                                                           \
        result &= true;                                             \
    }                                                               \
}
#define APPROX_EQUAL(arg1, arg2)                                    \
{                                                                   \
    float epsilon = 0.0000001;                                      \
    ((arg1 + epilson) > arg2 && (arg1 - epsilon) < arg2) ? true : false; \
}                                                                   \