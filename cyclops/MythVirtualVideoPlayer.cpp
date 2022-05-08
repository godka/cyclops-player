#include "MythVirtualVideoPlayer.hh"


MythVirtualVideoPlayer::MythVirtualVideoPlayer(mythVirtualDecoder* decoder){
	this->Text_texture = NULL;
	this->renderer = NULL;
	this->window = NULL;
	this->virtualdecoder = decoder;
	this->flag = 0;
	TTF_Init();
	font = TTF_OpenFont("font.ttf", 20);
	if (!font){
		//SDL_ShowSimpleMessageBox(0,"Error",SDL_GetError(),NULL);
		printf("Error = %s", SDL_GetError());
	}
}

MythVirtualVideoPlayer* MythVirtualVideoPlayer::CreateNew(mythVirtualDecoder* virtualdecoder){
	return new MythVirtualVideoPlayer(virtualdecoder);
}

void MythVirtualVideoPlayer::start(){}
void MythVirtualVideoPlayer::stop(){}
void MythVirtualVideoPlayer::pause(){}
void MythVirtualVideoPlayer::slow_next(){}
void MythVirtualVideoPlayer::fast_next(){}
int MythVirtualVideoPlayer::capture(){return 0;}
#ifdef _WIN32
int MythVirtualVideoPlayer::Create_Text(wchar_t *text,int x,int y)
{
	//SDL_Texture *tex;
	SDL_Surface *sur;

	int time0 = SDL_GetTicks();

	SDL_Color back_color = { 0, 0, 0, 255 };
	SDL_Color color = { 255, 255, 255, 255 };
	TTF_SetFontOutline(font, 3);
	SDL_Surface* surb = TTF_RenderUNICODE_Blended(font, (const Uint16*) text, back_color);
	//puts(text);
	TTF_SetFontOutline(font, 0);
	sur = TTF_RenderUNICODE_Blended(font, (const Uint16*) text, color);
	SDL_Rect rect = { 2, 2, 0, 0 };
	SDL_BlitSurface(sur, NULL, surb, &rect);
	Text_rect.w = sur->w; Text_rect.h = sur->h;
	Text_rect.x = x; Text_rect.y = y;
	Text_texture = SDL_CreateTextureFromSurface(renderer, surb);
	SDL_SetTextureAlphaMod(Text_texture, 200);
	SDL_FreeSurface(sur);
	int time1 = SDL_GetTicks();
	//printf("loading font time = %d\n", time1 - time0);
	// = tex;
	return 0;
}
#else
int MythVirtualVideoPlayer::Create_Text(const char* text, int x, int y)
{

	//SDL_Texture *tex;
	SDL_Surface *sur;
	TTF_Init();

	int time0 = SDL_GetTicks();

	font = TTF_OpenFont("font.ttf", 24);
	if (!font){
		//SDL_ShowSimpleMessageBox(0,"Error",SDL_GetError(),NULL);
		printf("Error = %s", SDL_GetError());
		return NULL;
	}
	SDL_Color back_color = { 0, 0, 0, 255 };
	SDL_Color color = { 255, 255, 255, 255 };
	TTF_SetFontOutline(font, 3);
	SDL_Surface* surb = TTF_RenderUTF8_Blended(font, text, back_color);
	//puts(text);
	TTF_SetFontOutline(font, 0);
	sur = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Rect rect = { 2, 2, 0, 0 };
	SDL_BlitSurface(sur, NULL, surb, &rect);
	Text_rect.w = sur->w; Text_rect.h = sur->h;
	Text_rect.x = x; Text_rect.y = y;
	Text_texture = SDL_CreateTextureFromSurface(renderer, surb);
	SDL_SetTextureAlphaMod(Text_texture, 200);
	SDL_FreeSurface(sur);
	TTF_CloseFont(font);
	TTF_Quit();
	int time1 = SDL_GetTicks();
	printf("loading font time = %d", time1 - time0);
	// = tex;
	return 0;
}
#endif
MythVirtualVideoPlayer::~MythVirtualVideoPlayer(void)
{
	TTF_CloseFont(font);
	TTF_Quit();
}

int MythVirtualVideoPlayer::SetAlpha(int alpha){ return 0; }
