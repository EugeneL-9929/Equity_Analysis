#include <iostream>
#include <string>
#include "curl/curl.h"
using namespace std;

namespace AV{
    // the data capture functions of this namespace are built with the service of alpha vantage.
    struct Stock{
        public:
            Stock(const string name, const string function="TIMES_SERIES_INTRADAY") : name{name} {
                this->function = "function=" + function;
                cout << "Stock " << name << " online!" <<endl;
            }

            string get_data(){
                string readBuffer;
                CURL *curl;
                CURLcode res;
                
            }

            string get_name(){
                return this->name;
            } 
            
            void change_api_key(const string &new_api_key){
                this->api_key = new_api_key;
            }

            ~Stock(){
                cout << "Stock " << name << " offline!" << endl;
            } 
        
        private:
            string name;
            string function{};
            const string api_url = "https://www.alphavantage.co/query?";
            string api_key = "O9QU43NA2E4L5451";
        };

}