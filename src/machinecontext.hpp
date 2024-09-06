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

#pragma once

#include <sdbusplus/async.hpp>
#include <xyz/openbmc_project/Inventory/Decorator/Asset/aserver.hpp>
#include <xyz/openbmc_project/Inventory/Item/NetworkInterface/aserver.hpp>

#include <map>

    //Q: is it important to have this stuff all in a namespace? Or is anon better? (Check style guide, I think there's info)



// map nodes to their path relative to node_base_path. I think both of these should be inside the class so that they can be swapped out - maybe 
// one per 'base path' we want to read code from? 
// would also be relatively trivial to just include full paths in the implimentation of code

//static const std::map<SupportedNodes, std::string> watched_nodes; //

class MachineContext :
    public sdbusplus::async::server_t<
        MachineContext,
        sdbusplus::aserver::xyz::openbmc_project::inventory::decorator::Asset,
        sdbusplus::aserver::xyz::openbmc_project::inventory::item::
            NetworkInterface>
{
  public:
    explicit MachineContext(sdbusplus::async::context& ctx, auto path) :
        sdbusplus::async::server_t<MachineContext,
                                   sdbusplus::aserver::xyz::openbmc_project::inventory::decorator::Asset,
                                   sdbusplus::aserver::xyz::openbmc_project::inventory::item::NetworkInterface>(ctx, path)
    {
        populateMachineContext();
    }
    //explicit MachineContext(sdbusplus::async::context& ctx, auto path); 

/* //Does build
    explicit MachineContext(sdbusplus::async::context& ctx, auto path) :
        sdbusplus::async::server_t<MachineContext,
                                   sdbusplus::aserver::xyz::openbmc_project::inventory::decorator::Asset,
                                   sdbusplus::aserver::xyz::openbmc_project::inventory::item::NetworkInterface>(ctx, path)
    {
        populateMachineContext();
    }
*/
    void populateMachineContext();

    //is it fine to include this as part of the class? Or would it be better to have this as an 'anonymous' function with no class to it?
    std::string bytesToHexString(
        char* byte_buffer,
        int buffer_size); // takes an array of bytes and returns a
                          // human-readable hex string
};
