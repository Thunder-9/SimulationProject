#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "echeancier.h"




double temps=0;
long int n=0;
int compteur=0;
double cumule=0;
echeancier Ech;

double Exp(int lamb){
	double r = (double)random()/RAND_MAX;

	while(r==0 || r==1){
		r = (double)random()/RAND_MAX;

	}
	return -(log(r)/(lamb*1.0));

}

void Ajouter_Ech(event e){

	if(Ech.taille<MAXEVENT){
		Ech.tab[Ech.taille] = e;
		Ech.taille++;

	}
	else {printf("echeancier plein");exit(0);}	
}

void Init_Ech(){
	
	event e;
	e.date=0;
	e.etat=0;
	e.type=0;
	Ech.taille=0;
	Ajouter_Ech(e);
}


void Afficher_echeancier(){
	event e;
	printf("temps  %f   n %ld taille %d",temps,n,Ech.taille);
	for(int i =0;i<Ech.taille;i++){
		e=Ech.tab[i];
		if(e.type==0){
			printf("Ac %lf, %d",e.date,e.etat);

		}
		if(e.type==1){
			printf("FS %lf %d",e.date,e.etat );
		}

	}
	printf("]\n\n");

}

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

int condition_arret(long double old,long double new){
	if(fabs(old-new)< EPSILON && temps >1000){
		compteur++;

		if(compteur>1e3)return 1;
	}
	return 0;
}



