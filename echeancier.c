#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "echeancier.h"


double waitTime[MAXEVENT];
long int nbWaitTime;
double lambda;
double temps;
long int n;
int compteur;
double cumule;
echeancier Ech;

double Exp(double lamb){
	double r = (double)random()/RAND_MAX;

	while(r==0 || r==1){
		r = (double)random()/RAND_MAX;

	}
	return -(log(r)/(lamb));

}

void Ajouter_Ech(event e){

	if(Ech.taille<MAXEVENT){
		e.indiceEch=Ech.taille;
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
	e.associe=0;
	Ech.taille=0;
	e.indiceEch=0;
	Ajouter_Ech(e);
}


void Afficher_echeancier(){
	event e;
	printf("temps  %f   n %ld taille %d\n",temps,n,Ech.taille);
	for(int i =0;i<Ech.taille;i++){
		e=Ech.tab[i];
		if(e.type==0){
			printf("Ac %lf, %d,  %d, %d\n",e.date,e.etat, e.nfile, e.associe);

		}
		if(e.type==1){
			printf("FS %lf, %d,  %d\n",e.date,e.etat, e.nfile );
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






void get_lambda(){
	FILE* fd=fopen("lambda.txt","r");
	if(fd==NULL){printf("le fichier lambda.txt n'est pas trouvé ou n'a pas pu être ouvert");exit(0);}
	//à finir 

}


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

double percentile(){
	int pos = nbWaitTime*0.9;
	return waitTime[pos];
}

double waitmoy(){
	long double tot=0;
	for(int i=0;i<nbWaitTime;i++){
		tot+=waitTime[i];
		//if(waitTime[i]>0.05)printf(" %d : %lf\n",i,waitTime[i]);
	}
	
	// for(int i=nbWaitTime-1;i>nbWaitTime-20;i--){
	// 	printf("waittime %d:  %lf\n",i, waitTime[i]);
	// }
	// printf("tot %lf    nbWaitTime  %ld \n", tot,nbWaitTime);
	return tot/(double)nbWaitTime;
}



void initWt(){
	nbWaitTime=0;
	for(int i=0;i<MAXEVENT;i++){
		waitTime[i]=0.0;
	}
}