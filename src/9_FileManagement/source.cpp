#include"file.h"

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject != NULL)
	{

	}
	return;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	if (DriverObject != NULL)
	{
		DriverObject->DriverUnload = DriverUnload;
	}

	if (RegistryPath != NULL)
	{

	}

	//ReadFileTest();
	//WriteFileTest();
	//RenameFileTest();

	UNICODE_STRING strEnumPath{};
	RtlInitUnicodeString(&strEnumPath, L"\\??\\C:\\Windows");
	EnumFileTest(strEnumPath);

	return STATUS_SUCCESS;
}