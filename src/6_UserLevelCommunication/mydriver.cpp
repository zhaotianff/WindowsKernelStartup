#include<ntddk.h>
#include<ntstrsafe.h>
#include<wdmsec.h>

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
	DbgPrint("Hello World\n");

	if (RegistryPath != NULL)
	{

	}

	if (DriverObject != NULL)
	{
		DriverObject->DriverUnload = DriverUnload;
	}

	return STATUS_SUCCESS;
}