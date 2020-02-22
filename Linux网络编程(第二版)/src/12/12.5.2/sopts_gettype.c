#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
int main(int argc,char **argv)
{
	int err = -1;								/*错误*/
	int s = -1;		  							/*Socket*/
	int so_type = -1;	 						/*Socket类型*/
	socklen_t len = -1;   						/*选项值长度*/
	
	s = socket(AF_INET,SOCK_STREAM,0);			/*建立一个流式套接字*/
	if(-1 == s){
		printf("socket error\n");
		return -1;
	}
	
	len = sizeof(so_type);
	err = getsockopt(s, SOL_SOCKET, SO_TYPE, &so_type,&len);
	/*获得SO_TYPE的值*/
	if(err == -1){
		printf("getsockopt error\n");
		close(s);
		return -1;	
	}
	/*输出结果*/
	printf("socket fd: %d\n",s);
	printf(" SO_TYPE : %d\n",so_type);
	close(s);
	return 0;
}
