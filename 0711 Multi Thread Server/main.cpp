#include <iostream>
#include <process.h>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

using namespace std;

int Money = 0;


CRITICAL_SECTION CalculateCS;


HANDLE MutexHandle;


unsigned WINAPI ThreadFunctionPlus(void* Parameter)
{
	//������ ��(�ڹ������)
	//EnterCriticalSection(&CalculateCS);
	//OpenMutexA((DWORD)MutexHandle, true, "Money");
	WaitForSingleObject(MutexHandle, INFINITE);

	for (int i = 0; i < 50; ++i)
	{
		++Money;
		cout << "Worker1 Thread   " << Money << endl;

	}
	//��� �������Ƿ� ����(�ڹ��� ����)
	//LeaveCriticalSection(&CalculateCS);
	ReleaseMutex(MutexHandle);
	
	//cout << "Worker Thread" << endl;
	

	return 0;
}

unsigned WINAPI ThreadFunctionMinus(void* Parameter)
{
	//������ ��(�ڹ������)
	//EnterCriticalSection(&CalculateCS);
	//OpenMutexA((DWORD)MutexHandle, true, "Money");

	WaitForSingleObject(MutexHandle, INFINITE);
	for (int i = 0; i < 50; ++i)
	{
		--Money;
		cout << "Worker2 Thread   "<< Money << endl;
	}
	ReleaseMutex(MutexHandle);
	//��� �������Ƿ� ����(�ڹ��� ����)
	//LeaveCriticalSection(&CalculateCS);
	//cout << "Worker Thread" << endl;

	return 0;
}



int main()
{

	HANDLE ThreadHandles[2];

	DWORD StartTime = timeGetTime();


	//�ʱ�ȭ(�ڹ��� �ʱ�ȭ)
	InitializeCriticalSection(&CalculateCS);
	MutexHandle = CreateMutexA(nullptr, TRUE, "Money");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		cout << "�̹� ���� ���Դϴ�." << endl;
		system("pause");
		exit(-1);

	}




	//C, C++ API ��� �� �ȱ����� ��� ����
	ThreadHandles[0] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunctionPlus, nullptr, 0, nullptr);
	ThreadHandles[1] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunctionMinus, nullptr, 0, nullptr);
	//ThreadFunction(nullptr);


	//Thread���� �Լ� ������ ����ġ�� ���� CloseHandle�� ȣ��Ǿ �߰��� ���� ���� �� �����Ƿ�
	//�Ϸ��Ҷ����� ��ٸ��� �Լ�(Block)
	WaitForMultipleObjects(2, ThreadHandles, true, INFINITE);



	//WaitForSingleObject(ThreadHandle1, INFINITE);
	//WaitForSingleObject(ThreadHandle2, INFINITE);

	//for (int i = 0; i < 100000; ++i);


	//cout << "Main Thread" << ThreadHandel << endl;
	cout <<"Main Thread   " << Money << endl;

	//CloseHandle(ThreadHandle1);
	//CloseHandle(ThreadHandle2);



	//�ݱ�
	DeleteCriticalSection(&CalculateCS);

	CloseHandle(MutexHandle);



	DWORD EndTime = timeGetTime();

	cout << "Elapsed Time : " << EndTime - StartTime << endl;



	return 0;
}