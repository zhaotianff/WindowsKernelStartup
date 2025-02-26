#include <ntddk.h>

typedef struct _ListPos
{
	int m_nX;
	int m_nY;
	LIST_ENTRY m_listEntry;
}ListPos, * PListPos;

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject != NULL)
	{

	}
	return;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	if (RegistryPath != NULL)
	{

	}

	if (DriverObject != NULL)
	{
		DriverObject->DriverUnload = DriverUnload;
	}

	LIST_ENTRY lst_Header{};
	InitializeListHead(&lst_Header);
	
	ListPos p_A{};
	p_A.m_nX = 0;
	p_A.m_nY = 0;

	ListPos p_B{};
	p_B.m_nX = 1;
	p_B.m_nY = 1;

	ListPos p_C{};
	p_C.m_nX = 2;
	p_C.m_nY = 2;

	//��ͷ������
	InsertHeadList(&lst_Header, &p_B.m_listEntry);
	InsertHeadList(&lst_Header, &p_A.m_listEntry);
	//��β������
	InsertTailList(&lst_Header, &p_C.m_listEntry);

	PLIST_ENTRY pListEntry = lst_Header.Flink;
	//���
	DbgPrint("Init Print.......");
	while (pListEntry != &lst_Header)
	{
		PListPos pListPos = CONTAINING_RECORD(pListEntry, ListPos, m_listEntry);
		DbgPrint("ListEntry = %p,ListPos = %p,x = %d y = %d",
			pListEntry, pListPos, pListPos->m_nX, pListPos->m_nY);
		pListEntry = pListEntry->Flink;
	}

	ListPos p_D{};
	p_D.m_nX = 3;
	p_D.m_nY = 3;

	ListPos p_E{};
	p_E.m_nX = 4;
	p_E.m_nY = 4;

	//��β���ٴβ��������ڵ�
	InsertTailList(&lst_Header, &p_D.m_listEntry);
	InsertTailList(&lst_Header, &p_E.m_listEntry);

	//�Ƴ�ͷ���ڵ�
	RemoveHeadList(&lst_Header);

	pListEntry = lst_Header.Flink;
	//��� 
	DbgPrint("Remove Header Print.......");
	while (pListEntry != &lst_Header)
	{
		PListPos pListPos = CONTAINING_RECORD(pListEntry, ListPos, m_listEntry);
		DbgPrint("ListEntry = %p,ListPos = %p,x = %d y = %d",
			pListEntry, pListPos, pListPos->m_nX, pListPos->m_nY);
		pListEntry = pListEntry->Flink;
	}

	//�Ƴ�ָ���ڵ�
	RemoveEntryList(&p_D.m_listEntry);

	BOOLEAN isEmpty = IsListEmpty(&lst_Header);

	pListEntry = lst_Header.Flink;
	//���
	DbgPrint("Remove entry Print,IsEmpty = %c", isEmpty);
	while (pListEntry != &lst_Header)
	{
		PListPos pListPos = CONTAINING_RECORD(pListEntry, ListPos, m_listEntry);
		DbgPrint("ListEntry = %p,ListPos = %p,x = %d y = %d",
			pListEntry, pListPos, pListPos->m_nX, pListPos->m_nY);
		pListEntry = pListEntry->Flink;
	}

	//�Ƴ�β���ڵ�
	RemoveTailList(&lst_Header);
	//���
	DbgPrint("Remove tail Print");
	while (pListEntry != &lst_Header)
	{
		PListPos pListPos = CONTAINING_RECORD(pListEntry, ListPos, m_listEntry);
		DbgPrint("ListEntry = %p,ListPos = %p,x = %d y = %d",
			pListEntry, pListPos, pListPos->m_nX, pListPos->m_nY);
		pListEntry = pListEntry->Flink;
	}

	return STATUS_SUCCESS;
}