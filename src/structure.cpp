#include <iostream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
using namespace std;

struct TreeNode
{
public:
    TreeNode(string value) {}
};

struct Tree
{
public:
    Tree(int layerNumber, int serialNumber)
    {
        cout << "" << endl;
    }

    nlohmann::json toJson()
    {
        nlohmann::json dataJson;
        dataJson = {};
        return dataJson;
    }

    void addChild()
    {
    }

    static TreeNode loadJson(nlohmann::json dataJson)
    {
        TreeNode rootNode{dataJson["layerNumber"]};
        return rootNode;
    }

private:
    int layerNumber;
    int serialNumber;
    TreeNode *parentNode;
    vector<Tree> children{};
};