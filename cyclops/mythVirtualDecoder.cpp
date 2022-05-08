#include "mythVirtualDecoder.hh"


mythVirtualDecoder::mythVirtualDecoder(void)
	:mythAvlist()
{
	flag = 0;
	m_timeid = SDL_AddTimer(1000, TimerCallbackStatic, this);
}
void mythVirtualDecoder::start()
{
}
void mythVirtualDecoder::stop()
{
}
mythVirtualDecoder* mythVirtualDecoder::CreateNew(void){
	return new mythVirtualDecoder();
}
mythVirtualDecoder::~mythVirtualDecoder(void)
{
	SDL_RemoveTimer(m_timeid);
}

Uint32 mythVirtualDecoder::TimerCallbackStatic(Uint32 interval, void *param)
{
	mythVirtualDecoder* decoder = (mythVirtualDecoder*) param;
	return decoder->TimerCallback(interval);
}

Uint32 mythVirtualDecoder::TimerCallback(Uint32 interval)
{
	int tmp = m_count - ori_count;
	ret_count = tmp / interval;
	ori_count = m_count;
	//printf("%d k/s\n", ret_count);
	return interval;
}
int  mythVirtualDecoder::getSpeed(){
	return ret_count;
}