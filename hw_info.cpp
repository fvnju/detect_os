#include "hw_info.hpp"
#include <iostream>
#include <sstream>

#ifdef _WIN32
std::string SystemInfo::getWMIValue(IWbemClassObject* pclsObj, const wchar_t* property) {
    VARIANT vtProp;
    VariantInit(&vtProp);
    HRESULT hr = pclsObj->Get(property, 0, &vtProp, 0, 0);
    if (SUCCEEDED(hr)) {
        if (vtProp.vt == VT_BSTR) {
            std::wstring wstr(vtProp.bstrVal);
            std::string str(wstr.begin(), wstr.end());
            VariantClear(&vtProp);
            return str;
        }
    }
    VariantClear(&vtProp);
    return "";
}

void SystemInfo::initializeCOM() {
    CoInitializeEx(0, COINIT_MULTITHREADED);
    CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
}

void SystemInfo::cleanupCOM() {
    CoUninitialize();
}

SystemInfo::CPUInfo SystemInfo::getCPUInfo() {
    CPUInfo info = {"Unknown", "Unknown", 0, 0};  // Initialize with default values
    initializeCOM();

    IWbemLocator* pLoc = NULL;
    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (SUCCEEDED(hr)) {
        IWbemServices* pSvc = NULL;
        BSTR strNamespace = SysAllocString(L"ROOT\\CIMV2");
        hr = pLoc->ConnectServer(strNamespace, NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
        SysFreeString(strNamespace);
        
        if (SUCCEEDED(hr)) {
            hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
            if (SUCCEEDED(hr)) {
                IEnumWbemClassObject* pEnumerator = NULL;
                BSTR strQuery = SysAllocString(L"SELECT * FROM Win32_Processor");
                hr = pSvc->ExecQuery(SysAllocString(L"WQL"), strQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
                SysFreeString(strQuery);
                
                if (SUCCEEDED(hr)) {
                    IWbemClassObject* pclsObj = NULL;
                    ULONG uReturn = 0;
                    while (pEnumerator) {
                        hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                        if (uReturn == 0) break;
                        
                        info.name = getWMIValue(pclsObj, L"Name");
                        info.manufacturer = getWMIValue(pclsObj, L"Manufacturer");
                        
                        try {
                            std::string cores = getWMIValue(pclsObj, L"NumberOfCores");
                            std::string logical = getWMIValue(pclsObj, L"NumberOfLogicalProcessors");
                            if (!cores.empty()) info.numberOfCores = std::stoi(cores);
                            if (!logical.empty()) info.numberOfLogicalProcessors = std::stoi(logical);
                        } catch (const std::exception&) {
                            // Keep default values if conversion fails
                        }
                        
                        pclsObj->Release();
                        break;
                    }
                    pEnumerator->Release();
                }
            }
            pSvc->Release();
        }
        pLoc->Release();
    }
    cleanupCOM();
    return info;
}

SystemInfo::OSInfo SystemInfo::getOSInfo() {
    OSInfo info;
    initializeCOM();

    IWbemLocator* pLoc = NULL;
    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (SUCCEEDED(hr)) {
        IWbemServices* pSvc = NULL;
        BSTR strNamespace = SysAllocString(L"ROOT\\CIMV2");
        hr = pLoc->ConnectServer(strNamespace, NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
        SysFreeString(strNamespace);
        
        if (SUCCEEDED(hr)) {
            hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
            if (SUCCEEDED(hr)) {
                IEnumWbemClassObject* pEnumerator = NULL;
                BSTR strQuery = SysAllocString(L"SELECT * FROM Win32_OperatingSystem");
                hr = pSvc->ExecQuery(SysAllocString(L"WQL"), strQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
                SysFreeString(strQuery);
                
                if (SUCCEEDED(hr)) {
                    IWbemClassObject* pclsObj = NULL;
                    ULONG uReturn = 0;
                    while (pEnumerator) {
                        hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                        if (uReturn == 0) break;
                        
                        info.name = getWMIValue(pclsObj, L"Caption");
                        info.version = getWMIValue(pclsObj, L"Version");
                        info.buildNumber = getWMIValue(pclsObj, L"BuildNumber");
                        info.architecture = getWMIValue(pclsObj, L"OSArchitecture");
                        
                        pclsObj->Release();
                        break;
                    }
                    pEnumerator->Release();
                }
            }
            pSvc->Release();
        }
        pLoc->Release();
    }
    cleanupCOM();
    return info;
}

SystemInfo::MemoryInfo SystemInfo::getMemoryInfo() {
    MemoryInfo info = {0};
    initializeCOM();

    IWbemLocator* pLoc = NULL;
    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (SUCCEEDED(hr)) {
        IWbemServices* pSvc = NULL;
        BSTR strNamespace = SysAllocString(L"ROOT\\CIMV2");
        hr = pLoc->ConnectServer(strNamespace, NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
        SysFreeString(strNamespace);
        
        if (SUCCEEDED(hr)) {
            hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
            if (SUCCEEDED(hr)) {
                IEnumWbemClassObject* pEnumerator = NULL;
                BSTR strQuery = SysAllocString(L"SELECT * FROM Win32_OperatingSystem");
                hr = pSvc->ExecQuery(SysAllocString(L"WQL"), strQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
                SysFreeString(strQuery);
                
                if (SUCCEEDED(hr)) {
                    IWbemClassObject* pclsObj = NULL;
                    ULONG uReturn = 0;
                    while (pEnumerator) {
                        hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                        if (uReturn == 0) break;
                        
                        info.totalPhysicalMemory = std::stoull(getWMIValue(pclsObj, L"TotalVisibleMemorySize"));
                        info.availablePhysicalMemory = std::stoull(getWMIValue(pclsObj, L"FreePhysicalMemory"));
                        info.totalVirtualMemory = std::stoull(getWMIValue(pclsObj, L"TotalVirtualMemorySize"));
                        info.availableVirtualMemory = std::stoull(getWMIValue(pclsObj, L"FreeVirtualMemory"));
                        
                        pclsObj->Release();
                        break;
                    }
                    pEnumerator->Release();
                }
            }
            pSvc->Release();
        }
        pLoc->Release();
    }
    cleanupCOM();
    return info;
}

std::vector<std::string> SystemInfo::getDiskDrives() {
    std::vector<std::string> drives;
    char driveLetter = 'A';
    char drivePath[4] = "A:\\";
    
    while (driveLetter <= 'Z') {
        drivePath[0] = driveLetter;
        if (GetDriveTypeA(drivePath) != DRIVE_NO_ROOT_DIR) {
            drives.push_back(drivePath);
        }
        driveLetter++;
    }
    return drives;
}

std::string SystemInfo::getComputerName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    }
    return "";
}

std::string SystemInfo::getUserName() {
    char userName[257];
    DWORD size = sizeof(userName);
    if (GetUserNameA(userName, &size)) {
        return std::string(userName);
    }
    return "";
}
#elif defined(__APPLE__)

SystemInfo::CPUInfo SystemInfo::getCPUInfo() {
    CPUInfo info = {"Unknown", "Apple", 0, 0};
    
    char brand[256];
    size_t size = sizeof(brand);
    if (sysctlbyname("machdep.cpu.brand_string", brand, &size, NULL, 0) == 0) {
        info.name = brand;
    }
    
    int cores = 0;
    size = sizeof(cores);
    if (sysctlbyname("hw.physicalcpu", &cores, &size, NULL, 0) == 0) {
        info.numberOfCores = cores;
    }
    
    int logical = 0;
    size = sizeof(logical);
    if (sysctlbyname("hw.logicalcpu", &logical, &size, NULL, 0) == 0) {
        info.numberOfLogicalProcessors = logical;
    }
    
    return info;
}

SystemInfo::OSInfo SystemInfo::getOSInfo() {
    OSInfo info;
    
    // Get OS name and version
    char osName[256];
    size_t size = sizeof(osName);
    if (sysctlbyname("kern.ostype", osName, &size, NULL, 0) == 0) {
        info.name = osName;
    }
    
    char osRelease[256];
    size = sizeof(osRelease);
    if (sysctlbyname("kern.osrelease", osRelease, &size, NULL, 0) == 0) {
        info.version = osRelease;
    }
    
    // Get build number
    char buildNumber[256];
    size = sizeof(buildNumber);
    if (sysctlbyname("kern.osversion", buildNumber, &size, NULL, 0) == 0) {
        info.buildNumber = buildNumber;
    }
    
    // Get architecture
    char arch[256];
    size = sizeof(arch);
    if (sysctlbyname("hw.machine", arch, &size, NULL, 0) == 0) {
        info.architecture = arch;
    }
    
    return info;
}

SystemInfo::MemoryInfo SystemInfo::getMemoryInfo() {
    MemoryInfo info = {0};
    
    // Get total physical memory
    uint64_t totalMem;
    size_t size = sizeof(totalMem);
    if (sysctlbyname("hw.memsize", &totalMem, &size, NULL, 0) == 0) {
        info.totalPhysicalMemory = totalMem;
    }
    
    // Get available memory using mach APIs
    mach_port_t host_port = mach_host_self();
    mach_msg_type_number_t host_size = sizeof(vm_statistics64_data_t) / sizeof(integer_t);
    vm_size_t page_size;
    vm_statistics64_data_t vm_stats;
    
    host_page_size(host_port, &page_size);
    
    if (host_statistics64(host_port, HOST_VM_INFO64, (host_info64_t)&vm_stats, &host_size) == KERN_SUCCESS) {
        uint64_t free_memory = vm_stats.free_count * page_size;
        info.availablePhysicalMemory = free_memory;
        
        // For virtual memory, we'll use the same values as physical for simplicity
        info.totalVirtualMemory = info.totalPhysicalMemory;
        info.availableVirtualMemory = info.availablePhysicalMemory;
    }
    
    return info;
}

std::vector<std::string> SystemInfo::getDiskDrives() {
    std::vector<std::string> drives;
    // On macOS, we'll just return the root directory
    drives.push_back("/");
    return drives;
}

std::string SystemInfo::getComputerName() {
    char name[256];
    size_t size = sizeof(name);
    if (sysctlbyname("kern.hostname", name, &size, NULL, 0) == 0) {
        return std::string(name);
    }
    return "";
}

std::string SystemInfo::getUserName() {
    char* user = getenv("USER");
    if (user) {
        return std::string(user);
    }
    return "";
}

#endif 