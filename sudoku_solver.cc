/**
 * EVO projekt 2016
 * proj19 - Evolucni reseni sudoku
 *
 * Heczkova Petra, xheczk04
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <ga/ga.h>

using namespace std;

//---------------------- Parametry ----------------------------//
int dim;
const int numb1 = 30;
const int numb2 = 3;
int numbs[numb1][numb2];
float pun = 50;

//--------------------- Fitness funkce ------------------------//
float FitnessFce(GAGenome& g){

	GA1DArrayGenome<int> &genome = (GA1DArrayGenome<int> &)g;

	int i=0,j=0,k=0,m=0;

	for(i=0;i<numb1;i++) if(numbs[i][0]==-1) break;
	int err=i*pun;

	for (i=0;i<dim*dim;i++){
		for (j=0;j<dim;j++){
			if(genome.gene(i)==j){ // mam gen
				// stejny sloupec
				for(k=1;k<dim;k++)	
					if(genome.gene((i+dim*k)%(dim*dim))==j) err++;
				// stejny radek
				m=i%dim;
				for(k=1;k<=m;k++)
					if(genome.gene(i-k)==j) err++;
				m=dim-1-m;
				for(k=1;k<=m;k++)
					if(genome.gene(i+k)==j) err++;
				// kontrola cisel zadanych uzivatelem
				for(k=0;k<numb1;k++){
					if(j==numbs[k][0] && i==numbs[k][1]+numbs[k][2]*dim) err-=pun;
				}
			}
		}
	}
 
   
return err;
  
}

//-------------------- Inicializacni fce ------------------------//
void InitializerFce(GAGenome& g) {

	GA1DArrayGenome<int> &genome = (GA1DArrayGenome<int> &) g;

	for (int i=0;i<dim*dim;i++) genome.gene(i,(i%dim));  

	for (int i=0;i<dim;i++) genome.mutator();

}      

//------------------------ Print fce -----------------------------//
void PrintPhenotype(const GAGenome &g) {

	GA1DArrayGenome<int> &genome = (GA1DArrayGenome<int> &)g;
	int i, j, x;

	for (i = 0; i < dim*dim; i++) {

		if(i%dim == 0){
			cout << "|" << endl;
			for (x = 0; x < (dim * 4) + 1; x++)
				cout << "-";
			cout << endl;
		}

		cout << "|";
		cout << " " << genome.gene(i)+1 << " ";
	}

	cout << "|";
	cout << endl;
	for (x = 0; x < (dim * 4) + 1; x++)
		cout << "-";
	cout << endl;

}

//----------------------------------------------------------------//
// vstup:	./jmeno -d=9 -2=10,3
int main(int argc, char **argv) {

	// zpracovani argumentu
	string s; int p1, p2;
	// zpracovani zadanych cisel a pozic
	::dim = atoi(string(argv[1]).substr(3).c_str());
	for(int i=0; i<numb1; i++){
		if(i+2 < argc){
			// pomocne promenne
			s = string(argv[i+2]);
			p1 = s.find("=");p2 = s.find(",");
			// zjisteni cisla a kontrola
			::numbs[i][0] = atoi(s.substr(1,p1-1).c_str())-1;
			if(numbs[i][0] >= dim*dim){
				return 2;
			}
			// zjisteni pozic a kontrola
			::numbs[i][1] = atoi(s.substr(p1+1,p2-p1-1).c_str())-1;
			::numbs[i][2] = atoi(s.substr(p2+1).c_str())-1;
			if(numbs[i][1] >= dim || numbs[i][2] >= dim){
				return 2;
			}
		}
		else {
			::numbs[i][0] = -1;::numbs[i][1] = -1;::numbs[i][2] = -1;
		}
	}
                     
	// nastaveni genetickeho
	GA1DArrayGenome<int> genome(dim*dim, FitnessFce);
      
	genome.initializer(::InitializerFce);
	genome.mutator(GA1DArrayGenome<int>::SwapMutator);
	genome.crossover(GA1DArrayGenome<int>::PartialMatchCrossover);
  
	GASimpleGA ga(genome);
	ga.minimize();
   
	// parametry
	ga.parameters(argc,argv);

	ga.populationSize(200);
	ga.nGenerations(5000);
	ga.pMutation(0.6);
	ga.pCrossover(0.7);
   
	// evoluce
	ga.initialize();
	while(!ga.done()){
		++ga; 
  
		if ((ga.statistics().generation() % ga.scoreFrequency()) == 0)
			//cout <<"Generation " << ga.statistics().generation() << " - Best fitness: " << ga.population().min() <<endl;
            
		if (ga.population().min() == 0) break;
	}
	ga.flushScores();
    
  
	//cout <<endl << "Final statistics:" << endl;
	//cout << ga.statistics() << endl;
   
	cout << "The best solution: " <<  ga.statistics().bestIndividual() <<" , Fitness = " << ga.population().min() << endl;
	PrintPhenotype(ga.statistics().bestIndividual());

cerr<<ga.population().min()<<endl;

	return 0;
}

