#pragma once

#include <atomic>

extern std::atomic_bool g_stopFlag;

/// <summary>
/// Registers POSIX signal (SIGINT, SIGTERM) handler for Linux or Ctrl+C handler for Windows.
/// Sets g_stopFlag to true when the signal is received.
/// </summary>
/// <returns>true if registering the signal handler was successful.</returns>
bool registerSignalHandler();
