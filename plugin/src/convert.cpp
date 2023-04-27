//
// Copyright (C) 2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
// 
// GPLv3
//
// This file is part of importFolder.
// 
// importFolder is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// 
// importFolder is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with importFolder. If not, see <https://www.gnu.org/licenses/>.
//

#include "convert.h"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

#include "igl/read_triangle_mesh.h"
#include "igl/remove_duplicate_vertices.h"
#include "igl/for_each.h"
#include "writeGoZFile.h"

extern "C" DLLEXPORT float convert(char *someText, double optValue, char *outputBuffer, int optBuffer1Size, char *pOptBuffer2, int optBuffer2Size, char **zData)
{
    ////
    // parse parameter (JSON)
    fs::path jsonPath(someText);
    std::ifstream ifs(jsonPath);
    nlohmann::json json = nlohmann::json::parse(ifs);
    ifs.close();

    const std::string rootString = json.at("root").get<std::string>();
    const fs::path rootPath(rootString);
    fs::path dataDirectory(rootPath);
    dataDirectory /= "data";
    fs::create_directories(dataDirectory);

    const std::string targetDirectoryString = json.at("targetDirectory").get<std::string>();
    const fs::path targetDirectory(targetDirectoryString);

    // we use lowercases
    const auto needImport = [&json](const std::string &extString)
    {
        return json.at("import").contains(extString) && json.at("import").at(extString).get<bool>();
    };

    for (const fs::directory_entry &entry : fs::directory_iterator(targetDirectory))
    {
        const fs::path ext = entry.path().extension();
        std::string extString = ext.string();
        std::transform(extString.begin(), extString.end(), extString.begin(), ::tolower);

        std::string destFileName = entry.path().stem().string();
        destFileName += ".GoZ";
        fs::path destPath(dataDirectory);
        destPath /= destFileName;

        if (extString == ".goz")
        {
            if (needImport(".goz"))
            {
                // no conversion is required.
                fs::copy_file(entry.path(), destPath);
            }
        }
        else
        {
            if (needImport(extString))
            {
                Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> V;
                Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> F;
                // currently, we only support its geometry
                FILE *fp = fopen(entry.path().string().c_str(), "rb");
                igl::read_triangle_mesh(extString.substr(1), fp, V, F);

                if (extString == ".stl")
                {
                    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> temp_V = std::move(V);
                    Eigen::Matrix<int, Eigen::Dynamic, 1> SVI, SVJ;
                    igl::remove_duplicate_vertices(temp_V, 0, V, SVI, SVJ);
                    std::for_each(F.data(), F.data() + F.size(), [&SVJ](int &f)
                                  { f = SVJ(f); });
                }

                std::vector<std::vector<float>> VVec;
                std::vector<std::vector<int>> FVec;
                VVec.resize(V.rows(), std::vector<float>(V.cols()));
                for (int vIdx = 0; vIdx < V.rows(); ++vIdx)
                {
                    for (int xyz = 0; xyz < V.cols(); ++xyz)
                    {
                        VVec.at(vIdx).at(xyz) = V(vIdx, xyz);
                    }
                }
                FVec.resize(F.rows(), std::vector<int>(F.cols()));
                for (int fIdx = 0; fIdx < F.rows(); ++fIdx)
                {
                    for (int abc = 0; abc < F.cols(); ++abc)
                    {
                        FVec.at(fIdx).at(abc) = F(fIdx, abc);
                    }
                }
                FromZ::writeGoZFile(destPath.string(), entry.path().stem().string(), VVec, FVec);
            }
        }
    }

    return 1.0f;
}
