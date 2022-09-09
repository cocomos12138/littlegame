#include <graphics.h>
#include <conio.h>
#include<windows.h>
#include<bits/stdc++.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
IMAGE bk;
IMAGE player;
IMAGE bulleti;
IMAGE monsteri[4];
IMAGE blood;
IMAGE monbulli;
IMAGE over;
const int width=405,high=877,bulletnum=50,mbnum=80;
int speed =3,monsterspeed=3;


void drawAlpha(IMAGE* picture, int  picture_x, int picture_y)
{
	// 变量初始化
	DWORD *dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD *draw = GetImageBuffer();
	DWORD *src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

struct air
{
    int x,y;
    bool dead;
    int fang;
    int health;
    int r;

}play,bullet[bulletnum],monster,monbull[mbnum];
void loadImg(){
    loadimage(&bk,_T()"bk.jpg",width,high);
    loadimage(&player,_T()"player.png",53,61);
    loadimage(&bulleti,_T()"bullet.png",15,18);
    loadimage(&monbulli,_T()"monbull.png",26,22);
    if(monster.health>0)
        loadimage(&blood,_T()"blood.png",4*monster.health,40);
    if(monster.health>=50){
        loadimage(&monsteri[0],"monster.png",156,194);
        loadimage(&monsteri[1],"monster3.png",156,194);
    }
    else if(monster.health>=0) loadimage(&monsteri[2],"monster2.png",156,194);
    else loadimage(&monsteri[3],"monster4.png",156,194);
}
void gamedraw(){
    loadImg();
    putimage(0,0,&bk);
    for(int i=1;i<mbnum;i++){
        if(!monbull[i].dead){
            drawAlpha(&monbulli,monbull[i].x,monbull[i].y);
        }
    }
    if(monster.health>0)
        drawAlpha(&blood,5,50);
    if(monster.health>=50)
        drawAlpha(&monsteri[monster.fang],monster.x,monster.y);
    else if(monster.health>=0) drawAlpha(&monsteri[2],monster.x,monster.y);
    else drawAlpha(&monsteri[3],monster.x,monster.y);
    drawAlpha(&player,play.x,play.y);

    for(int i=1;i<=bulletnum;i++){
        if(!bullet[i].dead){
            drawAlpha(&bulleti,bullet[i].x,bullet[i].y);
        }
    }
}
void gameinit(){
    loadImg();
    play.x=width/2;
    play.y=high-177;
    play.dead=0;
    for(int i=0;i<bulletnum;i++){
        bullet[i].dead=1;
        bullet[i].x=play.x;
        bullet[i].y=play.y;
    }
    for(int i=1;i<=mbnum;i++){
        monbull[i].dead=1;
        monbull[i].x=monster.x;
        monbull[i].y=monster.y;
    }
    monster.x=150;
    monster.y=100;
    monster.health=100;
    monster.fang=1;
    monster.dead=0;
}
void creatbullet(){
    for(int i=1;i<=bulletnum;i++){
        if(bullet[i].dead){
            bullet[i].dead=0;
            bullet[i].x=play.x+19;
            bullet[i].y=play.y;
            break;
        }
    }
}
void createmb(){
    for(int i=1;i<mbnum;i++){
        if(i<mbnum&&monbull[i].dead){
            monbull[i].dead=0;
            monbull[i].x=monster.x+75;
            monbull[i].y=monster.y+90;
            break;
        }
    }
}
void createmb2(){
    srand(time(0));
    for(int i=1;i<mbnum;i++){
        int j=0;
        if(i<mbnum&&monbull[i].dead){
            monbull[i].dead=0;
            monbull[i].x=monster.x+75;
            monbull[i].y=monster.y+90;
            monbull[i].fang=rand()%3;
            j++;
            if(j>3) break;
        }
    }
}
void mbmove(){
    if(monster.health>=50){
        for(int i=0;i<mbnum;i++){
            if(!monbull[i].dead){
                monbull[i].y+=5;
            }
            if(monbull[i].y>=800) monbull[i].dead=1;
        }
    }
    else {
        for(int i=0;i<mbnum;i++){
            if(!monbull[i].dead&&monbull[i].fang==0){
                monbull[i].y+=8;
            }
            if(!monbull[i].dead&&monbull[i].fang==1){
                monbull[i].y+=6;
                monbull[i].x+=1;
            }
            if(!monbull[i].dead&&monbull[i].fang==2){
                monbull[i].y+=6;
                monbull[i].x-=1;
            }
            if(monbull[i].y>=800) monbull[i].dead=1;
        }
    }
}
void bulletmove(){
    for(int i=1;i<=bulletnum;i++){
        if(!bullet[i].dead){
            bullet[i].y-=5;
        }
        if(bullet[i].y<=0) bullet[i].dead=1;
    }
}
void monstermove(){
    if(monster.health>=0){
        if(monster.fang){
            monster.x+=monsterspeed;
            if(monster.x>=250) monster.fang=0;
        }
        else {
            monster.x-=monsterspeed;
            if(monster.x<=0) monster.fang=1;
        }
    }

}
void hurt(){
    for(int i=1;i<=bulletnum;i++){
        if(bullet[i].dead) continue;
        if(bullet[i].x>=monster.x&&bullet[i].x<=monster.x+150
           &&bullet[i].y>=monster.y&&bullet[i].y<=monster.y+180){
            monster.health--;
            bullet[i].dead=1;
        }
        if(monster.health<=0) monster.dead=1;
    }
    for(int i=0;i<mbnum;i++){
        if(monbull[i].dead) continue;
        if(monbull[i].x>=play.x-20&&monbull[i].x<=play.x+49
           &&monbull[i].y>=play.y&&monbull[i].y<=play.y+30){
            monbull[i].dead=1;
            play.dead=1;
        }
    }
}
void movement(){
    if(KEY_DOWN('W')) play.y-=speed;
    if(KEY_DOWN('S')) play.y+=speed;
    if(KEY_DOWN('A')&&play.x>=0) play.x-=speed;
    if(KEY_DOWN('D')&&play.x<=350) play.x+=speed;
    static DWORD t1=0,t2=0,t3=0,t4=0;
    if(KEY_DOWN(VK_SPACE)&&t2-t1>400){
        creatbullet();
        t1=t2;
    }
    t2=GetTickCount();
    if(t4-t3>700){
        srand(time(0));
        int tem=rand()%5;
        if((tem==1||tem==2||tem==3)&&monster.health>=50)
            createmb();
        if(monster.health<50&&monster.health>0&&tem!=4)
            createmb2();
        t3=t4;
    }
    t4=GetTickCount();
}

void gameover(){
    loadimage(&bk,_T()"bk.jpg",width,high);
    loadimage(&over,_T()"gameover.png",407,264);
    putimage(0,0,&bk);
    drawAlpha(&over,15,300);

}
int main()
{
	initgraph(width, 825);
    gameinit();
	BeginBatchDraw();
	while(1){
        gamedraw();
        FlushBatchDraw();
        movement();
        bulletmove();
        monstermove();
        mbmove();
        hurt();
        if(play.dead) break;
	};
	if(play.dead){
            gameover();
            FlushBatchDraw();
            Sleep(8000);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}
