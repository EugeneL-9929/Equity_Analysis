#ifndef FX_H
#define FX_H

#include <iostream>
#include <vector>
#include <map>
#include "../include/stock.h"
#include "../include/fx.h"
using namespace std;

namespace AV
{
    struct Fx : public Stock
    {
    public:
        Fx(const vector<string> name, const string function = "FX_DAILY", const string outputsize = "full", const string interval = "Daily");

        nlohmann::json formatMarketData() override;

        ~Fx();

    private:
        map<string, string> name;
        string function{"function="};
        string outputsize{"&outputsize="};
        string intervalGetPurpose;
    };
}

#endif