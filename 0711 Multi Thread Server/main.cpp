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
	//문열고 들어감(자물쇠잠금)
	//EnterCriticalSection(&CalculateCS);
	//OpenMutexA((DWORD)MutexHandle, true, "Money");
	WaitForSingleObject(MutexHandle, INFINITE);

	for (int i = 0; i < 50; ++i)
	{
		++Money;
		cout << "Worker1 Thread   " << Money << endl;

	}
	//사용 다했으므로 나감(자물쇠 열음)
	//LeaveCriticalSection(&CalculateCS);
	ReleaseMutex(MutexHandle);
	
	//cout << "Worker Thread" << endl;
	

	return 0;
}

unsigned WINAPI ThreadFunctionMinus(void* Parameter)
{
	//문열고 들어감(자물쇠잠금)
	//EnterCriticalSection(&CalculateCS);
	//OpenMutexA((DWORD)MutexHandle, true, "Money");

	WaitForSingleObject(MutexHandle, INFINITE);
	for (int i = 0; i < 50; ++i)
	{
		--Money;
		cout << "Worker2 Thread   "<< Money << endl;
	}
	ReleaseMutex(MutexHandle);
	//사용 다했으므로 나감(자물쇠 열음)
	//LeaveCriticalSection(&CalculateCS);
	//cout << "Worker Thread" << endl;

	return 0;
}



int main()
{

	HANDLE ThreadHandles[2];

	DWORD StartTime = timeGetTime();


	//초기화(자물쇠 초기화)
	InitializeCriticalSection(&CalculateCS);
	MutexHandle = CreateMutexA(nullptr, TRUE, "Money");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		cout << "이미 실행 중입니다." << endl;
		system("pause");
		exit(-1);

	}




	//C, C++ API 사용 시 안깨지고 사용 가능
	ThreadHandles[0] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunctionPlus, nullptr, 0, nullptr);
	ThreadHandles[1] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunctionMinus, nullptr, 0, nullptr);
	//ThreadFunction(nullptr);


	//Thread에서 함수 실행을 끝마치기 전에 CloseHandle이 호출되어서 중간에 끊겨 버릴 수 있으므로
	//완료할때까지 기다리는 함수(Block)
	WaitForMultipleObjects(2, ThreadHandles, true, INFINITE);



	//WaitForSingleObject(ThreadHandle1, INFINITE);
	//WaitForSingleObject(ThreadHandle2, INFINITE);

	//for (int i = 0; i < 100000; ++i);


	//cout << "Main Thread" << ThreadHandel << endl;
	cout <<"Main Thread   " << Money << endl;

	//CloseHandle(ThreadHandle1);
	//CloseHandle(ThreadHandle2);



	//닫기
	DeleteCriticalSection(&CalculateCS);

	CloseHandle(MutexHandle);



	DWORD EndTime = timeGetTime();

	cout << "Elapsed Time : " << EndTime - StartTime << endl;



	return 0;
}