#include <string>
#include "../Shared/Packet.h"

extern "C" _declspec(dllexport) int ClientNetLayer_Initialize();
extern "C" _declspec(dllexport) int ClientNetLayer_Connect(std::string& _ip, int _port);
//extern "C" _declspec(dllexport) int ClientNetLayer_Connect_For_Unity3D();
extern "C" _declspec(dllexport) int ClientNetLayer_SendData(const char* _buffer, int _bufferSize);
extern "C" _declspec(dllexport) int ClientNetLayer_SendPacket(Packet* _pk);
extern "C" _declspec(dllexport) int ClientNetLayer_Uninitialize();

extern "C" _declspec(dllexport) int test()
{
	MessageBox(0, "from DLL", "Info", MB_OK);
	return 100;
}