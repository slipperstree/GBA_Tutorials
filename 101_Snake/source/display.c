#include "common.h"
#include "string.h"
#include "display.h"
#include "embSnake.h"
#include "font.h"
#include "embSnakeDevice.h"

// 各种字符串定义，可自由修改
// 屏幕较小的话要改短一些,大屏也不要太长,预留的缓冲区只有128字节,超出的话会发生未知问题
u8* STR_PRESS_ANY_KEY_CN             = (u8*)"  按任意键继续..";
u8* STR_PRESS_ANY_KEY_EN             = (u8*)"Press Any Key..";
u8* STR_DEMO_MENU_CN                 = (u8*)"壹速 貳声音 叁返";  //壹貳叁代表按钮①②③图标
u8* STR_DEMO_MENU_EN                 = (u8*)"壹Sound 貳Speed 叁Back";
u8* STR_GAME_MENU_CN                 = (u8*)"壹左 貳声音 叁右";  //壹貳叁代表按钮①②③图标
u8* STR_GAME_MENU_EN                 = (u8*)"壹Left 貳Foward 叁Right";
u8* STR_GAMEOVER_GAMEOVER_CN         = (u8*)"游戏结束";
u8* STR_GAMEOVER_GAMEOVER_EN         = (u8*)"Game Over";
u8* STR_GAMEOVER_NEWRECORD_CN        = (u8*)"恭喜,刷新记录!";
u8* STR_GAMEOVER_NEWRECORD_EN        = (u8*)"New Record";
u8* STR_GAMEOVER_HSCORE_CN           = (u8*)"记录:";
u8* STR_GAMEOVER_HSCORE_EN           = (u8*)"Record";
u8* STR_GAMEOVER_SCORE_CN            = (u8*)"得分:";
u8* STR_GAMEOVER_SCORE_EN            = (u8*)"Score:";

#define  TITLE_Y         15
#define  SCROE_Y         60
#define HSCROE_Y        100

// 框架厚度（drawFrame中计算并设置）
u8 frameThickness=1;

// 游戏区域左上角坐标Y偏移(也就是框架厚度)
u16 GAME_AREA_X_OFFSET=2;

// 游戏区域左上角坐标Y偏移（drawFrame中计算并设置）(去掉顶部框架和顶部信息区域以及上方横栏高度(也就是框架厚度))
u16 GAME_AREA_Y_OFFSET=0;

// 临时变量用(sprintf等),需要修改size的话请一并修改common.c中的同名变量,这里只是声明
extern u8 buff[128];

// 内部变量定义
u16 _ColorBack=0x0000, _ColorFore=0x0000;
#define COLOR_BG       _ColorBack
#define COLOR_FO       _ColorFore
u16 _ColorSnake=0x0000, _ColorApple=0x0000, _ColorFrame=0x0000;
#define COLOR_SNAKE    _ColorSnake
#define COLOR_APPLE    _ColorApple
#define COLOR_FRAME    _ColorFrame

// 函数声明
void clearScreen(void);     // 清屏
Font_Type* showChar(u16 x, u16 y, u8 *chr, Font_Type *font, u16 colorBg, u16 colorFont);

// 获取随机颜色
u16 randBGR565(){
    u8 r=0, g=0, b=0;

    r = My_real_rand() % 10;
    #define RAND_COLOR My_real_rand()%50+200
    switch (r)
    {
    case 0:
        r = 0;
        g = RAND_COLOR;
        b = RAND_COLOR;
        break;
    case 1:
        r = RAND_COLOR;
        g = 0;
        b = RAND_COLOR;
        break;
    case 2:
        r = RAND_COLOR;
        g = RAND_COLOR;
        b = 0;
        break;
    case 3:
        r = RAND_COLOR;
        g = 0;
        b = 0;
        break;
    case 4:
        r = 0;
        g = RAND_COLOR;
        b = 0;
        break;
    case 5:
        r = 0;
        g = 0;
        b = RAND_COLOR;
        break;
    case 6:
        r = RAND_COLOR;
        g = RAND_COLOR;
        b = RAND_COLOR;
        break;
    default:
        r = My_real_rand();
        g = My_real_rand();
        b = My_real_rand();
        while (r + g + b < 300 || r + g + b > 600)
        {
            r = My_real_rand();
            g = My_real_rand();
            b = My_real_rand();
        }
        break;
    }
    return RGB888toBGR555(r, g, b);
}

void clearScreen(void){
    devFillRectange(0,0,SCREEN_W,SCREEN_H,COLOR_BG);
}

void devShowString(u16 x,u16 y, u8 *str,Font_Type *fontType, u16 colorBg, u16 colorFont)
{
	u8 j=0;
    u8 charWidth;
    Font_Type *lastUseFont;
    char* str1 = NULL;

	while (str[j]!='\0')
	{
        lastUseFont = showChar(x, y, &str[j], fontType, colorBg, colorFont);
        charWidth = lastUseFont->fontWidth;

		// 每输出一个字符或汉字后，向后位移一个字符或汉字的宽度
        x+= lastUseFont->fontWidth;
        // 自动换行
		if(x>SCREEN_W-fontType->fontWidth){
            x=0;
            y+=fontType->fontHeight;
        }
        // 继续显示下一个字符或汉字
		j++;
        // 如果是汉字那么需要向后移动两个字节
        if (lastUseFont->type == FONT_TYPE_HZ) j++;
	}
}

void DISP_drawWall(){
    u8 x,y;
    for (x = 0; x < SNAKE_SIZE_X; ++x)
    {
        for (y = 0; y < SNAKE_SIZE_Y; ++y)
        {
            if (SNAKE_getView(x, y) == SNAKE_VIEW_WALL){
                DISP_updateGameBlock(x, y);
            }
        }
    }
}

// 局部刷新函数，根据传入的游戏MAP指定位置，在画面上相应的区域进行重绘
// x,y : 游戏MAP坐标
// 
// 由于显示设备的画面刷新单位区域各不相同，有一些可以指定某一个点，
// 但有一些（比如这款12864的oled屏）是以字节为单位进行刷新的
// 所以，由于想刷新的区域所在的字节里也可能包含其他不应该被刷新的部分，
// 从而也需要取到这部分不应该被刷新的部分的数据，再加上这次真正想刷新的数据的部分，才能进行刷新，否则原来的数据就丢失了
void DISP_updateGameBlock(u8 x, u8 y){
    u8 blockSts, blockView;
    u8 shiwei, gewei;
    u16 startX, startY;
    u8 width, height;

    blockSts = SNAKE_getMapData(x, y);
    blockView = SNAKE_getView(x, y);

    // 简单绘制方块 -------------------------------------------
    
    // #define BLOCK_PIC_SIZE 10
    // startX = x * BLOCK_PIC_SIZE + 1;
    // startY = y * BLOCK_PIC_SIZE + 1;
    // width = BLOCK_PIC_SIZE-2;
    // height = BLOCK_PIC_SIZE-2;
    // if (blockSts == STS_EMPTY) {
    //     devFillRectange(x * BLOCK_PIC_SIZE, y * BLOCK_PIC_SIZE, BLOCK_PIC_SIZE, BLOCK_PIC_SIZE, COLOR_BLACK);
    // } else if(blockSts == STS_APPLE) {
    //     //ILI9163LCD(pPIC[column - blockStartColumn],OLED_DATA);
    //     devFillRectange(startX, startY, width, height, 0x001f);
    // } else if(blockSts / 10 == STS_HEAD / 10) {
    //     devFillRectange(x * BLOCK_PIC_SIZE, y * BLOCK_PIC_SIZE, BLOCK_PIC_SIZE, BLOCK_PIC_SIZE, 0x07e0);
    // } else {
    //     devFillRectange(x * BLOCK_PIC_SIZE, y * BLOCK_PIC_SIZE, BLOCK_PIC_SIZE, BLOCK_PIC_SIZE, COLOR_BLACK);
    //     devFillRectange(startX, startY, width, height, COLOR_WHITE);
    // }

    // 用位图字体绘制
    width = BLOCK_VIEW_FONT.fontWidth;
    height = BLOCK_VIEW_FONT.fontHeight;

    // 绘图开始坐标，原始坐标加上偏移坐标（游戏区域开始位置，在drawFrame函数中计算并设置）
    startX = x * width + GAME_AREA_X_OFFSET;
    startY = y * height + GAME_AREA_Y_OFFSET;

    #if ISDEBUG
    sprintf(buff, "DISP_updateGameBlock 1 fontWidth=%b2d, fontHeight=%b2d\r\n", width, height);LOG(buff);
    sprintf(buff, "DISP_updateGameBlock 2 x=%b2d, y=%b2d, blockSts=%b2d, view=%b2d, startX=%d, startY=%d\r\n",
                     x, y, blockSts, blockView, startX, startY);LOG(buff);
    #endif
    
    // 根据view绘图，各种view常量名里的「UDLR」代表与自己相邻部位方块所处的方向
    // 比如HEAD_U表示是蛇头并且与蛇头相邻部位的蛇身在蛇头的上方，也就是需要绘制向下的蛇头（有点绕，但这样便于程序处理）
    if (blockView == SNAKE_VIEW_APPLE)
    {
        // 食物
        showChar(startX, startY, FONT_SNAKE_BLOCK_APPLE, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_APPLE);
    }
    else if (blockView == SNAKE_VIEW_HEAD_U) {
        // 蛇头向下
        showChar(startX, startY, FONT_SNAKE_BLOCK_HEAD_U, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_HEAD_D) {
        // 蛇头向上
        showChar(startX, startY, FONT_SNAKE_BLOCK_HEAD_D, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_HEAD_L) {
        // 蛇头向左
        showChar(startX, startY, FONT_SNAKE_BLOCK_HEAD_L, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_HEAD_R) {
        // 蛇头向右
        showChar(startX, startY, FONT_SNAKE_BLOCK_HEAD_R, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_BODY_V) {
        // 蛇身竖直
        showChar(startX, startY, FONT_SNAKE_BLOCK_BODY_V, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_BODY_LU) {
        // 蛇身左上弯曲 
        showChar(startX, startY, FONT_SNAKE_BLOCK_BODY_LU, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_BODY_UR) {
        // 蛇身右上弯曲
        showChar(startX, startY, FONT_SNAKE_BLOCK_BODY_UR, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_BODY_LD) {
        // 蛇身左下弯曲
        showChar(startX, startY, FONT_SNAKE_BLOCK_BODY_LD, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_BODY_DR) {
        // 蛇身右下弯曲
        showChar(startX, startY, FONT_SNAKE_BLOCK_BODY_DR, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_BODY_H) {
        // 蛇身水平
        showChar(startX, startY, FONT_SNAKE_BLOCK_BODY_H, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_TAIL_U) {
        // 蛇尾 尾尖朝下
        showChar(startX, startY, FONT_SNAKE_BLOCK_TAIL_U, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_TAIL_D) {
        // 蛇尾 尾尖朝上
        showChar(startX, startY, FONT_SNAKE_BLOCK_TAIL_D, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_TAIL_L) {
        // 蛇尾 尾尖朝右
        showChar(startX, startY, FONT_SNAKE_BLOCK_TAIL_L, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_TAIL_R) {
        // 蛇尾 尾尖朝左
        showChar(startX, startY, FONT_SNAKE_BLOCK_TAIL_R, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_SNAKE);
    }
    else if (blockView == SNAKE_VIEW_EMPTY) {
        // 清空方块
        devFillRectange(startX, startY, width, height, COLOR_BG);
    }
    else if (blockView == SNAKE_VIEW_WALL) {
        // 障碍物 不要在这里画，这里只负责游戏中会动态变化的内容
        //devFillRectange(startX, startY, width, height, COLOR_FO);
    }
    else {
        // TODO: 注意！当编译优化级别稍微调高的时候，下面这些无效代码如果不放开就会出现奇怪的现象，原因不明。
        //       目前经过测试，将编译级别设置为0-6都正常，只要大于等于7就会出问题。
        //       关于优化级别7的说明：扩展索引访问优化：适当时对寄存器变量用DPTR。对指针和数组访问进行执行速度和代码大小优化。
        //       还是不知道为什么会影响这个逻辑？
        //       参考 https://blog.csdn.net/tjcwt2011/article/details/94347250
        //       
        // shiwei = blockSts / 10*10;
        
        // switch (shiwei)
        // {
        // case STS_EMPTY:
        //     //devFillRectange(startX, startY, width, height, COLOR_BG);
        //     break;
        // default:
        //     break;
        // }
    }
}

//计算字符串在在指定字体下占用的实际像素宽度(不考虑换行的问题)
u16 calcStringWidth(u8 *str, Font_Type *fontType){
    u8 j=0;
    u16 width=0, asciiWidth=0, hzWidth=0;

    #if ISDEBUG
    //sprintf(buff, "calcStringWidth: str=%s\r\n", str); LOG(buff);
    #endif

    if (fontType->type == FONT_TYPE_HZ) {
        hzWidth = fontType->fontWidth;
        asciiWidth = ((Font_Type *)fontType->asciiZk)->fontWidth; // 使用汉字字体时，如果存在半角文字会显示外链的半角字体
    } else {
        asciiWidth = fontType->fontWidth;
        hzWidth = asciiWidth*2; // 使用ascii字体时，如果存在汉字会显示2个半角的X
    }
    while (str[j]!='\0')
	{
        if (str[j] >= 0x20 && str[j] <= 0x7E) {
            // 半角
            width += asciiWidth;
            j++;
        } else {
            // 全角
            width += hzWidth;
            j+=2;
        }
        //LOG("  calcStringWidth 5 in while...\r\n");
    }

    #if ISDEBUG
    //sprintf(buff, "calcStringWidth: width=%hd done.\r\n", width); LOG(buff);
    #endif

    return width;
}

Font_Type* showChar(u16 x, u16 y, u8 *chr, Font_Type *font, u16 colorBg, u16 colorFont){

    // 根据传入的chr和字体找到这个chr的fontData数组和其在数组中的偏移
    u8 nowRow=0,nowCol=0,fontWidth=0,fontHeight=0;
    u8 bytePerRow,bitIndex=0,charByteCnt=0;
    u16 charDataStartIndex=0;
    u8* fontData;
    u16 errSwitchColorTmp;
    Font_Type *useFont = NULL;

    // 获取当前字符/汉字的偏移index 以及 实际使用的Font(例如给定的字体是汉字字库，但指定的字符是半角字符，那么实际使用的font就是汉字字库里外链的半角字库)
    charDataStartIndex = FONT_getByteIndexAtFontData(chr, font, &useFont);
    // 点阵字体数组
    fontData = useFont->fontArray;
    // 点阵字体宽度
    fontWidth = useFont->fontWidth;
    // 点阵字体高度
    fontHeight = useFont->fontHeight;
    // 每行在字体数组中占用几个字节
    bytePerRow = (fontWidth-1)/8+1;

    if(charDataStartIndex == FONT_ERR_NOT_FOUND){
        // 如果在字库中没找到这个字, 试图显示一个半角的X并反色显示提示错误
        charDataStartIndex = 'X'-' ';
        errSwitchColorTmp = colorBg;
        colorBg = colorFont;
        colorFont = errSwitchColorTmp;
    }

	devPrepareForDrawArea(x, y, fontWidth, fontHeight);

	for(nowRow=0;nowRow<fontHeight;nowRow++){
		for(nowCol=0;nowCol<fontWidth;nowCol++){

			// 根据行列位置确定当前应该使用哪个字节哪个位
			bitIndex = nowCol%8;

			// 按位取点阵内容。1：绘制字体色，0：绘制背景色
			if ((fontData[charDataStartIndex+(bytePerRow*nowRow) + nowCol/8] << bitIndex) & 0x80)
			{
				devPointInDrawArea(colorFont);
			}else{
				devPointInDrawArea(colorBg);
			}
		}
	}

    return useFont;
}

//居中显示字符串,指定颜色
void showStringCenterColor(u16 y, u8 *str, Font_Type *fontType, u16 colorBg, u16 colorFo, u8 isClear){
    u16 strWidth;

    #if ISDEBUG
    //sprintf(buff, "showStringCenterColor: %s\r\n", str); LOG(buff);
    #endif

    strWidth = calcStringWidth(str, fontType);
    
    if (isClear)
    {
        devFillRectange((SCREEN_W - strWidth) / 2, y, strWidth, fontType->fontHeight, colorBg);
    } else {
        devShowString((SCREEN_W - strWidth) / 2, y, str, fontType, colorBg, colorFo);
    }

    #if ISDEBUG
    //sprintf(buff, "showStringCenterColor: %s done.\r\n", str); LOG(buff);
    #endif
}

//居中显示字符串
void showStringCenter(u16 y, u8 *str, Font_Type *fontType, u8 isClear){
    showStringCenterColor(y, str, fontType, COLOR_BG, COLOR_FO, isClear);
}

//居中显示带阴影的字符串
//TODO: 先绘制的阴影文字会被后绘制的本体的背景部分覆盖掉，想实现这个功能必须先取得当前像素点当前颜色，绘制本体非点阵部分时不要绘制前景色而是该点原来的颜色.效率较低暂时不做，
void showStringCenterShadow(u16 y, u8 offsetX, u8 offsetY, u8 *str, Font_Type *fontType){
    //先画阴影
    devShowString((SCREEN_W - fontType->fontWidth * strlen(str)) / 2 + offsetX, y + offsetY, str, fontType, COLOR_BG, COLOR_RED);
    //再画本体
    showStringCenter(y, str, fontType, 0);
}

void DISP_setBackColor(u16 color){
    _ColorBack = color;
}

void DISP_setForeColor(u16 color){
    _ColorFore = color;
}

void DISP_setSnakeColor(u16 color){
    _ColorSnake = color;
}

void DISP_setAppleColor(u16 color){
    _ColorApple = color;
}

void DISP_setFrameColor(u16 color){
    _ColorFrame = color;
}

// ########### 绘制各画面 ######################################################################################################
// 一般分成3种类型的函数
// 1-各画面的初始化函数。只会在进入该画面时被调用一次，可用来绘制画面上不会变化的内容
// 2-带有参数flashOnOff的函数。会被定时调用，flashOnOff 会在1和0之间来回切换，自己根据需要刷新画面上的动态内容，比如闪烁文字等
// 3-其他的不带flashOnOff的函数。各有各的用途，一般在特定事件发生时被调用并会传入相关数据，比如吃到食物的时候会传入最新的分数。

// ##### 首页 初始化时被调用一次(isStartUp：1=上电后第一次调用，0=非上电后第一次调用)
void DISP_drawWelcome(u8 isStartUp){
    u16 titleStartX, titleY, logoX, logoY;
    u16 r=50;

    devScreenOFF();

    clearScreen();

    #if ISDEBUG && DEBUG_SKIP_HOME
        //调试模式并跳过主画面
        return;
    #endif

    #ifdef UI_LANG_EN
        showStringCenter(20, "Snake+", &FONT32, 0);
        // showStringCenterShadow(20, 5, 5, "Snake+", &FONT32, 0); //暂时没有实现
        showStringCenter(60, "2021 C.L", &FONT16, 0);
    #endif
    #ifdef UI_LANG_CN
        // 标题 贪吃蛇
        titleStartX = (SCREEN_W - FONTHZ_TITLE64.fontWidth * 4) / 2;
        titleY = SCREEN_H/8;
        showChar(titleStartX, titleY, "贪", &FONTHZ_TITLE64, COLOR_BG, COLOR_TITLE1);
        showChar(titleStartX + FONTHZ_TITLE64.fontWidth + FONTHZ_TITLE64.fontWidth/2, titleY, "吃", &FONTHZ_TITLE64, COLOR_BG, COLOR_TITLE1);
        showChar(titleStartX + FONTHZ_TITLE64.fontWidth*3, titleY, "蛇", &FONTHZ_TITLE64, COLOR_BG, COLOR_TITLE2);

        devScreenON();

        // 学电LOGO
        logoX = (SCREEN_W - FONTHZ_XD_LOGO40.fontWidth) / 2;
        logoY = titleY + FONTHZ_TITLE64.fontHeight + FONTHZ_XD_LOGO40.fontHeight / 2;
        if (isStartUp)
        {
            My_delay_ms(1000);
            // 学电LOGO缓缓出现的动画效果
            while(1){
                r+=10;
                if (r>=256) break;
                showChar(logoX, logoY, FONT_HZ_XD_LOGO, &FONTHZ_XD_LOGO40, RGB888toBGR555(r, 0, 0), COLOR_BG);
            }
            // 出现后等待一会儿
            My_delay_ms(1000);
        } else {
            // 非上电画面，快速显示
            // 学电LOGO缓缓出现的动画效果
            while(r<255){
                r+=20;
                if (r>=256) r = 255;
                showChar(logoX, logoY, FONT_HZ_XD_LOGO, &FONTHZ_XD_LOGO40, RGB888toBGR555(r, 0, 0), COLOR_BG);
            }
            // 出现后等待一会儿
            My_delay_ms(500);
        }
    #endif
}

// ##### 首页 定期被调用
void DISP_flashWelcome(u8 flashOnOff){
    u16 logoX, logoY;

    u8 tmp=0;

    #ifdef UI_LANG_EN
        showStringCenter(80, STR_PRESS_ANY_KEY_EN, &FONT32, flashOnOff);

        // 变幻颜色
        // showStringCenter(60, "2021 C.L", &FONT16, 0);
    #endif
    #ifdef UI_LANG_CN
        #if ISDEBUG
        sprintf(buff, "  DISP_flashWelcome 1: call showStringCenter(str=%s pStr=%ld)\r\n", STR_PRESS_ANY_KEY_CN, &STR_PRESS_ANY_KEY_CN); LOG(buff);
        #endif
        
        // 学电LOGO变幻颜色
        logoX = (SCREEN_W - FONTHZ_XD_LOGO40.fontWidth) / 2;
        logoY = SCREEN_H/8 + FONTHZ_TITLE64.fontHeight + FONTHZ_XD_LOGO40.fontHeight / 2;
        showChar(logoX, logoY, FONT_HZ_XD_LOGO, &FONTHZ_XD_LOGO40, randBGR565(), COLOR_BG);

        // 闪烁文字
        showStringCenter(logoY + FONTHZ_XD_LOGO40.fontHeight + FONTHZ_XD_LOGO40.fontHeight / 2, STR_PRESS_ANY_KEY_CN, &FONTHZ32, flashOnOff);

        // 标题 贪吃蛇 变幻颜色
        // showChar(10, 20, "贪", &FONTHZ_TITLE64, COLOR_BG, randBGR565());
        // showChar(50, 20, "吃", &FONTHZ_TITLE64, COLOR_BG, randBGR565());
        // showChar(90, 20, "蛇", &FONTHZ_TITLE64, COLOR_BG, randBGR565());
    #endif
}

// ##### Demo页 初始化时被调用一次
void DISP_drawDemo(u8 soundOnOff){
    u8 topInfoY;

    #if ISDEBUG
    sprintf(buff, "DISP_drawDemo\r\n"); LOG(buff);
    #endif

    devScreenOFF();

    clearScreen();
    DISP_drawFrame();

    // 注意，这句计算必须放在DISP_drawFrame的后面。否则第一次计算时还未被设置正确的数值，为默认值1。
    // 误以为1是正确的值调整后面的位置第一次是对的，第二次就全错了。
    topInfoY =frameThickness + 2;

    // 顶部信息
    // 食物 = 0  (当前吃了多少个)
    showChar(frameThickness + 10, topInfoY+3, FONT_SNAKE_BLOCK_APPLE, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_FO);
    devShowString(frameThickness + 10 + BLOCK_VIEW_FONT.fontWidth, topInfoY, " =  0", &FONT20, COLOR_BG, COLOR_FO);
    // 居右显示文字
    devShowString(SCREEN_W - frameThickness - calcStringWidth("C.Ling 2022.3", &FONT20) - 5, topInfoY, "C.Ling 2022.3", &FONT20, COLOR_BG, COLOR_FO);

    // 游戏区域中央显示 演示模式 （显示区域需要设置为障碍物不让蛇通过）
    #ifdef UI_LANG_EN
        showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, STR_DEMO_MODE_EN, &FONT8, flashOnOff);
    #endif
    #ifdef UI_LANG_CN
        showChar(SNAKE_DEMO_TITLE_1_X * 12  + GAME_AREA_X_OFFSET, 
                 SNAKE_DEMO_TITLE_1_Y * 12 + GAME_AREA_Y_OFFSET, 
                 FONT_HZ_DEMO_MODE_YAN, &FONTHZ_DEMO_MOEDE24, COLOR_WINLOGO_R, COLOR_BG );
        showChar(SNAKE_DEMO_TITLE_2_X * 12  + GAME_AREA_X_OFFSET, 
                 SNAKE_DEMO_TITLE_2_Y * 12 + GAME_AREA_Y_OFFSET, 
                 FONT_HZ_DEMO_MODE_SHI1,&FONTHZ_DEMO_MOEDE24, COLOR_WINLOGO_G, COLOR_BG );
        showChar(SNAKE_DEMO_TITLE_3_X * 12  + GAME_AREA_X_OFFSET, 
                 SNAKE_DEMO_TITLE_3_Y * 12 + GAME_AREA_Y_OFFSET, 
                 FONT_HZ_DEMO_MODE_MO,  &FONTHZ_DEMO_MOEDE24, COLOR_WINLOGO_B, COLOR_BG );
        showChar(SNAKE_DEMO_TITLE_4_X * 12  + GAME_AREA_X_OFFSET, 
                 SNAKE_DEMO_TITLE_4_Y * 12 + GAME_AREA_Y_OFFSET, 
                 FONT_HZ_DEMO_MODE_SHI2,&FONTHZ_DEMO_MOEDE24, COLOR_WINLOGO_Y, COLOR_BG );
    #endif

    // 底部信息
    #ifdef UI_LANG_EN
        showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, STR_DEMO_MENU_EN, &FONT8, flashOnOff);
    #endif
    #ifdef UI_LANG_CN
        showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, STR_DEMO_MENU_CN, &FONTHZ32, 0);
    #endif

    // 绘制声音状态
    DISP_drawSound(soundOnOff);

    devScreenON();

    #if ISDEBUG
    sprintf(buff, "DISP_drawDemo done.\r\n"); LOG(buff);
    #endif
}

// ##### Demo页 吃到食物时被调用一次，更新画面上的分数
void DISP_updateDemoScore(u16 maxDemoScore, u16 nowDemoScore){
    u8 topInfoY = frameThickness + 2;

    // 顶部信息栏更新分数
    sprintf(buff, " =%3u", nowDemoScore);
    devShowString(frameThickness + 10 + BLOCK_VIEW_FONT.fontWidth, topInfoY, buff, &FONT20, COLOR_BG, COLOR_FO);
}

// ##### Demo页 死亡时被调用一次，可做一些显示处理，目前什么都没做
void DISP_updateDemoGameover(u16 maxDemoScore, u16 avgDemoScore, u16 lastDemoScore){
    //devShowString(0, SCREEN_H - FONT32.fontHeight, "                ", &FONT8, COLOR_BG, COLOR_FO);
    //sprintf(buff, "Max:%d Avg:%d", maxDemoScore, avgDemoScore, lastDemoScore);
    //devShowString(0, SCREEN_H - FONT32.fontHeight, buff, &FONT32, COLOR_BG, COLOR_FO);
}

// ##### Demo页 定期被调用 目前没用到
void DISP_flashDemo(u8 flashOnOff){
    #if ISDEBUG
    //sprintf(buff, "DISP_flashDemo: flashOnOff=%b2d ...\r\n", flashOnOff); LOG(buff);
    #endif

    

    #if ISDEBUG
    //sprintf(buff, "DISP_flashDemo: flashOnOff=%b2d done.\r\n", flashOnOff); LOG(buff);
    #endif
}

// ##### Game页 初始化时被调用一次
void DISP_drawGame(u8 soundOnOff){
    u8 topInfoY;

    #if ISDEBUG
    sprintf(buff, "DISP_drawGame\r\n"); LOG(buff);
    #endif

    devScreenOFF();

    clearScreen();
    DISP_drawFrame();

    // 注意，这句计算必须放在DISP_drawFrame的后面。否则第一次计算时还未被设置正确的数值，为默认值1。
    // 误以为1是正确的值调整后面的位置第一次是对的，第二次就全错了。
    topInfoY =frameThickness + 2;

    // 顶部信息
    // 食物 = 0  (当前吃了多少个)
    showChar(frameThickness + 10, topInfoY+3, FONT_SNAKE_BLOCK_APPLE, &BLOCK_VIEW_FONT, COLOR_BG, COLOR_FO);
    devShowString(frameThickness + 10 + BLOCK_VIEW_FONT.fontWidth, topInfoY, " =  0", &FONT20, COLOR_BG, COLOR_FO);
    // 居右显示文字
    devShowString(SCREEN_W - frameThickness - calcStringWidth("Snake v1.0", &FONT20) - 5, topInfoY, "Snake v1.0", &FONT20, COLOR_BG, COLOR_FO);

    // 底部信息
    #ifdef UI_LANG_EN
        showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, STR_GAME_MENU_EN, &FONT8, flashOnOff);
    #endif
    #ifdef UI_LANG_CN
        showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, STR_GAME_MENU_CN, &FONTHZ32, 0);
    #endif

    // 绘制声音状态
    DISP_drawSound(soundOnOff);

    devScreenON();

    #if ISDEBUG
    sprintf(buff, "DISP_drawGame done.\r\n"); LOG(buff);
    #endif
}

// ##### Game页 吃到食物时被调用一次，更新画面上的分数
void DISP_updateGameScore(u16 maxDemoScore, u16 nowDemoScore){
    u8 topInfoY = frameThickness + 2;

    // 顶部信息栏更新分数
    sprintf(buff, " =%3u", nowDemoScore);
    devShowString(frameThickness + 10 + BLOCK_VIEW_FONT.fontWidth, topInfoY, buff, &FONT20, COLOR_BG, COLOR_FO);
}

// ##### GameOver页 初始化时被调用一次
void DISP_drawGameOver(u16 score, u16 hiScore){
    u8 buff1[16];
    clearScreen();

    #ifdef UI_LANG_EN
        // 英文界面
        showStringCenter(5, "________________", &FONT8, 0);
        showStringCenter(TITLE_Y, STR_GAMEOVER_GAMEOVER_EN, &FONT16, 0);
        sprintf(buff1, "%s %d", STR_GAMEOVER_SCORE_EN, score);
        showStringCenter(SCROE_Y, buff, &FONT16, 0);
        showStringCenter(60, "________________", &FONT8, 0);
    #endif
    #ifdef UI_LANG_CN

        //showStringCenter(5, "__________", &FONT32, 0);

        // 标题，如果超过最高分，显示 刷新记录 否则 显示游戏结束
        if (score > hiScore)
        {
            showStringCenter(TITLE_Y, STR_GAMEOVER_NEWRECORD_CN, &FONTHZ32, 0);
        } else {
            showStringCenter(TITLE_Y, STR_GAMEOVER_GAMEOVER_CN, &FONTHZ32, 0);
        }

        // 得分
        sprintf(buff1, "%s %d", STR_GAMEOVER_SCORE_CN, score);
        showStringCenter(SCROE_Y, buff1, &FONTHZ32, 0);
        // 记录
        sprintf(buff1, "%s %d", STR_GAMEOVER_HSCORE_CN, hiScore);
        showStringCenter(HSCROE_Y, buff1, &FONTHZ32, 0);

        showStringCenter(150, "__________", &FONT32, 0);
    #endif
}

// ##### GameOver页 定期被调用
void DISP_flashGameOver(u8 flashOnOff, u8 isNewRecord){

    #ifdef UI_LANG_EN
        showStringCenter(70, STR_PRESS_ANY_KEY_EN, &FONTHZ32, flashOnOff);
    #endif
    #ifdef UI_LANG_CN
        if (isNewRecord)
        {
            // 刷新记录动态效果
            showStringCenterColor(TITLE_Y, STR_GAMEOVER_NEWRECORD_CN, &FONTHZ32, COLOR_BG, randBGR565(), 0);
        }

        // 底部闪烁文字 按任意键继续...
        showStringCenter(190, STR_PRESS_ANY_KEY_CN, &FONTHZ32, flashOnOff);
    #endif
}

// 绘制框架
void DISP_drawFrame() {
    u16 barTopY, barBottomY;

    // 墙体厚度 = (屏幕宽 - 游戏区域宽(地图SIZE_Y*蛇Block字体高)) / 2
    //frameThickness = (SCREEN_W - SNAKE_SIZE_X * BLOCK_VIEW_FONT.fontWidth) / 2;
    frameThickness = 4;

    // 游戏区域下方横栏位置Y = 屏幕高 - 底部边框高 - 底部信息栏高 - 下方横栏高(也是边框高)
    barBottomY = SCREEN_H - frameThickness - FRAME_BOTTOM_INFO_HEIGHT - frameThickness;

    // 游戏区域上方横栏位置Y = 游戏区域下方横栏位置Y - 游戏区域高(地图SIZE_Y*蛇Block字体高) - 上方横栏高(也是边框高)
    barTopY = barBottomY - (SNAKE_SIZE_Y * BLOCK_VIEW_FONT.fontHeight) - frameThickness;

    // 开始绘制(实心矩形拼接)
    devFillRectange(0, 0, SCREEN_W, frameThickness, COLOR_FRAME);                           // 顶
    devFillRectange(0, 0, frameThickness, SCREEN_H, COLOR_FRAME);                           // 左
    devFillRectange(SCREEN_W - frameThickness, 0, frameThickness, SCREEN_H, COLOR_FRAME);   // 右
    devFillRectange(0, barTopY, SCREEN_W, frameThickness, COLOR_FRAME);                     // 上横栏
    devFillRectange(0, barBottomY, SCREEN_W, frameThickness, COLOR_FRAME);                  // 下横栏
    devFillRectange(0, SCREEN_H - frameThickness, SCREEN_W, frameThickness, COLOR_FRAME);   // 底

    // 游戏区域Y偏移
    GAME_AREA_Y_OFFSET = barTopY + frameThickness;
    GAME_AREA_X_OFFSET = frameThickness + 3;
}

// 声音和静音状态
void DISP_drawSound(u8 soundOnOff) {
    if (soundOnOff)
    {
        #ifdef UI_LANG_EN
            showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, STR_GAME_MENU_EN, &FONT8, flashOnOff);
        #endif
        #ifdef UI_LANG_CN
            showStringCenter(SCREEN_H - FONT32.fontHeight - frameThickness - 1, "貳声音", &FONTHZ32, 0);
        #endif
    } else {
        // 绘制静音        
        devDrawLine(SCREEN_W/2-13, SCREEN_H-34, SCREEN_W/2-17+54, SCREEN_H-7, 3, COLOR_RED);
        devDrawLine(SCREEN_W/2-13, SCREEN_H-7,  SCREEN_W/2-17+54, SCREEN_H-34, 3, COLOR_RED);
    }
    
}

// 初始化
void DISP_init(void){
    // u32 clock;
    // u16 clockH;
    // u16 clockL;

    devDisplayInit(COLOR_BG);

    // 测试打印出系统时钟
    // clock=Get_SysClk_FRE();
    // clockH = clock >> 16;
    // clockL = clock & 0xFFFF;
    // sprintf(buff, "H:%u  L:%u", clockH, clockL);
    // devShowString(0,0,buff,&FONTHZ32,COLOR_BLACK, COLOR_WHITE);
    // while (1);
    
}
