#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <chrono>

using namespace std;

class CSP {
private:
    int n, m, constraintCount;
    int DemandedEnergies[100];
    int FruitEnergies[1000];
    vector<int> fullStatus;
    vector<vector<int>> usedFruits;
    vector<vector<int>> constraints;

public:
    void getInputs() {
        int i;
        vector<int> temp;

        //students and fruits count
        cin >> this->n >> this->m;

        //get students demanded energy
        for (i = 0; i < this->n; i++) {
            cin >> this->DemandedEnergies[i];
            this->usedFruits.push_back(temp);
            this->constraints.push_back(temp);
            this->fullStatus.push_back(0);
        }

        //get fruits' energy
        for (i = 0; i < this->m; i++) {
            cin >> this->FruitEnergies[i];
        }

        //get constraint count
        cin >> this->constraintCount;
        //cout<<"getting constraints!"<<this->constraintCount<<endl;
        //get constraints
        int a, b;
        for (i = 0; i < this->constraintCount; i++) {
            cin >> a >> b;
            a--;
            b--;
            this->constraints[a].push_back(b);
            this->constraints[b].push_back(a);
            //cout<<"constraint between "<<a<<" and "<<b<<endl;
        }
        //cout<<"inputs finished!"<<endl;
    }

    void printResults(vector<vector<int>> usedFruits) {
        //iterate over students
        int i, j;
        for (i = 0; i < this->n; i++) {
            cout << usedFruits[i].size()<<" ";
            for (j = 0; j < usedFruits[i].size(); j++) {
//                cout << usedFruits[i][j] + 1<<" ";
                cout << this->FruitEnergies[usedFruits[i][j]]<<" ";
            }
            cout<<endl;
        }
//        cout<<"*************************"<<endl;
    }
    bool printDomains(vector<vector<int>> domains, vector<int> fullStatus){
        int i, j;
        int fullCheck = 0;
        for (i = 0; i < this->n; i++) {
//            cout << "student " << i << ":";
//            for (j = 0; j < domains[i].size(); j++) {
//                cout << domains[i][j] << " ";
//            }
            if(fullStatus[i]==1){
                fullCheck++;
            }
//            cout << "|" << fullStatus[i]<<endl;
        }
        if(fullCheck==this->n){
            return true;
        }
        return false;
    }
    bool backtrackSearch(
            vector<vector<int>> domains,
            vector<vector<int>> usedFruits,
            vector<int> fullStatus,
            int selectedStudent = -1,
            int selectedFruit = -1
    ) {
        int i, j, k;
        int tempConstrainCount = 0;
//        cout << "initial student " << selectedStudent << " and initial fruit " << selectedFruit << endl;
        //update values for selected student and fruit
        //forward checking
        if (selectedStudent != -1) {
            int usedFruitValue = 0;
            //add to used fruits
            usedFruits[selectedStudent].push_back(selectedFruit);
            //update full status
            for (i = 0; i < usedFruits[selectedStudent].size(); i++) {
                usedFruitValue += this->FruitEnergies[usedFruits[selectedStudent][i]];
            }
            if (usedFruitValue >= this->DemandedEnergies[selectedStudent]) {
                fullStatus[selectedStudent] = 1;
            }

            //remove this fruit from everybody's domain
            for (j = 0; j < this->n; j++) {
                domains[j][selectedFruit] = -1;
            }

            //update constrain domains
            for (j = 0; j < constraints[selectedStudent].size(); j++) {
//                cout << "purifying domain for " << constraints[selectedStudent][j] << endl;
                for (i = 0; i < domains[constraints[selectedStudent][j]].size(); i++) {
                    if (domains[constraints[selectedStudent][j]][i] == -1) continue;
                    if (this->FruitEnergies[domains[constraints[selectedStudent][j]][i]] ==
                        this->FruitEnergies[selectedFruit]) {
//                        cout << "removed " << domains[constraints[selectedStudent][j]][i] << " from domain of "
//                             << constraints[selectedStudent][j] << endl;
                        domains[constraints[selectedStudent][j]][i] = -1;
                    }
                }
            }
//            cout << "assinged " << selectedFruit << " to " << selectedStudent << endl;
            selectedFruit = -1;
            selectedStudent = -1;
        }

        //select unassigned
        int tempFruit, madeConstraintCount, selectedFruitConstrains;
        bool backtrackResult;

        //Most Constraining Variable = Min Remaining Value
        int constraintCounter;
        tempConstrainCount = 0;
//        cout << "checking min remaining value" << endl;
        for (i = 0; i < n; i++) {
            constraintCounter = 0;
            for (j = 0; j < domains[i].size(); j++) {
                if (domains[i][j] == -1) constraintCounter++;
            }
            if (fullStatus[i] == 0 && constraintCounter >= tempConstrainCount) {
                selectedStudent = i;
                tempConstrainCount = constraintCounter;
//                cout << "selected student is " << selectedStudent << endl;
            }
        }
        //if no student is selected so all domains are empty
        //lets test if domains are empty
        if (selectedStudent == -1) {
            bool fullCheck;
            fullCheck = this->printDomains(domains, fullStatus);
            if (fullCheck) {
//                cout << "all students are full" << endl;
                this->printResults(usedFruits);
                return true;
            }
        }

        //check domain
        while (true) {
            selectedFruitConstrains = this->n*this->m + 1;
            //Least Constraining Value
            for (i = 0; i < domains[selectedStudent].size(); i++) {
                if (domains[selectedStudent][i] == -1) continue;
                tempFruit = domains[selectedStudent][i]; //select fruit
                madeConstraintCount = 0;
                //cout<<"check fruit "<<tempFruit<<endl;
                for (j = 0; j < this->constraints[selectedStudent].size(); j++) { //check which values from constrains are removed
                    if (fullStatus[this->constraints[selectedStudent][j]] == 0) {
                        for (k = 0; k < domains[constraints[selectedStudent][j]].size(); k++) {//check if constrain has this value for choosing
                            if(domains[constraints[selectedStudent][j]][k]==-1) continue;
                            if (this->FruitEnergies[domains[constraints[selectedStudent][j]][k]] ==  this->FruitEnergies[tempFruit]) {
                                madeConstraintCount++;
                            }
                        }
                    }
                }
//                cout<<"selectedFruitConstrains is "<<selectedFruitConstrains<<" and madeConstraintCount is "<<madeConstraintCount<<endl;
                if (selectedFruitConstrains >= madeConstraintCount) {
//                    cout<<"this fruit made it "<<tempFruit<<endl;
                    selectedFruit = tempFruit;
                    selectedFruitConstrains = madeConstraintCount;
                }
            }
//            cout<<"selected fruit "<<selectedFruit<<endl;
            if(selectedFruit==-1){
//                this->printDomains(domains, fullStatus);
//                this->printResults(usedFruits);
                return false;
            }else{
//                cout << "running recursive" << endl;
                backtrackResult = this->backtrackSearch(domains, usedFruits, fullStatus, selectedStudent, selectedFruit);
                if (!backtrackResult) {
//                    cout << "student " << selectedStudent << " failed with fruit " << selectedFruit << endl;
                    int domainDeletionCounter = 0;
                    for (i = 0; i < domains[selectedStudent].size(); i++) {
                        if (domains[selectedStudent][i] == -1){
                            domainDeletionCounter++;
                            continue;
                        }
                        if (this->FruitEnergies[domains[selectedStudent][i]] == this->FruitEnergies[selectedFruit]) {
                            domains[selectedStudent][i] = -1;
//                            cout<<"****************"<<endl;
//                            this->printDomains(domains, fullStatus);
//                            cout<<"****************"<<endl;
                            domainDeletionCounter++;
                        }
                    }

                    if(domainDeletionCounter>=this->m){
//                        this->printDomains(domains, fullStatus);
//                        this->printResults(usedFruits);
                        break;
                    }
                }else{
//                    this->printResults(usedFruits);
                    return true;
                }
            }
        }
        return false;
    }

    void getAnswer() {
        this->getInputs();

        vector<vector<int>> domains;
        vector<int> fruitNumbers;
        int i;
        for (i = 0; i < this->m; i++) {
            fruitNumbers.push_back(i);
        }
        for (i = 0; i < this->n; i++) {
            domains.push_back(fruitNumbers);
            //cout<<"student domain "<<i<<" is "<<domains[i].size()<<endl;
        }
        //cout<<"starting backtrack search!"<<endl;
        this->backtrackSearch(domains, this->usedFruits, this->fullStatus);

    }
};

int main() {
    CSP myCsp = CSP();
    myCsp.getAnswer();

    int dummy;
    cin >> dummy;
    return 0;
}