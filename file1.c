#include "echeancier.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <math.h>




extern int lambda;
extern double temps;
extern long int n;
extern int compteur;
extern double cumule;
extern echeancier;
extern double waitTime[MAXEVENT];




void Arrive_Event(event e){
	printf("j'execute AC \n");

	n++;
	event e1;
	e1.type = 0;
	e1.date = e.date + Exp(lambda);
	e1.etat = 0;
	Ajouter_Ech(e1);

	if(n<=N){
		event e2;
		e2.type = 1;
		e2.date=e.date+Exp(mu);
		e2.etat=0;
		Ajouter_Ech(e2);
	}

	temps = e.date;
}

void Service_Event(event e){
	printf("j'execute FS\n" );
	if(n>0){
		n--;
		if(n>=N){
			event e1;
			e1.type = 1;
			e1.date = e.date + Exp(mu);
			e1.etat = 0;
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

	while(condition_arret(oldmoyen,nmoyen)==0){
		e =Extraire();
		cumule += (e.date-temps)*n;
		printf("mu%d\n",mu);
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
	lambda=90;
	FILE* f1 =fopen("Simulation_MM1.data","w");
	 srandom(getpid()+time(NULL));
	simulateur(f1);
	fclose(f1);
	printf(" average wt %lf\n",waitmoy());
	// for (int i = 0; i < 10; ++i)
	// {
	// 	printf("%lf\n", waitTime[i]);
	// }
	exit(0);



}