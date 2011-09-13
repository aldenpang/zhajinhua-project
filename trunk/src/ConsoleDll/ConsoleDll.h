#include <string>

extern "C" _declspec(dllexport) int ClientNetLayer_Initialize();
extern "C" _declspec(dllexport) int ClientNetLayer_Connect(std::string& _ip, int _port);
//extern "C" _declspec(dllexport) int ClientNetLayer_Connect_For_Unity3D();
extern "C" _declspec(dllexport) int ClientNetLayer_Send(const char* _buffer);
extern "C" _declspec(dllexport) int ClientNetLayer_Uninitialize();

extern "C" _declspec(dllexport) int test()
{
	MessageBox(0, L"from DLL", L"Info", MB_OK);
	return 100;
}