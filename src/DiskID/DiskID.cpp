// DiskID.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "pch.h"
#include "HardDriveSerialNumer.h"
#include <iostream>

int main()

{
	char SerialNumber[1024] = { 0 };
	MasterHardDiskSerial mhds;
	mhds.GetSerialNo(SerialNumber);
	std::cout << "SN:\t" << SerialNumber << std::endl;
	system("pause");
	return 0;

}