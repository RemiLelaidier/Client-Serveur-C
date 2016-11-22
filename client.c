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

#define T_MAX 30

void main(int argc, char *argv[]){
    int looptime = 0;                                                           /* Numero de la boucle */
    socklen_t ls = sizeof(struct sockaddr_in);                                  /* Taille des adresses */

    /*---- Caracterisation de la socket d'émission ----------*/
    int sd0;                                                                    /* Descripteur  */
    int ps0 = 0;                                                                /* Port Client  */
    int ps1;                                                                    /* Port Serveur */
    struct sockaddr_in adr0, *padr0 = &adr0;                                    /* Adresse  */
    struct sockaddr_in adr1, *padr1 = &adr1;
    struct hostent *hp1;                                                        /* Host serveur */

    /*---- Buffers pour Messages -------------------------------*/
    char msg_in[T_MAX] = "0";                                                       /* Message recu de "0" a "99" */
    char msg_out[T_MAX] = "0";                                                      /* Message a envoyer "0" a "99" */

    struct sockaddr_in adr2, *padr2 = &adr2;

    /*---- (0) - Verifications de base ----*/
    if (argc != 3){
        fprintf(stderr,"Syntaxe d'appel : client <addr_server> <port> \n");
        exit(2);
    }
    else{
        ps1 = atoi(argv[2]);
    }

    /*---- (1) - Preparation du socket ----*/
        /*---- (a) - Creation du socket ----*/
    if((sd0 = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("[SOCK_DGRAM, AF_INET, 0]");
    else
        printf("socket [SOCK_DGRAM, AF_INET, 0] creee\n");

        /*---- (b) - Preparation de l'adresse d'attachement ----*/
    adr0.sin_family         = AF_INET;
    adr0.sin_addr.s_addr    = htonl(INADDR_ANY);
    adr0.sin_port           = htons(ps0);

        /*---- (c) - Demande d'attachement de la socket ----*/
    if(bind(sd0,(struct sockaddr *)(padr0),ls) == -1) {
        perror("Attachement de la socket impossible");
        close(sd0);
        exit(2);
    }

        /*---- (d) - Recuperation de l'adresse d'attachement ----*/
    getsockname(sd0, (struct sockaddr*)padr0, &ls);

    /*---- (2) - Synchro avec le serveur ----*/
    hp1=gethostbyname(argv[1]);
    if(hp1 == NULL){
        fprintf(stderr,"machine %s inconnue\n",argv[1]);
        exit(2);
    }
    else{   /*---------------------- Recuperation de l'adresse IP ---------------------*/
        memcpy(&adr1.sin_addr.s_addr, hp1->h_addr, hp1->h_length);
        adr1.sin_family = AF_INET;
        adr1.sin_port   = htons(ps1);
        fprintf(stdout,"machine %s --> %s \n", hp1->h_name, inet_ntoa(adr1.sin_addr));
    }

    /*---- (3) Boucle emission-reception type serveur----*/
    for(;;) {
        int i;
        struct sockaddr_in adr2,  *padr2 = &adr2;
        printf("\n------------------\n");

        /*---- On invite l'utilisateur a rentre le calcul a realiser ----*/
        printf("Entrez votre calcul (ex : 1 + 2)\nOu une assignation de variable(x,y ou z) x = 0\n");
        fgets(msg_out, T_MAX, stdin);

        /* (a) Emission */
        printf("\n\nEnvoi(%d) ... ", looptime);
        if (sendto(sd0,msg_out,sizeof(msg_out),0,(struct sockaddr *)padr1,ls) >0)
            printf("termine : valeur = %s !\n",msg_out);
        else
            printf("inacheve : %s !\n",msg_out);

        /* (b) Reception */
        printf("Attente de reception ... ");
        if (recvfrom(sd0,msg_in,sizeof(msg_in),0, (struct sockaddr *)padr1, &ls) == -1)
            printf("inachevee : %s !\n",msg_in);
        else  {
            printf("recue : valeur = %s !\n",msg_in);
        }
        looptime++;
    }
}