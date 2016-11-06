/** Fichier client.c
 * Cree un client qui communique avec un programme serveur
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void main(int argc, char *argv[]){
    int looptime = 0;                                                           /* Numero de la boucle */
    socklen_t ls = sizeof(struct sockaddr_in);                                  /* Taille des adresses */

    /*---- Caracterisation de la socket d'émission ----------*/
    int sd0;                                                                    /* Descripteur  */
    int ps0 = 5001;                                                             /* Port         */
    struct sockaddr_in adr0, *padr0 = &adr0;                                    /* Adresse  */

    /*---- (0) - Verifications de base ----*/
    if (argc != 2){
        fprintf(stderr,"Syntaxe d'appel : client <addr_server> \n");
        exit(2);
    }



    // TODO
}