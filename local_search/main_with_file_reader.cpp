#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <chrono>


//for reading data from file
#include <string>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <exception>

using namespace std;

const int FULL_SCORE_RATIO = 100;

class LocalSearch {
	private:
	int n,m;
	int DemandedEnergies[100];
	int FruitEnergies[1000];
	vector< vector<int> > topAnswer;
	int bestScore;

	public:
	void getInputs() {
		int i;

		//students and fruits count
		cin>>this->n>>this->m;
		// DemandedEnergies.resize(this->n);
		// FruitEnergies.resize(this->m);

		//get students demanded energy
		for(i=0; i<this->n; i++){
			cin>>this->DemandedEnergies[i];
		}

		//get fruits' energy
		for(i=0; i<this->n; i++){
			cin>>this->FruitEnergies[i];
		}
	}

	void getInputsViaTxt() {
		int i;
		string line;
		string temp;
		ifstream inputFile ("inputs.txt");

		//students and fruits count
		getline (inputFile,line);
		stringstream mainStream(line);

		getline(mainStream, temp, ' ');
		this->n = stoi(temp);

		getline(mainStream, temp, ' ');
		this->m = stoi(temp);

		// DemandedEnergies.resize(this->n);
		// FruitEnergies.resize(this->m);

		//get students demanded energy
		// getline (inputFile,line);
		// getline (inputFile,line);
		// stringstream studentStream(line);
		for(i=0; i < this->n; i++){
			getline(mainStream, temp, ' ');
			this->DemandedEnergies[i] = stoi(temp);
		}
		//get fruits' energy
		// getline (inputFile,line);
		// getline (inputFile,line);
		// stringstream fruitStream(line);
		for(i=0; i < this->m; i++){
			getline(mainStream, temp, ' ');
			this->FruitEnergies[i] = stoi(temp);
		}

	    inputFile.close();
	}

	void makeRandomDistribution(vector< vector<int> > &matrix){
		srand((unsigned int)time(NULL));
		int i,j;
		// int personCounter;
		int tempPersonNumber;
		// int tempFruitNumber;
		
		vector<bool> tempUsed;
		vector<int> fruitQue;
		vector<int> givenEnergy;
		vector<int> studentNumberQue;
		matrix.resize(this->n);
		
		srand ( unsigned ( time(0) ) );

		int totalDemanded = 0;
		int totalEnergy = 0;
		for(i=0; i<this->m; i++) {
			tempUsed.push_back(false);
			fruitQue.push_back(i);
			totalEnergy+=FruitEnergies[i];
		}
		for(i=0; i<this->n; i++) {
			givenEnergy.push_back(0);
			studentNumberQue.push_back(i);
			totalDemanded+=DemandedEnergies[i];
		}
		// cout<<"total demanded:"<<totalDemanded<<" total energy:"<<totalEnergy<<endl;

		j=0;
		i=0;
		int fullCounter = 0;
		random_shuffle( fruitQue.begin(), fruitQue.end() );
		random_shuffle( studentNumberQue.begin(), studentNumberQue.end() );
		while(j<this->m && fullCounter != this->n){
			fullCounter = 0;
			for(i=0; i<this->n && j<this->m; i++){
				tempPersonNumber = studentNumberQue[i];

				 if(givenEnergy[tempPersonNumber]>=DemandedEnergies[tempPersonNumber]){
				 	fullCounter++;
				 }else{
				 	matrix[tempPersonNumber].push_back(j);
				 	givenEnergy[tempPersonNumber]+=fruitQue[j];
				 	j++;
				 }
			}
		}
	}

	void fitness(vector< vector<int> > &matrix, int &score){
		int full = 0;
		int wasted = 0;
		int totalWant = 0;
		int totalDemanded = 0;
		int used = 0;
		int i,j;

		for(i=0; i<this->n; i++){
			used = 0;
			for(j=0; j < matrix[i].size(); j++){
				used+=FruitEnergies[matrix[i][j]];
			}
			totalDemanded+=DemandedEnergies[i];
			if(used>=DemandedEnergies[i]){
				full++;
				wasted+= (used - DemandedEnergies[i]);
			}else{
				totalWant = totalWant + (DemandedEnergies[i] - used);
				cout<<"demanded "<<DemandedEnergies[i]<<" and used "<<used<<endl;
			}
			// cout<<used<<endl;
		}

		cout<<"used :"<<used<<" demanded:"<<DemandedEnergies[i]<<endl;
		cout<<"full:"<<full<<" wasted:"<<wasted<<" total Demand:"<<totalDemanded<<" total Want: "<<totalWant<<endl;
		score = FULL_SCORE_RATIO*(this->n - full) + wasted + totalWant;
	}


	void getAnswer(){
   		// auto start = std::chrono::steady_clock::now();
		// this->getInputsViaTxt();
		this->getInputs();
		// clock_t begin = clock();;
		// clock_t end;
		// double elapsed_secs;

		vector< vector<int> > tempDistribution;
		int score;

		this->makeRandomDistribution(tempDistribution);
		this->fitness(tempDistribution, score);
		// end = clock();
		// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		for(int x=0; x<5 /*elapsed_secs<5*/; x++){
			this->successor(tempDistribution, score);
			// end = clock();
			// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    }
		// std::cout << "milliseconds since start: " << elapsed_secs << '\n';
	}

	void successor(vector< vector<int> > &matrix, int &score){
		//shuffle 2 student list
		//change each fruit with each other
			//if result was better go for it
		int i,j;
		int counter;
		int iFruit,jFruit;
		bool iFullFlag,jFullFlag;
		bool found=false;
		int iEnergy, jEnergy;
		int iEnergyTarget, jEnergyTarget;
		int currentDiff, targetDiff;
		int tempFruitContainer;
		int currentWaste, targetWaste;
		int tempScore;

		for (i=0; i<this->n && !found; i++){
			for (j=0; j<this->n && !found; j++){
				//i => student1
				//j => student2
				if(i==j) continue;
				iEnergy=0;
				jEnergy=0;

				for(counter=0; counter<matrix[i].size(); counter++)
					iEnergy+=FruitEnergies[matrix[i][counter]];
				for(counter=0; counter<matrix[j].size(); counter++)
					jEnergy+=FruitEnergies[matrix[j][counter]];
				iFullFlag = (DemandedEnergies[i] <= iEnergy) ? true : false;
				jFullFlag = (DemandedEnergies[j] <= jEnergy) ? true : false;

				for(iFruit=0; !found && iFruit<matrix[i].size(); iFruit++){
					for(jFruit=0; !found && jFruit<matrix[j].size(); jFruit++){
						//check if students are full
						tempScore = score;
						iEnergyTarget = iEnergy - FruitEnergies[matrix[i][iFruit]] +  FruitEnergies[matrix[j][jFruit]];
						jEnergyTarget = jEnergy - FruitEnergies[matrix[j][jFruit]] +  FruitEnergies[matrix[i][iFruit]];

						swap(matrix[i][iFruit], matrix[j][jFruit]);
						this->fitness(matrix, score);
						if(score>tempScore){
							swap(matrix[i][iFruit], matrix[j][jFruit]);
							score = tempScore;
						}

						if(tempScore!=score){
							tempScore = score;
							cout<<"score is "<<tempScore<<endl;
						}
					}
				}
			}
		}
		cout<<"score was => "<<score<<endl;
	}
};

int main() {
	LocalSearch myLocalSearch = LocalSearch();
	myLocalSearch.getAnswer();

	int dummy;
	cin>>dummy;
	return 0;
}
