#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

//Variables et structures:

typedef struct message{
	int ddebut;
	int dfin;
	char *texte;
	struct message * suiv;
}message_t;



// Prototypes de fonctions

void InsererElt (message_t ** prec, message_t *nouv);
message_t *CreerCell (int deb, int fin, char * texte);
message_t ** recherche_Prec(message_t ** liste, message_t * elt);
char * fgetsp (char *s, int size, FILE * fichier);
void ViderBuffer(FILE *f);

void CreerLch(char * nom, message_t ** liste);
void afficher(message_t * liste);

void afficherNonExpire(message_t * liste);
int Date();
void sauvegarde(message_t *liste);

bool EstExpire(message_t * mes);
void SupCell (message_t ** prec);

void SupprimerObsoletes(message_t ** lch);

void ChangerDate (message_t * lch, int viei, int nouv);
void AfficheSiMotif (message_t * lch, char * motif);



