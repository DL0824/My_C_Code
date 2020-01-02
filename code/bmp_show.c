/**
文件名: bmp_show.c
描述：把指定目录下的所有 BMP 图片 自动循环显示到屏幕
仅支持 800*480 分辨率的图片
**/



#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <dirent.h>





//函数声明

void bmp_show(const char *bmp_name);

void ** dir_seek(char * seek_dir, int *files, char * seek_str);


void get_xy(int *x,int *y)
{
	//打开触摸设备
	int fd_touch=open("/dev/input/event0",O_RDONLY);
	if(fd_touch == -1){
		printf("open touch screen err\n");
		return;
	}
	struct input_event tc;//读取触摸信息结构体
	
	/*struct input_event {
		struct timeval time; //按键时间
		__u16 type; //类型，在下面有定义
		__u16 code; //要模拟成什么按键
		__s32 value;//是按下还是释放
		};*/
	/*type: 
	EV_KEY,键盘 0x1
	EV_REL,相对坐标  0x2
	EV_ABS,绝对坐标  0x3
	value：
	事件的值.如果事件的类型代码是EV_KEY,
	当按键按下时值为1,松开时值为0;
	如果事件的类型代码是EV_ REL,value的正数值和负数值分别代表两个不同方向的值.*/
	
	//puts( " will read touch " );
	while(1)
	{
		read(fd_touch,&tc,sizeof(tc));//阻塞读取,读不到一直等待
		if(tc.type==3)//EV_ABS 绝对坐标
		{
			//触摸设备和显示设备不同，一个是800*480，一个是1024*600
			//这里的0，1代表了一个绝对的方向并不是具体的什么坐标了
			if(tc.code==0)
			{
				*x=tc.value*800/1024;//保存x触摸值
			}
			else if(tc.code==1)
			{
				*y=tc.value*800/1024;//保存y触摸值
			}
		}

		if((tc.code==330)&&(tc.value==0))
		{//在源码宏定义中表示按键，这里的意思是遇到案件并且松开(value==0)
			break;
		}
		
	}
	//printf("x=%d y=%d\n",*x,*y);
	close(fd_touch);
}


void delay(int time)//延时函数
{
	for(int i=0;i<time;++i)
	{
		for(int j=0;j<100;++j)
		{
			;//什么都不干，就是一个简单的延时
		}
	}
}

void xhshow(char(*pic_path)[100],int files)
{
	int i;
	for (i = 0; i<files; i++){ //循环显示图片
		bmp_show(pic_path[i]);
		//sleep(1);
		delay(1);
	}
}

void cmshow(char(*pic_path)[100],int files)
{
	int i=0;
	int x,y;

	puts( " cmshow " );

	while(1)
	{
		bmp_show(pic_path[i]);//先显示第一张图片
		get_xy(&x,&y);
		
		if( x < 200 && --i < 0 ){
			i = files -1; //最后一张的下标
		}else if( x > 600 && ++i == files){
			i = 0; //第一张的下标
		}
	}
}
//宏定义区

#define   SEEK_DIR "./bmp"//储存bmp格式图片文件的目录，必须给出路径







/**

函数功能： main 函数

参数1： void

返回值: int



**/

int main()

{

	char(*pic_path)[100]; // 数组指针 指向申请的 字符串数组首地址--》保存图片路径
	int files; //BMP 文件的个数
	
	pic_path = (char(*)[100])dir_seek(SEEK_DIR, &files, ".bmp");
	if (pic_path == NULL)
		return 1;
	//int i = 0;
	int n=1;
	int choice;
	while(n)
	{
		printf("1.循环播放       2.触摸显示     3.退出\n");
		printf("please enter your choice:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
			xhshow(pic_path,files);
			break;
			case 2:
			cmshow(pic_path,files);
			break;
			case 3:
			n=-1;
			break;
			default:
			printf("please enter again!\n");
			break;
		}
	}
	/*for (i = 0; i<files; i++){ //循环显示图片
		bmp_show(pic_path[i]);
		//sleep(1);
		delay(1);
	}*/
	free(pic_path);



	return 0;

}







/**

函数功能： bmp 图片全屏显示

参数1： bmp图片路径

返回值: void

备注: 此函数仅支持 800*480 分辨率图片



**/

void bmp_show(const char *bmp_name){
	//1. 打开 设备文件（LCD）
	int fd_lcd = open("/dev/fb0", O_RDWR);
	if (fd_lcd == -1){
		printf("open lcd err\n");
		return;
	}



	//2. fb0 映射

	unsigned char *fp = (unsigned char*)mmap(

		NULL,//要映射到哪个地址上去，一般为NULL,表示让操作系统自行分配

		800 * 480 * 4,//要映射的区域的长度

		PROT_READ | PROT_WRITE,//映射区域的权限

		MAP_SHARED,//映射标志，“共享映射”

		fd_lcd,//文件描述符

		0//文件偏移量，表示您要从文件的哪个位置开始映射

		);



	if (fp == (void*)-1){

		printf("mmap err\n");

		return;

	}



	//3. 打开bmp图片-->读取图片颜色数据

	int fd_bmp = open(bmp_name, O_RDONLY);



	if (fd_bmp == -1){

		printf("open bmp err\n");

		close(fd_lcd);

		munmap(fp, 800 * 480 * 4);

		return;

	}



	char bmp_buf[800 * 480 * 3];



	read(fd_bmp, bmp_buf, 54);

	read(fd_bmp, bmp_buf, 800 * 480 * 3);





	//4. 显示 bmp 颜色数据

	int i, j;
	
	//i表示横坐标，j表示纵坐标
	for (j = 0; j<480; j++){

		for (i = 0; i<800; i++){

			// fp[i*4+800*4*j] = bmp_buf[i*3+800*3*(479-j)]; //B

			// fp[i*4+1+800*4*j] = bmp_buf[i*3+800*3*(479-j)+1]; //G

			// fp[i*4+2+800*4*j] = bmp_buf[i*3+800*3*(479-j)+2]; //R

			memcpy(fp + i * 4 + 800 * 4 * j, bmp_buf + i * 3 + 800 * 3 * (479 - j), 3);

			fp[i * 4 + 3 + 800 * 4 * j] = 0;//将透明度置0，因为bmp格式图片没有透明度，其实可以不用管

		}

	}







	//5. 释放资源

	close(fd_lcd);

	close(fd_bmp);



	munmap(fp, 800 * 480 * 4);



}







/**

函数功能： 检索目录 指定后缀文件名 并返回得到的

保存文件名的内存首地址

参数1： 需要检索的路径

参数2： （地址传递） 用来保存 搜索到的文件个数

参数3： 指定搜索的后缀--》字符串（".bmp"）



返回值：保存文件名的内存首地址--》

使用时需要强制类型转换为(char (*)[100])



**/

void ** dir_seek(char * seek_dir, int *files, char * seek_str){







	DIR * dirp = opendir(seek_dir);//打开要检索的文件夹（目录）

	if (dirp == NULL){

		puts("open dir err");

		return NULL;

	}

	struct dirent * filep;//readdir的返回值





	//struct dirent

	//　　{

	//  　long d_ino;    /* inode number 索引节点号 */

	//	off_t d_off;     /* offset to this dirent 在目录文件中的偏移 */

	//	unsigned short d_reclen;     /* length of this d_name 文件名长 */

	//	unsigned char d_type;        /* the type of d_name 文件类型 */

	//	char d_name[NAME_MAX + 1];   /* file name (null-terminated) 文件名，最长255字符 */

	//	}



	//申请 5串 大小为100个字节空间的 字符串数组，每个字符串数组用来放检索到的文件名“xx.bmp”

	char(*name)[100] = (char(*)[100]) malloc(5 * 100);

	int itms = 0, n = 5;//n表示初始可以检索5个bmp格式文件



	while (1){

		//一次while循环读取一次

		filep = readdir(dirp);//循环读取打开的目录（文件夹），直到读完跳出

		if (filep == NULL){

			break;

		}




		char tmp[200] = { 0 };

		if (strstr(filep->d_name, seek_str) != NULL){

			//seek_str(就是".bmp"，bmp格式图片名字的后缀)

			//strstr返回第一次在d_name中出现seek_str的位置，此处的作用主要是用来检索文件名中是否出现.bmp后缀

			if (itms >= n){

				n += 5;//当检索的文件数量大于5个，先把n重新赋值，每次多加5个即可，

				//然后再重新在堆上申请空间

				name = (char(*)[100]) realloc(name, n * 100);

			}



			memset(tmp, 0, 200);

			sprintf(tmp, "%s/%s", seek_dir, filep->d_name);//将bmp格式图片的路径放入tmp数组

			//注意两个%s之间有一个“/”，seek_dir是目录名（文件夹），filep->d_name指向文件名

			strcpy(name[itms++], tmp);//将得到的路径放入开辟好的name字符数组中，itms记录文件名个数，并以此作为下标可用作检索

			// printf("-->%s\n",filep->d_name);

		}
	}
	*files = itms;//files作为“出参”，在主函数中充当文件数，用于循环检索用

	closedir(dirp);


	return (void **)name;//返回值之所以为二级指针，目的是为了防止当需要检索的文件数目大于5个时

	//在堆上重新开辟了空间，改变了name的地址，若为一级指针，则为简单的返回数值

	//若为二级指针，那么就可以返回地址

}

