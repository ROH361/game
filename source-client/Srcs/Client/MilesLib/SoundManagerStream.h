#ifndef __MILESLIB_CSOUNDMANAGERSTREAM_H__
#define __MILESLIB_CSOUNDMANAGERSTREAM_H__

#include "SoundBase.h"
#include "SoundInstance.h"

class CSoundManagerStream : public CSoundBase
{
	public:
		enum
		{
			MUSIC_INSTANCE_MAX_NUM = 3,
		};

	public:
		CSoundManagerStream();
		virtual ~CSoundManagerStream();

		bool					Initialize();
		void					Destroy();

		bool					SetInstance(DWORD dwIndex, const char* filename);
		CSoundInstanceStream *	GetInstance(DWORD dwIndex);

	protected:
		bool					CheckInstanceIndex(DWORD dwIndex);

	protected:
		CSoundInstanceStream	m_Instances[MUSIC_INSTANCE_MAX_NUM];
};

#endif
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
