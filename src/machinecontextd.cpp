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

#include "machinecontextd.hpp"
#include <fstream>

void MachineContext::populateMachineContext()
{
	//walk supported node paths
    for (std::pair<supportedNode, std::string> nodeData : nodePaths)
    {
		std::string nodeValue;
        std::string nodeRelativePath = nodeData.second;
        std::string nodeFullPath = nodeBasePath + nodeRelativePath;
        
        std::ifstream fruStream;
        fruStream.open(nodeFullPath);
           
		if (!fruStream || !std::getline(fruStream, nodeValue))
            continue;

		switch ((supportedNode)nodeData.first)
		{
			case supportedNode::model:
				MachineContext::model(nodeValue);
				break;
			case supportedNode::serial_number:
				MachineContext::serial_number(nodeValue);
				break;
			case supportedNode::mac_address:
				MachineContext::mac_address(nodeValue);
				break;
			case supportedNode::local_mac_address:
				MachineContext::local_mac_address(nodeValue);
				break;
			default:
				break;
		} 
    }
};

int main()
{
	constexpr auto path = MachineContext::instance_path;
	sdbusplus::async::context ctx;
	
	sdbusplus::server::manager_t manager{ctx, path};
	MachineContext c{ctx, path};
	
	ctx.spawn([](sdbusplus::async::context& ctx) -> sdbusplus::async::task<> {
        ctx.request_name(MachineContext::default_service);
        co_return;
    }(ctx));
	
        ctx.run();

    return 0;
}