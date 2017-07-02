#include "glue/registration.h"

/* MACRO STARTS HERE */

// very simplified... see the current and the C++11 version

class MyGlueCode {
private:
    StepInfo s = {
        .position = "/.../filename.cpp:12",
        .expressionType = "regex",
        .expression = "I eat (d+) cukes"
    };

    void f();

public:
    MyGlueCode() {
        /*
         * In reality this should register the step inside the library
         * before registering the ID with the core.
         */
        registerGlue(&s);
    }

    ~MyGlueCode() {
        unregisterGlue(&s);
    }
};

static MyGlueCode myreg;

void MyGlueCode::f() /* MACRO ENDS HERE */ {
    // this is the test body
}

/*
 * The library should register itself to be called by ID.
 */
