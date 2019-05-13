
#include "echeancier.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

long int Nf[10]; // Nombre de clients dans les files de chaque serveur
extern double lambda;
extern double temps;
extern long int n;
extern int compteur;
extern double cumule;
extern echeancier Ech;
extern double waitTime[MAXEVENT];

/* Récupère le premier évènement "arrivée client" traité et non associé à une fin de service */
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

/* Traite l'arrivée d'un client.
 * Si le serveur gérant la file du client traité est libre, une fin de service pour ce client 
 * est rajoutée dans l'échéancier.
 * Ajoute une nouvelle arrivée de client (non traitée) dans une file aléatoire. */
void Arrive_Event(event e){
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
		Ajouter_Ech(e2);
		Ech.tab[e.indiceEch].associe=1;
		ajoutWt((double)0);
	}
	temps = e.date;
}

/* Traite une fin de service.
 * S'il y a encore des clients dans la file d'attente, récupère le premier et
 * ajoute sa fin de service dans l'échéancier. */
void Service_Event(event e){
	if(Nf[e.nfile]>0){
		Nf[e.nfile]--;
		n--;
		if(Nf[e.nfile]>0){
			event e1;
			event tmp=Get_Client(e.nfile);
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


/* Simule la file d'attente et calcule la condition d'arrêt en fonction
 * des temps d'attente des clients */
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
		oldmoyen = nmoyen;
		nmoyen = cumule/temps;
		if( e.type ==0)
			Arrive_Event(e);
		if(e.type ==1)
			Service_Event(e);
	}
}

/* Fonction main. Pour chaque valeur de lambda dans le fichier lambda.txt,
 * simule la file d'attente et sauvegarde les temps moyens d'attente et les 
 * 90-percentiles dans le fichier simulation_file2.data */
int main(int argc,char const *argv[]){
	srandom(getpid()+time(NULL));
	FILE* f1 =fopen("simulation_file2.data","w");
	FILE* f2 = fopen("lambda.txt", "r");
	if(f2 == NULL)
		return fprintf(stderr, "lambda.txt n'existe pas\n"), -1;
	if(f1 == NULL)
		return fprintf(stderr, "impossible de créer ou ouvrir simulation_file1.data\n"),-1;
	
	while(fscanf(f2, "%lf\n", &lambda) != EOF){
		temps=0;
		cumule=0;
		n=0;
		compteur=0;
		initWt();
		printf("file 2 lambda %lf\n", lambda);
		simulateur(f1);
		fprintf(f1, "%lf %lf %lf\n",lambda, waitmoy(), percentile());
	}
	fclose(f1);
	fclose(f2);
	exit(0);
}
