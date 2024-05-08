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

#include "devtree_deviced.hpp"

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>
#include <xyz/openbmc_project/Common/error.hpp>

#include <fstream>
#include <vector>

DevTreeDaemon::DevTreeDaemon(
    boost::asio::io_service& io_, sdbusplus::asio::object_server& srv_,
    std::shared_ptr<sdbusplus::asio::connection>& conn_) :
    io(io_),
    server(srv_), conn(conn_)
{
    iface = server.add_interface(DevTreeDaemonPath, DevTreeDaemonIface);

    std::string nodeBasePath = "/sys/";
    std::string nodeFullPath; // will hold an absolute path to the node
    std::string nodeHandle;   // TODO: for filename alone (strip out everything
                            // except whatever is inside a pair of '/' marks,
                            // explicit or implied)

    std::vector<std::pair<std::string, std::string>> nodeInputs = {
        {"model", "model"},
        {"local-mac-address", "mac1"},
        {"mac-address", "mac2"},
        {"serial-number", "serial-number"}};

    // iterate through all supported nodes
    for (std::pair<std::string, std::string> nodeData :
         nodeInputs) // so we can modify the contents of nodeData
    {
        std::string nodeRelativePath = nodeData.first;
        std::string nodeValue;
        nodeFullPath = nodeBasePath + nodeRelativePath;

        // TODO: strip any '/' character from the end of the string, then strip
        // everything up to the 'last' / in the string this code will work fine
        // as-is as long as only 'simple' node paths are used
        nodeHandle =
            nodeRelativePath; // temporary, replace this later with something
                              // that actually looks at the string and strips
                              // things out (for more complex paths)

        std::ifstream fruStream;
        fruStream.open(nodeFullPath);
        if (!fruStream || !std::getline(fruStream, nodeValue))
            continue;

        std::string dBuspropertyName = nodeData.second;
        iface->register_property(dBuspropertyName, nodeValue);

        // if file exists, then read string @ first, then write data to @ secon
    }

    iface->initialize(true);
}

int main()
{
    boost::asio::io_service io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name(DevTreeDaemonServiceName);
    sdbusplus::asio::object_server server(conn);

    DevTreeDaemon devTreeDaemon(io, server, conn);

    io.run();

    return 0;
}
