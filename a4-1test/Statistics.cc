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
}

Statistics::Statistics(Statistics &copyMe)
{
    // map<vector<char*>, map<char*, ll, cmp_str>> copyTable = ; 
    for(auto it=copyMe.StatisticsTable.begin(); it!=copyMe.StatisticsTable.end(); it++){
        vector <char*> temp;
        for(int i=0; i< it->first.size(); i++){
            temp.push_back(it->first[i]); 
        }  
        map <char*, ll, cmp_str> tempMap; 
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

void Statistics::AddRel(char *relName, int numTuples)
{
    vector<char*> addRelName;
    addRelName.push_back(relName); 
    map<char*, ll , cmp_str> temp; 
    temp.insert(make_pair((char*)"total", numTuples)); 
    StatisticsTable.insert(make_pair(addRelName, temp));   
}

void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
    vector<char*> tableName; 
    // char newtempAtt[100]; 
    // strcpy(newtempAtt, attName); 
    tableName.push_back(relName); 
    if(StatisticsTable.find(tableName)== StatisticsTable.end()){
        cerr << "Relation "<<relName << " doesn't exist!!!\n"; 
        exit(0);  
    }
    ll total = StatisticsTable[tableName]["total"]; 
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
    vector <char*> temp; 
    temp.push_back(oldName); 
    if(StatisticsTable.find(temp) == StatisticsTable.end()){
        cerr << "Relation " << oldName << " doesn't exist!!\n"; 
        exit(0); 
    }
    vector <char*> temp2; 
    map <char*, ll, cmp_str> newMap; 
    temp2.push_back(newName); 
    // char** newAtt = new char*[100];
    for(auto it=StatisticsTable[temp].begin(); it!=StatisticsTable[temp].end(); it++){
        if(strcmp("total", it->first)==0){
            newMap.insert(make_pair(it->first, it->second)); 
            continue; 
        }
        // char* newAtt = new char[100] ;// = {}; //= (char*)"";
        char* newAtt = new char[100]; //= {}; 
        newAtt[0] = '\0'; 
        strcat(newAtt, newName); 
        strcat(newAtt, "."); 
        strcat(newAtt, it->first);  
        // sprintf(newAtt, "%s.%s",newName,it->first); 
        // cout << newAtt << "****************%%%\n"; 
        newMap.insert(make_pair((char*)newAtt, it->second)); 
    }
    StatisticsTable.insert(make_pair(temp2, newMap));  
}
	
void Statistics::Read(char *fromWhere)
{   
    FILE* inFile = fopen(fromWhere, "r");
    if(inFile==nullptr){
        cerr << "File " << fromWhere <<" doesn't exist!!\n"; 
        exit(0); 
    } 
    ll mapSize;
    fscanf(inFile, "%lld", &mapSize);  
    if(mapSize <=0){
        return; 
    }
    for(ll i=0; i<mapSize; i++){
        vector<char*> partition; 
        ll partitionSize; 
        fscanf(inFile, "%lld", &partitionSize); 
        for(ll j=0; j<partitionSize; j++){
            char* relName = new char[120]; 
            fscanf(inFile, "%s", relName);
            partition.push_back(relName); 
         }
         ll attsMapSize;
         map <char*, ll, cmp_str> attsMap; 
         fscanf(inFile, "%lld", &attsMapSize); 
         for(ll k=0; k<attsMapSize; k++){
             char* attName = new char[100]; 
             ll numDistincts; 
             fscanf(inFile,"%s", attName);
             fscanf(inFile,"%lld",&numDistincts);  
            attsMap.insert(make_pair(attName, numDistincts)); 
         }
         StatisticsTable.insert(make_pair(partition, attsMap));  
    }
    fclose(inFile);
}


void Statistics::Write(char *fromWhere)
{
    FILE* toFile = fopen(fromWhere, "w"); 
    fprintf(toFile, "%ld\n", StatisticsTable.size()); 
    for(auto it= StatisticsTable.begin(); it!=StatisticsTable.end(); it++){
        fprintf(toFile, "%ld", it->first.size()); 
        for(int i=0; i<it->first.size(); i++){
            fprintf(toFile, " %s",it->first[i]);  
        }
        fprintf(toFile,"\n"); 
        fprintf(toFile, "%ld ", it->second.size());
        for(auto it1=it->second.begin(); it1!= it->second.end(); it1++){ 
            fprintf(toFile, "%s %d ", it1->first, it1->second); 
        }
        fprintf(toFile, "\n");
    } 
    fclose(toFile); 
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin) 
{
    // CheckifRelsExist
    ll resultingNoOfTuples = (ll)Estimate(parseTree, relNames, numToJoin); 
    vector< vector<char*> > partitions; 
    int res = CheckifRelsExist(relNames, numToJoin, partitions); 
    if(res <= 1){
        return; 
    }
    //join
    vector<char*> newPartition; 
    // if(numToJoin)
    for(int i=0; i< numToJoin; i++){
        newPartition.push_back(relNames[i]); 
    }   
    map <char*, ll, cmp_str> newAtts; 
    for(int i=0; i< partitions.size(); i++){
       for(auto it=StatisticsTable[partitions[i]].begin();it!= StatisticsTable[partitions[i]].end(); it++){
           if(strcmp(it->first, "total")==0){
               continue;
           }
           newAtts.insert(make_pair(it->first, it->second)); 
       }
    }
    for(int i=0; i< tempState.size(); i++){
        newAtts[tempState[i].first] = tempState[i].second; 
    }
    newAtts.insert(make_pair((char*)"total", resultingNoOfTuples)); 
    //update total 
    //update atts 
    StatisticsTable.insert(make_pair(newPartition, newAtts)); 
    for(int i=0; i< partitions.size(); i++){
        StatisticsTable.erase(partitions[i]); 
    }
    // cout <<"End of apply-------------\n";
    // PrintStatistics();
}
double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{
    vector< vector<char*> > partitions; 
    int res = CheckifRelsExist(relNames, numToJoin, partitions); 
    // printPartitions(partitions); 
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
    // printf(" Total number of tuples after cross product = %ld \n", numOfTuples);  
    // ll interNumOfTuples = numOfTuples;  
    struct AndList* currParsing = parseTree; 
    double fract = 1.0; 
    while(currParsing != nullptr){
        double result = processOrlist(1, currParsing->left, partitions); 
        // cout<< "result val: " << result <<"\n"; 
        fract *= result;
        // cout<< "fract val: " << fract <<"\n"; 
        currParsing = currParsing->rightAnd; 
        // interNumOfTuples *= fract; 
    }
    return getNumOfTuples(partitions, min<double>(1.0, fract)); 
}

void Statistics::PrintStatistics(){
    // map <vector<char*>, map<char*, int> >::iterator it1; 
    // map<char*, int> it2; 
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

double Statistics::getNumOfTuples(vector <vector <char*> > &partitions, double fraction){ 
    double total = fraction; 
    for (int i=0; i<partitions.size(); i++){
        total *= (double)StatisticsTable[partitions[i]]["total"];
        // cout << "in get num Tups " << total <<"--" << total - 2000405<< "--"<<StatisticsTable[partitions[i]]["total"]<<"\n";
    }
    return round(total);
}   

double Statistics::processOrlist(ll numOfinputTuples, struct OrList* myOrlist, vector <vector <char*> > &partitions){
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
        int whichPartition = -1;
        int whichPartition2 = -1; 
        if(temp->left->code == NAME){
            if(temp->right->code != NAME){
                if(temp->code == EQUALS){
                    ll val = checkAndGetAttVal(partitions, temp->left->value); 
                    if(val==-2){
                        cerr << "Invalid CNF for "<<temp->left->value<<"!!\n"; 
                        exit(0); 
                    }
                    // cout << val << " ," << 1.0/(double)val << "\n"; 
                    parsedOrList[temp->left->value].push_back(1.0/(double)val); 
                }else{
                    parsedOrList[temp->left->value].push_back(1.0/3.0); 
                }

            }else{
                if(temp->code == EQUALS){
                    // parsedOrList.find();
                    ll leftVal = checkAndGetAttVal(partitions, temp->left->value);
                    ll rightVal = checkAndGetAttVal(partitions, temp->right->value);  
                    if(leftVal==-2 || rightVal==-2){
                        cerr << "Invalid CNF!! for "<<temp->left->value <<"-"<<temp->right->value<<"\n"; 
                        exit(0); 
                    } 
                    // cout << "Left val: "<<leftVal << "rightVal: " << rightVal <<"\n";   
                    ll minval = min<ll>(leftVal, rightVal);              
                    tempState.push_back(make_pair(temp->left->value,minval)); 
                    tempState.push_back(make_pair(temp->right->value, minval)); 
                    ll max1 = max<ll>(leftVal, rightVal); 
                    // cout << "" 
                    parsedOrList[temp->left->value].push_back( 1.0/(double)max1); 
                    // cout << parsedOrList[temp->left->value].size() << ": size, " << parsedOrList[temp->left->value][0] << "\n";
                }else{
                    parsedOrList[temp->left->value].push_back(1.0/3.0); 
                }

            }
        }else if(temp->right->code == NAME){
            if(temp->code == EQUALS){
                ll val = checkAndGetAttVal(partitions, temp->right->value); 
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
            // cout << itr->second[i] << "**" << tmpfract << " ";  
        }
        // cout << "\n" << tmpfract<<"\n";
        fract *= (1.0-tmpfract); 
    }
    return (1.0-fract); 
}

int Statistics::checkAndGetAttVal(vector <vector <char*> > &partitions, char* attName){
    int val=-2; 
    for(int i=0; i<partitions.size(); i++){
        // cout << "looking in relation " << partitions[i][0] << " for " << attName <<"1\n"; 
        if(StatisticsTable.find(partitions[i]) != StatisticsTable.end() && \
            StatisticsTable[partitions[i]].find(attName)!= StatisticsTable[partitions[i]].end()){
        //    printf("Printing  : %ld\n", StatisticsTable[partitions[i]][attName]);// << " for " << attName <<"\n";
        //    printf("Printing1  : %ld\n", StatisticsTable[partitions[i]][(char*)str.c_str()]);// << " for " << attName <<"\n";
            val = StatisticsTable[partitions[i]][attName]; 
            break;
        }   
    }
    return val; 
}