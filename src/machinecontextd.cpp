#include "machinecontextd.hpp"

#include <xyz/openbmc_project/MachineContext/aserver.hpp>
#include <sdbusplus/async.hpp>


#include <iostream>
#include <fstream>

#include <vector>
#include <string_view>

class MachineContext : public sdbusplus::aserver::xyz::openbmc_project::MachineContext<MachineContext>
{
	public:
    explicit MachineContext(sdbusplus::async::context& ctx, auto path) :
        sdbusplus::aserver::xyz::openbmc_project::MachineContext<MachineContext>(ctx, path)
    {
		populateMachineContext();
    }
	
	private:
	auto populateMachineContext()
	{
		std::vector<std::pair<std::string, std::string>> supportedNodes = {
            {"model", "model"},
            {"local-mac-address", "mac1"},
            {"mac-address", "mac2"},
            {"serial-number", "serial-number"}};

        MachineContext::model("N/A");
        //MachineContext::SerialNumber("N/A");
        //MachineContext::macAddress("N/A");
        //localMACAddress("N/A");

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
                    model(nodeValue);
            }
            else if (nodeData.first == "serial-number")
            {
          //      serialNumber(nodeValue);
            }
            else if (nodeData.first == "mac-address")
            {
            //    macAddress(nodeValue);
            }
            else if (nodeData.first == "serial-number")
            {
             //   localMACAddress(nodeValue);
            }
        }
	}
};

int main()
{
	constexpr auto path = MachineContextInstancePath;
	
	sdbusplus::async::context ctx;
	sdbusplus::server::manager_t manager{ctx, path};
	
	MachineContext c{ctx, path};
	
	ctx.spawn([](sdbusplus::async::context& ctx) -> sdbusplus::async::task<> {
        ctx.request_name(MachineContextServiceName);
        co_return;
    }(ctx));
	
        ctx.run();

    return 0;
}
