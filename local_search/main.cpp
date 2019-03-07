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

using namespace std;

const int FULL_SCORE_RATIO = 200;

class LocalSearch {
	private:
	int n,m;
	vector<int> DemandedEnergies;
	vector<int> FruitEnergies;
	vector< vector<int> > topAnswer;
	int bestScore;

	public:
	void getInputs() {
		int i;

		//students and fruits count
		cin>>this->n>>this->m;
		DemandedEnergies.resize(this->n);
		FruitEnergies.resize(this->m);

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

		DemandedEnergies.resize(this->n);
		FruitEnergies.resize(this->m);

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
			getline(mainStream, temp, ' ');
			cout<<temp<<endl;return;
		for(i=0; i < this->m; i++){
			getline(mainStream, temp, ' ');
			cout<<temp<<endl;return;
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

		//this was to check if distribution is correct
		/*
		cout<<"used fruits:"<<j<<endl;
		int size;
		for(int i=0; i<this->n; i++){
			size = matrix[i].size();
			for(j=0; j<size; j++){
				cout<<matrix[i][j]<<" ";
			}
			cout<<endl;
		}
		*/		

		//this was to check how the random distribution worked
		/*
		int full = 0;
		int waste = 0;
		for(i=0; i<this->n; i++) {
			cout<<givenEnergy[i]<<"|"<<DemandedEnergies[i]<<"|"<<givenEnergy[i]-DemandedEnergies[i]<<endl;
			if(givenEnergy[i]>=DemandedEnergies[i]){
				full+=1;
				waste+=(givenEnergy[i]-DemandedEnergies[i]);
			}
		}
		cout<<"fullCount:"<<full<<" | waste:"<<waste<<endl;
		*/
	}

	void fitness(vector< vector<int> > &matrix, int &score){
		int full = 0;
		int wasted = 0;
		int used = 0;
		int i,j;

		for(i=0; i<this->n; i++){
			used = 0;
			for(j=0; j < matrix[i].size(); j++){
				used+=FruitEnergies[matrix[i][j]];
			}
			if(used>DemandedEnergies[i]){
				full++;
				wasted+= (used - DemandedEnergies[i]);
			}
			// cout<<used<<endl;
		}
		//cout<<"full:"<<full<<" wasted:"<<wasted<<endl;
		score = FULL_SCORE_RATIO*(this->n - full) + wasted;
	}


	void getAnswer(){
   		// auto start = std::chrono::steady_clock::now();
		// this->getInputsViaTxt();
		this->getInputs();

		// clock_t begin = clock();
		// clock_t end;
		// double elapsed_secs;

		vector< vector<int> > tempDistribution;
		int score;

		this->makeRandomDistribution(tempDistribution);
		this->fitness(tempDistribution, score);

		// end = clock();
		// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		for(int x=0; x<5; x++){ // && elapsed_secs<2
			this->successor(tempDistribution, score);
			// end = clock();
			// elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    }
		// std::cout << "milliseconds since start: " << elapsed_secs << '\n';
	}

	void oldSuccessor(vector< vector<int> > &matrix, int score){
		vector< vector<int> > neighbor;
		int tempScore = this->n*FULL_SCORE_RATIO;
		int item1, item2;
		int i, j;
		// int changeList[2];
		bool found = false;
		bool loopCondition = true;

		int item1Energy = 0;
		int item2Energy = 0;
		int waste = 0;
		int targetWaste = 0;
		int targetEnergy1,targetEnergy2;
		int targetFullEnergy,targetHungryEnergy;
		int currentDiff,targetDiff;
		int tempFruitContainer;

		/*
		1: current fruit, target fruit
		2: current fruit, target fruit
		*/
		srand ( unsigned ( time(0) ) );
		int tries = 0;
		while(loopCondition==true || tries<100){
			tries++;
			//get 2 students
			item1 = rand() % this->n; 
			item2 = rand() % this->n;
			if(item1==item2) continue;
			//sort one asc and one desc
			// std::sort(matrix[item1].begin(), matrix[item1].end());
			// std::sort(matrix[item2].rbegin(), matrix[item2].rend());

			for(i=0; i<matrix[item1].size(); i++)
				item1Energy+=FruitEnergies[matrix[item1][i]];
			for(i=0; i<matrix[item2].size(); i++)
				item2Energy+=FruitEnergies[matrix[item2][i]];

			cout<<"items to check in "<<tries<<" are "<<item1<<" "<<item2<<endl;
			if(DemandedEnergies[item1]<=item1Energy && DemandedEnergies[item2]<=item2Energy){
				waste = (item1Energy-DemandedEnergies[item1])+(item2Energy-DemandedEnergies[item2]);

				for(i=0; !found && i<matrix[item1].size(); i++){
					for(j=0; !found && j<matrix[item2].size(); j++){
						targetEnergy1 = item1Energy - FruitEnergies[matrix[item1][i]] + FruitEnergies[matrix[item2][j]];
						// cout<<"items energy "<<item1Energy<<" F1 "<<FruitEnergies[matrix[item1][i]]<<" F2 "<<FruitEnergies[matrix[item2][j]];
						targetEnergy2 = item2Energy - FruitEnergies[matrix[item2][j]] + FruitEnergies[matrix[item1][i]];
						targetWaste = (targetEnergy1-DemandedEnergies[item1])+(targetEnergy2-DemandedEnergies[item2]);
						// cout<<"for "<<j<<" in 1 and "<<j<<" in 2 waste was "<<waste<<" and target is "<<targetWaste<<endl;
						if(DemandedEnergies[item1]<=targetEnergy1 && DemandedEnergies[item2]<=targetEnergy2/* && waste>targetWaste*/){
							tempFruitContainer = matrix[item1][i];
							matrix[item1][i] = matrix[item2][j];
							matrix[item2][j] = tempFruitContainer;
							found=true;
							loopCondition=false;
						}
					}
				}
			}else{
				int hungryItem = DemandedEnergies[item1]>item1Energy ? item1 : item2;
				int fullItem = hungryItem==item2 ? item1 : item2;
				int hungryEnergy = hungryItem==item1 ? item1Energy : item2Energy;
				int fullEnergy = fullItem==item1 ? item1Energy : item2Energy;
				for(i=0; !found && i<matrix[fullItem].size(); i++){
					for(j=0; !found && j<matrix[hungryItem].size(); j++){

						targetFullEnergy = fullEnergy - FruitEnergies[matrix[fullItem][i]] + FruitEnergies[matrix[hungryItem][j]];
						targetHungryEnergy = hungryItem - FruitEnergies[matrix[hungryEnergy][j]] + FruitEnergies[matrix[fullEnergy][i]];
						
						targetDiff =(DemandedEnergies[hungryItem]-targetHungryEnergy)*(DemandedEnergies[hungryItem]-targetHungryEnergy) +
						(DemandedEnergies[fullItem]-targetFullEnergy)*(DemandedEnergies[fullItem]-targetFullEnergy);
						
						currentDiff =(DemandedEnergies[hungryItem]-hungryEnergy)*(DemandedEnergies[hungryItem]-hungryEnergy) +
						(DemandedEnergies[fullItem]-fullEnergy)*(DemandedEnergies[fullItem]-fullEnergy);
						cout<<"diff was "<<currentDiff<<" and target is "<<targetDiff<<endl;

						if(targetDiff<currentDiff){
							tempFruitContainer = matrix[hungryItem][j];
							matrix[hungryItem][j] = matrix[fullItem][i];
							matrix[fullItem][i] = tempFruitContainer;

							loopCondition = false;
						}
					}
				}
			}


		}
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
					for(jFruit=0; !found && jFruit<matrix[i].size(); jFruit++){
						//check if students are full
						/*
						currentDiff = abs(DemandedEnergies[i] - iEnergy) + abs(DemandedEnergies[j] - jEnergy);

						iEnergyTarget = iEnergy - FruitEnergies[matrix[i][iFruit]] +  FruitEnergies[matrix[j][jFruit]];
						jEnergyTarget = jEnergy - FruitEnergies[matrix[j][jFruit]] +  FruitEnergies[matrix[i][iFruit]];
						targetDiff = abs(DemandedEnergies[i] - iEnergyTarget) + abs(DemandedEnergies[j] - jEnergyTarget);

						if(targetDiff<currentDiff){
							cout<<"found!"<<endl;
							swap(matrix[i][iFruit], matrix[j][jFruit]);
							score = this->fitness(matrix);
							cout<<"score in "<<"0"<<" is "<<score<<endl;

						}
						*/

						/*
						if(iFullFlag && jFullFlag){
							//if changing has no profit go to next cycle of loop
							currentWaste = (iEnergy - DemandedEnergies[i]) + (jEnergy - DemandedEnergies[j]);
							targetWaste = (iEnergyTarget - DemandedEnergies[i]) + (jEnergyTarget - DemandedEnergies[j]);
							if(iEnergyTarget<DemandedEnergies[i] || jEnergyTarget<DemandedEnergies[j]){continue;}
							if (currentWaste>targetWaste){
								swap(matrix[i][iFruit], matrix[j][jFruit]);
								score = score - currentWaste + targetWaste;
							}
						}else if(iFullFlag || jFullFlag){
							if(iEnergyTarget>=DemandedEnergies[i] && jEnergyTarget>=DemandedEnergies[j]){
								swap(matrix[i][iFruit], matrix[j][jFruit]);
								iFullFlag = true;
								jFullFlag = true;
								targetWaste = (iEnergyTarget - DemandedEnergies[i]) + (jEnergyTarget - DemandedEnergies[j]);
								
								if(iEnergy<DemandedEnergies[i] && iEnergyTarget>=DemandedEnergies[i]) score-=FULL_SCORE_RATIO;
								if(jEnergy<DemandedEnergies[j] && jEnergyTarget>=DemandedEnergies[j]) score-=FULL_SCORE_RATIO;
								//add old hungry waste
								//differ old full waste
								if(iEnergy>=DemandedEnergies[i]){
									score-= (iEnergy - iEnergyTarget);
									score+= (jEnergyTarget - DemandedEnergies[j]);
								}else{
									score-= (jEnergy - jEnergyTarget);
									score+= (iEnergyTarget - DemandedEnergies[i]);
								}
							}else if((iEnergyTarget>=DemandedEnergies[i] && iEnergy>=DemandedEnergies[i]) || 
								(jEnergyTarget>=DemandedEnergies[j] && jEnergy>=DemandedEnergies[j]) ){
								currentDiff = abs(DemandedEnergies[i] - iEnergy) + abs(DemandedEnergies[j] - jEnergy);
								targetDiff = abs(DemandedEnergies[i] - iEnergyTarget) + abs(DemandedEnergies[j] - jEnergyTarget);

								if(currentDiff>targetDiff){
									swap(matrix[i][iFruit], matrix[j][jFruit]);
									this->fitness(matrix, score);
								}
							}
						}else{
							if(iEnergyTarget>DemandedEnergies[i] || jEnergyTarget>DemandedEnergies[j]){
								iFullFlag = iEnergyTarget>DemandedEnergies[i];
								jFullFlag = jEnergyTarget>DemandedEnergies[j];
								swap(matrix[i][iFruit], matrix[j][jFruit]);
								this->fitness(matrix, score);
							}else{
								currentDiff = abs(DemandedEnergies[i] - iEnergy) + abs(DemandedEnergies[j] - jEnergy);
								targetDiff = abs(DemandedEnergies[i] - iEnergyTarget) + abs(DemandedEnergies[j] - jEnergyTarget);

								if(currentDiff>targetDiff){
									swap(matrix[i][iFruit], matrix[j][jFruit]);
									this->fitness(matrix, score);
								}
							}
						}
						*/
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
	}
};

int main() {
	LocalSearch myLocalSearch = LocalSearch();
	myLocalSearch.getAnswer();

	int dummy;
	cin>>dummy;
	return 0;
}
