#include "file.h"

VOID CreateFileTest()
{
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES objectAttributes{};
    IO_STATUS_BLOCK iosb{};
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING strFilePath;
    RtlInitUnicodeString(&strFilePath, L"\\??\\D:\\dd.txt");
    //创建文件
    InitializeObjectAttributes(&objectAttributes, &strFilePath, 
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwCreateFile(&hFile, GENERIC_READ, &objectAttributes,
        &iosb, NULL, FILE_ATTRIBUTE_NORMAL, 0,
        FILE_OPEN_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("failed....");
        return;
    }

    ZwClose(hFile);
}

VOID DeleteFileTest()
{
    NTSTATUS status = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES objectAttributes{};
    UNICODE_STRING strFilePath;
    RtlInitUnicodeString(&strFilePath, L"\\??\\D:\\dd.txt");
    InitializeObjectAttributes(&objectAttributes, &strFilePath,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    status = ZwDeleteFile(&objectAttributes);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("failed.....");
    }
}

VOID GetFileSizeTest()
{
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES objectAttributes{};
    IO_STATUS_BLOCK iosb{};
    NTSTATUS status = STATUS_SUCCESS;
    FILE_STANDARD_INFORMATION fsi{};
    UNICODE_STRING strFileName{};
    RtlInitUnicodeString(&strFileName, L"\\??\\C:\\Windows\\notepad.exe");
    InitializeObjectAttributes(&objectAttributes, &strFileName,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    status = ZwCreateFile(&hFile, GENERIC_READ,
        &objectAttributes, &iosb, NULL, 0, FILE_SHARE_READ,
        FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("open file failed, %d", status);
        return;
    }

    status = ZwQueryInformationFile(hFile, &iosb, &fsi,
        sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwQueryInformation %d", status);
        return;
    }

    ZwClose(hFile);
    DbgPrint("%wZ file size %lld", strFileName, fsi.EndOfFile.QuadPart);
}

VOID ReadFileTest()
{
    KdBreakPoint();
    HANDLE hFile = NULL;
    IO_STATUS_BLOCK iosb{};
    OBJECT_ATTRIBUTES objectAttributes{};
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING strFileName{};
    RtlInitUnicodeString(&strFileName, L"\\??\\C:\\aa.txt");

    InitializeObjectAttributes(&objectAttributes, &strFileName, 
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwCreateFile(&hFile, GENERIC_READ, &objectAttributes, &iosb, NULL,
        FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
        NULL, 0);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwCreateFile failed, %d" + status);
        return;
    }

    ULONG ulBufferSize = 128;
    ULONG ulReadDataSize = ulBufferSize;
    LARGE_INTEGER liOffset = { 0 };
    PUCHAR pBuffer = (PUCHAR)ExAllocatePool2(POOL_FLAG_NON_PAGED, ulBufferSize, 'dduh');

    if (pBuffer == NULL)
    {
        ZwClose(hFile);
        DbgPrint("ExallocatePool2 failed");
        return;
    }

    //%s (PCHAR)
    //%ws(PWCHAR)
    //%Z(PSTRING, PANSI_STRING, POEM_STRING)
    //%wZ(PUNICODE_STRING)

    RtlZeroMemory(&iosb, sizeof(iosb));
    status = ZwReadFile(hFile, NULL, NULL, NULL, &iosb, pBuffer, ulReadDataSize, &liOffset, NULL);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwReadFile failed,%d", status);
    }

    ulReadDataSize = (ULONG)iosb.Information;

    DbgPrint("%s", pBuffer);

    ZwClose(hFile);
    ExFreePoolWithTag(pBuffer,'dduh');
}

VOID WriteFileTest()
{
    KdBreakPoint();
    HANDLE hFile = NULL;
    IO_STATUS_BLOCK iosb{};
    OBJECT_ATTRIBUTES objectAttributes{};
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING strFileName{};
    RtlInitUnicodeString(&strFileName, L"\\??\\C:\\aa.txt");

    InitializeObjectAttributes(&objectAttributes, &strFileName, 
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    status = ZwCreateFile(&hFile, FILE_APPEND_DATA, &objectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwCreateFile failed,%d", status);
        return;
    }

    RtlZeroMemory(&iosb, sizeof(iosb));
    UNICODE_STRING str{};
    RtlInitUnicodeString(&str, L"Append Data");
    PUCHAR pBuffer = (PUCHAR)ExAllocatePool2(POOL_FLAG_NON_PAGED, str.Length, 'euiz');
    if (pBuffer == NULL)
    {
        DbgPrint("ExAllocatePool2 failed");
        return;
    }
    memcpy(pBuffer, str.Buffer, str.Length);
    status = ZwWriteFile(hFile, NULL, NULL, NULL, &iosb, str.Buffer, str.Length, NULL, NULL);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwWriteFile failed,%d", status);
    }

    //实际写入的数据
    //iosb.Information

    ZwClose(hFile);
    ExFreePoolWithTag(pBuffer, 'euiz');
}

VOID RenameFileTest()
{
    KdBreakPoint();
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES objectAttributes{};
    IO_STATUS_BLOCK iosb{};
    NTSTATUS status = STATUS_SUCCESS;
    PFILE_RENAME_INFORMATION pRenameInfo{};
    ULONG ulLength = (260 + sizeof(FILE_RENAME_INFORMATION)); //文件名

    pRenameInfo = (PFILE_RENAME_INFORMATION)ExAllocatePool2(POOL_FLAG_NON_PAGED, ulLength, 'uihf');

    if (pRenameInfo ==  NULL)
    {
        DbgPrint("ExAllocatePool2 failed");
        return;
    }

    RtlZeroMemory(pRenameInfo, ulLength);

    UNICODE_STRING  srcFileName{};
    UNICODE_STRING destFileName{};

    RtlInitUnicodeString(&srcFileName, L"\\??\\C:\\aa.txt");
    RtlInitUnicodeString(&destFileName, L"\\??\\C:\\bb.txt");

    pRenameInfo->FileNameLength = destFileName.Length;
    wcscpy(pRenameInfo->FileName, destFileName.Buffer);
    pRenameInfo->ReplaceIfExists = 0;
    pRenameInfo->RootDirectory = NULL;

    InitializeObjectAttributes(&objectAttributes, &srcFileName, 
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwCreateFile(&hFile, SYNCHRONIZE | DELETE, &objectAttributes, &iosb, NULL, 0, 
        FILE_SHARE_READ, FILE_OPEN, 
        FILE_SYNCHRONOUS_IO_NONALERT | FILE_NO_INTERMEDIATE_BUFFERING, NULL, 0);

    if (!NT_SUCCESS(status))
    {
        ExFreePoolWithTag(&pRenameInfo, 'uihf');
        return;
    }

    status = ZwSetInformationFile(hFile, &iosb, pRenameInfo, ulLength, FileRenameInformation);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwSetInformationFile failed.%d", status);
    }

    ZwClose(hFile);
    ExFreePoolWithTag(pRenameInfo, 'uihf');
}

VOID EnumFileTest(UNICODE_STRING strPath)
{
    KdBreakPoint();
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES objectAttributes{};
    IO_STATUS_BLOCK iosb{};
    NTSTATUS status = STATUS_SUCCESS;

    InitializeObjectAttributes(&objectAttributes, &strPath,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwCreateFile(&hFile,
        FILE_LIST_DIRECTORY | SYNCHRONIZE | FILE_ANY_ACCESS,
        &objectAttributes, &iosb, NULL,
        FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE,
        FILE_OPEN, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT,
        NULL, 0);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("ZwCreateFile failed,%d",status);
        return;
    }

    ULONG ulLength = (2 * 4096 + sizeof(FILE_BOTH_DIR_INFORMATION)) * 0x2000;
    PFILE_BOTH_DIR_INFORMATION pDir = (PFILE_BOTH_DIR_INFORMATION)ExAllocatePool2(POOL_FLAG_NON_PAGED, ulLength, 'uyhd');

    if (pDir == NULL)
    {
        ZwClose(hFile);
        DbgPrint("ExAllocatePool2 failed");
        return;
    }

    PFILE_BOTH_DIR_INFORMATION pBegindPtr = pDir;

    status = ZwQueryDirectoryFile(hFile, NULL, NULL, NULL, &iosb, pDir, 
        ulLength, FileBothDirectoryInformation, FALSE, NULL, FALSE);

    if (!NT_SUCCESS(status))
    {
        ExFreePoolWithTag(pDir, 'uyhd');
        ZwClose(hFile);
        DbgPrint("ZwQueryDirectoryFile failed,%d", status);
        return;
    }

    UNICODE_STRING strTemp;
    UNICODE_STRING strCurrentDir;
    UNICODE_STRING strParentDir;
    RtlInitUnicodeString(&strCurrentDir, L".");
    RtlInitUnicodeString(&strParentDir, L"..");
    WCHAR wcFileName[1024]{};

    while (TRUE)
    {
        RtlZeroMemory(wcFileName, 1024);
        RtlCopyMemory(wcFileName, pDir->FileName, pDir->FileNameLength);

        RtlInitUnicodeString(&strTemp, wcFileName);

        if ((0 != RtlCompareUnicodeString(&strTemp,&strCurrentDir,TRUE)) 
            && (0 != RtlCompareUnicodeString(&strTemp, &strParentDir, TRUE)))
        {
            if (pDir->FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                DbgPrint("[Directory]\t%wZ\n", &strTemp);
            }
            else
            {
                DbgPrint("[File]\t%wZ\n", &strTemp);
            }
        }

        if (0 == pDir->NextEntryOffset)
        {
            DbgPrint("[Enum Over]");
            break;
        }

        pDir = (PFILE_BOTH_DIR_INFORMATION)((PUCHAR)pDir + pDir->NextEntryOffset);
    }

    ExFreePoolWithTag(pBegindPtr, 'uyhd');
    ZwClose(hFile);
}

VOID IRPCreateFileTest()
{
    PFILE_OBJECT hFile = NULL;
    IO_STATUS_BLOCK iosb{};
    NTSTATUS status = STATUS_SUCCESS;

    UNICODE_STRING ustrFilePath;
    RtlInitUnicodeString(&ustrFilePath, L"\\??\\C:\\dd.txt");
   
    ULONG ulFileNameMaxSize = 512;
    WCHAR wszName[100] = { 0 };
    UNICODE_STRING ustrRootPath;
    OBJECT_ATTRIBUTES objectAttributes = { 0 };
    HANDLE hRootFile = NULL;
    PFILE_OBJECT pRootFileObject = NULL;
    PFILE_OBJECT pFileObject = NULL;
    PDEVICE_OBJECT pRootDeviceObject = NULL;
    PDEVICE_OBJECT pRootRealDevice = NULL;
    PIRP pIrp = NULL;
    KEVENT kEvent = { 0 };
    AUX_ACCESS_DATA auxAccessData = { 0 };
    IO_SECURITY_CONTEXT ioSecurityContext{};
    PIO_STACK_LOCATION pIoStackLocation = NULL;

    wcscpy(wszName, L"\\??\\A:\\");
    wszName[4] = ustrFilePath.Buffer[0];
    RtlInitUnicodeString(&ustrRootPath, wszName);
    DbgPrint("RootPath: %wZ\n", &ustrRootPath);
    InitializeObjectAttributes(&objectAttributes, &ustrRootPath, OBJ_KERNEL_HANDLE, NULL, NULL);
    status = IoCreateFile(&hRootFile, GENERIC_READ | SYNCHRONIZE,
        &objectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0, CreateFileTypeNone,
        NULL, IO_NO_PARAMETER_CHECKING);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("IoCreateFile Error[0x%X]", status);
        return;
    }

    status = ObReferenceObjectByHandle(hRootFile, FILE_READ_ACCESS, *IoFileObjectType, KernelMode, (PVOID*)& pRootFileObject, NULL);
    if (!NT_SUCCESS(status))
    {
        ZwClose(hRootFile);
        DbgPrint("ObReferenceObjectByHandle Error[0x%X]\n", status);
        return;
    }

    //获取磁盘根目录设备对象
    pRootDeviceObject = pRootFileObject->Vpb->DeviceObject;
    pRootRealDevice = pRootFileObject->Vpb->RealDevice;

    //关闭磁盘根目录句柄和对象
    ObDereferenceObject(pRootFileObject);
    ZwClose(hRootFile);

    //创建Irp
    pIrp = IoAllocateIrp(pRootDeviceObject->StackSize, FALSE);
    if (NULL == pIrp)
    {
        DbgPrint("IoAllocateIrp Error!\n");
        return;
    }

    KeInitializeEvent(&kEvent, EVENT_TYPE::SynchronizationEvent, FALSE);

    //创建空文件对象
    InitializeObjectAttributes(&objectAttributes, NULL, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = ObCreateObject
    
}