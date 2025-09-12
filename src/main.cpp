#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "stock.cpp"
#include "database.cpp"
#include <sqlite3.h>

using namespace std;



int main()
{
    cout << "hello, world!" << endl;
    AV::Stock stock{"QQQ", "TIME_SERIES_INTRADAY", "compact", "30min"};
    nlohmann::json output = stock.getMarketData();
    cout << output << endl;
    Database db{"../sqlite/stock.sqlite"};
    db.addStockTable(output, "QQQ");
    return 0;
}
