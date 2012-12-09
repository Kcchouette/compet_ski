/*
*    Copyright © 2012 Kcchouette
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdlib.h" //en cas d'appels system
#include "string.h" //en cas de chaînes
#include <stdio.h>


// Structure des pays
struct Pays
{
  char codePays[3]; //codePays
	char nomPays[11]; // Nom du pays
	int NInscrits; //nombre inscrits par nationalite
	int NClasses; //nombre de skieur dans le classement final
	int NParticipants; //nombre de skieurs n'ayant pas déclaré forfait
	float tempsTotal; //temps total d'arrivé des classés
};

// Structure des participants
struct participant //tableau de structure
{
    int rang; //classement du participant
    char nom[21]; // Nom du participant
    int codeNationalite; //indice du Code Nationalite
    float tempsRealise; //temps realise lors de la competition
    int dossard; //numéro de dossard du participant
};

//fonction permettant le trie par temps puis par ordre alphabétique
void determiner_rang(int classementTemps[], participant tab[],Pays nation[],int indiceParticipant, int nbArrivee);

//classement uniquement par ordre alphabétique
void determiner_disqual(int participantDisqualifie[], participant tab[],Pays nation[],int indiceParticipant ,int nbDisqual);

//fonction permettant l'affichage du classement provisoire et final
void classement(int compteurArrive,struct participant tab[],int classementTemps[],struct Pays nation[]);

//fonction permettant l'affichage des disqualifies et des elimines
void hors_classement(participant tab[], Pays nation[],int nb, int t[]);

void recapitulatif(Pays nation[]);
// Programme Principal
int main()
{
    int i,j; // variables utilisées comme compteur principalement
    int compteurInscrit=0; //nbre d'inscrits
    char nomInscription[21]; //nom qui sera entré
    char codeNat[3]; //codenationalité qui sera entré

    struct participant dataParticipant[21];

	// Liste des pays autorises et initialisation de la strcuture Pays
	struct Pays listeNationalite[]=
	{
		{"AL","ALLEMAGNE",0,0,0},
		{"AU","AUTRICHE",0,0,0},
		{"FR","FRANCE",0,0,0},
		{"IT","ITALIE",0,0,0},
		{"LU","LUXEMBOURG",0,0,0},
		{"PL","POLOGNE",0,0,0},
		{"CH","SUISSE",0,0,0},
	};

    /*
	****************************************************************
     Phase 1 : Inscriptions des participants
    ****************************************************************
	*/
	printf("\n\t\t ================================== ");
	printf("\n\t\t === PHASE 1 : Les Inscriptions === \n"); //phase 1
	printf("\t\t ================================== \n\n");
    printf("Entrer le nom du futur participant ou * pour signaler la fin des inscriptions : \t     "); //Demander le nom du partcipant
    gets(nomInscription); //Mettre * pour finir la phase 1 sinon ecrire le nom du participant

	while(nomInscription[0]!='*')
    {
        for(i=0;i<compteurInscrit&&(strcmp(dataParticipant[i].nom,nomInscription)!=0);i=i+1); //Vérifer la presence du participant dans le tableau, il ne doit pas etre present

        if(i<compteurInscrit) // Msg erreur : participant deja enregistre
        {
            printf("\nERREUR : Le nom %s est deja enregistre.\n",dataParticipant[i].nom);
            i=i-1;
        }
        else // Enregistrer le nom du participant
		{
            strcpy(dataParticipant[i].nom,nomInscription); // ajout du participant dans le tableau des participants

			printf("Entrer son code nationalite : \t"); //Demander le code de la nationalité
            gets(codeNat);

		do{
				for(j=0;j<7&&(strcmp(listeNationalite[j].codePays,codeNat)!=0);j=j+1); //Vérifier le code de la nationalité parmi la liste de ceux autorisés

				if (j>=7)//le code ne fait pas partie de la liste
				{
					printf("ERREUR : Le code nationalite %s ne fait pas parti des codes nationalites pre-enregistres.\n Re-entrer son code nationalite : \t",codeNat);
					gets(codeNat);
				}

			}while(j>=7) ; //on refait ça tant que le code n'appartient pas à la liste


			dataParticipant[i].codeNationalite=j; //on stocke l'indice du code nationalité dans la strcuture participant
			dataParticipant[i].tempsRealise=-1.0; //on initialise le temps et le rang maintenant pour optimiser
		 	dataParticipant[i].rang=0;
			listeNationalite[j].NInscrits++; //on ajoute un inscrit dans la nationalité
			compteurInscrit++;
		}

		printf("\nEntrer le nom du futur participant ou * pour signaler la fin des inscriptions : \t     ");
        gets(nomInscription); //reprise de la boucle tant que le nom différent de *
    }

	printf("\n\t Fin des Inscriptions\n\n"); //fin des inscriptions

	system("pause");
	system("cls");// Fin des inscriptions

// -------------------------------------------------------------------

	for(i=0;i<compteurInscrit;i=i+1) // Afficher les participants saisis
    {
        if(i==0) // si c'est la première boucle on détaille le tableau
        {
            printf("\t\t=== AFFICHAGE DES INSCRITS ===\n\n");
            printf("Numero Dossard\tNom du participant\tNationalite du participant\n");
        }

        printf("%-10d \t %-20s\t%-10s\n",i+1,dataParticipant[i].nom,listeNationalite[dataParticipant[i].codeNationalite].codePays);
        dataParticipant[i].dossard=i+1;
    }

	printf("\n");
	system("pause");
	system("cls");// Fin affichage

	// -------------------------------------------------------------------

    printf("\t== AFFICHAGE DES INSCRITS D'UNE MEME EQUIPE ==\n\n");
    printf("Entrer un Code Nationalite ou * pour quitter :\t");
    gets(codeNat);

    while(codeNat[0]!='*') // tant que le code de nationalite entré est différent de *
    {
		//on recherche le code entré parmi les inscrits

		for(j=0;j<7&&(strcmp(listeNationalite[j].codePays,codeNat)!=0);j++); //Vérifier le code de la nationalité parmi la liste de ceux autorisés

        if (j<7)//le code appartient au tableau des nationalités
        {
            if(listeNationalite[j].NInscrits>0)
			{    //on affiche les inscrits du meme code de nationalité
				for(i=0;i<compteurInscrit;i++)
				{
					if( dataParticipant[i].codeNationalite==j);
					{
					if(i==0)
						printf("\nCode Nationalite\tLibelle du pays\t\tNombre d'inscrits\n");

					printf("%-2s\t\t\t%-10s\t%20d\n",listeNationalite[dataParticipant[i].codeNationalite].codePays,listeNationalite[dataParticipant[i].codeNationalite].nomPays,listeNationalite[dataParticipant[i].codeNationalite].NInscrits);
					printf("\nNum Dossard\tNom de l'inscrits\n");
					printf("%-10d\t%-20s\n",i+1,dataParticipant[i].nom);
					}
				}
            }
			else
			{
				printf("code nationalite   Libelle \n");
				printf("%-2s %-10s AUCUN INSCRIT\n",listeNationalite[j].codePays, listeNationalite[j].nomPays);
			}
        }
        else //code de nationalité ne fait pas partie de ceux annoncés
        {
            printf("CODE NATIONALITE %s INCORRECT.\n",codeNat);
        }

		printf("\nEntrer un Code Nationalite ou * pour quitter :\t");
        gets(codeNat);
    }

	printf("\n");
	system("pause");
	system("cls");


	/*
	****************************************************************
     Phase 2 : La compétition
    ****************************************************************
	*/


	int compteurArrive=0;
	int compteurDisqual=0;
	int numDossard;
    int cTemps[21];
    int pDisqualifie[21];
    int pForfait[21];
    int indice;
    int compteurForfait;
	int nbInscrit=compteurInscrit;

	printf("\n\t\t ========================================= ");
	printf("\n\t\t === PHASE 2 : Suivi de la competition === \n"); //phase 2
	printf("\t\t ========================================= \n\n");
	fflush(stdin);

	printf("Saisir le numero de dossard ou 0 pour indiquer la fin de l'epreuve : \t"); // Indiquer le numéro de dossard du participant arrivé

	scanf("%d",&numDossard);
	indice=numDossard-1; //on retire 1 au numero de dosard pour trouver l'indice du participant dans le tableau

    while(numDossard!=0 && compteurInscrit!=0)
		{
			if(numDossard>0 && numDossard-1<=nbInscrit)
			{
				if(dataParticipant[indice].tempsRealise==-1.0) //verifie que le participant n'est pas encore arrivé, donc le temps d'arrive est de -1
				{
					printf("\nSaisir le temps realise du skieur en secondes: \t"); //indiquer le temps du participant

					scanf("%f",&(dataParticipant[indice].tempsRealise));

					if((dataParticipant[indice].tempsRealise)>0.0)// si le participant n'est pas disqualifié
                    {
                        compteurArrive++;
                        listeNationalite[dataParticipant[indice].codeNationalite].tempsTotal += dataParticipant[indice].tempsRealise;
                        listeNationalite[dataParticipant[indice].codeNationalite].NParticipants ++;
                        determiner_rang(cTemps,dataParticipant,listeNationalite,indice,compteurArrive); //Insertion des skieurs classé par temps dans le tableau d'indices ttemps
                        compteurInscrit--;
                    }

                    if((dataParticipant[indice].tempsRealise)==0.0)
                    {
                        compteurDisqual++;
                        determiner_disqual(pDisqualifie,dataParticipant,listeNationalite,indice,compteurDisqual);
                        compteurInscrit--;

                    }
                    printf("\n\n\t=====   CLASSEMENT PROVISOIRE   =====\n");

                    classement(compteurArrive,dataParticipant,cTemps,listeNationalite); //a verifier
                }

				else
                {
                    printf("Le skieur, dont le numero de dossard est %d, est deja arrive.\n",numDossard);
                }

        }
		else//ERREUR, le participant n'existe pas
		{
		    printf("Le numero de dossard %d est inexistant dans la base de donnee.\n",numDossard);
		}
        system("pause");
        system("cls");

        if((compteurDisqual+compteurArrive)<= nbInscrit)
        {
            printf("Inserer un numero de dossard ou 0 pour indiquer la fin de l'epreuve : \t");
            fflush(stdin);
            scanf("%d",&numDossard);
            indice=numDossard-1; //on retire 1 au numero de dosard pour trouver l'indice du participant dans le tableau
        }
	}
	compteurForfait=compteurInscrit-(compteurArrive+compteurDisqual); // compteur du nombre de skieur ayant déclaré forfait
    for (i=0, j=compteurForfait; i<compteurInscrit, j>0 && i>0; j--) // On met dans le tableau tforfait ceux qui ont un temps = à 1.0
    {
        if ((dataParticipant[i].tempsRealise)==(-1.0))
        {
            pForfait[j]=i;
            compteurForfait++;
        }
    }

	printf("\n");
	system("pause");
	system ("cls");

    /*
	****************************************************************
     Phase 3 : Les résultats
    ****************************************************************
	*/
	char demande;

	printf("\n\t ========================================= ");
	printf("\n\t === PHASE  : Les resultats finaux === \n"); //phase 3
	printf("\t ========================================= \n\n");

    printf("Affichage des resultats, saisir : \n");
    printf("\t'C' pour afficher le classement final \n");
    printf("\t'D' pour afficher la liste des disqualifies \n");
    printf("\t'F' pour afficher la liste des skieurs forfaits \n");
    printf("\t'T' pour afficher le tableau recapitulatif des equipes nationales\n");
    printf("\t'Q' pour quitter le programme \n");
    fflush(stdin);
    demande=getchar();
    while (demande!='Q')
    {
        switch(demande)
        {
            case 'C':   system("cls");
                        printf("\n\n\t======  CLASSEMENT FINAL  ======\n");
                        classement(compteurArrive,dataParticipant,cTemps,listeNationalite);
                        break;

            case 'D':   system("cls");
                        printf("\n\n\t===   LISTE DES DISQUALIFIES   ===\n");
                        if (compteurDisqual>0)
                            hors_classement(dataParticipant,listeNationalite,compteurDisqual, pDisqualifie);
                        else
                            printf("\tAUCUN DISQUALIFIE\n");
                        break;

            case 'F':   system("cls");
                        printf("\n\n\t==   LISTE DES SKIEURS FORFAITS   ==\n");
                        if (compteurForfait>0 )
                            hors_classement(dataParticipant,listeNationalite,compteurForfait, pForfait);
                        else
                            printf("\tAUCUN FORFAIT\n");
                        break;

            case 'T':   system("cls");
                        printf ("=   TABLEAU RECAPITULATIF DES EQUIPES NATIONALES   =\n");
                        recapitulatif(listeNationalite);
                        break;

            default :   printf("\nVous avez sans doute fait une erreur dans le choix de votre lettre alphabetique,\n et attention, ce programme est sensible a la casse\n");
                        break;
        }
        printf("\n\nAffichage des resultats, saisir : \n");
        printf("\t'C' pour afficher le classement final \n");
        printf("\t'D' pour afficher la liste des disqualifies \n");
        printf("\t'F' pour afficher la liste des skieurs forfaits \n");
        printf("\t'T' pour afficher le tableau recapitulatif des equipes nationales\n");
        printf("\t'Q' pour quitter le programme \n");
        fflush(stdin);
        demande=getchar();
    }
    system("pause");
    system("cls");
    return(0);
}

// trie par temps et par nom
void determiner_rang(int classementTemps[], participant tab[],Pays nation[],int indiceParticipant, int nbArrivee)
{
	    int i,j;

        i=indiceParticipant;

        for (j=nbArrivee-1; (j>0 && tab[i].tempsRealise<tab[classementTemps[j]].tempsRealise) || (j>0 && tab[i].tempsRealise==tab[classementTemps[j]].tempsRealise && (strcmp (tab[i].nom, tab[classementTemps[j]].nom)<0));j--)
        {
            classementTemps[j+1]=classementTemps[j]; //on decale l'indice du plus grand
        }

        nation[tab[i].codeNationalite].NClasses++;
        classementTemps[j+1]=i;

        //Affichage du skieur
        printf("%2d %20s %10s %5.2f\n",(indiceParticipant+1),tab[i].nom, nation[tab[i].codeNationalite].nomPays, tab[i].tempsRealise);
}


// trie par  nom
void determiner_disqual(int participantDisqualifie[],participant tab[],Pays nation[],int indiceParticipant,int nbDisqual)
{
	    int i,j;
		i=indiceParticipant;

		for (j=nbDisqual-1; j > 0 && strcmp(tab[i].nom, tab[participantDisqualifie[j]].nom); j--)
        {
            participantDisqualifie[j+1] = participantDisqualifie[j]; //on decale l'indice le plus grand
        }
        participantDisqualifie[j+1] = i;

		//Affichage du skieur
        printf("%2d %20s %10s disqualifie\n", indiceParticipant+1, tab[i].nom, nation[tab[i].codeNationalite].nomPays);
}


void classement(int compteurArrive,participant tab[],int classementTemps[],Pays nation[])
{
    printf("\n\nRang  Dossard  Nom du skieur   Pays  Temps \n");
    int i;
    for(i=0;i<compteurArrive;i++)
    {
        float tempsAvant=0.0;//initialisation d'une variable utilisée si le temps est le meme
        if(tab[classementTemps[i]].tempsRealise>tempsAvant) //dans ce cas on affiche le rang
        {
            printf("%3d",i+1);
            tempsAvant=tab[classementTemps[i]].tempsRealise;
        }
        //dans tous les cas on ne l'affiche pas
        printf("  %d  %-20s %-10s %5.2f \n",tab[i].dossard, tab[i].nom, nation[tab[i].codeNationalite].nomPays,tab[i].tempsRealise);
    }

}

void hors_classement(participant tab[], Pays nation[],int nb, int t[])
{
    int i, j;
    printf("\n\nRang  Dossard  Nom du skieur   Pays  Temps \n");
    for(i=0; i<nb;i++)
    {
        j=t[i];
        printf ("%-20s %-3d %-10s\n",tab[j].nom, j+1, nation[tab[j].codeNationalite].nomPays, tab[j].tempsRealise);
    }
}


void recapitulatif(Pays nation[])
{
    int i;
    float moy;
    printf("  Pays    Code Inscrits Participants  Nb de Classes  Moyenne \n");
    for(i=0; i<7; i++)
    {
        printf("%-10s %-4s %8d %12d %7d \n", nation[i].nomPays, nation[i].codePays,nation[i].NInscrits,nation[i].NParticipants,nation[i].NClasses);

        if(nation[i].NClasses == 0)
            moy = 0.0;
        else
            moy = nation[i].tempsTotal/nation[i].NClasses;

        printf("Moyenne du pays: %4.2f\n", moy);
    }
}

