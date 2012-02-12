#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

void main(void)
{
	int fd, fd2;

	/* O_RDWR: read & write 
	   only root can write action in /dev/device 
           but read action is all can do it */	
	fd = open("/dev/cdata", O_RDWR);
	fd2 = open("/dev/cdata3", O_RDWR);
	close(fd);
	close(fd2);
}
