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

#include <boost/asio/io_service.hpp>
#include <sdbusplus/asio/object_server.hpp>

static constexpr const char* DevTreeDaemonServiceName =
    "xyz.openbmc_project.U_Boot.Environment.DevTreeDaemon";
static constexpr const char* DevTreeDaemonIface =
    "xyz.openbmc_project.U_Boot.Environment.DevTreeDaemon";
static constexpr const char* DevTreeDaemonPath =
    "/xyz/openbmc_project/u_boot/environment/dtdaemon";

static constexpr const char* DevTreeVPDIface =
    "xyz.openbmc_project.MachineContext";
static constexpr const char* DevTreeVPDPath =
    "/xyz/openbmc_project/machinecontext";

// maybe have .first be the full path and another field for the property name to
// display as? perhaps as a path?
/*
static constexpr const std::vector<
                        std::pair<std::string, std::string>> nodeInputs = {
{"model", "model"},
                                                                            {"local-mac-address",
"mac1"},
                                                                            {"mac-address",
"mac2"},
                                                                            {"serial-number",
"serial-number"}
                                                                      }
  */

class DevTreeDaemon
{
    boost::asio::io_service& io;
    sdbusplus::asio::object_server& server;
    std::shared_ptr<sdbusplus::asio::connection> conn;
    std::shared_ptr<sdbusplus::asio::dbus_interface> iface;

    // Note: may want to create a function for refreshing data from device-tree
    // (but probably reaaaally don't need it for now, so skip it and consider it
    // for later on)

  public:
    DevTreeDaemon(boost::asio::io_service& io,
                  sdbusplus::asio::object_server& srv,
                  std::shared_ptr<sdbusplus::asio::connection>& conn);
};
