/**

文件名: bmp_show.c

描述：把指定目录下的所有 BMP 图片 自动循环显示到屏幕

仅支持 800*480 分辨率的图片



**/



#include <stdio.h>

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



	int i = 0;



	for (i = 0; i<files; i++)
    { //循环显示图片



		bmp_show(pic_path[i]);





		sleep(1);





	}



	free(pic_path);



	return 0;

}







/**

函数功能： bmp 图片全屏显示

参数1： bmp图片路径

返回值: void

备注: 此函数仅支持 800*480 分辨率图片



**/

void bmp_show(const char *bmp_name)
{



	//1. 打开 设备文件（LCD）

	int fd_lcd = open("/dev/fb0", O_RDWR);



	if (fd_lcd == -1)
    {

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



	if (fp == (void*)-1)
    {

		printf("mmap err\n");

		return;

	}



	//3. 打开bmp图片-->读取图片颜色数据

	int fd_bmp = open(bmp_name, O_RDONLY);



	if (fd_bmp == -1)
    {

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



	for (j = 0; j<480; j++)
    {

		for (i = 0; i<800; i++)
        {

			// fp[i*4+800*4*j] = bmp_buf[i*3+800*3*(479-j)]; //B

			// fp[i*4+1+800*4*j] = bmp_buf[i*3+800*3*(479-j)+1]; //G

			// fp[i*4+2+800*4*j] = bmp_buf[i*3+800*3*(479-j)+2]; //R

			memcpy(fp + i * 4 + 800 * 4 * j, bmp_buf + i * 3 + 800 * 3 * (479 - j), 3);

			fp[i * 4 + 3 + 800 * 4 * j] = 0;

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

void ** dir_seek(char * seek_dir, int *files, char * seek_str)
{






	DIR * dirp = opendir(seek_dir);//打开要检索的文件夹（目录）

	if (dirp == NULL)
    {

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



	while (1)
    {

		//一次while循环读取一次

		filep = readdir(dirp);//循环读取打开的目录（文件夹），直到读完跳出

		if (filep == NULL)
        {

			break;

		}



		char tmp[200] = { 0 };

		if (strstr(filep->d_name, seek_str) != NULL)
        {

			//seek_str(就是".bmp"，bmp格式图片名字的后缀)

			//strstr返回第一次在d_name中出现seek_str的位置，此处的作用主要是用来检索文件名中是否出现.bmp后缀

			if (itms >= n)
            {

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

