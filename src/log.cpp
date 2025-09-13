#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "../include/log.h"

using namespace std;

namespace Log
{
    nlohmann::json logJson(const string &logPath, const vector<string> &newEquity)
    {
        nlohmann::json tempJson;
        nlohmann::json returnJson;
        ifstream infile{logPath};
        if (infile)
            infile >> tempJson;
        for (const auto &data : tempJson.items())
            cout << data.key() << " : " << data.value().get<string>() << endl;
        infile.close();
        ofstream outfile{logPath};
        returnJson = tempJson;
        for (const auto &data : tempJson.items())
        {
            tempJson[data.key()] = to_string(stoi(data.value().get<string>()) + 1);
        }
        if (!newEquity.empty())
        {
            string temp;
            for (size_t i = 0; i < newEquity.size(); i++)
            {
                cout << "input a log value for " << newEquity[i] << ": " << endl;
                cin >> temp;
                tempJson[newEquity[i]] = temp;
            }
        }
        outfile << tempJson.dump(4);
        outfile.close();
        return returnJson;
    }
}
