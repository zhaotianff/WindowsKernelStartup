#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject != NULL)
		DbgPrint("Driver Upload,Driver 0bject Address :%p,CurrentIRQL = 0x%u\n", DriverObject, KeGetCurrentIrql());
	return;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DbgPrint("Hello Kernel world£¬CurrentIRQL = 0x%u\n", KeGetCurrentIrql());

	if (RegistryPath != NULL)
	{

	}

	if (DriverObject != NULL)
	{
		DriverObject->DriverUnload = DriverUnload;
	}

	return STATUS_SUCCESS;
}