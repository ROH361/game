#ifndef __INC_CRC32_H__
#define __INC_CRC32_H__

#include <windows.h>

DWORD GetCRC32(const char* buffer, size_t count);
DWORD GetCaseCRC32(const char * buf, size_t len);
DWORD GetHFILECRC32(HANDLE hFile);
DWORD GetFileCRC32(const char* c_szFileName);
DWORD GetFileSize(const char* c_szFileName);

#endif
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
