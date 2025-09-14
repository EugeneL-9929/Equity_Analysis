#include <iostream>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"
#include "../include/stock.h"
#include "../include/fx.h"
using namespace std;

namespace AV
{
    Fx::Fx(const vector<string> name, const string function, const string outputsize, const string interval) : Stock{name[0] + name[1], function, outputsize, ""}, intervalGetPurpose{interval}
    {
        this->name = {{"from_symbol", name[0]}, {"to_symbol", name[1]}};
        string symbol{};
        for (const auto &i : this->name)
            symbol = symbol + i.first + "=" + i.second + "&";
        this->function += function;
        this->outputsize += outputsize;
        string query_url = Stock::api_url + symbol + this->function + this->outputsize + Stock::api_key;
        Stock::resetQueryUrl(query_url);
        cout << "Fx " << name[0] << name[1] << " online!" << endl;
    }

    nlohmann::json Fx::formatMarketData()
    {
        string readBuffer{this->Stock::getMarketData()};
        nlohmann::json returnValue = nlohmann::json::parse(readBuffer)["Time Series FX (" + this->intervalGetPurpose + ")"];
        for (const auto &data : returnValue.items())
        {
            if (data.value().contains("1. open"))
            {
                returnValue[data.key()]["open"] = returnValue[data.key()]["1. open"];
                returnValue[data.key()].erase("1. open");
            }
            if (data.value().contains("2. high"))
            {
                returnValue[data.key()]["high"] = returnValue[data.key()]["2. high"];
                returnValue[data.key()].erase("2. high");
            }
            if (data.value().contains("3. low"))
            {
                returnValue[data.key()]["low"] = returnValue[data.key()]["3. low"];
                returnValue[data.key()].erase("3. low");
            }
            if (data.value().contains("4. close"))
            {
                returnValue[data.key()]["close"] = returnValue[data.key()]["4. close"];
                returnValue[data.key()].erase("4. close");
            }
        }
        return returnValue;
    }

    Fx::~Fx()
    {
        cout << "Fx " << name["from_symbol"] << name["to_symbol"] << " offline!" << endl;
    }

}