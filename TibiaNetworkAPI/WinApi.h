#pragma once
#ifndef WINAPI_H
#define WINAPI_H
class WinApi {
public:
	static DWORD GetProcessBaseAddress(DWORD processID);
};
#endif