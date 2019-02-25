#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
	char *path="/sys/class/net/eth0/device";
	char buf[256]={0};
	
	realpath(path, buf);
	printf("Real Path of %s=%s\n", path, buf);
	
	return 0;
}
