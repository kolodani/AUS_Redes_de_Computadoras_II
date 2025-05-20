#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RCVBUFFSIZE 512

int main()
{
    int sh, csh, l, msglen;
    struct sockaddr_in addr, caddr;
    char msg[RCVBUFFSIZE];

    if ((sh = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("failed to create socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(3333);

    if (bind(sh, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("failed to bind");
        close(sh);
        exit(1);
    }

    if (listen(sh, 5) < 0)
    {
        printf("failed to listen at port %d", 3333);
        close(sh);
        exit(1);
    }

    while (1)
    {
        l = sizeof(caddr);
        if ((csh = accept(sh, (struct sockaddr *)&caddr, &l)) < 0)
        {
            printf("failed to accept client");
            close(sh);
            exit(1);
        }

        if ((msglen = recv(csh, msg, RCVBUFFSIZE, 0)) < 0)
        {
            printf("failed to receive");
            close(csh);
            exit(1);
        }
        else
        {
            while (msglen > 0)
            {
                if (send(csh, msg, msglen, 0) != msglen)
                {
                    printf("failed to respond");
                    close(csh);
                    break;
                }
                if ((msglen = recv(csh, msg, RCVBUFFSIZE, 0)) < 0)
                {
                    printf("failed to receive");
                    close(csh);
                    break;
                }
            }
        }
    }

    exit(0);
}