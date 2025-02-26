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
	if (DriverObject != NULL)
	{

	}

	if (RegistryPath != NULL)
	{

	}

	//直接申请内存
	PVOID pAllocFromPool = ExAllocatePool2(POOL_FLAG_NON_PAGED, 10240, 0);

	if (pAllocFromPool != NULL)
		ExFreePoolWithTag(pAllocFromPool, 0);


	//使用后备列表(Lookaside Lists)
	PNPAGED_LOOKASIDE_LIST pLookasideList = (PNPAGED_LOOKASIDE_LIST)ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(NPAGED_LOOKASIDE_LIST), 'urfh');

	if (pLookasideList != NULL)
	{
		memset(pLookasideList, 0, sizeof(NPAGED_LOOKASIDE_LIST));
		//初始化
		ExInitializeNPagedLookasideList(pLookasideList, NULL, NULL, 0, 1024, 'urfh', 0);

		//分配
		PVOID pAlloc = ExAllocateFromNPagedLookasideList(pLookasideList);

		if (pAlloc != NULL)
		{
			DbgPrint("Memory Allocate First:%p", pAlloc);

			//释放
			ExFreeToNPagedLookasideList(pLookasideList, pAlloc);
		}

		//再次分配
		pAlloc = ExAllocateFromNPagedLookasideList(pLookasideList);

		if (pAlloc != NULL)
		{
			DbgPrint("Memory Allocate Second:%p", pAlloc);

			//释放
			ExFreeToNPagedLookasideList(pLookasideList, pAlloc);
		}

		//删除Lookaside Lists
		ExDeleteNPagedLookasideList(pLookasideList);
		//释放
		ExFreePoolWithTag(pLookasideList, 'urfh');
	}
	


	
	return STATUS_SUCCESS;
}