#include "echeancier.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

long int Nf[10];
extern double lambda;
extern double temps;
extern long int n;
extern int compteurT;
extern int compteurP;
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
	// printf("j'execute AC \n");
	n++;
	int r = getmin();
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
		ajoutWt((double)0);
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
			ajoutWt(e.date-tmp.date);

	}
	temps=e.date;
	}
}



int simulateur(FILE *f1){
	event e;
	 double oldT=0;
	 double newT=0;
	 double oldP=0;
	 double newP=0;
	Init_Ech();

	for(int i=0;i<10;i++){
		Nf[i]=0;
	}

	while((condition_arret(oldT,newT,0)==0 || condition_arret(oldP,newP,1)==0) && temps<5000){
		e =Extraire();
		oldT=newT;
		newT=waitmoy();
		oldP=newP;
		newP=percentile();
		
	if( e.type ==0)
		Arrive_Event(e);
	if(e.type ==1)
		Service_Event(e);


	}
	printf("temps %f\n", temps);
	printf("cpt %d\n", compteurT);
	printf("cpp %d\n", compteurP);
	//aucun converge
	if(condition_arret(oldT,newT,0)==0 && condition_arret(oldP,newP,1)==0)
		return 0;
	//le percentile ne converge pas
	if(condition_arret(oldT,newT,0)==1 && condition_arret(oldP,newP,1)==0)
		return 1;
	//le remps d'attente ne converge pas
	if(condition_arret(oldT,newT,0)==0 && condition_arret(oldP,newP,1)==1)
		return 2;
	//tout converge
	if(condition_arret(oldT,newT,0)==1 && condition_arret(oldP,newP,1)==1)
		return 3;

}


int main(int argc,char const *argv[]){
	srandom(getpid()+time(NULL));
	FILE* f1 =fopen("simulation_file3.data","w");
	FILE* f2 = fopen("lambda.txt", "r");
	int converge;
	while(fscanf(f2, "%lf\n", &lambda) != EOF){
		n=0;
		compteurT=0;
		compteurP=0;
		initWt();
		printf("file 3 lambda %lf\n", lambda);
		converge=simulateur(f1);
		if(converge==0)
			fprintf(f1, "%lf %d %d\n",lambda, -1, -1);
		if(converge==1)
			fprintf(f1, "%lf %lf %d\n",lambda, waitmoy(), -1);
		if(converge==2)
			fprintf(f1, "%lf %d %lf\n",lambda, -1, percentile());
		if(converge==3)
			fprintf(f1, "%lf %lf %lf\n",lambda, waitmoy(), percentile());
		
	}
	fclose(f1);
	fclose(f2);
	exit(0);



}