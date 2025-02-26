#include<ntddk.h>
#include<ntstrsafe.h>

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

	//≥ı ºªØ
	UNICODE_STRING str{};
	RtlInitUnicodeString(&str, L"HelloWorld");
	DbgPrint("String:%wZ", &str);

	//øΩ±¥
	WCHAR strBuffer[128]{};
	UNICODE_STRING str_2{};
	RtlInitEmptyUnicodeString(&str_2, strBuffer, sizeof(strBuffer));
	RtlUnicodeStringCopyString(&str_2, L"HelloWorld");
	DbgPrint("%wZ", &str_2);

	return STATUS_SUCCESS;
}