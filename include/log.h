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
    nlohmann::json logJson(const string &logPath, const vector<string> &newEquity = {});
}

#endif