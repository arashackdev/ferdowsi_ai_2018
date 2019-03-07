#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
//#include <chrono>

using namespace std;

const int FULL_SCORE_RATIO = 100;
const int SIMULATED_ANNEALING_STEP_TO_DECREASE = 3;
const bool SIMULATED_ANNEALING = true;
const int POPULATIONS_COUNT = 10;

//global array
int scores[POPULATIONS_COUNT];


class LocalSearch {
	protected:
		int n,m;
		int DemandedEnergies[100];
		int FruitEnergies[1000];
		//vector< vector<int> > topAnswer;
		//int bestScore;
		int totalDemand;
		int totalEnergy;
		int tempreture;

	public:
		void getInputs(){
			int i;

			//students and fruits count
			cin>>this->n>>this->m;
			// DemandedEnergies.resize(this->n);
			// FruitEnergies.resize(this->m);

			this->totalDemand = 0;
			this->totalEnergy = 0;

			//get students demanded energy
			for(i=0; i<this->n; i++){
				cin>>this->DemandedEnergies[i];
				this->totalDemand+=this->DemandedEnergies[i];
			}

			//get fruits' energy
			for(i=0; i<this->m; i++){
				cin>>this->FruitEnergies[i];
				this->totalEnergy+=this->FruitEnergies[i];
			}
		}

		virtual void makeRandomDistribution(vector< vector<int> > &matrix){
			srand((unsigned int)time(NULL));
			int i,j;
			// int personCounter;
			int tempPersonNumber;
			// int tempFruitNumber;
		
			vector<bool> tempUsed;//real num
			vector<int> fruitQue;//shuffled num
			vector<int> givenEnergy;//real num
			vector<int> studentNumberQue;//shuffled num
			matrix.resize(this->n);
			fruitQue.resize(this->m);
			tempUsed.resize(this->m);
			givenEnergy.resize(this->n);
			studentNumberQue.resize(this->n);

			srand ( unsigned ( time(0) ) );

			for(i=0; i<this->m; i++) {
				tempUsed[i] = false;
				fruitQue[i] = i;
			}
			for(i=0; i<this->n; i++) {
				givenEnergy[i] = 0;
				studentNumberQue[i] = i;
			}

			//random_shuffle( fruitQue.begin(), fruitQue.end() );
			//random_shuffle( studentNumberQue.begin(), studentNumberQue.end() );

			//for(i=0; i<this->n; i++){
			//	//cout<<fruitQue[i]<<" ";
			//	cout<<studentNumberQue[i]<<" ";
			//}
			//cout<<endl;

			j=0;
			i=0;
			int fullCounter = 0;
			//cout<<this->m<<endl;
			while(j<this->m && fullCounter <= this->n){
				for(i=0; i<this->n && j<this->m; i++){
					tempPersonNumber = studentNumberQue[i];

					 //if(givenEnergy[tempPersonNumber]<DemandedEnergies[tempPersonNumber]){
				 		matrix[tempPersonNumber].push_back(fruitQue[j]);
						givenEnergy[tempPersonNumber]+=FruitEnergies[fruitQue[j]];
						//cout<<"assigned "<<fruitQue[j]<<" with "<<FruitEnergies[fruitQue[j]]<<" to "<<tempPersonNumber<<endl;
				 		j++;
						if(givenEnergy[tempPersonNumber]>=DemandedEnergies[tempPersonNumber]){
							fullCounter++;
						}else if(this->totalDemand > this->totalEnergy){
							i--;
						}
					 //}
				}
			}
			//cout<<"full counts are:"<<fullCounter<<endl;
			//cout<<"i/n:"<<i<<this->n<<endl;
			//cout<<"j/m:"<<j<<this->m<<endl;
		}

		virtual void fitness(vector< vector<int> > &matrix, int &score){
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
					//cout<<"demanded "<<DemandedEnergies[i]<<" and used "<<used<<endl;
				}
				// cout<<used<<endl;
			}

			//cout<<"full:"<<full<<" wasted:"<<wasted<<" total Demand:"<<totalDemanded<<" total Want: "<<totalWant<<endl;
			score = FULL_SCORE_RATIO*(this->n - full) + wasted;
		}

		virtual void successor(vector< vector<int> > &matrix, int &score){
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
			float jumpProbability;
			int simCounter = 0;
			for (i=0; i<this->n && this->tempreture>2; i++){
				for (j=0; j<this->n && this->tempreture>2; j++){
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

					for(iFruit=0; iFruit<matrix[i].size() && this->tempreture>2; iFruit++){
						for(jFruit=0; jFruit<matrix[j].size() && this->tempreture>2; jFruit++){
							//check if students are full
							
							/*
							currentDiff = 0;
							targetDiff = 0;
							if(DemandedEnergies[i]>iEnergy) currentDiff+= DemandedEnergies[i]-iEnergy;
							if(DemandedEnergies[j]>jEnergy) currentDiff+= DemandedEnergies[j]-jEnergy;

							if(DemandedEnergies[i]>iEnergyTarget) currentDiff+= DemandedEnergies[i]-iEnergyTarget;
							if(DemandedEnergies[j]>jEnergyTarget) currentDiff+= DemandedEnergies[j]-jEnergyTarget;
							*/

							//pass fruit
							if(iFullFlag != jFullFlag){//xor full flags
								if(iFullFlag && ((iEnergy-FruitEnergies[matrix[i][iFruit]])>=DemandedEnergies[i]) ){
									if(iFruit+1 == matrix[i].size()) continue;
									//pass fruit to j
									iEnergy = iEnergy-FruitEnergies[matrix[i][iFruit]];
									jEnergy = jEnergy+FruitEnergies[matrix[i][iFruit]];
									iFullFlag = (DemandedEnergies[i] <= iEnergy) ? true : false;
									jFullFlag = (DemandedEnergies[j] <= jEnergy) ? true : false;
									
									matrix[j].push_back(matrix[i][iFruit]);
									//cout<<"in :" << iFruit<<" from "<<matrix[i].size()<< endl;
									if(iFruit+1 == matrix[i].size()){
										matrix[i].pop_back();
									}else{
										matrix[i].erase(matrix[i].begin() + iFruit);
									}
									//cout<<"changed "<<i<<" and "<<j<<endl;
									this->fitness(matrix, score);
									continue;
								}
							}

							tempScore = score;
							iEnergyTarget = iEnergy - FruitEnergies[matrix[i][iFruit]] +  FruitEnergies[matrix[j][jFruit]];
							jEnergyTarget = jEnergy - FruitEnergies[matrix[j][jFruit]] +  FruitEnergies[matrix[i][iFruit]];

							//change fruit
							if(iEnergyTarget>=DemandedEnergies[i]){
								if(iEnergy>=DemandedEnergies[i]){
									tempScore = tempScore - (DemandedEnergies[i]-iEnergy) + (DemandedEnergies[i]-iEnergyTarget);//- oldWaste + newWaste
								}else{
									tempScore = tempScore + (DemandedEnergies[i]-iEnergyTarget) - FULL_SCORE_RATIO;//+ newWaste - FULL_RATIO
								}
							}else{
								if(iEnergy>=DemandedEnergies[i]){
									tempScore = tempScore - (DemandedEnergies[i]-iEnergy) + FULL_SCORE_RATIO;//- oldWaste + FULL_RATIO
								}
							}
							if(jEnergyTarget>=DemandedEnergies[j]){
								if(jEnergy>=DemandedEnergies[j]){
									tempScore = tempScore - (DemandedEnergies[j]-jEnergy) + (DemandedEnergies[j]-jEnergyTarget);//- oldWaste + newWaste
								}else{
									tempScore = tempScore + (DemandedEnergies[j]-jEnergyTarget) - FULL_SCORE_RATIO;//+ newWaste - FULL_RATIO
								}
							}else{
								if(jEnergy>=DemandedEnergies[j]){
									tempScore = tempScore - (DemandedEnergies[j]-jEnergy) + FULL_SCORE_RATIO;//- oldWaste + FULL_RATIO
								}
							}

							if(tempScore<score){
								score = tempScore;
								iEnergy = iEnergyTarget;
								jEnergy = jEnergyTarget;
								iFullFlag = (DemandedEnergies[i] <= iEnergy) ? true : false;
								jFullFlag = (DemandedEnergies[j] <= jEnergy) ? true : false;

								swap(matrix[i][iFruit], matrix[j][jFruit]);
								//cout<<"score is "<<tempScore<<endl;
							}else if(SIMULATED_ANNEALING){
								jumpProbability = pow( 2.71, ((score-tempScore)/(this->tempreture)) );

								if((rand()%100) < jumpProbability){
									simCounter++;
									if(simCounter%SIMULATED_ANNEALING_STEP_TO_DECREASE==0){
										this->tempreture--;
										//cout<<"tempreture is "<<this->tempreture<<endl;
									}

									score = tempScore;
									iEnergy = iEnergyTarget;
									jEnergy = jEnergyTarget;
									iFullFlag = (DemandedEnergies[i] <= iEnergy) ? true : false;
									jFullFlag = (DemandedEnergies[j] <= jEnergy) ? true : false;

									swap(matrix[i][iFruit], matrix[j][jFruit]);

								}

							}
							/*
							swap(matrix[i][iFruit], matrix[j][jFruit]);
							this->fitness(matrix, score);

							if(tempScore>score){
								tempScore = score;
								iEnergy = iEnergyTarget;
								jEnergy = jEnergyTarget;

								//cout<<"score is "<<tempScore<<endl;
							}else{
								swap(matrix[i][iFruit], matrix[j][jFruit]);
								score = tempScore;
							}
							*/
						}
					}
					//cout<<"people: "<<i<<" "<<j<<"score was => "<<score<<endl;
				}
			}
			//cout<<"score was => "<<score<<endl;
		}

		virtual void getAnswer(){
			this->getInputs();
			this->tempreture = 100;

			vector< vector<int> > tempDistribution;
			int score;

			this->makeRandomDistribution(tempDistribution);
			this->fitness(tempDistribution, score);
			//cout<<score<<endl;

			for(int x=0; x<10; x++){
				this->successor(tempDistribution, score);
			}
			this->printDistribution(tempDistribution);

		}

		void printDistribution(vector< vector<int> > &matrix){
			int wasted = 0;
			int full = 0;
			int used;
			int i,j;
			int total;

			for(i=0; i<this->n; i++){
				used = 0;
				for(j=0; j < matrix[i].size(); j++){
					used+=FruitEnergies[matrix[i][j]];
				}

				if(used>=DemandedEnergies[i]){
					full++;
					wasted+= (used - DemandedEnergies[i]);
				}
			}
			cout<<full<<" "<<wasted<<endl;

			for(i=0; i<this->n; i++){
				total = 0;
				cout<<matrix[i].size()<<" ";
				for(j=0; j < matrix[i].size(); j++){
					cout<<matrix[i][j]+1<<" ";//<<"("<<FruitEnergies[matrix[i][j]]<<")"<<" ";
					//total+=FruitEnergies[matrix[i][j]];
				}
				//cout<<":"<<total<<"/"<<DemandedEnergies[i];
				cout<<endl;
			}
		}
};

int randomSeed (int i) { return rand()%i;}

bool scoreSortOperator(int i, int j) { return scores[i]>scores[j]; }

class GeneticAlgorithm: public LocalSearch {
	protected:
		vector<int> FruitAssignments[POPULATIONS_COUNT];
		vector< vector<int> > FruitDistributions[POPULATIONS_COUNT];
		int p_total;

	public:

		void makeRandomDistributions(){
			srand((unsigned int)time(NULL));

			int i;
			vector<int> studentNumberQue;
			for(i=0; i<this->n; i++){
				studentNumberQue.push_back(i);
			}
			for(i=0; i<POPULATIONS_COUNT; i++){
				std::random_shuffle( studentNumberQue.begin(), studentNumberQue.end(), randomSeed);
				this->makeRandomDistribution(FruitDistributions[i], FruitAssignments[i], studentNumberQue);
				this->fitness(FruitAssignments[i], scores[i]);
			}
		}

		void makeRandomDistribution(vector< vector<int> > &FruitDistribution, vector<int> &FruitAssignment, vector<int> &studentNumberQue) {
			int i,j;
			// int personCounter;
			int tempPersonNumber;
			// int tempFruitNumber;
		
			vector<bool> tempUsed;//real num
			vector<int> fruitQue;//shuffled num
			vector<int> givenEnergy;//real num
			//vector<int> studentNumberQue;//shuffled num
			FruitDistribution.resize(this->n);
			fruitQue.resize(this->m);
			tempUsed.resize(this->m);
			givenEnergy.resize(this->n);
			FruitAssignment.resize(this->m);
			//studentNumberQue.resize(this->n);

			srand ( unsigned ( time(0) ) );

			for(i=0; i<this->m; i++) {
				tempUsed[i] = false;
				fruitQue[i] = i;
			}
			for(i=0; i<this->n; i++) {
				givenEnergy[i] = 0;
				//studentNumberQue[i] = i;
			}

			//std::random_shuffle( fruitQue.begin(), fruitQue.end() );
			
			j=0;
			i=0;
			int fullCounter = 0;
			while(j<this->m && fullCounter <= this->n){
				for(i=0; i<this->n && j<this->m; i++){
					tempPersonNumber = studentNumberQue[i];

				 		FruitDistribution[tempPersonNumber].push_back(fruitQue[j]);
						givenEnergy[tempPersonNumber]+=FruitEnergies[fruitQue[j]];
						FruitAssignment[j] = tempPersonNumber;
				 		j++;
						if(givenEnergy[tempPersonNumber]>=DemandedEnergies[tempPersonNumber]){
							fullCounter++;
						}else if(this->totalDemand > this->totalEnergy){
							i--;
						}
				}
			}
		}

		void fitness(vector<int> &FruitAssignments, int &score){
			vector<int> givenEnergy;
			int i;
			int full = 0;
			int wasted = 0;

			for(int i=0; i<this->n; i++) givenEnergy.push_back(0);

			for(i=0; i<this->m; i++){
				givenEnergy[ FruitAssignments[i] ] += FruitEnergies[i];
			}

			for(i=0; i<this->n; i++){
				if(givenEnergy[i] >= DemandedEnergies[i]){
					full++;
					wasted+= (givenEnergy[i] - DemandedEnergies[i]);
				}
			}
			
			score = FULL_SCORE_RATIO*(this->n - full) + wasted;
		}

		void successor(int breakPoint){
			srand((unsigned int)time(NULL));

			vector<int> ranking;
			int propbabilities[POPULATIONS_COUNT];
			int i;
			int tempKeeper = 0;
			int totalProbs = 0;

			for(i=0; i<POPULATIONS_COUNT; i++) ranking.push_back(i);

			sort(ranking.begin(), ranking.end(), scoreSortOperator);
			for(i=0; i<POPULATIONS_COUNT; i++){
				propbabilities[ranking[i]] = ((POPULATIONS_COUNT - i)*100)/ (p_total);
				totalProbs+=propbabilities[ranking[i]];
			}

			for(i=0; i<POPULATIONS_COUNT; i++){
				propbabilities[i] = propbabilities[i]+tempKeeper;
				tempKeeper = propbabilities[i];
			}
			
			int parent1, parent2, child;
			bool nodeFound = false;
			parent1 = rand()%totalProbs;
			for (i = 0; i < POPULATIONS_COUNT; i++)
			{
				if(parent1 < propbabilities[i]){
					parent1 = i;
					break;
				}
			}
			while(nodeFound == false){
				parent2 = rand()%totalProbs;
				for (i = 0; i < POPULATIONS_COUNT; i++)
				{
					if(parent2 < propbabilities[i]){
						parent2 = i;
						break;
					}
				}
				if(parent2 != parent1){
					nodeFound=true;
				}
			}
			nodeFound = false;
			while(nodeFound == false){
				child = rand()%totalProbs;
				for (i = 0; i < POPULATIONS_COUNT; i++)
				{
					if(child < propbabilities[i]){
						child = i;
						break;
					}
				}
				if(child != parent1 && child!=parent2){
					nodeFound=true;
				}
			}

			this->crossOver(parent1, parent2, child, breakPoint);
		}

		void crossOver(int parent1, int parent2, int child, int breakPoint){
			srand((unsigned int)time(NULL));
			int i;

			for(i=0; i<breakPoint-1; i++){
				FruitAssignments[child][i] = FruitAssignments[parent1][i];
			}
			if(rand()%2 == 0){
				//mutate
				FruitAssignments[child][breakPoint] = rand()%this->n;
				
				breakPoint++;
			}
			//cout<<" parent1 is "<<parent1<<" parent2  is "<<parent2<<" child is "<<child<<endl;
			for(i=breakPoint; i<this->m; i++){
				FruitAssignments[child][i] = FruitAssignments[parent2][i];
			}

			this->fitness(FruitAssignments[child], scores[child]);
			this->convertAssignmentToDistribution(FruitAssignments[child], FruitDistributions[child]);
			///cout<<"child score is "<<scores[child]<<endl;
		}

		void convertAssignmentToDistribution(vector<int> &FruitAssignment, vector< vector<int> > &FruitDistribution){
			int i;
			for(i=0; i<this->n; i++){
				FruitDistribution[i].clear();
			}
			for(i=0; i<this->m; i++){
				FruitDistribution[FruitAssignment[i]].push_back(i);
			}
		}

		void getAnswer(){
			int i;
			int breakPoint;
			this->getInputs();
			this->makeRandomDistributions();
			p_total = (POPULATIONS_COUNT + POPULATIONS_COUNT*POPULATIONS_COUNT)/2;

			for(int x=0; x<3000; x++){
				breakPoint = (rand()%(this->m - this->m/5) + this->m/5);
				this->successor(breakPoint);
			}
			this->printBestDistribution();
		}


		void printBestDistribution(){
			int wasted = 0;
			int full = 0;
			int used;
			int i,j;
			int total;

			int bestScore = this->m * 2 * FULL_SCORE_RATIO;
			int bestPop;
			for(i=0; i<POPULATIONS_COUNT; i++){
				if(scores[i]<bestScore){
					bestPop = i;
					bestScore = scores[i];
				}
			}
			//cout<<"best score "<<bestScore<<endl;
			for(i=0; i<this->n; i++){
				used = 0;
				for(j=0; j < FruitDistributions[bestPop][i].size(); j++){
					used+=FruitEnergies[FruitDistributions[bestPop][i][j]];
				}

				if(used>=DemandedEnergies[i]){
					full++;
					wasted+= (used - DemandedEnergies[i]);
				}
			}
			cout<<full<<" "<<wasted<<endl;

			for(i=0; i<this->n; i++){
				total = 0;
				cout<<FruitDistributions[bestPop][i].size()<<" ";
				for(j=0; j < FruitDistributions[bestPop][i].size(); j++){
					cout<<FruitDistributions[bestPop][i][j]+1<<" ";//<<"("<<FruitEnergies[matrix[i][j]]<<")"<<" ";
					//total+=FruitEnergies[matrix[i][j]];
				}
				//cout<<":"<<total<<"/"<<DemandedEnergies[i];
				cout<<endl;
			}

		}

};

int main(){
	//LocalSearch myLocalSearch = LocalSearch();
	GeneticAlgorithm myLocalSearch = GeneticAlgorithm();
	myLocalSearch.getAnswer();

	int dummy;
	cin>>dummy;
}