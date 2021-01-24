#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>


#define PORT 1310
#define BUFFER_SIZE 256
#define MAX 3

void *funkcija_niti(void*);

int st_clients;
pthread_mutex_t nClients_mutex;

int main(int argc, char **argv){

	int iResult;
	int listener=socket(AF_INET, SOCK_STREAM, 0);

	if (listener == -1) {
		printf("Error creating socket\n");
		return 1;
	}
	sockaddr_in  listenerConf;
	listenerConf.sin_port=htons(PORT);
	listenerConf.sin_family=AF_INET;
	listenerConf.sin_addr.s_addr=INADDR_ANY;

	iResult = bind( listener, (sockaddr *)&listenerConf, sizeof(listenerConf));
	if (iResult == -1) {
		printf("Bind failed\n");
		close(listener);
		return 1;
    }

	if ( listen( listener, 5 ) == -1 ) {
		printf("Listen failed\n");
		close(listener);
		return 1;
	}

	int clientSock;
	st_clients = 0;

	while(1){
		if (st_clients < MAX){
			clientSock = accept(listener,NULL,NULL);
			if (clientSock == -1) {
				printf("Server: Accept failed\n");
				close(listener);
				return 1;
			}
			printf("Server: Nov client!\n");

			pthread_t nit;
			pthread_create(&nit, NULL, listen, (void *)(&clientSock));
			pthread_detach(nit);

			printf("Server: novi povezu\n");

			pthread_mutex_lock(&nClients_mutex);
			st_clients++;
			pthread_mutex_unlock(&nClients_mutex);
		}
	}
	close(listener);
}

void *listen(void *args) {

    int *a = (int*) args;

	int clientSock = *a;

	int iResult;
	char buff[BUFFER_SIZE];

	do{
		iResult = recv(clientSock, buff, BUFFER_SIZE, 0);
		if (iResult > 0) {
			printf("Nit#%d: Bytes received: %d\n", clientSock, iResult);

			iResult = send(clientSock, buff, iResult, 0 );
			if (iResult == -1) {
				printf("Nit#%d: Send failed!\n", clientSock);
				close(clientSock);
				break;
			}
			printf("Nit#%d: Bytes sent: %d\n", clientSock, iResult);
		}
		else if (iResult == 0)
			printf("Nit#%d: Connection closing...\n", clientSock);
		else{
			printf("Nit#%d: Recv failed!\n", clientSock);
			close(clientSock);
			break;
		}

	} while (iResult > 0);
	
	pthread_mutex_lock(&nClients_mutex);
	st_clients--;
	pthread_mutex_unlock(&nClients_mutex);
	
	close(clientSock);
	printf("Nit#%d: Uničena\n", clientSock);

	return 0;
}
