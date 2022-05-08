#include "mythStreamDecoder.hh"

mythStreamDecoder::mythStreamDecoder(char* ip, int port, int CameraID)
	:mythVirtualDecoder()
{
	m_ip = ip;
	m_port = port;
	m_cameraid = CameraID;
	m_count = 0;
	ori_count = 0;
}
mythStreamDecoder* mythStreamDecoder::CreateNew(char* ip, int CameraID){
	return new mythStreamDecoder(ip, 5834, CameraID);
}
mythStreamDecoder* mythStreamDecoder::CreateNew(char* ip, int port, int CameraID){
	return new mythStreamDecoder(ip, port, CameraID);
}
void mythStreamDecoder::start(){
	startthread = SDL_CreateThread(decodethreadstatic,"decode",this);
}
void mythStreamDecoder::stop(){
	//this->startthread = SDL_CreateThread(decodethreadstatic,"decode",this);
	flag = 1;
	SDL_WaitThread(startthread,NULL);
	return;
}
int mythStreamDecoder::decodethreadstatic(void* data){
	mythStreamDecoder* m_decoder = (mythStreamDecoder*) data;
	return m_decoder->decodethread();
}
int mythStreamDecoder::decodethread(){
#define BUFF_COUNT 1024*1024	
	char* buf = new char[BUFF_COUNT];
	msocket = MythSocket::CreatedNew(m_ip,m_port);
	char tmpsendstr[100];
	sprintf(tmpsendstr,"GET /CameraID=%d&Type=zyh264 HTTP/1.0\r\n\r\n",m_cameraid);
	msocket->socket_SendStr(tmpsendstr,-2);
	SDL_Delay(100);
	while(flag == 0){
		int rc = msocket->socket_ReceiveDataLn2(buf,BUFF_COUNT,"Content_Length: ");
		if (rc > 0) {
			m_count += rc;
			put((unsigned char*)buf,rc);
			//ffmpeg->ProcessFrame((unsigned char*)buf,rc,disPlaySDL);
		}else{
			msocket->socket_CloseSocket();
			delete msocket;
			SDL_Delay(1000);
			msocket = MythSocket::CreatedNew(m_ip,m_port);
			msocket->socket_SendStr(tmpsendstr,-2);
		}
	}
	msocket->socket_CloseSocket();
	this->free();
	return 0;
}
mythStreamDecoder::~mythStreamDecoder(void)
{
}

unsigned int mythStreamDecoder::GetTimeCount()
{
	return ret_count;
}
