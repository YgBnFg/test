#include <json/json.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
std::vector<std::vector<float>> p;
int main() {
    for (int i = 0; i < 3; i++) {
        p.push_back({ 1,2,3 });
    }
    std::ifstream jsonFile("./param.json", std::ios::in);
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(jsonFile, root)) {
        std::cout << "cont parse" << std::endl;
        jsonFile.close();
        return 0;
    }
    jsonFile.close();
    std::string serialNumber = root["serialNumber"].asString();
    std::cout << serialNumber << std::endl;
    auto &thisSensor = root[serialNumber];
    if (!thisSensor.isNull()) {
        auto cameraMatrix = thisSensor["cameraMatrix"];
        auto scale = thisSensor["scale"].asFloat();
        for (auto &r:cameraMatrix) {
            for (int i = 0; i < r.size(); i++) {
                std::cout << r[i].asFloat() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << scale << std::endl;
    }
    else {
        auto &cameraMatrix = thisSensor["cameraMatrix"];
        thisSensor["scale"]= 0.1234556788;
        for (auto x : p) {
            Json::Value y;
            for (int i = 0; i < x.size(); i++) {
                y.append(x[i]);
                std::cout << x[i] << std::endl;
            }
            cameraMatrix.append(y);
        }
    }
    std::ofstream outFile("./param.json", std::ios::out);
    Json::StyledWriter   writer;
    std::string tmp = writer.write(root);
    outFile << tmp;
    outFile.close();
    getchar();
    return 0;
}