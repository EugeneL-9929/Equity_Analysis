#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "stock.cpp"
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
    sessionJson.addObservable("QQQ", 0);
    sessionJson.addObservable("SPY", 0);
    sessionJson.addObservable("DIA", 0);
    sessionJson.addObservable("GLDM", 0);
    sessionJson.addObservable("NVDA", 0);
    sessionJson.addObservable("AAPL", 0);
    for (const auto &data : sessionJson.currentState.items())
    {
        if (stoi(data.value().get<string>()) == 0)
        {
            AV::Stock *stockPtr = new AV::Stock{data.key()};
            db.addStockTable(stockPtr->getMarketData(), data.key());
            delete stockPtr;
        }
    }
    sessionJson.update();

    /*
    nlohmann::json session = Log::logJson("Log.json");
    cout << session << endl;

    AV::Stock stockQQQ{"QQQ"};
    db.addStockTable(stockQQQ.getMarketData(), "QQQ");

    AV::Stock stockSPY{"SPY"};
    db.addStockTable(stockSPY.getMarketData(), "SPY");

    AV::Stock stockDIA{"DIA"};
    db.addStockTable(stockDIA.getMarketData(), "DIA");

    AV::Stock stockGLDM{"GLDM"};
    db.addStockTable(stockGLDM.getMarketData(), "GLDM");
    */

    return 0;
}
