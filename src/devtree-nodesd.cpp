#include <xyz/openbmc_project/MachineContext/server.hpp>
#include <sdbusplus/server.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

using MachineContext_inherit =
    sdbusplus::server::object_t<sdbusplus::server::xyz::openbmc_project::MachineContext>;

/** Example implementation of net.poettering.Calculator */
struct MachineContext : MachineContext_inherit
{
    /** Constructor */
    MachineContext(sdbusplus::bus_t& bus, const char* path) :
        MachineContext_inherit(bus, path)
    {

        std::vector<std::pair<std::string, std::string>> supportedNodes = {
            {"model", "model"},
            {"local-mac-address", "mac1"},
            {"mac-address", "mac2"},
            {"serial-number", "serial-number"}};

        static constexpr const char* nodeBasePath = "/proc/device-tree/";

        MachineContext::model("N/A");
        MachineContext::serialnumber("N/A");
        MachineContext::maCaddress("N/A");
        MachineContext::localMACaddress("N/A");

        for (std::pair<std::string, std::string> nodeData : supportedNodes)
        {
            std::string nodeRelativePath = nodeData.first;
            std::string nodeFullPath = nodeBasePath + nodeRelativePath;
            std::string nodeValue;

            std::ifstream fruStream;
            fruStream.open(nodeFullPath);
            if (!fruStream || !std::getline(fruStream, nodeValue))
                continue;

            if (nodeData.first == "model")
            {
                    MachineContext::model(nodeValue);
            }
            else if (nodeData.first == "serial-number")
            {
                MachineContext::serialnumber(nodeValue);
            }
            else if (nodeData.first == "mac-address")
            {
                MachineContext::maCaddress(nodeValue);
            }
            else if (nodeData.first == "serial-number")
            {
                MachineContext::localMACaddress(nodeValue);
            }

            // dtree node was sucessfully read, write the data to dbus under given
            // property name
            //std::string dBuspropertyName = nodeData.second;
            //iface->register_property(dBuspropertyName, nodeValue);
        }

        
        //MachineContext::serialnumber("testing"); 
    }
};

int main()
{
    // Create a new bus and affix an object manager for the subtree path we
    // intend to place objects at..
    auto b = sdbusplus::bus::new_default();
    sdbusplus::server::manager_t m{b, MachineContext::instance_path};

    // Reserve the dbus service name : net.poettering.Calculator
    b.request_name(MachineContext::default_service);

    // Create a calculator object at /net/poettering/calculator
    MachineContext m1{b, MachineContext::instance_path};

    // Handle dbus processing forever.
    b.process_loop();
}
