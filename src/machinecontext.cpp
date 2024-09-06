/*
// Copyright (c) 2024 Hewlett Packard Enterprise
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "machinecontext.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

namespace DTParse {

static constexpr const char* node_base_path = "/proc/device-tree/"; 

enum SupportedNodes
{
    model,
    serial_number
};

static const std::map<SupportedNodes, std::string> watched_nodes = {
        {SupportedNodes::model, "model"},
        {SupportedNodes::serial_number, "serial-number"} };
};

using namespace DTParse;

void MachineContext::populateMachineContext()
{

    // walk supported node paths
    for (std::pair<SupportedNodes, std::string> node_data : watched_nodes)
    {
        std::string node_value_str;

        std::string node_rel_path = node_data.second;
        std::string node_full_path = node_base_path + node_rel_path;

        std::ifstream vpd_stream;

        vpd_stream.open(node_full_path);

        if (!vpd_stream || !std::getline(vpd_stream, node_value_str))
            continue;

        switch (node_data.first)
        {
            case SupportedNodes::model:
    
                MachineContext::Asset::model(node_value_str);

                break;

            case SupportedNodes::serial_number:

                MachineContext::Asset::serial_number(node_value_str);

                break;

            default:
                break;
        }
    }
};