#include "include.h"
#define _CRT_SECURE_NO_WARNINGS
namespace CMemory
{
	namespace Functions
	{
		const wchar_t* GetWC(const char* c);
		int get_pid_by_name(const char* procName);
	}

	namespace MemLib
	{
		void writeMemory(HANDLE handle, DWORD32 memoryvalue, void* addr);
		void writeFloat(HANDLE handle, float memoryvalue, void* addr);
		void writeDouble(HANDLE handle, double memoryvalue, void* addr);
		void writeBool(HANDLE handle, bool memoryvalue, void* addr);
		void writeByte(HANDLE handle, BYTE memoryvalue, void* addr);
		
		std::string readMemory(HANDLE handle, void* addr);
	}
}