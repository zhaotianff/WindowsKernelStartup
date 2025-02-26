#pragma once

#include<ntifs.h>

typedef struct _AUX_ACCESS_DATA {
	PPRIVILEGE_SET PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK AccessesToAudit;
	ACCESS_MASK MaximumAuditMask;
	ULONG UnKnow[256];
} AUX_ACCESS_DATA, * PAUX_ACCESS_DATA;

#pragma region Native_API
VOID CreateFileTest();
VOID DeleteFileTest();
VOID GetFileSizeTest();
VOID ReadFileTest();
VOID WriteFileTest();
VOID RenameFileTest();
VOID EnumFileTest(UNICODE_STRING strPath);
#pragma endregion

#pragma region IRP
VOID IRPCreateFileTest();
#pragma endregion


