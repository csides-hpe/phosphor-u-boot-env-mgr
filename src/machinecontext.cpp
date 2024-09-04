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

//in the future, would like to have this set via config file, possibly 
//with bundles of nodes associated with a given node_base_path
//const char*

namespace DTParse {

constexpr const char* node_base_path = "/proc/device-tree/"; //should this be static? does constexpr apply? what about 'const' on top?
    //static means it could be accessed from outside a class object (so has to be static to be declared up here)
    //constexpr means that we're telling the compiler the value will be set before compiletime, not at runtime
    //const means that the value won't be changing. = "/proc/device-tree/";

enum SupportedNodes
{
    model,
    serial_number,
    local_mac_address
};

static const std::map<SupportedNodes, std::string> watched_nodes = {
        {SupportedNodes::model, "model"},
        {SupportedNodes::serial_number, "serial-number"} };
        
};

using namespace DTParse;
    //{SupportedNodes::local_mac_address, "/soc/??/local-mac-address"}}
    

    //would be trivial to include a check for nodes at deeper paths...
    //but in the future, it could make sense to have entire blocks
    //that only get read if the base-path is present
    //
    //For now, let's assume we only need to read from a few nodes, and 
    //that designing for more is over-engineering.
    //
    //But if we did start caring about reading more (way more?) data from DT,
    //we could move the node_paths and base_path into the class and then
    //create one class for each basepath we care about with a set of associated notes.
    //
    //For now, I think this is fine 

MachineContext::MachineContext(sdbusplus::async::context& ctx, auto path) :
        sdbusplus::async::server_t<MachineContext,
                                   sdbusplus::aserver::xyz::openbmc_project::inventory::decorator::Asset,
                                   sdbusplus::aserver::xyz::openbmc_project::inventory::item::NetworkInterface>(ctx, path)
    {
        populateMachineContext();
    }

void MachineContext::populateMachineContext()
{
    int mac_buffer_size = 6;

    // walk supported node paths
    for (std::pair<SupportedNodes, std::string> node_data : watched_nodes)
    {
        std::string node_value_str;
        char* mac_buffer_bytes;

        std::string node_rel_path = node_data.second;
        std::string node_full_path = node_base_path + node_rel_path;

        std::ifstream vpd_stream;

        switch (node_data.first)
        {
            case SupportedNodes::model:

                vpd_stream.open(node_full_path);

                if (!vpd_stream || !std::getline(vpd_stream, node_value_str))
                    continue;

                MachineContext::Asset::model(node_value_str);

                break;

            case SupportedNodes::serial_number:

                vpd_stream.open(node_full_path);

                if (!vpd_stream || !std::getline(vpd_stream, node_value_str))
                    continue;

                MachineContext::Asset::serial_number(node_value_str);

                break;

            case SupportedNodes::local_mac_address:

                vpd_stream.open(node_full_path, std::fstream::binary);

                if (!vpd_stream)
                    continue;

                mac_buffer_bytes = new char[mac_buffer_size];

                if (!vpd_stream.read(mac_buffer_bytes, mac_buffer_size))
                    continue;

                node_value_str = bytesToHexString(mac_buffer_bytes,
                                                  mac_buffer_size);

                MachineContext::NetworkInterface::mac_address(node_value_str);

                break;

            default:
                break;
        }
    }
};

std::string MachineContext::bytesToHexString(char* byte_buffer, int buffer_size)
{
    std::stringstream hex_val;
    hex_val << "0x";
    hex_val << std::hex;

    for (int i = 0; i < buffer_size; i++)
    {
        hex_val << std::setw(2) << std::setfill('0') << (int)byte_buffer[i];
    }

    return hex_val.str();
};

int main()
{
    constexpr auto path = "/xyz/openbmc_project/Inventory/MachineContext";
    sdbusplus::async::context ctx;

    sdbusplus::server::manager_t manager{ctx, path};

    MachineContext c{ctx, path};

    ctx.spawn([](sdbusplus::async::context& ctx) -> sdbusplus::async::task<> {
        ctx.request_name("xyz.openbmc_project.Inventory.MachineContext");
        co_return;
    }(ctx));

    ctx.run();

    return 0;
};