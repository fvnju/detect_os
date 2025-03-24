#include <iostream>
#include <stdio.h>

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifdef _WIN32
#include <windows.h>
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
#endif

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
  return 0;
}