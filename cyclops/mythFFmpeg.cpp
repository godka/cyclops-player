#include "mythFFmpeg.hh"
#include <stdio.h>
mythFFmpeg::mythFFmpeg(void* phwnd)
{
	avFrame = NULL;
	this->avCodecContext =  NULL;
	this->avCodec = NULL;
	hwnd = phwnd;
	Init();
}

void mythFFmpeg::yuv2RGB(int width,int height,
	const void* ysrc,const void* usrc,const void* vsrc,
	int ysize,int usize,int vsize,void** dst){
		const void* src[] = {ysrc,vsrc,usrc};
		int src_linesize[] = {ysize,vsize,usize};
		yuv2RGB(width,height,(const void**)&src,src_linesize,dst);
}
void mythFFmpeg::yuv2RGB(int width,int height,const void** src,int* src_linesize,void** dst){
	struct SwsContext *img_convert_ctx = sws_getContext(
		width,height,PIX_FMT_YUV420P,
		width,height,PIX_FMT_RGBA,
		SWS_FAST_BILINEAR,NULL,NULL,NULL);
	int dstwidth[] = {width*4,width*4,width*4};
	if(img_convert_ctx){
		sws_scale(img_convert_ctx,(const uint8_t *const*)src,src_linesize,0,height,
			(uint8_t *const*)dst,dstwidth);
	}
	sws_freeContext(img_convert_ctx);
}
mythFFmpeg* mythFFmpeg::CreateNew(void* phwnd)
{
	return new mythFFmpeg(phwnd);
}
bool mythFFmpeg::Init(){
	//if it has been initialized before, we should do cleanup first
	Cleanup();

	av_register_all();
	avcodec_register_all();
	avCodec = avcodec_find_decoder(CODEC_ID_H264);
	avCodecContext = avcodec_alloc_context();
	if (!avCodecContext) {
		//failed to allocate codec context
		Cleanup();
		return false;
	}
	if (avcodec_open(avCodecContext, avCodec) < 0) {
		//failed to open codec
		Cleanup();
		return false;
	}
	if (avCodecContext->codec_id == CODEC_ID_H264){
		avCodecContext->flags2 |= CODEC_FLAG2_CHUNKS;
		//avCodecContext->flags2 |= CODEC_FLAG2_SHOW_ALL;
	}
	avFrame = avcodec_alloc_frame();
	if (!avFrame){
		//failed to allocate frame
		Cleanup();
		return false;
	}
	return true;
}
mythFFmpeg::~mythFFmpeg(void)
{
	Cleanup();
}
void mythFFmpeg::Cleanup(){
	if (avFrame != NULL){
		av_free(avFrame);
		avFrame = NULL;
	}
	if(avCodecContext != NULL){
		avcodec_close(avCodecContext);
		av_free(avCodecContext);
		avCodecContext = NULL;
	}
}

void mythFFmpeg::ProcessFrame(unsigned char* framePtr, int frameSize,responseHandler* response){
	AVPacket avpkt = {0};
	av_init_packet(&avpkt);
	avpkt.data = framePtr;
	avpkt.size = frameSize;
	if(avCodecContext == NULL && avFrame == NULL)return;
	while (avpkt.size > 0) {
		int got_frame = 0;
		try
		{
			int len = avcodec_decode_video2(avCodecContext, avFrame, &got_frame, &avpkt);
			if (len < 0) {
				return;
			}
			if (got_frame){
				//callback
				response(this->hwnd, (char**) avFrame->data, avFrame->linesize, avFrame->width, avFrame->height);
			}
			avpkt.size -= len;
			avpkt.data += len;
		}
		catch (void* e)
		{
			
		}
	}
	//av_free_packet(&avpkt);
}
