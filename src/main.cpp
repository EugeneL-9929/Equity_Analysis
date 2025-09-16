#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "../include/stock.h"
#include "../include/fx.h"
#include "database.cpp"
#include "../include/log.h"
#include <sqlite3.h>

using namespace std;

int main()
{
    cout << "hello, world!" << endl;
    vector<string> selectorUpdate{"QQQ", "SPY", "DIA", "GLDM"};
    Database db{"../sqlite/stock.sqlite"};
    Log::LogJson sessionJson{"../log/Log.json"};
    Log::LogJson sessionFxUSDJson{"../log/FxUSDLog.json"};
    Log::LogJson sessionFxHKDJson{"../log/FxHKDLog.json"};
    sessionJson.addObservable("TSLA", 0);
    sessionJson.addObservable("MSFT", 0);
    sessionJson.addObservable("CRCL", 0);
    sessionJson.addObservable("NFLX", 0);
    sessionJson.addObservable("SNPS", 0);
    
    sessionFxUSDJson.addObservable("JPY", 4);

    sessionFxHKDJson.addObservable("JPY", 0);

    for (const auto &data : sessionJson.currentState.items())
    {
        if (stoi(data.value().get<string>()) == 0)
        {
            AV::Stock *stockPtr = new AV::Stock{data.key()};
            db.addStockTable(stockPtr->formatMarketData(), data.key());
            delete stockPtr;
        }
    }

    for (const auto &data : sessionFxUSDJson.currentState.items())
    {
        if (stoi(data.value().get<string>()) == 0)
        {
            vector<string> name{data.key(), "USD"};
            AV::Fx *fxPtr = new AV::Fx{name};
            db.addFxTable(fxPtr->formatMarketData(), data.key()+"USD");
            delete fxPtr;
        }
    }

    for (const auto &data : sessionFxHKDJson.currentState.items())
    {
        if (stoi(data.value().get<string>()) == 0)
        {
            vector<string> name{data.key(), "HKD"};
            AV::Fx *fxPtr = new AV::Fx{name};
            db.addFxTable(fxPtr->formatMarketData(), data.key()+"HKD");
            delete fxPtr;
        }
    }
    sessionJson.update();
    sessionFxUSDJson.update();
    sessionFxHKDJson.update();

    return 0;
}
