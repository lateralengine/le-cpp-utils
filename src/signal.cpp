#include <le-cpp-utils/signal.h>

std::atomic_bool g_stopFlag = false;

///
/// POSIX signal (SIGINT, SIGTERM) handler for Linux or Ctrl+C handler for Windows
/// 
#ifdef _WIN32
#include <Windows.h>
BOOL WINAPI signalHandler(DWORD signal)
{
	if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT)
		g_stopFlag = true;
	return TRUE;
}
bool registerSignalHandler()
{
	if (!SetConsoleCtrlHandler(signalHandler, TRUE))
		return false;
	return true;
}
#else
#include <csignal>
void signalHandler(int signal)
{
	g_stopFlag = true;
}
bool registerSignalHandler()
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);
	return true;
}
#endif
