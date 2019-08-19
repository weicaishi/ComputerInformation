﻿// CpuInf.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#if _MSC_VER >=1400    // VC2005才支持intrin.h
#include <intrin.h>    // 所有Intrinsics函数
#endif


#if defined(_WIN64)
// 64位下不支持内联汇编. 应使用__cpuid、__cpuidex等Intrinsics函数。
#else
#if _MSC_VER < 1600    // VS2010. 据说VC2008 SP1之后才支持__cpuidex
void __cpuidex(INT32 CPUInfo[4], INT32 InfoType, INT32 ECXValue)
{
	if (NULL == CPUInfo)    return;
	_asm {
		// load. 读取参数到寄存器
		mov edi, CPUInfo;    // 准备用edi寻址CPUInfo
		mov eax, InfoType;
		mov ecx, ECXValue;
		// CPUID
		cpuid;
		// save. 将寄存器保存到CPUInfo
		mov[edi], eax;
		mov[edi + 4], ebx;
		mov[edi + 8], ecx;
		mov[edi + 12], edx;
	}
}
#endif    // #if _MSC_VER < 1600    // VS2010. 据说VC2008 SP1之后才支持__cpuidex

#if _MSC_VER < 1400    // VC2005才支持__cpuid
void __cpuid(INT32 CPUInfo[4], INT32 InfoType)
{
	__cpuidex(CPUInfo, InfoType, 0);
}
#endif    // #if _MSC_VER < 1400    // VC2005才支持__cpuid

#endif    // #if defined(_WIN64)

// 取得CPU厂商（Vendor）
//
// result: 成功时返回字符串的长度（一般为12）。失败时返回0。
// pvendor: 接收厂商信息的字符串缓冲区。至少为13字节。
int cpu_getvendor(char* pvendor)
{
	INT32 dwBuf[4];
	if (NULL == pvendor)    return 0;
	// Function 0: Vendor-ID and Largest Standard Function
	__cpuid(dwBuf, 0);
	// save. 保存到pvendor
	*(INT32*)&pvendor[0] = dwBuf[1];    // ebx: 前四个字符
	*(INT32*)&pvendor[4] = dwBuf[3];    // edx: 中间四个字符
	*(INT32*)&pvendor[8] = dwBuf[2];    // ecx: 最后四个字符
	pvendor[12] = '\0';
	return 12;
}

// 取得CPU商标（Brand）
//
// result: 成功时返回字符串的长度（一般为48）。失败时返回0。
// pbrand: 接收商标信息的字符串缓冲区。至少为49字节。
int cpu_getbrand(char* pbrand)
{
	INT32 dwBuf[4];
	if (NULL == pbrand)    return 0;
	// Function 0x80000000: Largest Extended Function Number
	__cpuid(dwBuf, 0x80000000);
	if (dwBuf[0] < 0x80000004)    return 0;
	// Function 80000002h,80000003h,80000004h: Processor Brand String
	__cpuid((INT32*)&pbrand[0], 0x80000002);    // 前16个字符
	__cpuid((INT32*)&pbrand[16], 0x80000003);    // 中间16个字符
	__cpuid((INT32*)&pbrand[32], 0x80000004);    // 最后16个字符
	pbrand[48] = '\0';
	return 48;
}


int main(int argc, _TCHAR* argv[])
{
	//调用系统wmic cpu get processorid
	printf("************************************************\r\n");
	printf("call cmd [wmic cpu get processorid]\r\n");
	system("wmic cpu get processorid");
	printf("************************************************\r\n");

	char szBuf[64] = { 0 };
	INT32 dwBuf[4] = { 0 };

	__cpuidex(dwBuf, 01, 0);
	printf("processorid:\t%.8X%.8X\r\n", dwBuf[3], dwBuf[0]);

	

	cpu_getvendor(szBuf);
	printf("CPU Vendor:\t%s\n", szBuf);

	cpu_getbrand(szBuf);
	printf("CPU Name:\t%s\n", szBuf);

	system("pause");
	return 0;
}