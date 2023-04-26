//
// Copyright (C) 2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
// 
// GPLv3
//
// This file is part of ZImportFolder.
// 
// ZImportFolder is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// 
// ZImportFolder is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with ZImportFolder. If not, see <https://www.gnu.org/licenses/>.
//

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
