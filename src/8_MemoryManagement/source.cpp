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

	//ֱ�������ڴ�
	PVOID pAllocFromPool = ExAllocatePool2(POOL_FLAG_NON_PAGED, 10240, 0);

	if (pAllocFromPool != NULL)
		ExFreePoolWithTag(pAllocFromPool, 0);


	//ʹ�ú��б�(Lookaside Lists)
	PNPAGED_LOOKASIDE_LIST pLookasideList = (PNPAGED_LOOKASIDE_LIST)ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(NPAGED_LOOKASIDE_LIST), 'urfh');

	if (pLookasideList != NULL)
	{
		memset(pLookasideList, 0, sizeof(NPAGED_LOOKASIDE_LIST));
		//��ʼ��
		ExInitializeNPagedLookasideList(pLookasideList, NULL, NULL, 0, 1024, 'urfh', 0);

		//����
		PVOID pAlloc = ExAllocateFromNPagedLookasideList(pLookasideList);

		if (pAlloc != NULL)
		{
			DbgPrint("Memory Allocate First:%p", pAlloc);

			//�ͷ�
			ExFreeToNPagedLookasideList(pLookasideList, pAlloc);
		}

		//�ٴη���
		pAlloc = ExAllocateFromNPagedLookasideList(pLookasideList);

		if (pAlloc != NULL)
		{
			DbgPrint("Memory Allocate Second:%p", pAlloc);

			//�ͷ�
			ExFreeToNPagedLookasideList(pLookasideList, pAlloc);
		}

		//ɾ��Lookaside Lists
		ExDeleteNPagedLookasideList(pLookasideList);
		//�ͷ�
		ExFreePoolWithTag(pLookasideList, 'urfh');
	}
	


	
	return STATUS_SUCCESS;
}