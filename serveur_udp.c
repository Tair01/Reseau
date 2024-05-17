/*
* Code du serveur
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <errno.h> //Pour faire une exception des erreurs
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <strings.h>
/* Port local du serveur */
#define PORT 9600

int Socket(int domain, int type, int protocol){
    int resultat = socket(domain,type,protocol);
    if(resultat == -1){
        perror("Erreur de socket");
        exit(EXIT_FAILURE);
    }
    return resultat;
}

void Bind(int sockfd, struct sockaddr *addr, int addrlen) {
    int resultat = bind(sockfd, addr, addrlen);
    if (resultat == -1) {
        perror("Erreur de bind");
        exit(EXIT_FAILURE);
    }
}
int Recvfrom(int sockfd, char *buf, int len, int flags,struct sockaddr *from, socklen_t *fromlen){
    int resultat = recvfrom(sockfd, buf, len, flags,from,fromlen);
    if(resultat == -1){
        perror("Erreur de recvfrom");
        exit(EXIT_FAILURE);
    }
    return resultat;
}
int Sendto(int sockfd, const char *buf, int len, int flags,struct sockaddr *to, socklen_t tolen){
    int resultat = sendto(sockfd, buf, len, flags,to,tolen);
    if(resultat == -1){
        perror("Erreur de sendto");
        exit(EXIT_FAILURE);
    }
    return resultat;
}

int main(){
    /*
    * Variables du serveur
    * Déclarer ici les variables suivantes :
    * - sockfd le descripteur de socket
    * - structure d’adresse locale du serveur
    * - structure d’adresse du client
    * - taille de l’adresse du client
    */
    int sockfd = Socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serveur_udp = {0};
    struct sockaddr_in client_udp;
    socklen_t client_udp_size = sizeof(client_udp);
    char buffer[1024];
    
    bzero(&serveur_udp, sizeof(serveur_udp));
    serveur_udp.sin_family = AF_INET; 
    serveur_udp.sin_port = htons(PORT);
    serveur_udp.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(sockfd, (struct sockaddr*)&serveur_udp, sizeof(serveur_udp));
    
    while (1) {
        int n = Recvfrom(sockfd,buffer, sizeof(buffer),0,(struct sockaddr *)&client_udp, &client_udp_size);
        write(1,"Un datagramme reçu:\n ",21);
        write(1, buffer, n);
        n = Sendto(sockfd, "\nJ'ai reçu ton message\n",23,0,(struct sockaddr *)&client_udp, sizeof(client_udp));
    }
    return 0;
}