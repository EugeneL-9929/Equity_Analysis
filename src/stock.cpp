#include <iostream>
#include <string>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
using namespace std;

namespace AV
{
    // the data capture functions of this namespace are built with the service of alpha vantage.
    struct Stock
    {
    public:
        Stock(const string name, const string function = "TIME_SERIES_INTRADAY", const string outputsize = "full", const string interval = "5min") : name{name}
        {
            this->function += function;
            this->symbol += name;
            this->outputsize += outputsize;
            this->interval += interval;
            this->query_url = this->api_url + this->symbol + this->function + this->outputsize + this->interval + this->api_key;
            cout << query_url << endl;
            cout << "Stock " << name << " online!" << endl;
        }

        Stock &operator=(const Stock &other)
        {
            cout << "Stock " << other.name << " being copied and assigned succesfully!" << endl;
            if (this == &other)
            {
                return *this;
            }
            this->name = other.name;
            this->symbol = other.symbol;
            this->function = other.function;
            this->outputsize = other.outputsize;
            this->interval = other.interval;
            this->api_key = other.api_key;
            this->query_url = other.query_url;
            return *this;
        }

        Stock(const Stock &other) : name{other.name}
        {
            cout << "Stock " << other.name << " being copied successfully!" << endl; 
        }

        string get_marketData()
        {
            string readBuffer;
            CURL *curl;
            CURLcode res;

            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (curl)
            {
                curl_easy_setopt(curl, CURLOPT_URL, this->query_url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res != CURLE_OK)
                {
                    cerr << "Infomations: " << curl_easy_strerror(res) << endl;
                    return "";
                }
            }
            else
            {
                cout << "Initialize CURL failed!" << endl;
                return "";
            }

            cout << readBuffer << endl;

            return readBuffer;
        }

        string get_name() const
        {
            return this->name;
        }

        void set_apiKey(const string &new_api_key)
        {
            this->api_key = "apikey=" + new_api_key;
        }

        ~Stock()
        {
            cout << "Stock " << name << " offline!" << endl;
        }

    private:
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *userp)
        {
            size_t totalSize = size * nmemb;
            userp->append((char *)contents, totalSize);
            return totalSize;
        }

        string name;
        string symbol{"symbol="};
        string function{"&function="};
        string outputsize{"&outputsize="};
        string interval{"&interval="};
        string api_key = "&apikey=O9QU43NA2E4L5451";
        const string api_url = "https://www.alphavantage.co/query?";
        string query_url;
    };

    struct Fx
    {
    public:
        Fx(const string name) : name{name}
        {
            cout << "Fx " << name << " online!" << endl;
        }

        ~Fx()
        {
            cout << "Fx " << name << " offline!" << endl;
        }

    private:
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *userp)
        {
            size_t totalSize = size * nmemb;
            userp->append((char *)contents, totalSize);
            return totalSize;
        }

        string name;
        string symbol{"symbol="};
        string function{"function="};
        const string api_url = "https://www.alphavantage.co/query?";
    };

}