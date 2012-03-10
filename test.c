#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "cdata_ioctl.h"


int main(int argc, char *argv[])
{
	int fd;
	int i=0, j=0;
	unsigned char pix[4] = {0xff, 0x00, 0x00, 0xff};
	pid_t pid;

#if 0
	unsigned int cmd=0;
	if(argc == 2)
	{
		switch(atoi(argv[1]))
		{
			case 0:
				cmd = CDATA_CLEAR;
			break;
			case 1:
				cmd = CDATA_RED;
			break;
			case 2:
				cmd = CDATA_GREEN;
			break;
			case 3:
				cmd = CDATA_BLUE;
			break;
			case 4:
				cmd = CDATA_BLACK;
			break;
			case 5:
				cmd = CDATA_WHITE;
			break;
			default:
				printf("wrong cmd\n");
				return 0;
		}
	}
	else
	{
		printf("test <cmd>\n");
		printf("cmd : 0 = clear, 1 = red, 2 = green, 3 = blue, 4 = black, 5= white\n");
		return 0;		
	}
#endif
	/* O_RDWR: read & write 
	   only root can write action in /dev/device 
       but read action is all can do it */	
	//ioctl(fd, cmd, &i);
	printf("do open\n");
	fd = open("/dev/cdata", O_RDWR);
	ioctl(fd, CDATA_CLEAR, &i);
#if 0
	printf("do fork\n");
	pid = fork();
	if(pid == 0)
	{
		pix[0] = 0xff;
		pix[1] = 0xff;
		pix[2] = 0xff;
		while(1)
		{
			write(fd, pix, 4);
		}
	}
	else
	{
		pix[0] = 0xff;
		pix[1] = 0x00;
		pix[2] = 0x00;
		while(1)
		{
			write(fd, pix, 4);
		}
	}
#else 
# if 0
	while(1)
	{
		write(fd, pix, 4);
	}
#else
	mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	sleep(15);
#endif
#endif
	close(fd);
	return 0;
}
