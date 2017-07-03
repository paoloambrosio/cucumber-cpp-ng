#include "glue/registration.h"

#include <iostream>

/* MACRO STARTS HERE */

// very simplified... see the current and the C++11 version

extern "C" void myGlueCodeBody(char *args[], struct ResultReporter *reporter);

StepInfo myGlueCodeInfo = {
        .position = "/.../filename.cpp:12",
        .expressionType = "regex",
        .expression = "I eat (d+) cukes",
        .body = myGlueCodeBody
};

class MyGlueCode {
private:
    void f();

public:
    MyGlueCode() {
        /*
         * In reality this should register the step inside the library
         * before registering the ID with the core.
         */
        registerGlue(&myGlueCodeInfo);
    }

    void body(char *args[], struct ResultReporter *reporter) {
        f(); // of course this is not all it should do
    }

    ~MyGlueCode() {
        unregisterGlue(&myGlueCodeInfo);
    }
};

static MyGlueCode myreg;

void myGlueCodeBody(char *args[], struct ResultReporter *reporter) {
    myreg.body(args, reporter);
}

void MyGlueCode::f() /* MACRO ENDS HERE */ {
    std::cout << "C++ step executed" << std::endl;
}

/*
 * The library should register itself to be called by ID.
 */
