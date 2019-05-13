#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "echeancier.h"

double waitTime[MAXEVENT]; // Tableau des temps d'attente. Permet de calculer le 90-percentile
long int nbWaitTime; // Nombre d'éléments dans le tableau waitTime
int lambda; // Valeur de lambda
double temps; // Temps dans la simuation
long int n; // Nombre de clients dans la file
int compteur; // compteur pour la condition d'arrêt 
double cumule; // cumul du temps d'attente
echeancier Ech; // Tableau d'évènements

/* Retourne un variable aléatoire suivant une loi exponentielle de paramètre lamb */
double Exp(int lamb){
	double r = (double)random()/RAND_MAX;

	while(r==0 || r==1){
		r = (double)random()/RAND_MAX;

	}
	return -(log(r)/(lamb*1.0));

}

/* Ajoute un évènement dans l'échéancier */
void Ajouter_Ech(event e){

	if(Ech.taille<MAXEVENT){
		e.indiceEch=Ech.taille;
		Ech.tab[Ech.taille] = e;
		Ech.taille++;

	}
	else {printf("echeancier plein");exit(0);}	
}

/* Initialise l'échéancier */
void Init_Ech(){
	event e;
	e.date=0;
	e.etat=0;
	e.type=0;
	e.associe=0;
	Ech.taille=0;
	e.indiceEch=0;
	Ajouter_Ech(e);
}

/* Extrait le premier évènement non traité (celui dont la date est la plus petite)
 *	et le retourne */
event Extraire(){
	int imin;
	event min;

	for(int i=0;i<Ech.taille;i++){
		if(Ech.tab[i].etat == 0){
			min = Ech.tab[i];
			imin=i;
			break;
		}

	}

	for(int i=0;i<Ech.taille;i++){
		if(min.date>Ech.tab[i].date && Ech.tab[i].etat == 0){
			min=Ech.tab[i];
			imin=i;
		}
	}

	Ech.tab[imin].etat = 1;
	return min;
}

/* Condition d'arrêt de la simulation */
int condition_arret(long double old,long double new){
	if(fabs(old-new)< EPSILON && temps >1000){
		compteur++;

		if(compteur>1e3)return 1;
	}
	return 0;
}

/* Ajoute un nouveau temps d'attente dans le tableau waitTime */
void ajoutWt(double attente){
	int i=0;
	while(waitTime[i]<attente && i<nbWaitTime)
		i++;
	if(i>=nbWaitTime){
		nbWaitTime++;
		waitTime[i]=attente;
	}
	else{
		double tmp=waitTime[i], tmp2;
		waitTime[i]=attente;
		nbWaitTime++;
		for(i=i+1; i<nbWaitTime;i++){
			tmp2=waitTime[i];
			waitTime[i]=tmp;
			tmp=tmp2;
		}
	}
}

/* Renvoie le 90-percentile */
double percentile(){
	int pos = nbWaitTime*0.9;
	return waitTime[pos];
}

/* Calcule le temps moyen d'attente */
double waitmoy(){
	double tot=0;
	for(int i=0;i<nbWaitTime;i++){
		tot+=waitTime[i];
	}
	
	return tot/(double)nbWaitTime;
}

/* Initialise le tableau des temps d'attente */
void initWt(){
	nbWaitTime=0;
	for(int i=0;i<MAXEVENT;i++){
		waitTime[i]=0;
	}
}