#pragma once
#include "mythvirtualdecoder.hh"
#include "MythSocket.hh"
#include <string>
using namespace std;
#define WAIT_FOR_TIME 3000
#define BOSH 0x00
#define AXIS 0x01
#define HAIKEY 0x02
#define BOSH 0x03
#define SONY 0x04
#define ONVIF 0x05
#define DLINK 0x06

#define	OPTION 0x00
#define	DESCRIBE 0x01
#define	SETUP 0x02
#define	PLAY 0x03
#define	PLAYING 0x04
#define	NONE 0x05

class mythSelfDecoder :
	public mythVirtualDecoder
{
public:
	~mythSelfDecoder(void);
	static mythSelfDecoder* CreateNew(char* ip,int type);
	void start();
protected:
	mythSelfDecoder(char* ip,int type);
	string m_strRemoteIP;
	MythSocket* msocket;
	//bosh,axis...
	int m_emDeviceType;
	int AssertSocket(int index);
};

