#include <iostream>
#include <vector>
#include <queue>
#include <time.h>
#include <algorithm>
#include <set>

using namespace std;

const int INFINITY = 2147483647;

class Puzzle {
public:
    int successStatus[9];
    int currentStatus[9];
    int currentEmptyPosition;
    int price;
    vector<int> moves;
    int heuristic;
    int rbfsCost;
    /* MOVES :
     * 0 => up
     * 1 => right
     * 2 => down
     * 3 => left
     * */
    Puzzle() {

    }

    Puzzle(Puzzle *tempPuzzle) {
        int i, j;
        for (i = 0; i < 8; i++) {
            this->successStatus[i] = i + 1;
        }
        this->successStatus[8] = 0;

        for (i = 0; i < 9; i++) {
            currentStatus[i] = tempPuzzle->currentStatus[i];
            if (currentStatus[i] == 0) {
                this->currentEmptyPosition = i;
            }
        }

        this->price = tempPuzzle->price;
        this->moves = tempPuzzle->moves;
    }

    Puzzle(int type) {
        int i, j;
        for (i = 0; i < 8; i++) {
            this->successStatus[i] = i + 1;
        }
        this->successStatus[8] = 0;

        for (i = 0; i < 8; i++) {
            this->currentStatus[i] = i + 1;
        }
        this->currentStatus[8] = 0;
        this->currentEmptyPosition = 8;

        this->price = 0;
    }

    void create() {
        int i, j;
        for (i = 0; i < 8; i++) {
            this->successStatus[i] = i + 1;
        }
        this->successStatus[8] = 0;

        for (i = 0; i < 9; i++) {
            cin >> currentStatus[i];
            if (currentStatus[i] == 0) {
                this->currentEmptyPosition = i;
            }
        }

        this->price = 0;
    }

    void getAvailableMoves(int *moves) {
        moves[0] = 1;
        moves[1] = 1;
        moves[2] = 1;
        moves[3] = 1;

        switch (this->currentEmptyPosition) {
            case 0:
                moves[1] = 0;
                moves[2] = 0;
                break;
            case 1:
                moves[2] = 0;
                break;
            case 2:
                moves[3] = 0;
                moves[2] = 0;
                break;
            case 3:
                moves[1] = 0;
                break;
            case 5:
                moves[3] = 0;
                break;
            case 6:
                moves[0] = 0;
                moves[1] = 0;
                break;
            case 7:
                moves[0] = 0;
                break;
            case 8:
                moves[0] = 0;
                moves[3] = 0;
                break;
        }
    }

    vector<int> getCurrentStatus() {
        vector<int> res;
        for (int i = 0; i < 9; i++) {
            res.push_back(this->currentStatus[i]);
        }
        return res;
    }

    // gets TYPE returns PRICE paid
    int move(int type) {
        int price = 0;
        int i = this->currentEmptyPosition;

        switch (type) {
            case 0: // move up
                //while (i < 6) {
                this->currentStatus[i] = this->currentStatus[i + 3];
                price += abs(i - this->currentStatus[i]);
                i += 3;
                //}
                break;
            case 1: // move right
                //while (i % 3 != 0) {
                this->currentStatus[i] = this->currentStatus[i - 1];
                price += abs(i - this->currentStatus[i]);
                i--;
                //}
                break;
            case 2: // move down
                //while (i > 2) {
                this->currentStatus[i] = this->currentStatus[i - 3];
                price += abs(i - this->currentStatus[i]);
                i -= 3;
                //}
                break;
            case 3: // move left
                //while ((i + 1) % 3 != 0) {
                this->currentStatus[i] = this->currentStatus[i + 1];
                price += abs(i - this->currentStatus[i]);
                i++;
                //}
                break;
        }
        this->currentEmptyPosition = i;
        this->currentStatus[i] = 0;
        this->price += price;
        this->moves.push_back(type);
        this->makeHeuristic();
        return price;
    }

    bool checkSuccess() {
        int match = 0;
        for (int i = 0; i < 9; i++) {
            if (this->currentStatus[i] == this->successStatus[i]) match++;
        }
        return (match == 9) ? true : false;
    }

    void makeHeuristic(){
        int diff = 0;

        for(int i=0; i<9; i++){
            if(currentStatus[i]==0) continue;
            diff+= abs(currentStatus[i]-(i+1));
        }

        this->heuristic = diff;
    }
    void print() {
        for (int i = 0; i < 9; i++) {
            cout << this->currentStatus[i] << " ";
        }
        cout << endl;
    }

    void printMoves(){
        for(int i=0; i<this->moves.size(); i++){
            switch(this->moves[i]){
                case 0: cout<<"top "; break;
                case 1: cout<<"right "; break;
                case 2: cout<<"down "; break;
                case 3: cout<<"left "; break;
            }
        }
        cout<<endl;
    }

    void printMovesBackwards(){
        for(int i=this->moves.size()-1; i>=0; i--){
            switch(this->moves[i]){
                case 0: cout<<"top "; break;
                case 1: cout<<"right "; break;
                case 2: cout<<"down "; break;
                case 3: cout<<"left "; break;
            }
        }
    }
};

struct puzzleCostStruct {
    bool operator() (Puzzle const & lhs, Puzzle const & rhs) const
    {
        return lhs.price < rhs.price;
    }
};

struct puzzleHeuristicStruct {
    bool operator() (Puzzle const & lhs, Puzzle const & rhs) const
    {
        return lhs.heuristic+rhs.price <= rhs.heuristic+rhs.price;
    }
};

struct rbfsStruct {
    bool operator() (Puzzle const & lhs, Puzzle const & rhs) const
    {
        return lhs.rbfsCost <= rhs.rbfsCost;
    }
};

struct puzzleHeuristicAndPriceStruct {
    bool operator() (Puzzle const & lhs, Puzzle const & rhs) const
    {
        return (lhs.heuristic+lhs.price) < (rhs.heuristic+rhs.price);
    }
};

class SearchAlgorithm {
protected:
    //bds dfs
    vector<vector<int> > seenPuzzles;
    //bds
    vector<vector<int> > topSeenPuzzles;
    vector<vector<int> > bottomSeenPuzzles;

    int counter;
public:
    SearchAlgorithm() {
        this->counter = 0;
    }

    bool checkSeenPuzzles(vector<int> tempStatus) {
        int score, i, j;

        for (j = 0; j < this->seenPuzzles.size(); j++) {
            score = 0;
            for (i = 0; i < 9; i++) {
                if (tempStatus[i] == this->seenPuzzles[j][i]) score++;
            }
            if (score == 9) {
                return false;
            }
        }
        this->seenPuzzles.push_back(tempStatus);
        return true;
    }

    bool checkBdsSeenPuzzles(vector<int> tempStatus, int type = 0) { // 0 => top | 1 => bottom
        // 3 => check if from bottom exists in top
        // 4 => check if from top exists in bottom
        int score, i, j;

        if (type == 0 || type == 3) { //top
            for (j = 0; j < this->topSeenPuzzles.size(); j++) {
                score = 0;
                for (i = 0; i < 9; i++) {
                    if (tempStatus[i] == this->topSeenPuzzles[j][i]) score++;
                }
                if (score == 9) {
                    return type==3 ? true : false;
                }
            }
            if(type!=3) this->topSeenPuzzles.push_back(tempStatus);
        } else {//bottom
            for (j = 0; j < this->bottomSeenPuzzles.size(); j++) {
                score = 0;
                for (i = 0; i < 9; i++) {
                    if (tempStatus[i] == this->bottomSeenPuzzles[j][i]) score++;
                }
                if (score == 9) {
                    return type==4 ? true : false;
                }
            }
            if(type!=4) this->bottomSeenPuzzles.push_back(tempStatus);
        }
        return true;
    }

    bool checkBdsSuccess() {
        int score, i, j, k;

        for (k = 0; k < this->topSeenPuzzles.size(); k++) {
            for (j = 0; j < this->bottomSeenPuzzles.size(); j++) {
                score = 0;
                for (i = 0; i < 9; i++) {
                    if (this->topSeenPuzzles[k][i] == this->bottomSeenPuzzles[j][i]) score++;
                }
                if (score == 9) {
                    return true;
                }
            }
        }
        return false;
    }

    void bfs(Puzzle *initPuzzle) {
        queue<Puzzle *> bfsList;
        int i;
        int moves[4];
        Puzzle *frontPuzzle;
        int counter = 0;
        vector<int> tempHolder;
        bfsList.push(initPuzzle);
        tempHolder = initPuzzle->getCurrentStatus();
        this->seenPuzzles.push_back(tempHolder);
        bool found = false;

        while (!bfsList.empty()) {
            frontPuzzle = new Puzzle(bfsList.front());
            bfsList.pop();
            if (frontPuzzle->checkSuccess()) {
                cout << "found" << endl;
                cout << "price was " << frontPuzzle->price << endl;
                found = true;
                frontPuzzle->print();
                break;
            }

            //cout<<"poped this ";
            //frontPuzzle->print();

            counter++;
//            cout << "on run :" << counter << endl;

            frontPuzzle->getAvailableMoves(moves);
            //frontPuzzle->print();

            for (i = 0; i < 4; i++) {
                if (moves[i] == 1) {
                    Puzzle *tempPuzzle = new Puzzle(frontPuzzle);
                    tempPuzzle->move(i);
                    tempHolder = tempPuzzle->getCurrentStatus();

                    if (this->checkSeenPuzzles(tempHolder)) {
                        bfsList.push(tempPuzzle);
                    }
                }
            }
        }
        if (!found) cout << "no success state found!" << endl;
    }

    void dfs(Puzzle *frontPuzzle) {
        int i;
        int moves[4];
        vector<int> tempHolder;

        this->counter++;
        cout<<"on run : "<<this->counter<<endl;
//        cout << "RUN" << endl;

        if (frontPuzzle->checkSuccess()) {
            cout << "found" << endl;
            cout << "price was " << frontPuzzle->price << endl;
            frontPuzzle->print();
        }
        frontPuzzle->getAvailableMoves(moves);
        for (i = 0; i < 4; i++) {
            if (moves[i] == 1) {
                Puzzle *tempPuzzle = new Puzzle(frontPuzzle);
                tempPuzzle->move(i);
                tempHolder = tempPuzzle->getCurrentStatus();
                if (this->checkSeenPuzzles(tempHolder)) {
                    tempPuzzle->print();
                    this->dfs(tempPuzzle);
                }
            }
        }
    }

    void bds(Puzzle *initPuzzle) {
        queue<Puzzle *> topBfsList;
        queue<Puzzle *> bottomBfsList;
        int i;
        int moves[4];
        Puzzle *frontPuzzle;
        Puzzle *backPuzzle;

        vector<int> tempHolder;
        int counter = 0;

        //make top bfs list
        topBfsList.push(initPuzzle);
        tempHolder = initPuzzle->getCurrentStatus();
        this->seenPuzzles.push_back(tempHolder);

        //make bottom bfs list
        Puzzle *tempPuzzle = new Puzzle(1);
        bottomBfsList.push(tempPuzzle);
        tempHolder = tempPuzzle->getCurrentStatus();
        this->seenPuzzles.push_back(tempHolder);

        bool found = false;

        while (!topBfsList.empty() || !bottomBfsList.empty()) {
            if (!topBfsList.empty()) {
                frontPuzzle = new Puzzle(topBfsList.front());
                topBfsList.pop();

                frontPuzzle->getAvailableMoves(moves);
                for (i = 0; i < 4; i++) {
                    if (moves[i] == 1) {
                        Puzzle *tempPuzzle = new Puzzle(frontPuzzle);
                        tempPuzzle->move(i);
                        tempHolder = tempPuzzle->getCurrentStatus();

                        if (this->checkBdsSeenPuzzles(tempHolder, 0)) {
                            topBfsList.push(tempPuzzle);
                        }

                        if(this->checkBdsSeenPuzzles(tempHolder, 4)){
                            cout<<"found in BDS!"<<endl;
                            found = true;
                            break;
                        }
                    }
                }
            }

            if (!bottomBfsList.empty()) {
                backPuzzle = new Puzzle(bottomBfsList.front());
                bottomBfsList.pop();

                backPuzzle->getAvailableMoves(moves);
                for (i = 0; i < 4; i++) {
                    if (moves[i] == 1) {
                        Puzzle *tempPuzzle = new Puzzle(backPuzzle);
                        tempPuzzle->move(i);
                        tempHolder = tempPuzzle->getCurrentStatus();

                        if (this->checkBdsSeenPuzzles(tempHolder, 1)) {
                            bottomBfsList.push(tempPuzzle);
                        }

                        if(this->checkBdsSeenPuzzles(tempHolder, 3)){
                            cout<<"found in BDS!"<<endl;
                            found = true;
                            break;
                        }
                    }
                }
            }

            counter++;
            cout << "on run :" << counter << endl;

            //check success
            if(this->checkBdsSuccess()){
            }

        }
        if (!found) cout << "no success state found!" << endl;
    }

    void ucs(Puzzle *initPuzzle){
        multiset<Puzzle, puzzleCostStruct> ucsList;
        int i;
        int moves[4];
        Puzzle *frontPuzzle;
        int counter = 0;
        vector<int> tempHolder;
        ucsList.insert(initPuzzle);
        tempHolder = initPuzzle->getCurrentStatus();
        this->seenPuzzles.push_back(tempHolder);
        bool found = false;

        while (!ucsList.empty()) {
            auto newPuzzle = ucsList.begin();
            frontPuzzle = new Puzzle(*newPuzzle);
            ucsList.erase(newPuzzle);
            frontPuzzle->print();

            if (frontPuzzle->checkSuccess()) {
                cout << "found" << endl;
                cout << "price was " << frontPuzzle->price << endl;
                frontPuzzle->printMoves();
                frontPuzzle->print();
                found = true;
                break;
            }

            counter++;
            cout << "on run :" << counter << endl;

            frontPuzzle->getAvailableMoves(moves);
            for (i = 0; i < 4; i++) {
                if (moves[i] == 1) {
                    Puzzle *tempPuzzle = new Puzzle(frontPuzzle);
                    tempPuzzle->move(i);
                    tempHolder = tempPuzzle->getCurrentStatus();

                    if (this->checkSeenPuzzles(tempHolder)) {
                        ucsList.insert(tempPuzzle);
//                        cout<<"new state!"<<endl;
                    }else{
//                        cout<<"seen before!"<<endl;
                    }
                }
            }
        }
        if (!found) cout << "no success state found!" << endl;
    }

    void a_star(Puzzle *initPuzzle){
        multiset<Puzzle, puzzleHeuristicStruct> aStarList;
        int i;
        int moves[4];
        Puzzle *frontPuzzle;
        int counter = 0;
        vector<int> tempHolder;
        aStarList.insert(initPuzzle);
        tempHolder = initPuzzle->getCurrentStatus();
        this->seenPuzzles.push_back(tempHolder);
        bool found = false;

        while (!aStarList.empty()) {
            auto newPuzzle = aStarList.begin();
            frontPuzzle = new Puzzle(*newPuzzle);
            aStarList.erase(newPuzzle);
            frontPuzzle->print();

            if (frontPuzzle->checkSuccess()) {
                cout << "found" << endl;
                cout << "price was " << frontPuzzle->price << endl;
                frontPuzzle->printMoves();
                frontPuzzle->print();
                found = true;
                break;
            }

            counter++;
            cout << "on run :" << counter << endl;

            frontPuzzle->getAvailableMoves(moves);
            for (i = 0; i < 4; i++) {
                if (moves[i] == 1) {
                    Puzzle *tempPuzzle = new Puzzle(frontPuzzle);
                    tempPuzzle->move(i);
                    tempHolder = tempPuzzle->getCurrentStatus();

                    if (this->checkSeenPuzzles(tempHolder)) {
                        aStarList.insert(tempPuzzle);
//                        cout<<"new state!"<<endl;
                    }else{
//                        cout<<"seen before!"<<endl;
                    }
                }
            }
        }
        if (!found) cout << "no success state found!" << endl;
    }

    bool rbfs(Puzzle *&initPuzzle, int &limit){
        multiset<Puzzle, rbfsStruct > rbfsList;
        int i;
        int moves[4];
        bool result;
        vector<int> tempHolder;
        Puzzle *frontPuzzle;
        int totalCost = initPuzzle->price+initPuzzle->heuristic;
        if(totalCost>limit) return false;
        cout<<"initial limit is "<<limit<<endl;

        if (initPuzzle->checkSuccess()) {
            cout << "found" << endl;
            cout << "price was " << initPuzzle->price+initPuzzle->heuristic << endl;
            initPuzzle->print();
            return true;
        }else{
            initPuzzle->print();

        }

        initPuzzle->getAvailableMoves(moves);
        int childCounter = 0;
        for (i = 0; i < 4; i++) {
            if (moves[i] == 1) {
                Puzzle *tempPuzzle = new Puzzle(initPuzzle);
                tempPuzzle->move(i);
                tempPuzzle->rbfsCost = max(initPuzzle->rbfsCost, tempPuzzle->price+tempPuzzle->heuristic);
                tempHolder = tempPuzzle->getCurrentStatus();
                if (this->checkSeenPuzzles(tempHolder)) {
                    childCounter++;
                    rbfsList.insert(tempPuzzle);
                }
            }
        }
        int secondBestLimit;
        if(childCounter==0){
            return  false;
        }else {
            while(true) {
                auto newPuzzle = rbfsList.begin();
                frontPuzzle = new Puzzle(*newPuzzle);
                rbfsList.erase(newPuzzle);
                if (childCounter > 1) {
                    auto newPuzzle = rbfsList.begin();
                    secondBestLimit = newPuzzle->rbfsCost;
                } else {
                    secondBestLimit = INFINITY;
                }
                secondBestLimit = min(secondBestLimit, limit);
                cout<<"limit is "<<secondBestLimit<<endl;
                result = this->rbfs(frontPuzzle, secondBestLimit);
                frontPuzzle->rbfsCost = secondBestLimit;
                if(result) return result;
                if(secondBestLimit<=limit){
                    rbfsList.insert(frontPuzzle);
                }else{
//                    initPuzzle = frontPuzzle;
                    return false;
                }
            }
        }
        //get best first and second best first
        //recursive run for the moves
    }

};

int main() {
//    cout << "type in puzzle" << endl;
    Puzzle *myPuzzle = new Puzzle();
    myPuzzle->create();

    SearchAlgorithm *mySearch = new SearchAlgorithm();
//    mySearch->bfs(myPuzzle);
//    mySearch->dfs(myPuzzle);
//    mySearch->bds(myPuzzle);
//    mySearch->ucs(myPuzzle);
//    mySearch->a_star(myPuzzle);
    int tempInfinity = INFINITY;
    mySearch->rbfs(myPuzzle, tempInfinity);

    int dummy;
    cin >> dummy;
}