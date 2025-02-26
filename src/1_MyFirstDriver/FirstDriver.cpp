#include<ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject != NULL)
	{
		DbgPrint("Driver Unload...Driver Object Address: %p\n", DriverObject);
	}

	return;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	KdBreakPoint();
	DbgPrint("Hello World\n");

	if (RegistryPath != NULL)
	{
		DbgPrint("Driver RegistryPath: %wZ\n", RegistryPath);
	}

	if (DriverObject != NULL)
	{
		DbgPrint("Driver Object Address: %p\n", DriverObject);
		DriverObject->DriverUnload = DriverUnload;
	}

	KeBugCheckEx(INVALID_DATA_ACCESS_TRAP, NULL, NULL, NULL, NULL);

	//return STATUS_FAILED_DRIVER_ENTRY;
}