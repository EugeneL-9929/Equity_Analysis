#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
using namespace std;

struct Tree
{
public:
    Tree()
    {
        cout << "" << endl;
    }

    nlohmann::json toJson()
    {
        nlohmann::json dataJson;
        dataJson = {};
    }

    void addChild()
    {
    }

    static Tree loadJson(nlohmann::json dataJson)
    {
    }

private:
    int layerNumber;
    int serialNumber;
    vector<Tree> children{};
};