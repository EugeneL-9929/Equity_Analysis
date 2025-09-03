#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "stock.cpp"


using namespace std;

int main(){
    cout << "hello, world!" << endl;
    AV::Stock stock{"QQQ"};
    stock.get_market_data();
    return 0;
}


