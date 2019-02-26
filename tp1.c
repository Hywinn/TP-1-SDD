#include "tp1.h"

/* ------------------------------------------------------------------
 * InsererElt	Insere un element dans la liste apres le prec 
 * 
 * En entree : - un pointeur de pointeur prec sur un message_t, 
 * indiquant la position a inserer.
 * 			   - un pointeur sur un message_t, nouv, 
 * l'element a  inserer.
 *
 * Interne : - un pointeur sur un message_t temp utilise comme variable
 * temporaire pour l'insertion.
 * 
 * Strategie : Affectation de prec dans une variable temporaire, de nouv
 * dans la variable pointee dans prec, puis de temp dans le suivant de
 * nouv.
 * -----------------------------------------------------------------*/
void InsererElt (message_t ** prec, message_t *nouv)
{
	message_t * temp = *prec;
	*prec = nouv;
	nouv->suiv = temp;
}

/* ------------------------------------------------------------------
 * CreerCell	Alloue une nouvelle cellule et en remplit les informations
 * 
 * En entree : - un entier pour la date de reception
 *			   - un entier pour la date de peeremption
 * 			   - le texte du message 
 *
 * Interne : - un pointeur sur un message_t nouv correspondant a la nouvelle
 * cellule allouée
 * 
 * Strategie : Initialisation et allocation d'un pointeur sur une nouvelle 
 * cellule.
 *		Si l'allocation s'est bien passée, on remplit les informations de la
 * cellule, avec une allocation dynamique pour la taille du texte. met le bloc
 * suivant a NULL et on retourne nouv, ou NULL si l'allocation a echoue.
 * 
 * Sortie : pointeur sur la cellule nouvellement alloue, ou NULL.
 * -----------------------------------------------------------------*/
message_t *CreerCell (int deb, int fin, char * texte)
{
	message_t * nouv =(message_t *)malloc(sizeof(message_t));
	if (nouv)
	{ 
		nouv->ddebut = deb;
		nouv->dfin = fin;
		nouv->texte=(char*)malloc(strlen(texte)*sizeof(char));
		if(nouv->texte) strcpy(nouv->texte,texte);
		nouv->suiv=NULL;
	}
	return nouv;
}
	
/* ------------------------------------------------------------------
 * RecherchePrec	Recherche et renvoie un pointeur sur pointeur du precedent de
 * l element recherche.
 * 
 * En entree : - un pointeur sur pointeur de la liste
 *			   - un pointeur sur l element recherche
 *
 * Interne : - cour, un pointeur sur message_t de parcours de liste
 * - prec, pointeur de pointeur sur l element precedent 
 * 
 * Strategie : Initialisation des pointeurs de parcours, puis parcours
 * tant que la liste n'est pas terminee et que la date de debut de l
 * element courant n est pas superieure a celle de l element recherche,
 * on deplace cour et prec. Finalement, on retourne prec.
 * 
 * Sortie : pointeur sur pointeur sur message_t du precedent de
 * l element recherche.
 * -----------------------------------------------------------------*/
message_t ** RecherchePrec(message_t ** liste, message_t * elt)
{
	message_t ** prec = liste;
	message_t * cour = * liste; 
	while (cour !=NULL && cour->ddebut < elt->ddebut) 
	{
		prec = &cour->suiv;
		cour = cour->suiv; 
	}
	return prec ;
}	


/* ------------------------------------------------------------------
 * fgetsp	: fgets modifie s'assurant que le message se termine  a
 * la fin de la ligne.
 * 
 * En entree : - une chaine de caracteres comprenant le texte du message
 *			   - la taille initiale du message
 *			   - un pointeur sur le fichier ouvert
 *
 * Interne : - 
 * 
 * Strategie : lecture du texte, puis troncage à la fin de la ligne.
 * 
 * Sortie : Texte du message lu, tronque.
 * -----------------------------------------------------------------*/
char * fgetsp(char *s, int size, FILE * fichier)
{
//	char *c;
	fgets(s, size, fichier);
	//ViderBuffer(fichier);
/*	if(strchr(s,"\n")==NULL){
		printf("Long");
		fscanf(c,"%*c");
	}
	else s[strcspn(s, "\n")]=0;*/
	s[strcspn(s, "\n")]=0;
	return s;
} 

/*void ViderBuffer(FILE *f)
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = fgetc(f);
    }
}*/
	
void CreerLch (char * nom, message_t ** liste)
{
	FILE * fichier;
	int db, df;
	char texte[101]; //Possible d'allouer dynamiquement
	char *c;  
	fichier=fopen(nom,"r");
	if(fichier)
	{
		message_t *nouv;
		
		fscanf(fichier,"%d %d ",&db, &df);
		while(!feof(fichier))
		{
				fgetsp(texte,101,fichier);
				nouv = CreerCell(db,df,texte);
				nouv->suiv=NULL;
				InsererElt(RecherchePrec(liste, nouv),nouv);
				fscanf(fichier,"%d %d ",&db, &df);
				
		}
		fclose(fichier);
	}
	else liste = NULL;
}

void Afficher(message_t * liste){
	int compteur=1;
	message_t *cour;
	cour = liste;
	while( cour !=NULL ){
		printf("\nMessage no %d: \n",compteur);
		printf( "Date de reception : %d\nDate de peremption : %d\nTexte : %s\n",cour->ddebut,cour->dfin,cour->texte);
		cour = cour->suiv;
		compteur++;
	}
} 


void AfficherNonExpire(message_t * liste){
	message_t *cour;
	cour = liste;
	printf("\nMessages non expires:\n");
	while( cour !=NULL ){
		if(!EstExpire(cour)){
		printf( "Date de reception : %d\nDate de peremption : %d\nTexte : %s\n",cour->ddebut,cour->dfin,cour->texte);
		}
		cour = cour->suiv;
	}
} 

int Date(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);	//Initialise le temps
	int date = (tm.tm_year+1900)*10000+(tm.tm_mon+1)*100+tm.tm_mday;	//Calcule au format demandï¿½
	
	//printf("now: %d\n", date);	
	
	return date;
}

void Sauvegarde(message_t *liste){
	message_t *cour=liste;
	FILE * sauvegarde;
	sauvegarde=fopen("test.txt","w");
	if(sauvegarde){
		while(cour != NULL){
			fprintf(sauvegarde,"%d %d %s",cour->ddebut,cour->dfin,cour->texte);
			cour=cour->suiv;
		}
		fclose(sauvegarde);
	}
}


// voici la suite (phase3)


bool EstExpire(message_t * mes)
{
	return (Date()>mes->dfin);
}

		 
void SupCell (message_t ** prec)
{
	message_t * cour = * prec;
	* prec = cour->suiv;
	free(cour); 
}

void SupprimerObsoletes(message_t ** lch)
{ 
    message_t * cour=*lch;
	message_t ** prec=lch;

	

  	while (cour != NULL )
    {
    	if(cour->dfin < Date()){
        	SupCell(prec);
        }
        else{
        	prec = &cour->suiv;
		}
		cour = *prec;

    }
}


void ChangerDate (message_t * lch, int viei, int nouv)
{
	message_t * cour = lch;
	while (cour != NULL && cour->ddebut <= viei){
		if (cour->ddebut == viei)
			cour->ddebut = nouv;
		cour = cour->suiv;
	}
}

// la partie 4
void AfficheSiMotif (message_t * lch, char * motif)
{
	message_t * cour;
	printf( "Les messages contenant le motif: %s\n", motif);
	while (cour != NULL)
	{
		if (strcmp(motif, cour->texte)) // pas trop sur
		     printf( "%s \n", cour->texte);
	}
}




int main(int argc, char * argv[]){
	message_t *lch = NULL;
	if(argc>1) {
		CreerLch (argv[1], &lch);
		Afficher(lch);
		
		AfficherNonExpire(lch);
				
		ChangerDate(lch, 0,20190221);
		Afficher(lch);
		
		Sauvegarde (lch);
		free(lch);
	}
	else printf("Veuillez preciser un nom de fichier\n");
	
	return 0;
}


