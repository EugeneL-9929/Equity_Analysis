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
    sessionJson.addObservable("QQQ", 0);
    sessionJson.addObservable("SPY", 0);
    sessionJson.addObservable("DIA", 0);
    sessionJson.addObservable("GLDM", 25);
    sessionJson.addObservable("NVDA", 25);
    sessionJson.addObservable("AAPL", 25);
    sessionFxUSDJson.addObservable("JPY", 0);
    sessionFxUSDJson.addObservable("EUR", 0);
    sessionFxUSDJson.addObservable("GBP", 0);
    sessionFxUSDJson.addObservable("CNY", 0);
    sessionFxUSDJson.addObservable("AUD", 0);
    sessionFxUSDJson.addObservable("CHF", 25);
    sessionFxUSDJson.addObservable("CAD", 25);
    sessionFxUSDJson.addObservable("SGD", 25);
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
            db.addFxTable(fxPtr->formatMarketData(), data.key());
            delete fxPtr;
        }
    }
    sessionJson.update();
    sessionFxUSDJson.update();
    
    return 0;
}
