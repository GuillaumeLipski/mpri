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
int tour = 0;
	
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

  //return 1;
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

  int i;
  for(i=0;i<10;i++){
    m[i]=drand48()*(0.8)+0.1;
  }
  m[1] = 0.9;

}

int glouton(Data data, int t){
  
  if(tour<100){
    return drand48()*9;
  }
  return 1;
}

int eps(Data data, int t){
  return 1;
}

int ucb(Data data, int tr){
  return 1;
}


int main(void) {
	int i;
	srand(time(NULL));
	
	Data data_optimal;
	data_optimal.gain = 0;
	
	Data data_gouton;
	data_gouton.gain = 0;
	
	Data data_eps;
	data_eps.gain = 0;
	
	Data data_ucb;
	data_ucb.gain = 0;
	
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
	int choixgouton;
	int choixeps;
	int choixucb;
	double xt[K]; 
	
	double regret_bidon = 0;
	double rglouton = 0;
	double resp = 0;
	double rucb = 0;
	
	while ( t < T ) {
		// tirage des K bras
	  tour = t;
	  
		for ( i=0; i< K; i++)
			xt[i] = tirerbras(i);
		
		// Appel de la méthode bidon
		choix = bidon(data_bidon, t); 		
		miseajour(&data_bidon, choix, xt[choix]);
		
		// TODO: autres méthodes + calcul du regret
		
		
		// Appel de la méthode glouton
		choixgouton = glouton(data_gouton, t); 		
		miseajour(&data_gouton, choix, xt[choix]);
		
		
		// Appel de la méthode eps
		choixeps = eps(data_eps, t); 		
		miseajour(&data_eps, choix, xt[choix]);
		
		
		// Appel de la méthode ucb
		choixucb = ucb(data_ucb, t); 		
		miseajour(&data_ucb, choix, xt[choix]);

		int som = 0;

		regret_bidon = regret_bidon + mu[1]-mu[choix];
		rglouton = rglouton + mu[1]-mu[choixgouton];
		
		printf("iter #%d, regret bidon = %lf ,regret glouton = %lf \n",t,regret_bidon,rglouton);
		t++;		
	}

	return 0;
}
