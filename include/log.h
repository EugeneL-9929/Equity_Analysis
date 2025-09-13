#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;

namespace Log
{
    struct LogJson
    {
    public:
        LogJson(const string &logPath); // show current state
        void update(); // every observable increases by 1
        void addObservable(const string &newEquity, int counter);
        void deleteObservable(const string &oldEquity);
        nlohmann::json logJson(const string &logPath, const vector<string> &newEquity = {});
        ~LogJson(); // show updated state

        nlohmann::json currentState{};

    private:
        const string logPath;
    };

    nlohmann::json logJson(const string &logPath, const vector<string> &newEquity = {});
}

#endif