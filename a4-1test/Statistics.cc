#include "Statistics.h"
#include <iostream>
#include <cmath>
using namespace std; 


void printPartitions(vector < vector<char*> > &partitions){
    for(int i=0; i<partitions.size(); i++){
        for(int j=0; j<partitions[i].size(); j++){
            cout << partitions[i][j] << " "; 
        }
        cout << "\n"; 
    }
}

Statistics::Statistics()
{
    // vector<char*> addRelnames, addRelnames1; 
    // addRelnames.push_back("orders");
    // addRelnames.push_back("customer"); 
    // addRelnames1.push_back("nation");
    // map <char*, long> tempMap; 
    // tempMap.insert(make_pair((char *)"total", 10));
    // StatisticsTable.insert(make_pair(addRelnames, tempMap)); 
    // StatisticsTable.insert(make_pair(addRelnames1, tempMap)); 
}

Statistics::Statistics(Statistics &copyMe)
{
}
Statistics::~Statistics()
{
}

void Statistics::AddRel(char *relName, int numTuples)
{
    vector<char*> addRelName;
    addRelName.push_back(relName); 
    map<char*, long, cmp_str> temp; 
    temp.insert(make_pair((char*)"total", numTuples)); 
    StatisticsTable.insert(make_pair(addRelName, temp));   
}
void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
    vector<char*> tableName; 
    tableName.push_back(relName); 
    long total = StatisticsTable[tableName]["total"]; 
    if(StatisticsTable.find(tableName) != StatisticsTable.end()){
        if(numDistincts == -1){
            StatisticsTable[tableName].insert(make_pair(attName, total)); 
        }else{
            StatisticsTable[tableName].insert(make_pair(attName, numDistincts)); 
        }
    }
}
void Statistics::CopyRel(char *oldName, char *newName)
{
}
	
void Statistics::Read(char *fromWhere)
{
}
void Statistics::Write(char *fromWhere)
{
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin) 
{
    // CheckifRelsExist

}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
    vector< vector<char*> > partitions; 
    int res = CheckifRelsExist(relNames, numToJoin, partitions); 
    printPartitions(partitions); 
    if(res == -1){
        cerr << "Given relations cannot be used for estimation!!!\n";
        exit(0); 
    }
    // if(res >1){
    //     //Join 
    //     printf("join operations \n"); 
    // }else{
    //     printf("select operations \n"); 
    // }
    long numOfTuples = getNumOfTuples(partitions); 
    cout << numOfTuples; 
    // printf(" Total number of tuples after cross product = %ld \n", numOfTuples);  
    long interNumOfTuples = numOfTuples;  
    struct AndList* currParsing = parseTree; 
    double fract = 1.0; 
    while(currParsing != nullptr){
        double result = processOrlist(interNumOfTuples, currParsing->left, partitions); 
        cout<< "result val: " << result <<"\n"; 
        fract *= result;
        cout<< "fract val: " << fract <<"\n"; 
        currParsing = currParsing->rightAnd; 
        interNumOfTuples *= fract; 
    }
    cout << "\n"; 
    return min<double>(numOfTuples, numOfTuples* fract); 
}

void Statistics::PrintStatistics(){
    // map <vector<char*>, map<char*, int> >::iterator it1; 
    // map<char*, int> it2; 
    for(auto it=StatisticsTable.begin(); it!= StatisticsTable.end(); it++){
        for(int i=0; i<it->first.size(); i++){
            cout << it->first[i] << "|";
        }
        cout<<"\n";
        for(auto it2 = it->second.begin(); it2!= it->second.end(); it2++){
            cout << it2->first << ":\t" << it2->second << "\n";
        }
        cout<<"\n***********************************\n";
    }
}

int Statistics::CheckifRelsExist(char* relNames[], int numToJoin, vector <vector <char*> > &partitions){
    map <char*, int> relNamesMap; 
    int size1 = numToJoin, opType =0 ; 
    for(int i=0; i<numToJoin; i++){
        // relNamesMap.insert(make_pair(relNames[i], 0)); 
        ++relNamesMap[relNames[i]];
    } 
    for(auto it = StatisticsTable.begin(); it!= StatisticsTable.end(); it++){
        int count=0;
        for(int i=0; i < it->first.size(); i++){
            if(relNamesMap.find(it->first[i]) != relNamesMap.end()){
                ++count; 
            }
        }
        if(count==it->first.size()){
            int count1 = relNamesMap[it->first[0]]; 
            for(int i=0; i< it->first.size(); i++){
                if(relNamesMap[it->first[i]] == count1){
                    size1 -= count1; 
                    relNamesMap[it->first[i]] = 0; 
                }else{
                    return -1; 
                }
            }
            opType+= count1; 
            for(int i=0; i<count1; i++){
                partitions.push_back(it->first); 
            }
        }else if(count!=0){
            return -1; 
        }
    }
    if(size1 != 0){
        return -1; 
    }
    return opType;  
}

long Statistics::getNumOfTuples(vector <vector <char*> > &partitions){ 
    long total = 1; 
    for (int i=0; i<partitions.size(); i++){
        total *= StatisticsTable[partitions[i]]["total"];
    }
    return total;
}   

double Statistics::processOrlist(long numOfinputTuples, struct OrList* myOrlist, vector <vector <char*> > &partitions){
    if(myOrlist == nullptr){
        return 1.0; 
    }
    double fract = 1.0; 
    // map < char*, vector<pair<int, Operand* > > > parsedOrList; 
    // vector <double> individualFracts; 
    map < char*, vector<double >,cmp_str> parsedOrList; 
    struct OrList* currOp = myOrlist;
    while(currOp != nullptr){ 
        struct ComparisonOp* temp = currOp->left; 
        if(temp->left->code == NAME){
            if(temp->right->code != NAME){
                if(temp->code == EQUALS){
                    long val = checkAndGetAttVal(partitions, temp->left->value); 
                    if(val==-2){
                        cerr << "Invalid CNF for !!"<<temp->left->value<<"\n"; 
                        exit(0); 
                    }
                    cout << val << " ," << 1.0/(double)val << "\n"; 
                    parsedOrList[temp->left->value].push_back(1.0/(double)val); 
                }else{
                    parsedOrList[temp->left->value].push_back(1.0/3.0); 
                }

            }else{
                if(temp->code == EQUALS){
                    // parsedOrList.find();
                    long leftVal = checkAndGetAttVal(partitions, temp->left->value);
                    long rightVal = checkAndGetAttVal(partitions, temp->right->value); 
                    if(leftVal==-2 || rightVal==-2){
                        cerr << "Invalid CNF!! for "<<temp->left->value <<"-"<<temp->right->value<<"\n"; 
                        exit(0); 
                    } 
                    cout << "Left val: "<<leftVal << "rightVal: " << rightVal <<"\n";  
                    long max1 = max<long>(leftVal, rightVal); 
                    // cout << "" 
                    parsedOrList[temp->left->value].push_back( 1.0/(double)max1); 
                    cout << parsedOrList[temp->left->value].size() << ": size, " << parsedOrList[temp->left->value][0] << "\n";
                }else{
                    parsedOrList[temp->left->value].push_back(1.0/3.0); 
                }

            }
        }else if(temp->right->code == NAME){
            if(temp->code == EQUALS){
                long val = checkAndGetAttVal(partitions, temp->right->value); 
                if(val==-2){
                    cerr << "Invalid CNF for !!"<<temp->right->value<<"\n"; 
                    exit(0); 
                }
                    parsedOrList[temp->right->value].push_back(1.0/(double)val); 
                }else{
                    parsedOrList[temp->right->value].push_back(1.0/3.0); 
                }
        }
        currOp = currOp->rightOr; 
    }
    // double prev=1.0;
    for(auto itr=parsedOrList.begin();itr!=parsedOrList.end();itr++){
        double tmpfract=0.0;
        for(int i=0;i<itr->second.size();i++){
            tmpfract+=itr->second[i];
            cout << itr->second[i] << "**" << tmpfract << " ";  
        }
        cout << "\n" << tmpfract<<"\n";
        fract *= (1.0-tmpfract); 
    }
    return (1.0-fract); 
}

int Statistics::checkAndGetAttVal(vector <vector <char*> > &partitions, char* attName){
    int val=-2; 
    // for(auto it = partitions.begin(); it != partitions.end(); it++){ 
    //     if(StatisticsTable.find(it->front) != StatisticsTable.end()); 
    // }
    // printPartitions(partitions); 
    // PrintStatistics();
    // string  str=attName;
    for(int i=0; i<partitions.size(); i++){
        cout << "looking in relation " << partitions[i][0] << " for " << attName <<"1\n"; 
        if(StatisticsTable.find(partitions[i]) != StatisticsTable.end() && \
            StatisticsTable[partitions[i]].find(attName)!= StatisticsTable[partitions[i]].end()){
           printf("Printing  : %ld\n", StatisticsTable[partitions[i]][attName]);// << " for " << attName <<"\n";
        //    printf("Printing1  : %ld\n", StatisticsTable[partitions[i]][(char*)str.c_str()]);// << " for " << attName <<"\n";
            val = StatisticsTable[partitions[i]][attName]; 
            break;
        }   
    }
    return val; 
}