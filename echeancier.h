#ifndef ECH_H
#define ECH_H
#define lambda 7
#define mu  10
#define MAXEVENT 1000000
#define MAXTEMPS 5000
#define EPSILON 1e-6

typedef struct Event{
	int type; //0 pour arriver, 1 pour service
	double date;
	int etat; // 0 pour non traité, 1 pour traité
}event;

typedef struct echeancier{
	event tab[MAXEVENT];
	int taille;
}echeancier;

double Exp(int lamb);
void Ajouter_Ech(event e);
void Init_Ech();
void Afficher_echeancier();
event Extraire();
int condition_arret(long double old,long double new);

