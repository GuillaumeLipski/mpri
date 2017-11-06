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
	double G[K] ;
	int param_int ; // à tout hasard
	double param_double; // à tout hasard 
} Data; 

double mu[K]; // moyennes des Xi
int tour = 0;

// Fonction permettant de tirer le bras i : retourne la récompense observée x_i
double tirerbras( int i ) { //drand48
	//srand(time(NULL));
	double x = 1.0 * (drand48() <= mu[i]); // bernouilli avec moyenne mu[i]
//	printf("%f\n",x);
	return x;
}


void affdouble(double  t[], int taille){
	int i = 0 ;
	for(i = 0 ; i < taille;i++){
		printf("%lf \t",t[i]);
	}
	printf("\n");
}

void affint(int  t[], int taille){
	int i = 0 ;
	for(i = 0 ; i < taille;i++){
		printf("%d \t",t[i]);
	}
	printf("\n");
}

void zero(int  t[], int taille){
	int i = 0 ;
	for(i = 0 ; i < taille;i++){
		t[i]=0;
	}
	//printf("\n");
}

void zerod(double  t[], int taille){
	int i = 0 ;
	for(i = 0 ; i < taille;i++){
		t[i]=0.0;
	}
	//printf("\n");
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
	// ...
	
	int i,choix = 0;
	for (i=1;i<K;i++)
		if ( data.mu[i] > data.mu[choix] )
			choix = i;
		
	return choix;	

  //return 1;
}


void miseajour(Data *data, int i, double xt) {
	/*
		: mettre à jour les données après observé la récompense xt en tirant le bras i
		
		data->gain = ... 
		data->mu = ... 
		...
	*/
	data->gain += xt;
	data->mu[i] = (double)(data->mu[i] * data->N[i] + xt )/ (double)(data->N[i]+1.0);
	data->N[i] ++;
	data->G[i] = data->G[i] + xt; 
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
	double a =(double)drand48()*(0.8)+0.1;
	m[i] = a;
  }
  m[1] = 0.9;
  m[2] = 0.8;

}

int glouton(Data data, int t){
  
  if(t<100){
	int z = 0;
  	z = drand48()*9;
    return z;
  }
  
  double mu2[10];
  zerod(mu2,10);
  int i = 0;
  for(i =0; i <10;i++){
	mu2[i]=  (1.0/(data.N[i]+1.0))	* data.G[i];
  }
  
  int res = 0;
  double val = mu2[0];
  
    for(i =0; i <10;i++){
    	if(mu2[i]>=val){
    		val = mu2[i];
    		res = i;
		}
  }
  
  //printf("%d",res);
  return res;
}

int eps(Data data, int t){
	
	double eps = 0.1;
	
	double val = drand48();
	
	//printf("%lf %lf\n",val,1-eps);
	
	if(val<1-eps){
	  //printf("glouton \n");
		return glouton(data,t);
	}else{
	  int tmp = drand48()*9;
	  //printf("eps %d\n",tmp);
		return (int)tmp;
	}
	
}

double conf(Data data, int tr, int bras ){
	
	double haut = (3.0*log(tr));
	double bas = (2.0*(data.N[bras]+1.0));

	
	double x = (haut/bas);
	x = sqrt(x);
	
	//double truc = sqrt((2.0*log(tr))/data.N[bras]);
	
	return x;
}

int ucb(Data data, int tr){
  double mu2[10];
  zerod(mu2,10);
  int i = 0;
  for(i =0; i <10;i++){	
	mu2[i]=  (1.0/(data.N[i]+1.0))	* data.G[i];
  }	
  int j = 0;
  for(j = 0 ; j <10 ; j++){
  	mu2[j]= mu2[j]+conf(data,tr,j);	
  }
  int res = 0;
  double val = mu2[0];
  int z = 0 ;
    for(z =0; z <10;z++){
    	if(mu2[z]>=val){
    		val = mu2[z];
    		res = z;
		}
  }
	
  return res;
}


int main(void) {
	
	srand48(time(NULL));
	
	FILE* fichG = NULL;
	
	fichG = fopen("gain.dat","w+");
	
	FILE* fichR = NULL;
	
	fichR = fopen("regret.dat","w+");
	
	int test = 0;
	
	int glou = 0;
	int bid = 0;
	int tourr = 0;
	
	
	int i;
	
	Data data_optimal;
	data_optimal.gain = 0;
	
	Data data_glouton;
	data_glouton.gain = 0;
	zero(data_glouton.N,10);
	
	
	Data data_eps;
	data_eps.gain = 0;
	zero(data_eps.N,10);
	
	Data data_ucb;
	data_ucb.gain = 0;
	zero(data_ucb.N,10);
	
	Data data_bidon;
	data_bidon.gain = 0;
	data_bidon.param_int = 2;



	
	// Initialisation des mu 
	// ... 

	alea(mu);
	//affdouble(mu,10);
	
	
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
			
		//printf("%d\n",t);	
		//afftab(xt,10);

		  tour = t;
	  
		for ( i=0; i< K; i++)
			xt[i] = tirerbras(i);
		
		// Appel de la méthode bidon
		choix = optimal(data_bidon); 		
		miseajour(&data_bidon, choix, xt[choix]);
		
		
		// Appel de la méthode glouton
		choixgouton = glouton(data_glouton, t); 		
		miseajour(&data_glouton, choixgouton, xt[choixgouton]);
		
		
		// Appel de la méthode eps
		choixeps = eps(data_eps, t); 
		//printf("%d\n",choixeps);		
		miseajour(&data_eps, choixeps, xt[choixeps]);
		
		
		// Appel de la méthode ucb
		choixucb = ucb(data_ucb, t); 		
		miseajour(&data_ucb, choixucb, xt[choixucb]);

		//calcul des regrets
		
		regret_bidon		= regret_bidon		+ mu[1]-(mu[choix]);
		rglouton 		= rglouton 		+ mu[1]-(mu[choixgouton]);
		resp 			= resp 			+ mu[1]-(mu[choixeps]);
		rucb 			= rucb 			+ mu[1]-(mu[choixucb]);
		
		//printf("iter #%d, regret bidon = %lf ,regret glouton = %lf \n",t,regret_bidon,rglouton);
		//printf("iter #%d, gain bidon = %lf ,gain glouton = %lf \n",t,data_bidon.gain,data_glouton.gain);
		t++;		

	
		// ecriture dans les fichiers
		fprintf(fichG,"%lf %lf %lf\n",data_glouton.gain,data_eps.gain,data_ucb.gain);
		fprintf(fichR,"%lf %lf %lf\n",rglouton,resp,rucb);
		
	}
		

	fclose(fichG);
	fclose(fichR);
	return 0;
}
