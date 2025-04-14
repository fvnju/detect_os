#pragma once

#include <string>
#include <vector>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#endif

class SystemInfo {
public:
    struct CPUInfo {
        std::string name;
        std::string manufacturer;
        int numberOfCores;
        int numberOfLogicalProcessors;
    };

    struct OSInfo {
        std::string name;
        std::string version;
        std::string buildNumber;
        std::string architecture;
    };

    struct MemoryInfo {
        uint64_t totalPhysicalMemory;
        uint64_t availablePhysicalMemory;
        uint64_t totalVirtualMemory;
        uint64_t availableVirtualMemory;
    };

    static CPUInfo getCPUInfo();
    static OSInfo getOSInfo();
    static MemoryInfo getMemoryInfo();
    static std::vector<std::string> getDiskDrives();
    static std::string getComputerName();
    static std::string getUserName();

private:
#ifdef _WIN32
    static std::string getWMIValue(IWbemClassObject* pclsObj, const wchar_t* property);
    static void initializeCOM();
    static void cleanupCOM();
#endif
};
