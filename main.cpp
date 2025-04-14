#include <iostream>
#include <stdio.h>
#include <iomanip>
#include "is_64bit.hpp"
#include "hw_info.hpp"
#include "cpu_count.h"
#include <thread>

void print_data_types() {
    std::cout << "\nC++ Data Type Representations" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // Helper lambda for formatting
    auto print_type = [](const char* name, size_t size, const char* range, const char* notes = "") {
        std::cout << std::setw(20) << std::left << name 
                  << std::setw(10) << std::right << size << " bytes"
                  << std::setw(40) << std::left << range
                  << notes << std::endl;
    };

    std::cout << "\nFundamental Types:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    print_type("bool", sizeof(bool), "0 or 1", "Implementation-defined size, typically 1 byte");
    print_type("char", sizeof(char), "-128 to 127", "Minimum size: 1 byte, may be signed or unsigned");
    print_type("unsigned char", sizeof(unsigned char), "0 to 255", "Minimum size: 1 byte");
    print_type("signed char", sizeof(signed char), "-128 to 127", "Minimum size: 1 byte");
    print_type("wchar_t", sizeof(wchar_t), "Implementation-defined", "Wide character type, size varies by platform");
    print_type("char16_t", sizeof(char16_t), "0 to 65535", "UTF-16 character");
    print_type("char32_t", sizeof(char32_t), "0 to 4294967295", "UTF-32 character");

    std::cout << "\nInteger Types:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    print_type("short", sizeof(short), "-32768 to 32767", "Minimum size: 2 bytes");
    print_type("unsigned short", sizeof(unsigned short), "0 to 65535", "Minimum size: 2 bytes");
    print_type("int", sizeof(int), "-2147483648 to 2147483647", "Minimum size: 2 bytes, typically 4 bytes");
    print_type("unsigned int", sizeof(unsigned int), "0 to 4294967295", "Minimum size: 2 bytes, typically 4 bytes");
    print_type("long", sizeof(long), "Implementation-defined", "Minimum size: 4 bytes");
    print_type("unsigned long", sizeof(unsigned long), "Implementation-defined", "Minimum size: 4 bytes");
    print_type("long long", sizeof(long long), "-2^63 to 2^63-1", "Minimum size: 8 bytes");
    print_type("unsigned long long", sizeof(unsigned long long), "0 to 2^64-1", "Minimum size: 8 bytes");

    std::cout << "\nFloating-Point Types:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    print_type("float", sizeof(float), "±1.17549e-38 to ±3.40282e+38", "IEEE 754 single precision (32-bit)");
    print_type("double", sizeof(double), "±2.22507e-308 to ±1.79769e+308", "IEEE 754 double precision (64-bit)");
    print_type("long double", sizeof(long double), "Implementation-defined", "Extended precision, typically 80-bit or 128-bit");

    std::cout << "\nPointer Types:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    print_type("void*", sizeof(void*), "Implementation-defined", "Pointer to any type");
    print_type("size_t", sizeof(size_t), "0 to 2^64-1", "Unsigned integer type for sizes");
    print_type("ptrdiff_t", sizeof(ptrdiff_t), "Implementation-defined", "Signed integer type for pointer differences");

    std::cout << "\nNotes:" << std::endl;
    std::cout << "1. All sizes are implementation-defined and may vary by platform" << std::endl;
    std::cout << "2. Integer types follow two's complement representation" << std::endl;
    std::cout << "3. Floating-point types follow IEEE 754 standard where supported" << std::endl;
    std::cout << "4. Pointer sizes depend on the target architecture (32-bit vs 64-bit)" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
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
    if (is_64bit()) {
        std::cout << "Is 64 bit?: " << "YES" << std::endl;
    } else {
        std::cout << "Is 64 bit?: " << "NO" << std::endl;
    }

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
    // Convert bytes to GB (1 GB = 1024^3 bytes)
    const double GB = 1024.0 * 1024.0 * 1024.0;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total Physical Memory: " << (memInfo.totalPhysicalMemory / GB) << " GB" << std::endl;
    std::cout << "Available Physical Memory: " << (memInfo.availablePhysicalMemory / GB) << " GB" << std::endl;
    std::cout << "Total Virtual Memory: " << (memInfo.totalVirtualMemory / GB) << " GB" << std::endl;
    std::cout << "Available Virtual Memory: " << (memInfo.availableVirtualMemory / GB) << " GB" << std::endl;

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