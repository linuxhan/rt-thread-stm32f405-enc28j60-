#include <rtthread.h>
#include <lwip/netdb.h> 
#include <lwip/sockets.h> /* ??BSD socket,????sockets.h??? */


ALIGN(4)
const char send_data[] = "This is UDP Client from RT-Thread.\n";

void udpclient(const char* url, int port, int count)
{
	int sock;
	struct hostent *host;
	struct sockaddr_in server_addr;
	rt_kprintf("udp test!\n");
	/* ????????url??host??(?????,??????) */
	host = (struct hostent *) gethostbyname(url);
	/* ????socket,???SOCK_DGRAM,UDP?? */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		rt_kprintf("Socket error\n");
		return;
	}
	else
	{
		rt_kprintf("Socket begin\n");
	}
	/* ???????????? */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *) host->h_addr);
	rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	/* ????count??? */
	while (count)
	{
		/* ????????? */
		sendto(sock, send_data, strlen(send_data), 0,
		(struct sockaddr *) &server_addr, sizeof(struct sockaddr));
		/* ???????? */
		rt_thread_delay(50);
		/* ????? */
		count--;
	}
/* ????socket */
	lwip_close(sock);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
/* ??udpclient???finsh shell? */
FINSH_FUNCTION_EXPORT(udpclient, startup udp client);
#endif