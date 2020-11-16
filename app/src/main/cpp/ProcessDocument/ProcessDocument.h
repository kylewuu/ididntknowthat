//
// Created by kyle- on 2020-10-19.
//
#include <iostream>
#include <string>
#include <jni.h>

#ifndef TFK_MAIN_H
#define TFK_MAIN_H

using std::string;

class ProcessDocument {
public:
    ProcessDocument(string x);
    string mainLoop();
    string summarizer();
    void removeSquareBrackets();
    void replace(string original, string replacement);
    void removeWhiteSpace();
    void removeNewLines();
    void breakDownSentences();
    void countWordFrequency();
    void rankSentences();
};


#endif //TFK_MAIN_H