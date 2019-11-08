#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define uint64_t long long 


int getDuration(unsigned char* fileContent, int len)
{
	uint64_t duration=0;//语音持续时长
	//音频帧在八种编码模式下的帧内容占用字节数
	int AMR_FRAME_SIZE[] = { 13, 14, 16, 18, 20, 21, 27, 32, 6, 1, 1, 1, 1, 1, 1, 1, 1};
	size_t pos=0;//pos用来指示位置
	size_t length=len;
	unsigned char toc;//toc每次代表帧头,通过运算可以知道属于哪种编码模式
	
	pos+=6;//跳过文件头的6个字节
	while(pos<length)
	{
		toc=fileContent[pos];//toc赋值帧头
		//printf("toc = %d\n", toc);
        if (toc & 0x04 != 0)
		    duration+=20;//每一个数据帧对应20ms
                
		toc=(toc>>3) & 0x0f;//对照表格帧头的编码一目了然
		if(toc>=16)
			toc=15;
		
		pos+=AMR_FRAME_SIZE[(int)toc];
	}
 
	printf("duration=%lld\n", duration);
	return duration;
}

int main(int argc, char* argv[])
{
	int mRecorderFd;
	mRecorderFd = open(argv[1], O_RDONLY|O_CREAT, 0666);
	if (mRecorderFd <= 0) {
		printf("open path:%s error", argv[1]);
		return -1;
	}
	char amr_bf[1024*20];
	memset(amr_bf, 0, sizeof(amr_bf));
	int ret;
	ret = read(mRecorderFd, amr_bf, 1024*20);
	printf("read count = %d\n", ret);
	
	close(mRecorderFd);
	ret = getDuration(amr_bf, ret);
	
	printf("len = %d s\n", ret);
	
	
	return 0;
}
