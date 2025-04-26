#pragma once

typedef std::pair<DWORD, std::string> CRCPair;

void ProcessScanner_Destroy();
bool ProcessScanner_Create();
void ProcessScanner_ReleaseQuitEvent();

bool ProcessScanner_PopProcessQueue(std::vector<CRCPair>* pkVct_crcPair);
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
