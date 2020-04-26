#include "Statistics.h"
#include <iostream>
#include <string>
#include <cmath>
using namespace std; 

//Default constructor. 
Statistics::Statistics()
{
}

//Takes the copyMe Statistics object and performs deep copy.
//Initializes the current Object state with the state of copyMe object. 
Statistics::Statistics(Statistics &copyMe)
{
    for(auto it=copyMe.StatisticsTable.begin(); it!=copyMe.StatisticsTable.end(); it++){
        vector <string> temp;
        for(int i=0; i< it->first.size(); i++){
            temp.push_back(string(it->first[i])); 
        }  
        map <string, ll> tempMap; 
        for(auto it1 = it->second.begin(); it1 != it->second.end(); it1++){ 
            tempMap.insert(make_pair(it1->first,it1->second));  
        }
        StatisticsTable.insert(make_pair(temp, tempMap));
        temp.clear(); 
    }
}

Statistics::~Statistics()
{
}

//Adds the relation to the Statistics object. 
void Statistics::AddRel(char *relName, int numTuples)
{
    vector<string> addRelName;
    addRelName.push_back(string(relName)); 
    map<string, ll > temp; 
    temp.insert(make_pair((char*)"total", numTuples));  
    StatisticsTable.insert(make_pair(addRelName, temp));   
}

//Adds the attribute corresponding to an existing relation in the Statistics object. 
//Exits the program if relation doesn't exist.
void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
    vector<string> tableName; 
    tableName.push_back(string(relName)); 
    if(StatisticsTable.find(tableName)== StatisticsTable.end()){
        cerr << "Relation "<<relName << " doesn't exist!!!\n"; 
        exit(1);  
    }
    ll total = StatisticsTable[tableName]["total"]; 
    if(StatisticsTable.find(tableName) != StatisticsTable.end()){
        if(numDistincts == -1){
            //If numDisctincts = -1, then it assumed that all the tuples have distinct values. 
            StatisticsTable[tableName].insert(make_pair(attName, total)); 
        }else{
            StatisticsTable[tableName].insert(make_pair(attName, numDistincts)); 
        }
    }
}

//Produces a copy of the relation given by "oldName" under "newName". 
void Statistics::CopyRel(char *oldName, char *newName)
{
    vector <string> temp; 
    temp.push_back(string(oldName));     
    if(StatisticsTable.find(temp) == StatisticsTable.end()){
        cerr << "Relation " << oldName << " doesn't exist!!\n"; 
        exit(1); 
    }
    vector <string> temp2; 
    map <string, ll> newMap; 
    temp2.push_back(newName); 
    for(auto it=StatisticsTable[temp].begin(); it!=StatisticsTable[temp].end(); it++){
        if(it->first.compare(string("total"))==0){
            newMap.insert(make_pair(it->first, it->second)); 
            continue; 
        }
        // char* newAtt = new char[100];  
        // newAtt[0] = '\0'; 
        // strcat(newAtt, newName); 
        // strcat(newAtt, "."); 
        // strcat(newAtt, it->first);   
        string newAtt; 
        newAtt.append(string(newName)); 
        newAtt.append(string(".")); 
        newAtt.append(it->first); 
        newMap.insert(make_pair(newAtt, it->second)); 
    }
    StatisticsTable.insert(make_pair(temp2, newMap));  
}

//Initializes the state of the current Statistics object 
//using the data available in the file "fromWhere". 
void Statistics::Read(char *fromWhere)
{   
    FILE* inFile = fopen(fromWhere, "r");
    if(inFile==nullptr){
        cerr << "File " << fromWhere <<" doesn't exist!!\n"; 
        exit(1); 
    } 
    ll mapSize;
    fscanf(inFile, "%lld", &mapSize);  
    if(mapSize <=0){
        return; 
    }
    for(ll i=0; i<mapSize; i++){
        vector<string> partition; 
        ll partitionSize; 
        fscanf(inFile, "%lld", &partitionSize); 
        for(ll j=0; j<partitionSize; j++){
            char* relName = new char[120]; 
            fscanf(inFile, "%s", relName);
            partition.push_back(string(relName)); 
         }
         ll attsMapSize;
         map <string, ll> attsMap; 
         fscanf(inFile, "%lld", &attsMapSize); 
         for(ll k=0; k<attsMapSize; k++){
             char* attName = new char[100]; 
             ll numDistincts; 
             fscanf(inFile,"%s", attName);
             fscanf(inFile,"%lld",&numDistincts);  
            attsMap.insert(make_pair(string(attName), numDistincts)); 
         }
         StatisticsTable.insert(make_pair(partition, attsMap));  
    }
    fclose(inFile);
}

//Writes the state of the current Statistics object to the file "fromWhere". 
void Statistics::Write(char *fromWhere)
{
    FILE* toFile = fopen(fromWhere, "w"); 
    fprintf(toFile, "%ld\n", StatisticsTable.size()); 
    for(auto it= StatisticsTable.begin(); it!=StatisticsTable.end(); it++){
        fprintf(toFile, "%ld", it->first.size()); 
        for(int i=0; i<it->first.size(); i++){
            fprintf(toFile, " %s",it->first[i].c_str());  
        }
        fprintf(toFile,"\n"); 
        fprintf(toFile, "%ld ", it->second.size());
        for(auto it1=it->second.begin(); it1!= it->second.end(); it1++){ 
            fprintf(toFile, "%s %d ", it1->first.c_str(), it1->second); 
        }
        fprintf(toFile, "\n");
    } 
    fclose(toFile); 
}

//Applies the join operation in the current Statistics object. 
//Merges the partitions which are used in the join operation and updates the 
//information on Total Number of tuples and the number of distinct values for each attribute. 
void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin) 
{
    ll resultingNoOfTuples = (ll)Estimate(parseTree, relNames, numToJoin); 
    vector< vector<string> > partitions; 
    int res = CheckifRelsExist(relNames, numToJoin, partitions); 
    if(res <= 1){
        return; 
    }
    //join
    vector<string> newPartition; 
    for(int i=0; i< numToJoin; i++){
        newPartition.push_back(string(relNames[i])); 
    }   
    //During the estimate, the possible changes to Statistics object are stored in tempState. 
    //These changes are committed here. 
    map <string, ll> newAtts; 
    for(int i=0; i< partitions.size(); i++){
       for(auto it=StatisticsTable[partitions[i]].begin();it!= StatisticsTable[partitions[i]].end(); it++){
           if(it->first.compare(string("total"))==0){
               continue;
           }
           newAtts.insert(make_pair(it->first, it->second)); 
       }
    }
    for(int i=0; i< tempState.size(); i++){
        newAtts[tempState[i].first] = tempState[i].second; 
    }
    newAtts.insert(make_pair(string("total"), resultingNoOfTuples)); 
    StatisticsTable.insert(make_pair(newPartition, newAtts)); 
    for(int i=0; i< partitions.size(); i++){
        StatisticsTable.erase(partitions[i]); 
    }
    tempState.clear(); 
}

//Returns the estimated number of output tuples for the given operation(CNF) in parseTree.  
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
    vector< vector<string> > partitions; 
    int res = CheckifRelsExist(relNames, numToJoin, partitions);  
    if(res == -1){
        cerr << "Given relations cannot be used for estimation!!!\n";
        exit(1); 
    } 
    struct AndList* currParsing = parseTree; 
    double fract = 1.0; 
    while(currParsing != nullptr){
        double result = processOrlist(1, currParsing->left, partitions); 
        fract *= result;
        currParsing = currParsing->rightAnd; 
    }
    return getNumOfTuples(partitions, min<double>(1.0, fract)); 
}

//Prints the StatisticsTable. Used for debugging.
void Statistics::PrintStatistics(){
    for(auto it=StatisticsTable.begin(); it!= StatisticsTable.end(); it++){
        cout << "\n"; 
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

//This method checks if the given relations given are consistent with the existing relation Partitions
//in the StatisticsTable. Returns 1 if the operation type is Selection. 
//Returns an integer >1 if the operation type is Join. Returns -1 if the given set of relNames is invalid. 
int Statistics::CheckifRelsExist(char* relNames[], int numToJoin, vector <vector <string> > &partitions){
    map <string, int> relNamesMap; 
    int size1 = numToJoin, opType =0 ; 
    for(int i=0; i<numToJoin; i++){
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


//It takes the selectivity factor as input and 
//returns the total number of output tuples for the given operation. 
double Statistics::getNumOfTuples(vector <vector <string> > &partitions, double fraction){ 
    double total = fraction; 
    for (int i=0; i<partitions.size(); i++){
        total *= (double)StatisticsTable[partitions[i]]["total"];
    }
    return round(total);
}   

//Processes the given orList and returns the selectivity factor of the entire orList.
double Statistics::processOrlist(ll numOfinputTuples, struct OrList* myOrlist, vector <vector <string> > &partitions){
    if(myOrlist == nullptr){
        return 1.0; 
    }
    double fract = 1.0; 
    map < char*, vector<double >,cmp_str> parsedOrList; 
    struct OrList* currOp = myOrlist;
    while(currOp != nullptr){ 
        struct ComparisonOp* temp = currOp->left; 
        int whichPartition = -1;
        int whichPartition2 = -1; 
        if(temp->left->code == NAME){
            if(temp->right->code != NAME){
                if(temp->code == EQUALS){
                    ll val = checkAndGetAttVal(partitions, temp->left->value); 
                    if(val==-2){
                        cerr << "Invalid CNF for "<<temp->left->value<<"!!\n"; 
                        exit(1); 
                    }
                    parsedOrList[temp->left->value].push_back(1.0/(double)val); 
                }else{
                    parsedOrList[temp->left->value].push_back(1.0/3.0); 
                }

            }else{
                if(temp->code == EQUALS){
                    ll leftVal = checkAndGetAttVal(partitions, temp->left->value);
                    ll rightVal = checkAndGetAttVal(partitions, temp->right->value);  
                    if(leftVal==-2 || rightVal==-2){
                        cerr << "Invalid CNF!! for "<<temp->left->value <<"-"<<temp->right->value<<"\n"; 
                        exit(0); 
                    }   
                    ll minval = min<ll>(leftVal, rightVal);              
                    tempState.push_back(make_pair(temp->left->value,minval)); 
                    tempState.push_back(make_pair(temp->right->value, minval)); 
                    ll max1 = max<ll>(leftVal, rightVal); 
                    parsedOrList[temp->left->value].push_back( 1.0/(double)max1); 
                }else{
                    parsedOrList[temp->left->value].push_back(1.0/3.0); 
                }

            }
        }else if(temp->right->code == NAME){
            if(temp->code == EQUALS){
                ll val = checkAndGetAttVal(partitions, temp->right->value); 
                if(val==-2){
                    cerr << "Invalid CNF for !!"<<temp->right->value<<"\n"; 
                    exit(1); 
                }
                    parsedOrList[temp->right->value].push_back(1.0/(double)val); 
                }else{
                    parsedOrList[temp->right->value].push_back(1.0/3.0); 
                }
        }
        currOp = currOp->rightOr; 
    }
    for(auto itr=parsedOrList.begin();itr!=parsedOrList.end();itr++){
        double tmpfract=0.0;
        for(int i=0;i<itr->second.size();i++){
            tmpfract+=itr->second[i];  
        }
        fract *= (1.0-tmpfract); 
    }
    return (1.0-fract); 
}

//This method checks if the given CNF in the parseTree is consistent with the relations in the relNames. 
//Returns 1 if consistent. Exits the program if inconsistent. 
int Statistics::checkAndGetAttVal(vector <vector <string> > &partitions, char* attName){
    int val=-2; 
    for(int i=0; i<partitions.size(); i++){
        if(StatisticsTable.find(partitions[i]) != StatisticsTable.end() && \
            StatisticsTable[partitions[i]].find(attName)!= StatisticsTable[partitions[i]].end()){
            val = StatisticsTable[partitions[i]][attName]; 
            break;
        }   
    }
    return val; 
}