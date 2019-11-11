#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <signal.h>   
#include <sys/ioctl.h>   
#include <linux/if.h>

int getip(char *ip){   
  int  MAXINTERFACES=16;   
    int fd, intrface, retn = 0;   
    struct ifreq buf[MAXINTERFACES]; ///if.h   
    struct ifconf ifc; ///if.h   
     char szip[80]; 
    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) //socket.h   
    {   
        ifc.ifc_len = sizeof buf;   
        ifc.ifc_buf = (caddr_t) buf;   
        if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) //ioctl.h   
        {   
            intrface = ifc.ifc_len / sizeof (struct ifreq);    
            while (intrface-- > 0)   
            {   
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))   
                {   
                 sprintf(ip,"%s", inet_ntoa( ((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr) );                   
                 break;   
                }   
                       
            }   
               
        }   
        close (fd);   
    }   
    return 0;   
}

int send_udp_str(char *ip, int port, char* str) {
	int ret = 0;
    	int i = 0;
	if (str == NULL) {
		ret = -1;
		return ret;
	}
	struct sockaddr_in server;
	int sockfd = 0;
	int server_len = sizeof(struct sockaddr_in);
        char str_message[256] = {0};
	char ipaddr_self[18] = {0};
       
        getip(ipaddr_self);
        sprintf(str_message, "%s : %s", ipaddr_self, str);
 
	/* setup a socket，attention: must be SOCK_DGRAM */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		ret = -1;
		return ret;
	}
 
	/*complete the struct: sockaddr_in*/
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip);
 
	/* send the string to server*/
	if (sendto(sockfd, str_message, strlen(str_message), 0, (struct sockaddr *) &server,
			server_len) < 0) {
		printf("sendto error\n");
		ret = -2;
	}
	close(sockfd);
	return ret;
}

int main()
{
    send_udp_str("101.37.33.102", 8889, "ok");
    return 0;
}
