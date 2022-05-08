#pragma once
#include <stdio.h>
#include "mythavlist.hh"

class mythVirtualDecoder :
	public mythAvlist
{
public:
	static mythVirtualDecoder* CreateNew(void);
	virtual void start();
	virtual void stop();
	static Uint32 TimerCallbackStatic(Uint32 interval, void *param);
	Uint32 TimerCallback(Uint32 interval);
	int getSpeed();
protected:
	mythVirtualDecoder(void);
	virtual ~mythVirtualDecoder(void);
	int flag;
	SDL_TimerID m_timeid;
	unsigned int m_count;
	unsigned int ori_count;
	unsigned int ret_count;
};

