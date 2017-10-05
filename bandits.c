/*
		bandits manchots : 
		comparer les 3 politiques en parallele : glouton, eps-glouton et UCB... 

	
		
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define K 10

// Structure de données pour une méthode quelconque
typedef struct {
	double mu[K] ;  // mu[i] = estimation de mu(i)
	int N[K] ;		// N[i] = nombre de tirages du bras i
	double gain; // gain accumulé depuis le début
	int param_int ; // à tout hasard
	double param_double; // à tout hasard 
} Data; 

double mu[K]; // moyennes des Xi
	
// Fonction permettant de tirer le bras i : retourne la récompense observée x_i
double tirerbras( int i ) {
	double x = 1.0 * (drand48() <= mu[i]); // bernouilli avec moyenne mu[i]
	return x;
}
	
// Exemple d'une methode bidon qui tire tour à tour chaque bras
int bidon(Data data, int t) {	
	int choix;
	choix = (t + data.param_int) % K;
	return choix; 
}


// Méthode optimale retournant l'indice du meilleur bras
// grâce à des connaissances que les autres méthodes n'ont pas
int optimal(Data data) {
	// TODO...
	
	int i,choix = 0;
	for (i=1;i<K;i++)
		if ( data.mu[i] > data.mu[choix] )
			choix = i;
	return choix;	
}


void miseajour(Data *data, int i, double xt) {
	/*
		TODO: mettre à jour les données après observé la récompense xt en tirant le bras i
		
		data->gain = ... 
		data->mu = ... 
		...
	*/
	data->gain += xt;
	data->mu[i] = (double)(data->mu[i] * data->N[i] + xt )/ (double)(data->N[i]+1.0);
	data->N[i] ++; 
}


void nonAlea(double* m){

	m[0] = 0.2;	
	m[1] = 0.9;
	m[2] = 0.5;
	m[3] = 0.4;
	m[4] = 0.7;
	m[5] = 0.5;
	m[6] = 0.3;
	m[7] = 0.25;
	m[8] = 0.65;
	m[9] = 0.75;

}

void alea(double* m){

  for(int i=0;i<10;i++){
    m[i]=drand48()*(0.8)+0.1;
  }
  m[1] = 0.9;

}

void glouton(){

}

int main(void) {
	int i;
	srand(time(NULL));
	
	Data data_optimal;
	data_optimal.gain = 0;
	
	Data data_bidon;
	data_bidon.gain = 0;
	data_bidon.param_int = 2;



	
	// Initialisation des mu 
	// ... 

	nonAlea(mu);
	
	
	// Début de la simulation
		
	int T = 10000; // temps max 
	int t = 0;
	int choix;
	double xt[K]; 
	double regret_bidon = 0;
	while ( t < T ) {
		// tirage des K bras
		for ( i=0; i< K; i++)
			xt[i] = tirerbras(i);
		
		// Appel de la méthode bidon
		choix = bidon(data_bidon, t); 		
		miseajour(&data_bidon, choix, xt[choix]);
		
		// TODO: autres méthodes + calcul du regret

		int som = 0;

		regret_bidon = regret_bidon + mu[1]-mu[choix];
		
		printf("iter #%d, regret bidon = %lf \n",t,regret_bidon);
		t++;		
	}

	return 0;
}
