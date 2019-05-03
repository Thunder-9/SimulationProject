#include "echeancier.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

long int Nf[10];
extern int lambda;
extern double temps;
extern long int n;
extern int compteur;
extern double cumule;
extern echeancier;
extern double waitTime[MAXEVENT];


int getmin(){
	int imin=0;
	int min=Nf[0];
	for(int i=1;i<N;i++){
		if(Nf[i]<min){
			min=Nf[i];
			imin=i;
		}
	}
	return imin;
}



void Arrive_Event(event e){
	printf("j'execute AC \n");
	n++;
	int r = getmin();
	Nf[r]++;
	event e1;
	e1.type = 0;
	e1.date = e.date + Exp(lambda);
	e1.etat = 0;
	e1.nfile=r;
	Ajouter_Ech(e1);

	if(Nf[r]==1){
		event e2;
		e2.type = 1;
		e2.date=e.date+Exp(mu);
		e2.etat=0;
		e2.nfile=r;
		Ajouter_Ech(e2);
	}
	temps = e.date;
}

void Service_Event(event e){
	printf("j'execute FS\n" );
	if(Nf[e.nfile]>0){
		Nf[e.nfile]--;n--;
		if(Nf[e.nfile]>0){


			event e1;
			e1.type = 1;
			e1.date = e.date + Exp(mu);
			e1.etat = 0;
			e1.nfile=e.nfile;
			Ajouter_Ech(e1);
	}
	ajoutWt(e.date-temps);
	temps=e.date;
	}
}



void simulateur(FILE *f1){
	event e;
	long double oldmoyen;
	long double nmoyen;
	Init_Ech();

	for(int i=0;i<10;i++){
		Nf[i]=0;
	}

	while(condition_arret(oldmoyen,nmoyen)==0){
		e =Extraire();
		cumule += (e.date-temps)*n;
		printf("cum %f\n",cumule);
		oldmoyen = nmoyen;
		nmoyen = cumule/temps;
		if(temps==0){
		// printf("temps = %F et n = %ld et  nmoyen = %LF\n",temps,n,nmoyen );
		// exit(0);
			printf("temps = 0 et n =0 etnmoyen =0 \n");
			fprintf(f1,"0   0 \n");
		}
		else{
			printf("temps = %F et n = %ld et  nmoyen = %LF\n",temps,n,nmoyen );
			fprintf(f1,"%F   %LF \n",temps,nmoyen);
		}
	

	if( e.type ==0)
		Arrive_Event(e);
	if(e.type ==1)
		Service_Event(e);
	}
}


int main(int argc,char const *argv[]){
	temps=0;
	cumule=0;
	n=0;
	compteur=0;	
	lambda=70;
	FILE* f1 =fopen("Simulation_MM1.data","w");
	 srandom(getpid()+time(NULL));
	simulateur(f1);
	fclose(f1);
	exit(0);



}