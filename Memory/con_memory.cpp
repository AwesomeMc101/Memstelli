#include "con_memory.hpp"

const wchar_t* CMemory::Functions::GetWC(const char* c) //tyty stackoverflow
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}
int CMemory::Functions::get_pid_by_name(LPCSTR procName)
{
	//const wchar_t* name = GetWC(procName);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	int pid = 0;
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "inv handle";
	}
	//std::cout << "NAME: " << name << "\n";
	PROCESSENTRY32 proce;
	proce.dwSize = sizeof(PROCESSENTRY32);

	BOOL hResult = Process32First(hSnapshot, &proce);

	std::cout << "Initial result: " << hResult;

	while (hResult != FALSE)
	{
	//	if (_wcsicmp(proce.szExeFile, procName) == 0)
		if (strcmp(proce.szExeFile, procName) == 0)
		{
			printf("\nprocess located\n");
			pid = proce.th32ProcessID;
			CloseHandle(hSnapshot);
			return pid;
		}
		hResult = Process32Next(hSnapshot, &proce);
		if (pid)
		{
			hResult = false; //break loop
		}
	}
	printf("FAILED");
}

std::string CMemory::MemLib::readMemory(HANDLE handle, void* addr)
{
	//char buffer[10000]{""};
	DWORD buffer;
	DWORD bytesRead = 0;
	if (ReadProcessMemory(handle, addr, &buffer, sizeof(buffer), &bytesRead))
	{
		std::cout << "READ: " << buffer << "\n";
		return std::to_string(buffer);
	}
	else
	{
		std::cout << "COULD NOT READ";
	}
}

void CMemory::MemLib::writeMemory(HANDLE handle, DWORD32 memoryvalue, void* addr)
{
	if (WriteProcessMemory(handle, addr, &memoryvalue, sizeof(memoryvalue), NULL))
	{
		//cool :)
	}
	else
	{
		printf("Write failure");
	}
}

void CMemory::MemLib::writeFloat(HANDLE handle, float memoryvalue, void* addr)
{
	{
		if (WriteProcessMemory(handle, addr, &memoryvalue, sizeof(memoryvalue), NULL))
		{
			//cool :)
		}
		else
		{
			printf("Write failure");
		}
	}
}
void CMemory::MemLib::writeDouble(HANDLE handle, double memoryvalue, void* addr)
{
	if (WriteProcessMemory(handle, addr, &memoryvalue, sizeof(memoryvalue), NULL))
	{
		//cool :)
	}
	else
	{
		printf("Write failure");
	}
}

void CMemory::MemLib::writeBool(HANDLE handle, bool memoryvalue, void* addr)
{
	if (WriteProcessMemory(handle, addr, &memoryvalue, sizeof(memoryvalue), NULL))
	{
		//cool :)
	}
	else
	{
		printf("Write failure");
	}
}

void CMemory::MemLib::writeByte(HANDLE handle, BYTE memoryvalue, void* addr)
{
	if (WriteProcessMemory(handle, addr, &memoryvalue, sizeof(memoryvalue), NULL))
	{
		//cool :)
	}
	else
	{
		printf("Write failure");
	}
}