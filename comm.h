#pragma once
#include "utils.h"
#include "encrpyt.h"
#include <mutex>
#include "fontSF.h"

std::mutex isuse;

class Driver
{
public:
	UINT ProcessId;

	const bool Init(const BOOL PhysicalMode) {
		this->bPhysicalMode = PhysicalMode;
		this->hDriver = CreateFileA((E("\\\\.\\\svchost")), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (this->hDriver != INVALID_HANDLE_VALUE) {
			if (this->SharedBuffer = VirtualAlloc(0, sizeof(REQUEST_DATA), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)) {
				UNICODE_STRING RegPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\fortnite2");
				if (!RegistryUtils::WriteRegistry(RegPath, RTL_CONSTANT_STRING(L"xxxxxxxx"), &this->SharedBuffer, REG_QWORD, 8)) {
					return false;
				}
				PVOID pid = (PVOID)GetCurrentProcessId();
				if (!RegistryUtils::WriteRegistry(RegPath, RTL_CONSTANT_STRING(L"xxxxxxxxx"), &pid, REG_QWORD, 8)) {
					return false;
				}
				auto OLD_MAGGICCODE = this->MAGGICCODE;
				SendRequest(99, 0);
				if (this->MAGGICCODE == OLD_MAGGICCODE)
					this->MAGGICCODE = (ULONG64)RegistryUtils::ReadRegistry<LONG64>(RegPath, RTL_CONSTANT_STRING(L"xxxxxxxxxx"));
				return true;
			}
		}
		return false;
	}

	const NTSTATUS SendRequest(const UINT type, const PVOID args) {
		std::scoped_lock lock(isuse);
		REQUEST_DATA req;
		NTSTATUS status;
		req.MaggicCode = &this->MAGGICCODE;
		req.Type = type;
		req.Arguments = args;
		req.Status = &status;
		memcpy(this->SharedBuffer, &req, sizeof(REQUEST_DATA));
		FlushFileBuffers(this->hDriver);
		return status;
	}

	NTSTATUS ReadProcessMemory(uint64_t src, void* dest, uint32_t size) {
		REQUEST_READ req;
		req.ProcessId = ProcessId;
		req.Src = src;
		req.Dest = dest;
		req.Size = size;
		req.bPhysicalMem = bPhysicalMode;
		return SendRequest(REQUEST_TYPE::READ, &req);
	}
	NTSTATUS WriteProcessMemory(PVOID src, PVOID dest, DWORD size) {
		REQUEST_WRITE req;
		req.ProcessId = ProcessId;
		req.Src = src;
		req.Dest = dest;
		req.Size = size;
		req.bPhysicalMem = bPhysicalMode;
		return SendRequest(REQUEST_TYPE::WRITE, &req);
	}

	const UINT GetProcessThreadNumByID(DWORD dwPID)
	{
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return 0;

		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		BOOL bRet = ::Process32First(hProcessSnap, &pe32);;
		while (bRet)
		{
			if (pe32.th32ProcessID == dwPID)
			{
				::CloseHandle(hProcessSnap);
				return pe32.cntThreads;
			}
			bRet = ::Process32Next(hProcessSnap, &pe32);
		}
		return 0;
	}

	const UINT GetProcessId(const wchar_t* process_name) {
		UINT pid = 0;

		DWORD dwThreadCountMax = 0;

		// Create toolhelp snapshot.
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 process;
		ZeroMemory(&process, sizeof(process));
		process.dwSize = sizeof(process);
		// Walkthrough all processes.
		if (Process32First(snapshot, &process))
		{
			do
			{
				if (wcsstr(process.szExeFile, process_name))
				{
					DWORD dwTmpThreadCount = GetProcessThreadNumByID(process.th32ProcessID);
					if (dwTmpThreadCount > dwThreadCountMax)
					{
						dwThreadCountMax = dwTmpThreadCount;
						pid = process.th32ProcessID;
						break;
					}
				}
			} while (Process32Next(snapshot, &process));
		}
		CloseHandle(snapshot);
		return pid;
	}

	const bool Attach(const wchar_t* Processname, const wchar_t* Classname = 0) {
		if (Classname) {
			while (!FindWindowW(Classname, 0)) { Sleep(50); }
		}
		if (this->ProcessId = this->GetProcessId(Processname))
			return true;

		return false;
	}

	const uint64_t GetModuleBase(const wchar_t* ModuleName = 0) {
		if (bPhysicalMode) {
			REQUEST_MAINBASE req;
			uint64_t base = NULL;
			req.ProcessId = ProcessId;
			req.OutAddress = (PBYTE*)&base;
			SendRequest(REQUEST_TYPE::MAINBASE, &req);
			return { base };
		}
		else {
			if (!ModuleName)
				return { 0 };
			REQUEST_MODULE req;
			uint64_t base = NULL;
			DWORD size = NULL;
			req.ProcessId = ProcessId;
			req.OutAddress = (PBYTE*)&base;
			req.OutSize = &size;
			wcscpy_s(req.Module, sizeof(req.Module) / sizeof(req.Module[0]), ModuleName);
			SendRequest(REQUEST_TYPE::MODULE, &req);
			return { base };
		}
	}


private:
	PVOID SharedBuffer;
	HANDLE hDriver;
	ULONG64 MAGGICCODE = 0x18C9732B1794A;
	BOOL bPhysicalMode = FALSE;
	typedef enum _REQUEST_TYPE : UINT {
		WRITE,
		READ,
		PROTECT,
		ALLOC,
		FREE,
		MODULE,
		MAINBASE,
		THREADCALL,
	} REQUEST_TYPE;

	typedef struct _REQUEST_DATA {
		ULONG64* MaggicCode;
		UINT Type;
		PVOID Arguments;
		NTSTATUS* Status;
	} REQUEST_DATA, * PREQUEST_DATA;

	typedef struct _REQUEST_WRITE {
		DWORD ProcessId;
		PVOID Dest;
		PVOID Src;
		DWORD Size;
		BOOL bPhysicalMem;
	} REQUEST_WRITE, * PREQUEST_WRITE;

	typedef struct _REQUEST_READ {
		DWORD ProcessId;
		void* Dest;
		uint64_t Src;
		uint32_t Size;
		BOOL bPhysicalMem;
	} REQUEST_READ, * PREQUEST_READ;

	typedef struct _REQUEST_PROTECT {
		DWORD ProcessId;
		PVOID Address;
		DWORD Size;
		PDWORD InOutProtect;
	} REQUEST_PROTECT, * PREQUEST_PROTECT;

	typedef struct _REQUEST_ALLOC {
		DWORD ProcessId;
		PVOID OutAddress;
		DWORD Size;
		DWORD Protect;
	} REQUEST_ALLOC, * PREQUEST_ALLOC;

	typedef struct _REQUEST_FREE {
		DWORD ProcessId;
		PVOID Address;
	} REQUEST_FREE, * PREQUEST_FREE;

	typedef struct _REQUEST_MODULE {
		DWORD ProcessId;
		WCHAR Module[0xFF];
		PBYTE* OutAddress;
		DWORD* OutSize;
	} REQUEST_MODULE, * PREQUEST_MODULE;

	typedef struct _REQUEST_MAINBASE {
		DWORD ProcessId;
		PBYTE* OutAddress;
	} REQUEST_MAINBASE, * PREQUEST_MAINBASE;
};

static Driver* driver = new Driver;

template <typename T>
T read(const uintptr_t address)
{
	T buffer{ };
	driver->ReadProcessMemory(address, &buffer, sizeof(T));
	return buffer;
}
template <typename T>
T write(const uintptr_t address, T buffer)
{
	driver->WriteProcessMemory((PVOID)&buffer, (PVOID)address, sizeof(T));
	return buffer;
}
std::string readwtf(uintptr_t Address, void* Buffer, SIZE_T Size)
{
	driver->ReadProcessMemory(Address, Buffer, Size);

	char name[255] = { 0 };
	memcpy(&name, Buffer, Size);

	return std::string(name);
}

/*uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets) {
	uint64_t result = read<uint64_t>(base + offsets.at(0));
	for (int i = 1; i < offsets.size(); i++) {
		result = read<uint64_t>(result + offsets.at(i));
	}
	return result;
}*/


