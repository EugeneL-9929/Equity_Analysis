#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "stock.cpp"

using namespace std;

int main()
{
    cout << "hello, world!" << endl;
    AV::Stock stock{"QQQ"};
    cout << stock.get_marketData() << endl;

    return 0;
}
