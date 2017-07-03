#ifndef CUCUMBER_CPP_GLUE_REGISTRATION_H
#define CUCUMBER_CPP_GLUE_REGISTRATION_H

#ifdef __cplusplus
#include <boost/dll.hpp>
extern "C" {
#else
#define BOOST_SYMBOL_EXPORT
#endif

/*
 * Let's invert the control here again. We can define a method
 * that is called by the library when it is loaded, and another
 * to be called when it is unloaded.
 *
 * In C++ this is easy with statics, but what about C?
 * https://stackoverflow.com/questions/22763945/dll-main-on-windows-vs-attribute-constructor-entry-points-on-linux
 */

struct ResultReporter {
//    void (*passed)(); // default if we don't call any other!
    void (*failed)(char *position, char *description);
    void (*skipped)(char *example);
};

struct StepInfo {
    const char *position;       // e.g. "/.../filename.cpp:12"
    const char *expressionType; // e.g. "regex"
    const char *expression;     // e.g. "I eat (d+) cukes"
    void (*body)(char *[], struct ResultReporter *);
};

/**
 * Register library-defined step
 */
BOOST_SYMBOL_EXPORT void registerGlue(struct StepInfo *step);

/**
 * Unregister library-defined step
 */
BOOST_SYMBOL_EXPORT void unregisterGlue(struct StepInfo *step);

#ifdef __cplusplus
}
#endif


#endif //CUCUMBER_CPP_GLUE_REGISTRATION_H
