#include "tp1.h"

/* ------------------------------------------------------------------
 * InsererElt	Insere un element dans la liste après le prec 
 * 
 * En entree : - un pointeur de pointeur prec sur un message_t, 
 * indiquant la position où insérer.
 * 			   - un pointeur sur un message_t, nouv, 
 * l'élément à insérer.
 *
 * Interne : - un pointeur sur un message_t temp utilisé comme variable
 * temporaire pour l'insertion.
 * 
 * Stratégie : Affectation de prec dans une variable temporaire, de nouv
 * dans la variable pointée dans prec, puis de temp dans le suivant de
 * nouv.
 * -----------------------------------------------------------------*/
void InsererElt (message_t ** prec, message_t *nouv)
{
	message_t * temp = *prec;
	*prec = nouv;
	nouv->suiv = temp;
}

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
	
message_t ** recherche_Prec(message_t ** liste, message_t * elt)
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

char * fgetsp(char *s, int size, FILE * fichier)
{
	fgets(s, size, fichier);
	//ViderBuffer(fichier);
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
				InsererElt(recherche_Prec(liste, nouv),nouv);
				fscanf(fichier,"%d %d ",&db, &df);
				
		}
		fclose(fichier);
	}
	else liste = NULL;
}

void afficher(message_t * liste){
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


void afficherNonExpire(message_t * liste){
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
	int date = (tm.tm_year+1900)*10000+(tm.tm_mon+1)*100+tm.tm_mday;	//Calcule au format demand�
	
	//printf("now: %d\n", date);	
	
	return date;
}

void sauvegarde(message_t *liste){
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
	
	/*liste->ddebut=19990520;
	liste->dfin=20190520;
	
	InsererElt (liste, CreerCell(20040223,20240223,"Bonjour"));
	sauvegarde(liste);
	free(liste);*/
	message_t *lch = NULL;
	//lch = NULL;
	if(argc>1) {

		CreerLch (argv[1], &lch);
		afficher(lch);
		
		afficherNonExpire(lch);
		
		//printf("\nSuppression des messages expires :\n");
		//SupprimerObsoletes(&lch);
		//afficher(lch);
		
		ChangerDate(lch, 0,20190221);
		afficher(lch);
		
		//AfficheSiMotif(lch, "ooooo");
		
		sauvegarde (lch);
		free(lch);
	}
	else printf("Veuillez preciser un nom de fichier\n");
	/*printf("Affichage : %d %d \n",lch->ddebut,lch->dfin);
	printf("Affichage : %s %p \n",lch->texte,lch->suiv);*/

	return 0;
}

