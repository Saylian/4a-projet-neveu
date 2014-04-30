#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#define MAX_SIZE 50

int main()
{
    int listen_desc, conn_desc; // main listening descriptor and connected descriptor
    int maxfd, maxi; // max value descriptor and index in client array
    int i,j,k;  // loop variables
    fd_set tempset, savedset;  // descriptor set to be monitored
    int client[FD_SETSIZE], numready; // array of client descriptors
    struct sockaddr_in serv_addr, client_addr;
    char buff[MAX_SIZE];

    listen_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_desc < 0)
        printf("Failed creating socket\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(1234);

    if (bind(listen_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        printf("Failed to bind\n");

    listen(listen_desc, 5);

    maxfd = listen_desc; // Initialize the max descriptor with the first valid one we have
    maxi = -1; // index in the client connected descriptor array
    for (i=0; i<FD_SETSIZE; i++)
        client[i] = -1;  // this indicates the entry is available. It will be filled with a valid descriptor
    FD_ZERO(&savedset); // initialize the descriptor set to be monitored to empty
    FD_SET(listen_desc, &savedset); // add the current listening descriptor to the monitored set

    while(1) // main server loop
    {
        // assign all currently monitored descriptor set to a local variable. This is needed because select
        // will overwrite this set and we will lose track of what we originally wanted to monitor.
        tempset = savedset;
        numready = select(maxfd+1, &tempset, NULL, NULL, NULL); // pass max descriptor and wait indefinitely until data arrives

        //printf("Waiting\n");

        if(FD_ISSET(listen_desc, &tempset)) // new client connection
        {
            printf("new client connection\n");
            int size = sizeof(client_addr);
            conn_desc = accept(listen_desc, (struct sockaddr *)&client_addr, &size);
            for (j=0; j<FD_SETSIZE; j++)
                if(client[j] < 0)
                {
                    client[j] = conn_desc; // save the descriptor
                    break;
                }

                FD_SET(conn_desc, &savedset); // add new descriptor to set of monitored ones
                if(conn_desc > maxfd)
                    maxfd = conn_desc; // max for select
                if(j > maxi)
                    maxi = j;   // max used index in client array
        }

        for(k=0; k<=maxi; k++) // check all clients if any received data
        {
            if(client[k] > 0)
            {
                if(FD_ISSET(client[k], &savedset))
                {
                    int num_bytes;
                    if( (num_bytes = read(client[k], buff, MAX_SIZE)) > 0)
                    {
                        buff[num_bytes] = '\0';
                        printf("Received:- %s", buff);
                    }

                    if(num_bytes == 0)  // connection was closed by client
                    {
                        close(client[k]);
                        FD_CLR(client[k], &savedset);
                        client[k] = -1;
                    }

                    if(--numready <=0) // num of monitored descriptors returned by select call
                        break; 
                }
            }
        }
    } // End main listening loop

    close(listen_desc);
    return 0;
}
