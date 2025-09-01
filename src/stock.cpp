#include <iostream>
#include <string>
#include "curl/curl.h"
using namespace std;

namespace AV{
    struct Stock{
        public:
            Stock(const string name, const string function="TIMES_SERIES_INTRADAY") : name{name} {
                this->function = "function=" + function;
                cout << "Stock " << name << " online!" <<endl;
            }
            string get_name(){
                return this->name;
            } 
            ~Stock(){
                cout << "Stock " << name << " offline!" << endl;
            } 
        private:
            string name;
            string function{};
            string api_url = "https://www.alphavantage.co/query?";
        };

}