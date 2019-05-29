#ifndef ECH_H
#define ECH_H

#define mu  1 // Paramètre mu
#define MAXEVENT 1000000 // Nombre maximum d'évènemets
#define EPSILON 1e-4 // Condition d'arrêt de la simulation
#define N 10 // Nombre de serveurs

typedef struct Event{
	int type; // 0 pour une arrivée de client, 1 pour une fin de service
	double date; // date d'arrivée dans la file d'attente ou de fin de service
	int etat; // 0 pour un évènement non traité, 1 s'il est  traité
	int nfile; // numéro de la file. Utile pour les simulations des files 2 et 3.
	int associe; // 0 si une arrivée client n'est pas associé à une fin de service, 1 sinon
	int indiceEch; // indice d'un évènement dans l'échéancier
}event;

typedef struct echeancier{
	event tab[MAXEVENT];
	int taille;
}echeancier;

double Exp(double lamb);
void Ajouter_Ech(event e);
void Init_Ech();
event Extraire();
int condition_arret(long double old,long double new,int tORp);
void ajoutWt();
double waitmoy();
void initWt();
double percentile();

#endif