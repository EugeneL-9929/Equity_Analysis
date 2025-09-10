#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "nlohmann/json.hpp"
using namespace std;

struct Content
{
public:
    nlohmann::json toJson() const
    {
        nlohmann::json backup{};
        return backup;
    }

    void loadJson(const nlohmann::json &reload)
    {
    }
};

struct NodeN2N
{
public:
    NodeN2N() { cout << "Loading structure..." << endl; }

    NodeN2N(const int &layerNumber, const int &serialNumber) : layerNumber{layerNumber}, serialNumber{serialNumber}
    {
        cout << "Construct node " << layerNumber << "." << serialNumber << "!" << endl;
    }

    int getSerialNumber() { return this->serialNumber; }

    int getLayerNumber() { return this->layerNumber; }

    void addChild()
    {
        NodeN2N *newNode = new NodeN2N{this->layerNumber + 1, this->childCount};
        this->childPtrNodes[to_string(this->layerNumber + 1) + "." + to_string(this->childCount)] = newNode;
        this->childCount++;
        newNode->parentPtrNodes[to_string(this->layerNumber) + "." + to_string(this->serialNumber)] = this;
    }

    void addChild(NodeN2N *node)
    {
        this->childPtrNodes[to_string(this->layerNumber + 1) + "." + to_string(this->childCount)] = node;
        this->childCount++;
        node->parentPtrNodes[to_string(this->layerNumber) + "." + to_string(this->serialNumber)] = this;
    }

    nlohmann::json toJson() const
    {
        nlohmann::json backup;
        backup["layerNumber"] = this->layerNumber;
        backup["serialNumber"] = this->serialNumber;
        backup["content"] = this->nodeContent.toJson();
        for (const auto &node : this->childPtrNodes)
            backup["children"][node.first] = node.second->toJson();
        return backup;
    }

    void loadJson(const nlohmann::json &reload)
    {
        this->layerNumber = reload["layerNumber"];
        this->serialNumber = reload["serialNumber"];
        this->nodeContent.loadJson(reload["content"]);
        for (const auto &json : reload["children"].items())
        {
            NodeN2N *childNode = new NodeN2N{};
            childNode->loadJson(json.value());
            this->addChild(childNode);
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
