#include<ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject != NULL)
	{
		DbgPrint("[%ws]Driver Unload,Driver Address:%p,CurrentProcessId = 0x%p\n", __FUNCTIONW__, DriverObject, PsGetCurrentProcessId());
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

	DbgPrint("[%ws]Driver Entry,CurrentProcessId = 0x%p\n", __FUNCTIONW__, PsGetCurrentProcessId());

	return STATUS_SUCCESS;
}