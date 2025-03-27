#include <iostream>
#include <stdio.h>
#include <iomanip>
#include "is_64bit.hpp"
#include "hw_info.hpp"
#include "cpu_count.h"
#include <thread>

void print_data_types() {
    std::cout << "\nData Type Sizes:" << std::endl;
    std::cout << std::setw(20) << std::left << "Data Type" << std::setw(10) << "Size (bytes)" << std::endl;
    std::cout << std::string(32, '-') << std::endl;
    
    std::cout << std::setw(20) << std::left << "bool" << std::setw(10) << sizeof(bool) << std::endl;
    std::cout << std::setw(20) << std::left << "char" << std::setw(10) << sizeof(char) << std::endl;
    std::cout << std::setw(20) << std::left << "short" << std::setw(10) << sizeof(short) << std::endl;
    std::cout << std::setw(20) << std::left << "int" << std::setw(10) << sizeof(int) << std::endl;
    std::cout << std::setw(20) << std::left << "long" << std::setw(10) << sizeof(long) << std::endl;
    std::cout << std::setw(20) << std::left << "long long" << std::setw(10) << sizeof(long long) << std::endl;
    std::cout << std::setw(20) << std::left << "float" << std::setw(10) << sizeof(float) << std::endl;
    std::cout << std::setw(20) << std::left << "double" << std::setw(10) << sizeof(double) << std::endl;
    std::cout << std::setw(20) << std::left << "long double" << std::setw(10) << sizeof(long double) << std::endl;
    std::cout << std::setw(20) << std::left << "void*" << std::setw(10) << sizeof(void*) << std::endl;
    std::cout << std::setw(20) << std::left << "size_t" << std::setw(10) << sizeof(size_t) << std::endl;
    std::cout << std::setw(20) << std::left << "wchar_t" << std::setw(10) << sizeof(wchar_t) << std::endl;
    std::cout << std::setw(20) << std::left << "char16_t" << std::setw(10) << sizeof(char16_t) << std::endl;
    std::cout << std::setw(20) << std::left << "char32_t" << std::setw(10) << sizeof(char32_t) << std::endl;
}

void print_system_info() {
    std::cout << "\nSystem Information:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    // Get and display OS information
    auto osInfo = SystemInfo::getOSInfo();
    std::cout << "Operating System: " << osInfo.name << std::endl;
    std::cout << "Version: " << osInfo.version << std::endl;
    std::cout << "Build Number: " << osInfo.buildNumber << std::endl;
    std::cout << "Architecture: " << osInfo.architecture << std::endl;

    // Get and display CPU information
    auto cpuInfo = SystemInfo::getCPUInfo();
    std::cout << "\nCPU Information:" << std::endl;
    std::cout << "Processor: " << cpuInfo.name << std::endl;
    std::cout << "Manufacturer: " << cpuInfo.manufacturer << std::endl;
    std::cout << "Number of Physical Cores: " << cpu_count() << std::endl;
    //may return 0 when not able to detect
    const auto processor_count = std::thread::hardware_concurrency();
    std::cout << "Logical Processors: " << processor_count << std::endl;

    // Get and display memory information
    auto memInfo = SystemInfo::getMemoryInfo();
    std::cout << "\nMemory Information:" << std::endl;
    std::cout << "Total Physical Memory: " << (memInfo.totalPhysicalMemory / 1024.0 / 1024.0) << " GB" << std::endl;
    std::cout << "Available Physical Memory: " << (memInfo.availablePhysicalMemory / 1024.0 / 1024.0) << " GB" << std::endl;
    std::cout << "Total Virtual Memory: " << (memInfo.totalVirtualMemory / 1024.0 / 1024.0) << " GB" << std::endl;
    std::cout << "Available Virtual Memory: " << (memInfo.availableVirtualMemory / 1024.0 / 1024.0) << " GB" << std::endl;

    // Get and display disk drives
    auto drives = SystemInfo::getDiskDrives();
    std::cout << "\nDisk Drives:" << std::endl;
    for (const auto& drive : drives) {
        std::cout << drive << " ";
    }
    std::cout << std::endl;
}

int main() {
    print_system_info();
    print_data_types();
    return 0;
}