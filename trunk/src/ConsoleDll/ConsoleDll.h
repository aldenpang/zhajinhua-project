

_declspec(dllexport) int ClientNetLayer_Initialize();
_declspec(dllexport) int ClientNetLayer_Connect(const char* _ip, const int _port);
_declspec(dllexport) int ClientNetLayer_Send(const char* _buffer);
_declspec(dllexport) int ClientNetLayer_Uninitialize();