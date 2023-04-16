#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "convert.h"

int main()
{
    char buf[255];
    char **zData = nullptr;

    nlohmann::json json;
    json["root"] = "example";
    json["targetDirectory"] = "example";
    json["import"] = nlohmann::json::object();
    json["import"][".goz"] = true;
    json["import"][".obj"] = true;
    json["import"][".mesh"] = false;
    json["import"][".ply"] = false;
    json["import"][".stl"] = true;
    json["import"][".off"] = true;
    json["import"][".wrl"] = true;

    std::ofstream ofs("example/parameters.txt");
    ofs << json.dump(4) << std::endl;
    ofs.close();

    convert("example/parameters.txt", 0.0, buf, 0, buf, 0, zData);
    return 0;
}
