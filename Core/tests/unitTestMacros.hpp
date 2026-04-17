/** @file           unitTestMacros.hpp
 *  @author         @RyanMcB8
 *  @brief          This file defines a set of macros which may be called upon
 *                  to test various different tests for the Tromboneless App.
 */

 /* Adding the necessary libraries and headers.  */
 #include <stdio.h>
 #include <iostream>
 


/*  Set/get test for int type. */
#define SET_GET_INT(object, setter, getter, newValue, className, result)\
{                                                                   \
    int original = object.getter();                                 \
    if ((int)newValue == original) {                                \
        newValue = newValue + 1;                                              \
    }                                                               \
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

#define SET_GET_INT(object, setter, getter, newValue, className, result)\
{                                                                   \
    int original = object.getter();                                 \
    if ((int)newValue == original) {                                \
        newValue = newValue + 1;                                              \
    }                                                               \
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
    (std::fabs(arg1 - arg2) <  0.000001)

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
do { \
    float original = object.getter(); \
    if (APPROX_EQUAL((newValue), original)) { \
        (newValue) += 0.1f; \
    } \
    object.setter((float)(newValue)); \
    float updated = object.getter(); \
    bool failed = (!APPROX_EQUAL(updated, (float)(newValue)) || \
                   APPROX_EQUAL(updated, original)); \
    if (failed) { \
        std::cerr << "[FAIL] " #setter "/" #getter " in " #className " failed\n"; \
        result &= false; \
    } else { \
        result &= true; \
    } } while(0);

/*  End of file. */