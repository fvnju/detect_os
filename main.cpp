#include <iostream>
#include <stdio.h>
#include <iomanip>

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifdef _WIN32
#include <windows.h>
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
#endif

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

bool is_64bit() {
  #if defined(__APPLE__)
  char arch[32];
  size_t size = sizeof(arch);
  return sysctlbyname("hw.machine", arch, &size, NULL, 0) == 0 && 
          std::string(arch).find("64") != std::string::npos;

  #elif defined(_WIN32)
  BOOL isWow64 = FALSE;

  auto fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(
    GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process")
  );

  if (fnIsWow64Process && fnIsWow64Process(GetCurrentProcess(), &isWow64)) {
    return !isWow64;
  }

  return sizeof(void*) == 8;
  #endif
}

int main() {
  char* device_type = new char[20];
  #ifdef __APPLE__
    snprintf(device_type, 14, " Mac Machine.");
  #elif _WIN32
    snprintf(device_type, 18, " Windows Machine.");
  #endif
  if (is_64bit()) {
    std::cout << "You are on a 64-bit" << device_type << std::endl;
  } else {
    std::cout << "You are on a 32-bit" << device_type << std::endl;
  }
  delete[] device_type;
  
  print_data_types();
  return 0;
}