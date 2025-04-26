#pragma once

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
#include "Packet.h"

class CPythonAcce : public CSingleton<CPythonAcce>
{
public:
//	DWORD	dwPrice;
	long long	dwPrice;
	typedef std::vector<TAcceMaterial> TAcceMaterials;

public:
	CPythonAcce();
	virtual ~CPythonAcce();

	void	Clear();
//	void	AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos);
	void	AddMaterial(long long dwRefPrice, BYTE bPos, TItemPos tPos);
	void	AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs);
//	void	RemoveMaterial(DWORD dwRefPrice, BYTE bPos);
	void	RemoveMaterial(long long dwRefPrice, BYTE bPos);
//	DWORD	GetPrice() {return dwPrice;}
	long long	GetPrice() {return dwPrice;}
	bool	GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell);
	void	GetResultItem(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);

protected:
	TAcceResult	m_vAcceResult;
	TAcceMaterials	m_vAcceMaterials;
};
#endif
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
