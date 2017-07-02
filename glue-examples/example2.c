#include "glue/registration.h"

struct StepDef {
    struct StepInfo *info;
    void (*body)();
};

// naming might be constructed as something_fileidfromcmake_linenumber

/* MACRO STARTS HERE */
struct StepInfo s1_i = {
    .position = "/.../filename.c:42", // __FILE__:__LINE__
    .expressionType = "regex",
    .expression = "I eat (d+) cukes" // macro param
};

void s1_f();

struct StepDef s1_d = {
    .info = &s1_i,
    .body = s1_f
};

void s1_f() /* MACRO ENDS HERE */ {
    // this is the test body
}


/*
 * This can be generated by cmake before compiling, defining our function...
 *
 *   ADD_CUCUMBER_C_STEPS(example2.c)
 *
 * ...that creates a file with the steps variable by parsing source files.
 */

struct StepDef *steps[] = {
    &s1_d // , ...
};

/*
 * This must be included with all step library.
 * Compiler-dependent (different between MSVC and the others).
 */

//extern struct StepDef *steps[];

__attribute__((constructor)) void begin() {
    for (struct StepDef **i = steps; *i != 0; ++i)
        registerGlue((*i)->info);
}

__attribute__((destructor)) void end() {
    for (struct StepDef **i = steps; *i != 0; ++i)
        unregisterGlue((*i)->info);
}

/*
 * The library should register itself to be called by ID.
 */