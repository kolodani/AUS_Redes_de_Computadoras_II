#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFSIZE 512

int main()
{
    int sh, len;
    struct sockaddr_in addr;
    char msgsend[] = "Hola Mundo\n\r";
    char msgrecv[BUFFSIZE];

    if ((sh = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("failed to create socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3333);

    if (connect(sh, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("failed to connect echo server");
        close(sh);
        exit(1);
    }

    if (send(sh, msgsend, sizeof(msgsend), 0) != sizeof(msgsend))
    {
        printf("failed to send %s", msgsend);
        close(sh);
        exit(1);
    }

    if ((len = recv(sh, msgrecv, BUFFSIZE, 0)) < 0)
    {
        printf("failed to receive echo");
        close(sh);
        exit(1);
    }

    printf("Echo %s", msgrecv);

    close(sh);
    exit(0);
}
