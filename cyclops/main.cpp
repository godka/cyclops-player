#include <iostream>
#include "mythStreamDecoder.hh"
#include "mythLive555Decoder.hh"
#include "MythH264VideoPlayer.hh"
using namespace std;
int main(int args, char** argv)
{
	try
	{
		mythVirtualDecoder* virtualdecoder = NULL;
		SDL_Init(SDL_INIT_EVERYTHING);
		if (args >= 3){
			virtualdecoder = mythStreamDecoder::CreateNew(argv[1], atoi(argv[2]));
		}else
			virtualdecoder = mythStreamDecoder::CreateNew("127.0.0.1",6830);
		//mythVirtualDecoder* virtualdecoder = mythLive555Decoder::CreateNew("rtsp://192.168.31.128:10554/tcp/av0_0", "admin", "888888");
		MythH264VideoPlayer* videoplayer = MythH264VideoPlayer::CreateNew(virtualdecoder,0,1280,720);
		//videoplayer->Create_Text(L"那就不用考虑了，肯定是吹牛的", 15, 15);
		videoplayer->start();
		puts("exit");
	}
	catch (void* e)
	{
		printf("fuck you\n");
	}
	return 0;
}
