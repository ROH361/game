#pragma once

#include "Packet.h"

class CPythonExchange : public CSingleton<CPythonExchange>
{
	public:
		enum
		{
			EXCHANGE_ITEM_MAX_NUM = 12,
		};

		typedef struct trade
		{
			char					name[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_LEVEL_IN_TRADE
			DWORD					level;
#endif

			DWORD					item_vnum[EXCHANGE_ITEM_MAX_NUM];
			BYTE					item_count[EXCHANGE_ITEM_MAX_NUM];
			DWORD					item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
			TPlayerItemAttribute	item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];

			BYTE					accept;
			long long				elk;
#ifdef ENABLE_CHEQUE_SYSTEM
			DWORD					cheque;
#endif
		} TExchangeData;

	public:
		CPythonExchange();
		virtual ~CPythonExchange();

		void			Clear();

		void			Start();
		void			End();
		bool			isTrading();

		// Interface

		void			SetSelfName(const char *name);
		void			SetTargetName(const char *name);

		char			*GetNameFromSelf();
		char			*GetNameFromTarget();

#ifdef ENABLE_LEVEL_IN_TRADE
		void			SetSelfLevel(DWORD level);
		void			SetTargetLevel(DWORD level);

		DWORD			GetLevelFromSelf();
		DWORD			GetLevelFromTarget();
#endif

		void			SetElkToTarget(long long elk);
		void			SetElkToSelf(long long elk);

		long long		GetElkFromTarget();
		long long		GetElkFromSelf();

		void			SetItemToTarget(DWORD pos, DWORD vnum, BYTE count);
		void			SetItemToSelf(DWORD pos, DWORD vnum, BYTE count);

		void			SetItemMetinSocketToTarget(int pos, int imetinpos, DWORD vnum);
		void			SetItemMetinSocketToSelf(int pos, int imetinpos, DWORD vnum);

		void			SetItemAttributeToTarget(int pos, int iattrpos, BYTE byType, short sValue);
		void			SetItemAttributeToSelf(int pos, int iattrpos, BYTE byType, short sValue);

		void			DelItemOfTarget(BYTE pos);
		void			DelItemOfSelf(BYTE pos);

		DWORD			GetItemVnumFromTarget(BYTE pos);
		DWORD			GetItemVnumFromSelf(BYTE pos);

		BYTE			GetItemCountFromTarget(BYTE pos);
		BYTE			GetItemCountFromSelf(BYTE pos);

		DWORD			GetItemMetinSocketFromTarget(BYTE pos, int iMetinSocketPos);
		DWORD			GetItemMetinSocketFromSelf(BYTE pos, int iMetinSocketPos);

		void			GetItemAttributeFromTarget(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);
		void			GetItemAttributeFromSelf(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);

		void			SetAcceptToTarget(BYTE Accept);
		void			SetAcceptToSelf(BYTE Accept);

		bool			GetAcceptFromTarget();
		bool			GetAcceptFromSelf();

		bool			GetElkMode();
		void			SetElkMode(bool value);
#ifdef ENABLE_CHEQUE_SYSTEM
		void			SetChequeToTarget(DWORD cheque);
		void			SetChequeToSelf(DWORD cheque);

		DWORD			GetChequeFromTarget();
		DWORD			GetChequeFromSelf();
#endif

	protected:
		bool				m_isTrading;

		bool				m_elk_mode;
		TExchangeData		m_self;
		TExchangeData		m_victim;
};
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
