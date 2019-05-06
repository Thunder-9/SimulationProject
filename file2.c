
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
extern echeancier Ech;
extern double waitTime[MAXEVENT];



event Get_Client(int nfile){
	double min;
	int imin=-1;
	for(int i=0;i<Ech.taille;i++){
		if(Ech.tab[i].etat == 1 && Ech.tab[i].type == 0 && Ech.tab[i].associe == 0 && Ech.tab[i].nfile==nfile){
			min=Ech.tab[i].date;
			imin=i;
			break;
		}

	}

	for(int i=imin;i<Ech.taille;i++){
		if(Ech.tab[i].date<min && Ech.tab[i].etat == 1 && Ech.tab[i].type == 0 && Ech.tab[i].associe == 0 && Ech.tab[i].nfile==nfile){
			imin=i;
			min=Ech.tab[i].date;
			
		}
	}
	Ech.tab[imin].associe=1;
	return Ech.tab[imin];
}


void Arrive_Event(event e){
	// printf("j'execute AC \n");
	n++;
	int r = random()%N;
	Nf[r]++;
	Ech.tab[e.indiceEch].nfile=r;
	event e1;
	e1.type = 0;
	e1.date = e.date + Exp(lambda);
	e1.etat = 0;
	e1.associe=0;
	Ajouter_Ech(e1);

	if(Nf[r]==1){
		event e2;
		e2.type = 1;
		e2.date=e.date+Exp(mu);
		e2.etat=0;
		e2.nfile=r;
		e2.date_ac=e.date;
		Ajouter_Ech(e2);
		Ech.tab[e.indiceEch].associe=1;
	}
	temps = e.date;
}

void Service_Event(event e){
	// printf("j'execute FS\n" );
	if(Nf[e.nfile]>0){
		Nf[e.nfile]--;n--;
		if(Nf[e.nfile]>0){
			event e1;
			event tmp=Get_Client(e.nfile);
			e1.date_ac=tmp.date;
			e1.type = 1;
			e1.date = e.date + Exp(mu);
			e1.etat = 0;
			e1.nfile=e.nfile;
			Ajouter_Ech(e1);
	}
	ajoutWt(e.date-e.date_ac);
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
		//printf("cum %f\n",cumule);
		oldmoyen = nmoyen;
		nmoyen = cumule/temps;
		// if(temps==0){
		// // printf("temps = %F et n = %ld et  nmoyen = %LF\n",temps,n,nmoyen );
		// // exit(0);
		// 	//printf("temps = 0 et n =0 etnmoyen =0 \n");
		// //	fprintf(f1,"0   0 \n");
		// }
		// else{
		// 	//printf("temps = %F et n = %ld et  nmoyen = %LF\n",temps,n,nmoyen );
		// //	fprintf(f1,"%F   %LF \n",temps,nmoyen);
		// }
	

	if( e.type ==0)
		Arrive_Event(e);
	if(e.type ==1)
		Service_Event(e);
	}
}


int main(int argc,char const *argv[]){
	lambda=90;
	srandom(getpid()+time(NULL));
	FILE* f1 =fopen("simulation_file2.data","w");
	FILE* f2 = fopen("lambda.txt", "r");
	while(fscanf(f2, "%d\n", &lambda) != EOF){
		temps=0;
		cumule=0;
		n=0;
		compteur=0;
		initWt();
		printf("file 2 lambda %d\n", lambda);
		simulateur(f1);
		fprintf(f1, "%d %lf %lf\n",lambda, waitmoy(), percentile());
	}
	fclose(f1);
	fclose(f2);
	exit(0);
}
