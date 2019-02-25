/* 
 * File:   NCSFConnector.cpp
 * Author: jjdengjj
 * 
 * Created on November 29, 2009, 9:20 PM
 */

#include "NTCFConnector.hpp"

NTCFConnector::NTCFConnector() {
}

NTCFConnector::NTCFConnector(const NTCFConnector& orig) {
}

NTCFConnector::~NTCFConnector() {
}

std::string NTCFConnector::Validate(std::string IP)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sockaddr_in;
    sockaddr_in.sin_family=AF_INET;
    sockaddr_in.sin_port=htons(_MAINSERVER_SERVER_PORT);
    char* SERVERIP;
    NTCFConnector::getServerIP(&SERVERIP);
    inet_aton(SERVERIP, &(sockaddr_in.sin_addr));
    free(SERVERIP);
    bzero(&(sockaddr_in.sin_zero),8);
    if(connect(sockfd, (struct sockaddr *)&sockaddr_in, sizeof(struct sockaddr)) ==0 )
    {
        //REQ
        char* strREQ;
        NTCFPackage pkgREQ;
        pkgREQ.set_MajorCode(_NCSFPC_MJ_CMD_REQ_);
        pkgREQ.set_MinorCode(_NCSFPC_MN_TYPE_HTTP_);
        pkgREQ.set_DataInfo(IP.c_str());
        pkgREQ.get_Base64Str(&strREQ);
        send(sockfd, strREQ, strlen(strREQ)+1, 0);
        free(strREQ);
        usleep(50*1000);

        //RTN
        int length = 0;
        int rlen = 0;
        char recvbuf[4096] = {0};
        do
        {
            rlen = recv(sockfd, recvbuf+length, 4096, 0);
            length += rlen;
        }while(recvbuf[length-1] != '\0');
        NTCFPackage pkgRTN;
        pkgRTN.set_Base64Str(recvbuf);
        char* data;
        pkgRTN.get_DataInfo(&data);
        std::string rtnstr = std::string(data);
        free(data);
        usleep(50*1000);

        //CLS
        char* strCLS;
        NTCFPackage pkgCLS;
        pkgCLS.set_MajorCode(_NCSFPC_MJ_COM_CLS_);
        pkgCLS.get_Base64Str(&strCLS);
        send(sockfd, strCLS, strlen(strCLS)+1, 0);
        free(strCLS);
        usleep(50*1000);

        close(sockfd);
        return rtnstr;
    }
    else
    {
        return std::string(_NCSFPC_RTN_CLOSE_);
    }
}

bool NTCFConnector::getServerIP(char** IP)
{
	char path[256] = {0};
	getcwd(path, 256);
	strcat(path, "/.NTCFConfig");
	int fd = open(path, O_RDONLY);
	char ip[32];
	int rc = read(fd, ip, 32);
	if(rc > 16)
		return false;
	*IP = (char*)malloc(rc*sizeof(char));
	bzero(*IP,rc);
	memcpy(*IP,ip,(rc-1)*sizeof(char));
	close(fd);
}

