#include<ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject != NULL)
	{

	}

	return;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	if (DriverObject != nullptr)
	{
		DriverObject->DriverUnload = DriverUnload;
	}

	if (RegistryPath != NULL)
	{
		
	}

	KIRQL oldIrql;

	DbgPrint("[%ws]Before Raise IRQL:CurrentIRQL = 0x%u", __FUNCTIONW__, KeGetCurrentIrql());

	KeRaiseIrql(DISPATCH_LEVEL, &oldIrql);

	DbgPrint("[%ws]After Raise IRQL:CurrentIRQL = 0x%u", __FUNCTIONW__, KeGetCurrentIrql());

	KeLowerIrql(oldIrql);

	DbgPrint("[%ws]After Lower IRQL:CurrentIRQL = 0x%u", __FUNCTIONW__, KeGetCurrentIrql());

	return STATUS_SUCCESS;
}