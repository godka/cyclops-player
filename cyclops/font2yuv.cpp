#include <ft2build.h>  
#include <freetype/freetype.h>     
#include <freetype/ftglyph.h>
// 全局句柄     
FT_Library g_ftLib;    
FT_Face g_ftFace; 

void renderFont(unsigned char *render);
int FontInit(const char* fontFileName)    
{    
    // 初始化字体库  
    if ( FT_Init_FreeType(&g_ftLib) ) { return 2; }    
    if ( FT_New_Face(g_ftLib,fontFileName,0,&g_ftFace) ) { return 1; }    
    return 0;    
}   

//----------------------------------------------------------------------------//     
unsigned char* FontToYUV(wchar_t uchar,int size,int* width,int*height,int* x,int* y) 
{    
    // 设置字体大小  
    FT_Set_Char_Size(g_ftFace,0<<6,size<<6,96,96);    
    // 加载并渲染字体GLYPH     
    FT_Load_Glyph(g_ftFace,FT_Get_Char_Index(g_ftFace,uchar),FT_LOAD_DEFAULT);   
	//FT_Outline_Translate(
    FT_Glyph glyph;    
    FT_Get_Glyph(g_ftFace->glyph,&glyph);    
    FT_Render_Glyph(g_ftFace->glyph,FT_RENDER_MODE_NORMAL);    
    // 拷贝到图片写入文件     
    FT_Bitmap *pBmp = &g_ftFace->glyph->bitmap;   
	
    *width = pBmp->width;    
	*height = pBmp->rows;  
	*x = g_ftFace->glyph->bitmap_left;
	*y = g_ftFace->glyph->bitmap_top;
	//FT_Done_Glyph(glyph);
	//glyph = NULL;
	return pBmp->buffer;
}   
void renderFont(unsigned char *render){
	if(*render == 0)*render = 1;
}
int StringToYUV(unsigned char *buff,int buffwidth,int buffheight,wchar_t* uchar,int size,int* width,int*height) 
{    
	int length = wcslen(uchar);
	//calc width
	int stringlength = 0;
	int stringheight = 0;
	memset(buff,0,buffwidth * buffheight);
	for(int i = 0;i<length;i++){
		int ww,hh;
		int xx,yy;
		unsigned char* tmpstr = FontToYUV(uchar[i],size,&ww,&hh,&xx,&yy);
		if(50 - yy + hh > stringheight) stringheight = 50 - yy + hh;
		
		for(int y = 0;y<hh;y++){
			memcpy(buff + (50 - yy + y + 1) * buffwidth + 1 + xx + stringlength,tmpstr,ww);
			tmpstr+=ww;
		}
		stringlength+=ww + 4;
	}
	*width = stringlength + 2;
	*height = stringheight + 4;

#ifdef FASTRENDER		
		for(int y = 1;y<*height - 1;y++){
			for(int x = 1;x<*width -1;x++){		
				//if(buff[(y + 1) * buffwidth + x + 1 + stringlength] > 0){
				if(buff[(y) * buffwidth + x] > 1){
					//buff[(y) * buffwidth + x] = 1;
					renderFont(&buff[y * buffwidth + x - 1]);
					renderFont(&buff[(y + 1) * buffwidth + x]);
					renderFont(&buff[y * buffwidth + x + 1]);
					renderFont(&buff[(y - 1) * buffwidth + x]);
				}
			}
		}
#endif
	return 0;
}  
//----------------------------------------------------------------------------//     
void FontClose(void)    
{    
    FT_Done_Face(g_ftFace);    
    FT_Done_FreeType(g_ftLib);    
}    