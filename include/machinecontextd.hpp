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

#include <xyz/openbmc_project/MachineContext/aserver.hpp>
#include <sdbusplus/async.hpp>
#include <map>

static constexpr const char* nodeBasePath = "/proc/device-tree/"; 

enum supportedNode { model, 
                      serial_number,
                      local_mac_address,
                      mac_address
};

//map nodes to their path (relative to nodeBasePath)
static const std::map<supportedNode, std::string> nodePaths = { { model, "model" },
                                              { serial_number, "serial-number" },
                                              { local_mac_address, "local-mac-address" },
                                              { mac_address, "mac-address" } 
                                              };

class MachineContext : public sdbusplus::aserver::xyz::openbmc_project::MachineContext<MachineContext>
{
	public:
    explicit MachineContext(sdbusplus::async::context& ctx, auto path) :
        sdbusplus::aserver::xyz::openbmc_project::MachineContext<MachineContext>(ctx, path)
    {
		populateMachineContext();
    }
	
	void populateMachineContext();
};

