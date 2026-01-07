#pragma once

#include <string>

void StartIpcServer();
void StopIpcServer();
void BroadcastData(const std::string& data);
