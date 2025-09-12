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
    AV::Stock stock{"QQQ"};
    // cout << stock.get_marketData() << endl;
    Database db{"../sqlite/stock.sqlite"};
    return 0;
}
