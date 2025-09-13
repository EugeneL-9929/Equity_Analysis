#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "../include/log.h"

using namespace std;

namespace Log
{
    LogJson::LogJson(const string &logPath) : logPath{logPath}
    {
        ifstream infile{logPath};
        if (infile)
            infile >> this->currentState;
        else
        {
            cout << "empty json log!" << endl;
            ofstream outfile{logPath};
            outfile.close();
        }
        for (const auto &data : this->currentState.items())
            cout << data.key() << " : " << data.value().get<string>() << endl;
        infile.close();
    }

    LogJson::~LogJson()
    {
        for (const auto &data : this->currentState.items())
            cout << data.key() << " : " << data.value().get<string>() << endl;
    }

    void LogJson::update()
    {
        ofstream outfile{this->logPath};
        for (const auto &data : this->currentState.items())
            this->currentState[data.key()] = to_string(stoi(data.value().get<string>()) + 1);
        outfile << this->currentState.dump(4);
        outfile.close();
    }

    void LogJson::addObservable(const string &newEquity, int counter)
    {
        ofstream outfile(this->logPath);
        if (!this->currentState.contains(newEquity))
            this->currentState[newEquity] = to_string(counter);
        outfile << this->currentState.dump(4);
        outfile.close();
    }

    void LogJson::deleteObservable(const string &oldEquity)
    {
        ofstream outfile(this->logPath);
        if (this->currentState.contains(oldEquity))
            this->currentState.erase(oldEquity);
        else
            cout << oldEquity << " does not exist!" << endl;
        outfile << this->currentState.dump(4);
        outfile.close();
    }

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
