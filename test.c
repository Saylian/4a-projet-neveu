#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_SIZE 50

int main()

{
	// Descripteur de socket qui sont juste des int.
	int sock_descriptor, conn_desc;
	//Des structures d'adresse, une pour le serveur et l'autre pour le client.
	struct sockaddr_in serv_addr, client_addr;
	// le buff stocker les donnés du cliens ( message qu'il envoit)
	char buff[MAX_SIZE];
	// Création de la socket avec une adresse IP AF_INET, STREAM = TCP, et protocole 0 ( non specifié)
	sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock_descriptor < 0)
	//Si ca marche pas ca renvoit 0 donc en s'informe de ce qui se passe.
	{
		printf("Creation socket echouer");
	}
	// Initalise la structure d'adresse serveur a 0
	bzero((char *)&serv_addr, sizeof(serv_addr));
	//On indique dans la variable quel type d'adresse le serveur utilise
	serv_addr.sin_family = AF_INET;
	//On authorise les connexion de toute les adresse
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//Le port sur lequel le serveur ecoute
	//htons est une fonction qui permet de forcer l'interpretation en int de la valeurs ( ca veut rien dire ca non ??°
	serv_addr.sin_port = htons(1235);
	//Cette ligne attache le port a la socket(qui est dans la structure serv_addr) si ca retourne pas 1 ca déconne on affiche un message d'erreur.
	if (bind(sock_descriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Bind Echec");
	}
	//On dit au serveur d'ecouter sur la socket qu'on a mis en place le 5 signifie que 5 personnes peuvent se connecter simultanément.

	listen(sock_descriptor,5);
	
	printf("En attente de connection ...");
	int size = sizeof(client_addr);
	//Le serveur attend, lorsqu'une connexion est etablie ca retourne un descripteur de socket different.
	conn_desc = accept(sock_descriptor, (struct sockaddr *)&client_addr, &size);
	//Si c est égale a -1 ca a deconné
	if (conn_desc == -1)
	{
		printf("Echec de connection");
	}
	else
	//sinon ca marche
	{
		printf("Connected \n");
	}
	//On peut lire directement le nouveau descripteur, buff est le message recu. on l'affiche.
	if ( read(conn_desc, buff, sizeof(buff) -1) > 0)
	{
		printf("Received %s", buff);
	}
	else
	{
		printf("Echec de reception \n");
	}
	//On ferme les socket apres la receptions il faut toujours les fermes.
	//on ferme la socket qui ecoute et la socket qui est connecté.
	close(conn_desc);
	close(sock_descriptor);
	return 0;
}
