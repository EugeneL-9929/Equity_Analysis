#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "nlohmann/json.hpp"
using namespace std;

struct Content
{
    nlohmann::json toJson()
    {
        nlohmann::json backup{};
        return backup;
    }
};

struct NodeN2N
{
public:
    NodeN2N(const int &layerNumber, const int &serialNumber) : layerNumber{layerNumber}, serialNumber{serialNumber}
    {
        cout << "Construct node " << layerNumber << "." << serialNumber << "!" << endl;
    }

    int getSerialNumber() { return this->serialNumber; }

    int getLayerNumber() { return this->layerNumber; }

    void addChild(const Content &content)
    {
        NodeN2N *newNode = new NodeN2N{this->layerNumber + 1, this->childCount};
        this->childPtrNodes[to_string(this->layerNumber + 1) + "." + to_string(this->childCount)] = newNode;
        this->childCount++;
    }

    nlohmann::json toJson() const
    {
        nlohmann::json backup;
        backup["layerNumber"] = this->layerNumber;
        backup["serialNumber"] = this->serialNumber;
        for (const auto &node : this->parentPtrNodes)
            backup['parent'][node.first] = node.second->toJson();
        for (const auto &node : this->childPtrNodes)
            backup["children"][node.first] = node.second->toJson();
        return backup;
    }

    void loadJson(const nlohmann::json &reload)
    {
        if (reload.contains("layerNumber"))
        {
            layerNumber = reload["layerNumber"];
        }
        if (reload.contains("serialNumber"))
        {
            serialNumber = reload["serialNumber"];
        }
    }

    map<string, NodeN2N *> parentPtrNodes{};

    ~NodeN2N()
    {
        cout << "Destruct node" << layerNumber << "." << serialNumber << endl;
    }

private:
    int layerNumber;
    int serialNumber;
    int childCount{};
    map<string, NodeN2N *> childPtrNodes{};
    Content nodeContent;
};

struct Net
{
public:
    Net()
    {
        cout << "Construct net structure!" << endl;
    }

private:
};

struct Tree
{
public:
    Tree(int layerNumber, int serialNumber) : layerNumber{layerNumber}, serialNumber{serialNumber}, parentNode{nullptr}
    {
        cout << "" << endl;
    }

    nlohmann::json toJson()
    {
        nlohmann::json dataJson;
        dataJson = {};
        return dataJson;
    }

    void addChild(const Tree &childNode)
    {
        this->children.push_back();
    }

    static Tree loadJson(nlohmann::json dataJson)
    {
        Tree rootNode{dataJson["layerNumber"]};
        return rootNode;
    }

private:
    int layerNumber;
    int serialNumber;
    Tree *parentNode;
    vector<Tree> children{};
    vector<Tree> layerRepeated{};
};