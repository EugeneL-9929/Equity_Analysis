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

    Database db{"../sqlite/stock.sqlite"};

    vector<string> selector{"QQQ", "SPY", "DIA", "GLDM"};
    Log::logJson("Log.json");

    /*
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
