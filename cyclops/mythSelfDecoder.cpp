#include "mythSelfDecoder.hh"
#include "base64.h"
mythSelfDecoder::mythSelfDecoder(char* ip,int type)
	:mythVirtualDecoder()
{
	this->m_strRemoteIP = ip;
	this->m_emDeviceType = type;
}

static mythSelfDecoder* CreateNew(char* ip,int type){
	return new mythSelfDecoder(ip,type);
}
mythSelfDecoder::~mythSelfDecoder(void)
{
}
int mythSelfDecoder::AssertSocket(int index){
	return 0;
}
void mythSelfDecoder::start(){
	
	msocket = MythSocket::CreatedNew(m_strRemoteIP.c_str(),554);
	string Interleaved;
	string sRecordUrl ="" ;
	string Authorization = "cm9vdDpwYXNz";
	string SessionString = "";
	if(m_emDeviceType == BOSH)
	{
		Authorization = "cm9vdDpwYXNz";
		sRecordUrl= "/?line=1";
	}
	else if(m_emDeviceType == AXIS)
	{		Authorization = "cm9vdDpwYXNz";
	sRecordUrl= "/axis-media/media.amp";
	}
	else if(m_emDeviceType== HAIKEY)
	{
		Authorization = "YWRtaW46MTIzNDU=";
		sRecordUrl= "/h264/ch1/main/av_stream";
	}
	else if(m_emDeviceType== SONY)
	{
		Authorization = "YWRtaW46MTIzNDU=";
		sRecordUrl= "/media/video1";
	}
	else if(m_emDeviceType == ONVIF)
	{
		sRecordUrl = "/MainStream";
	}
	else if(m_emDeviceType == DLINK)
	{
		Authorization = "cm9vdDpwYXNz";
		sRecordUrl ="/h264";
	}
	else
		sRecordUrl = "";
	int iCameraID = 1001;
	int urlSplit = sRecordUrl.find("?");
	urlSplit = urlSplit>=0 ?urlSplit:0;
	string profile1 = sRecordUrl.substr(0,urlSplit);
	string profile2 = sRecordUrl.substr(urlSplit,sRecordUrl.length()-urlSplit);
	string profile0 = "rtsp://" + m_strRemoteIP+":554";
	string SendStr0 ="",SendStr1 ="",SendStr2 = "",SendStr3 = "",SendStr4 = "";
	string rtspTmpStr;
	char tmpBuff[2000];
	char govHeader[200];
	int govHeaderLength = 0;
	char tHeader[200];
	int tHeaderLength = 0;
	string  GOVHeader;
	int curOrder = 0;
	char H65[5] = {0x00,0x00,0x00,0x01,0x65};
	char H41[5]={0x00,0x00,0x00,0x01,0x41};;
	char H61[5]={0x00,0x00,0x00,0x01,0x61};

	char sH65[4] = {0x00,0x00,0x01,0x65};
	char sH41[4]={0x00,0x00,0x01,0x41};;
	/*char head7[129] = {0x00,0x00,0x00,0x01,0x67,0x64,0x00,0x1F,0xAD,0x84,0x05,0x45,0x62,0xB8,0xAC,0x54,0x74,0x20,0x2A,0x2B,0x15,0xC5,0x62,0xA3,0xA1,0x01,0x51,0x58,0xAE,0x2B,0x15,0x1D,0x08,0x0A,0x8A,0xC5,0x71,0x58,0xA8,0xE8,0x40,0x54,0x56,0x2B,0x8A,0xC5,0x47,0x42,0x02,0xA2,0xB1,0x5C,0x56,0x2A,0x3A,0x10,0x24,0x85,0x21,0x39,0x3C,0x9F,0x27,0xE4,0xFE,0x4F,0xC9,0xF2,0x79,0xB9,0xB3,0x4D,0x08,0x12,0x42,0x90,0x9C,0x9E,0x4F,0x93,0xF2,0x7F,0x27,0xE4,0xF9,0x3C,0xDC,0xD9,0xA6,0xB4,0x02,0x80,0x2D,0xD8,0x0A,0xA4,0x00,0x00,0x03,0x00,0x04,0x00,0x00,0x03,0x00,0xF1,0x81,0x00,0x03,0x0D,0x40,0x00,0x36,0xEE,0xAF,0x7B,0xE1,0x78,0x44,0x23,0x50,0x00,0x00,0x00,0x01,0x68,0xEE,0x3C,0xB0};*/

	char * resultBuffer;
	resultBuffer = new char[1024 * 1024];
	char *tempbuf;
	tempbuf = new char[256];
	int badLoop = 0;
	event_set(rstpEvent);
	while(this->flag)
	{
	
		//this->ReadAll();
		bool bGetIframe = false;
		bool frameFinished = false;
		rtspTmpStr = "";
		//				DateTime currTime;
		string tmpLength;
		int currWaitTime;

		curOrder = OPTION;
		SendStr0 = "OPTIONS " + profile0 + profile1 + profile2 + " RTSP/1.0\r\nCSeq: "+IntToStr(++m_iCSeq)+"\r\nUser-Agent: LIVE555 Streaming Media v2013.02.11\r\n\r\n";
	/*	event_reset(rstpEvent);
		SendData(SendStr0.c_str(),SendStr0.length());*/

		DWORD dw =event_timedwait(rstpEvent,WAIT_FOR_TIME);
		if(dw != WAIT_OBJECT_0)
			event_reset(rstpEvent);
		string G1 = "",G11 ="",G2 = "",G22 = "";

	
		curOrder = DESCRIBE;
			SendStr1 = "DESCRIBE " + profile0 + profile1 + profile2 + " RTSP/1.0\r\nCSeq: "+IntToStr(++m_iCSeq)+"\r\nUser-Agent: LIVE555 Streaming Media v2013.02.11\r\nAccept: application/sdp\r\nAuthorization: Basic " + Authorization + "\r\n\r\n";
		msocket->socket_SendStr(SendStr1.c_str(),SendStr1.length());
		//SendData(SendStr1.c_str(),SendStr1.length());
		dw = event_timedwait(rstpEvent,WAIT_FOR_TIME);
		if(dw != WAIT_OBJECT_0)
		{
			continue;
		}
			SendStr2 = "SETUP " + profile0 + profile1 + profile2 + "/trackID=1?videocodec=h264 RTSP/1.0\r\nCSeq: "+IntToStr(++m_iCSeq)+"\r\nUser-Agent: LIVE555 Streaming Media v2013.02.11\r\nTransport: RTP/AVP/TCP;unicast;interleaved=0-1\r\nAuthorization: Basic " + Authorization + "\r\n\r\n";

		int controlIndex = 0;
		if(m_emDeviceType == SONY)
			printf("error====\r\n");

		while(rtspTmpStr.find("a=control:",controlIndex)>=0&&m_emDeviceType == ONVIF||m_emDeviceType == DLINK)
		{
			int controlIndex1 = rtspTmpStr.find("a=control:",controlIndex);
			int controlIndex2 = rtspTmpStr.find("\r\n",controlIndex1);
			controlIndex = controlIndex2+1;
			string controlTemp = rtspTmpStr.substr(controlIndex1+strlen("a=control:"),controlIndex2-controlIndex1-strlen("a=control:"));
			if(controlTemp.length()>2)
			{
				//SendStr2 = "SETUP " + profile0 + profile1 + profile2 +"/"+controlTemp+ "?videocodec=h264 RTSP/1.0\r\nCSeq: 3\r\nUser-Agent: LIVE555 Streaming Media v2013.02.11\r\nTransport: RTP/AVP/TCP;unicast;interleaved=0-1\r\nAuthorization: Basic " + Authorization + "\r\n\r\n";
				if(m_emDeviceType == DLINK||m_emDeviceType== SONY)
					SendStr2 = "SETUP " + profile0 + profile1 + profile2 +"/"+controlTemp+ " RTSP/1.0\r\nCSeq:  "+IntToStr(m_iCSeq)+"\r\nUser-Agent: LIVE555 Streaming Media v2013.02.11\r\nTransport: RTP/AVP/TCP;unicast;interleaved=0-1\r\nAuthorization: Basic " + Authorization + "\r\n\r\n";

				else
					SendStr2 = "SETUP " + profile0 + profile1 + profile2 +"/"+controlTemp+ "?videocodec=h264 RTSP/1.0\r\nCSeq:  "+IntToStr(m_iCSeq)+"\r\nUser-Agent: LIVE555 Streaming Media v2013.02.11\r\nTransport: RTP/AVP/TCP;unicast;interleaved=0-1\r\nAuthorization: Basic " + Authorization + "\r\n\r\n";

				break;
			}
		}
		if(rtspTmpStr.find("sprop-parameter-sets=")>=0&&rtspTmpStr.length())
		{
			GOVHeader = rtspTmpStr.substr(rtspTmpStr.find("sprop-parameter-sets=") + 21,rtspTmpStr.length());
			if(GOVHeader.find("\r\n")>=0)
				GOVHeader = GOVHeader.substr(0,GOVHeader.find("\r\n") );
			G1 = GOVHeader.substr(0,GOVHeader.find(",") );
			G2 = GOVHeader.substr(GOVHeader.find(",") + 1,GOVHeader.length());
			string temp ="";
			char* p;
			int tlen1=0;
			if(G1.length()>0&&G2.length()>=0)
			{
				char * baseBuf = new char[256];
				memset(baseBuf,0,256);
				baseBuf = base64Decode(G1.c_str(),G1.length(),&tlen1);
				G1 = string(baseBuf,tlen1);
				//	memset(baseBuf,0,256);
				baseBuf = base64Decode(G2.c_str(),G2.length(),&tlen1);
				G2 = string(baseBuf,tlen1);
				delete  baseBuf;
			}
		}

		GOVHeader = string(H65,4) + G1;
		GOVHeader = GOVHeader + string(H65,4);
		int len1 = GOVHeader.length();
		//	TRACE("\r\n%d",len1);
		GOVHeader = GOVHeader + G2;
		len1 = GOVHeader.length();
		//	TRACE("\r\n%d",len1);


		/*	if(GOVHeader.length()<=15)
		GOVHeader = string(head7,129);*/
	
		curOrder = SETUP;
		msocket->socket_SendStr(SendStr2.c_str(),SendStr2.length());
		//SendData(SendStr2.c_str(),SendStr2.length());
		dw = event_timedwait(rstpEvent,WAIT_FOR_TIME);
		if(dw != WAIT_OBJECT_0)
			continue;
			//Í¬¾°  LibVLC/2.0.6 (LIVE555 Streaming Media v2012.12.18)
	SendStr3 = "PLAY " +  profile0 + profile1 + profile2 + " RTSP/1.0\r\nCSeq: "+IntToStr(++m_iCSeq)+"\r\nUser-Agent:LIVE555 Streaming Media v2013.02.11\r\nSession: ";
	//SendStr4 = "\r\nRange: npt=0.000-\r\n\r\n";
	SendStr4 = "\r\nRange: npt=0.000-\r\nAuthorization: Basic " + Authorization + "\r\n\r\n";
	
		curOrder = PLAY;
		SendStr4 = SendStr3+SessionString+SendStr4;
		msocket->socket_SendStr(SendStr4.c_str(),SendStr4.length());
		//SendData(SendStr4.c_str(),SendStr4.length());
		dw = event_timedwait(rstpEvent,WAIT_FOR_TIME);
		if(dw != WAIT_OBJECT_0)
			continue;
		memcpy(govHeader,GOVHeader.c_str(),GOVHeader.length());
		govHeaderLength = GOVHeader.length();
		memcpy(resultBuffer,GOVHeader.c_str(),GOVHeader.length());

		//streamDataInfo.CopyBeginData(GOVHeader.c_str(),GOVHeader.length());

		int resultBufferLength = GOVHeader.length();
		//int resultBufferLength =0;
		//Synchronize(UpdateImage);

		//	TRACE(_T("ShowImage-header\r\n"));
		//SingleH264ToYuv(resultBuffer,resultBufferLength);

		resultBufferLength = 0;
		printf("start playing \r\n");
		while(this->flag)
		{

			if(!AssertSocket(16))
			{
				this->flag = false;
				continue;
			}

			msocket->ReadSocket(tmpBuff,16);
			if((unsigned char)tmpBuff[0] != 36)
			{
				break;
			}
			int StreamLength = (unsigned char)tmpBuff[2] * 256 + (unsigned char)tmpBuff[3] - 12;
			if((unsigned char)tmpBuff[1] != atoi(Interleaved.c_str()))
			{
				//TRACE("Read(tmpBuff,StreamLength);%d\r\n",StreamLength);

				if(!AssertSocket(StreamLength))
				{
					this->flag = false;
					continue;
				}

				msocket->ReadSocket(tmpBuff,StreamLength);
				continue;
			}

			if(!AssertSocket(1))
			{
				this->flag = false;
				continue;
			}

			msocket->ReadSocket(tHeader,1);
			StreamLength = StreamLength - 1;

			if((unsigned char)tHeader[0] == 0x67||(unsigned char)tHeader[0] == 0x68||(unsigned char)tHeader[0] == 0x06)
			{

				if(!AssertSocket(StreamLength))
				{
					this->flag = false;
					continue;
				}

				msocket->ReadSocket(tmpBuff,StreamLength);
				/*	if((unsigned char)tHeader[0] == 0x67)
				GOVHeader = string(H65,4) + tmpBuff;
				else if((unsigned char)tHeader[0] == 0x68)
				{
				GOVHeader = GOVHeader + string(H65,4);
				GOVHeader = GOVHeader + G2;
				}	*/
				continue;
			}
			if((unsigned char)tHeader[0] == 0x7C)
			{

				if(!AssertSocket(1))
				{
					this->flag = false;
					continue;
				}

				msocket->ReadSocket(tHeader,1);
				StreamLength -= 1;
			
				if((unsigned char)tHeader[0] == 0x85)
				{
					memcpy(tHeader,govHeader,govHeaderLength);
					bGetIframe = true;
					memcpy(tHeader + govHeaderLength,H65,5);
					tHeaderLength = govHeaderLength + 5;

				}
				else if((unsigned char)tHeader[0] == 0x81||(unsigned char)tHeader[0] == 0x45)
				{
					switch(m_emDeviceType)
					{
					case BOSH:
						memcpy(tHeader,H61,5);
						tHeaderLength = 5;break;
					case SONY:
						memcpy(tHeader ,H41,5);
						tHeaderLength =  5;
						break;
					case HAIKEY:
						memcpy(tHeader ,H61,5);
						tHeaderLength =  5;
						break;
					default:
						memcpy(tHeader,sH41,4);
						tHeaderLength = 4;
						break;
					}
				}
			}
			else if((unsigned char)tHeader[0] == 0x5C)
			{

				if(!AssertSocket(1))
				{
					this->flag = false;
					continue;
				}

				msocket->ReadSocket(tHeader,1);
				StreamLength -= 1;
				if((unsigned char)tHeader[0] == 0x81)
				{
					memcpy(tHeader,govHeader,govHeaderLength);
					if(m_emDeviceType == BOSH)
					{
						memcpy(tHeader + govHeaderLength,H61,5);
						tHeaderLength = govHeaderLength + 5;
					}
					else if(m_emDeviceType == AXIS)
					{
						memcpy(tHeader + govHeaderLength,H41,5);
						tHeaderLength = govHeaderLength + 5;
					}
					else if(m_emDeviceType == HAIKEY)
					{
						memcpy(tHeader + govHeaderLength,H61,5);
						tHeaderLength = govHeaderLength + 5;
					}
					else
					{
						memcpy(tHeader + govHeaderLength,sH41,4);
						tHeaderLength = govHeaderLength + 4;
					}

				}
				else
				{
					memcpy(tHeader,H41,5);
					tHeaderLength = 5;
				}

			}
			else if((unsigned char)tHeader[0] == 0x65)
			{
				bGetIframe = true;
				memcpy(tHeader,govHeader,govHeaderLength);
				if(m_emDeviceType == BOSH)
				{
					memcpy(tHeader + govHeaderLength,H65,5);
					tHeaderLength = govHeaderLength + 5;
				}
				else if(m_emDeviceType == AXIS)
				{
					memcpy(tHeader + govHeaderLength,H65,5);
					tHeaderLength = govHeaderLength + 5;
				}
				else if(m_emDeviceType == HAIKEY)
				{
					memcpy(tHeader + govHeaderLength,H65,5);
					tHeaderLength = govHeaderLength + 5;
				}
				else
				{
					memcpy(tHeader + govHeaderLength,H65,5);
					tHeaderLength = govHeaderLength + 5;
				}
			}
			else
			{
				if(m_emDeviceType == BOSH)
				{
					memcpy(tHeader,H61,5);
					tHeaderLength = 5;
				}
				else if(m_emDeviceType == AXIS)
				{
					memcpy(tHeader,H41,5);
					tHeaderLength = 5;
				}
				else if(m_emDeviceType == HAIKEY)
				{
					memcpy(tHeader,H41,5);
					tHeaderLength = 5;
				}
				else
				{
					memcpy(tHeader ,sH41,4);
					tHeaderLength =  4;
				}
			}
			if(resultBufferLength == 0)
			{
				memcpy(resultBuffer + resultBufferLength,tHeader,tHeaderLength);

				resultBufferLength += tHeaderLength;
				//streamDataInfo.CopyBeginData(tHeader,tHeaderLength);
			}
			//TRACE("\r\n5");
			if((unsigned char)tmpBuff[5] == 0x63||(unsigned char)tmpBuff[5] == 0x60||(unsigned char)tmpBuff[5] == 0x23||(unsigned char)tmpBuff[5] == 0x69)
			{

				if(!AssertSocket(StreamLength))
				{
					this->flag = false;
					continue;
				}

				msocket->ReadSocket(resultBuffer + resultBufferLength,StreamLength);
				resultBufferLength += StreamLength;
				
				frameFinished = false;
				//	TRACE(_T("\r\n image adding"));
			}

			if((unsigned char)tmpBuff[5] == 0xE3||(unsigned char)tmpBuff[5] == 0xE0||(unsigned char)tmpBuff[5] == 0xA3|(unsigned char)tmpBuff[5] == 0xE9)
			{

				if(!AssertSocket(StreamLength))
				{
					this->flag = false;
					continue;
				}
				msocket->ReadSocket(resultBuffer + resultBufferLength,StreamLength);
			
				resultBufferLength += StreamLength;
				put((unsigned char*)resultBuffer,resultBufferLength);
				//streamDataInfo.CopyBeginData(resultBuffer,resultBufferLength,bGetIframe);
				//if(bGetIframe)
				//	printf("curAvaiData = %d  Address %s \r\n",this->m_AvailableData,this->m_strRemoteIP.c_str());

				bGetIframe = false;
				//SDL_LockMutex(m_videoClientLock);
				//for (list<CVideoClient*>::iterator iter=vcVideoClient.begin();iter!=vcVideoClient.end();iter++)
				//{

				//	if((*iter)->IsConnected())
				//	{
				//		if((*iter)->m_bFirstSendData)
				//		{
				//			string head = "HTTP/1.1 200 OK\r\n";
				//			head+="Server: WWeb/2.0\r\n";
				//			head+="Connection: Close\r\n";
				//			head+="Content-Type: multipart/x-mixed-replace;boundary=--myboundary\r\n\r\n \n\n--myboundary\n";
				//			//Send(head,head.GetLength());
				//			(*iter)->SendData(head.c_str(),head.length());
				//			(*iter)->m_bFirstSendData = false;
				//		}
				//		sprintf(tempbuf,
				//			"Content-Type: image/h264\r\nContent_Length: %06d Stamp:07dd010c 000b1e0b 75 0000%d\n\n",
				//			resultBufferLength,

				//			(*iter)->iFrameCount);
				//		(*iter)->iFrameCount++;
				//		(*iter)->SendData(tempbuf,strlen(tempbuf));
				//		(*iter)->SendData(resultBuffer,resultBufferLength);
				//		string head= " \n\n--myboundary\n";
				//		(*iter)->SendData(head.c_str(),head.length());

				//	}
				//}
			//	SDL_UnlockMutex(m_videoClientLock);
				event_set(sendEvent);
				frameFinished = true;
				resultBufferLength = 0;
				//TRACE("\r\n7");

			}
			badLoop = 0;
		}
		//	TRACE(_T("End------\r\n"));
	}
	printf("CGetStreamByRtsp  thread end %s\r\n",this->m_strRemoteIP.c_str());
	delete tempbuf;
	delete resultBuffer;
}