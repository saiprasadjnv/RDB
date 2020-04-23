
#include <iostream>
#include "ParseTree.h"
#include "Statistics.h"
#include "QueryOptimizer.h" 
#include <stdlib.h>
#include <stdio.h>
// #include "y.tab.c"
using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

extern struct TableList* tables; 
extern struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
extern struct AndList *boolean; // the predicate in the WHERE clause
extern struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
extern struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
extern int distinctFunc;  // 1

void printFunction(struct FuncOperator *currFunction){
	if(currFunction == NULL){ 
		return; 
	}
	printFunction(currFunction->leftOperator); 
	cout << "code " << finalFunction->code << " ";
	if(finalFunction->leftOperand != NULL){
		cout << " ,left Operand " << finalFunction->leftOperand->code << " : " << finalFunction->leftOperand->value << "\n"; 
	} 
		
	printFunction(currFunction->right); 
}

void printAndList(struct AndList *currAndlist){
	while(currAndlist != NULL){
		struct OrList* currOrlst = currAndlist->left; 
		while(currOrlst != NULL){
			cout << "Left Operand: " << currOrlst->left->left->value << " code: " << currOrlst->left->code << " Right Operand: " << currOrlst->left->right->value << "; ";   
			currOrlst = currOrlst->rightOr; 
		}
		cout << "\n"; 
		currAndlist = currAndlist->rightAnd; 
	}
}


int main () {

	yyparse(); 
	// struct TableList* currTable = tables;
	// cout << "\nPrinting table: \n";
	// while (currTable != NULL)
	// {
	// 	/* code */
	// 	cout << currTable->tableName << " alias: " << currTable->aliasAs << " "; 
	// 	currTable = currTable->next; 
	// }
	// cout << "\n"; 

	// cout << "Printing function: \n"; 
	// printFunction(finalFunction); 
	// cout << "\n";
	// cout << "Printing final And List: \n"; 
	// printAndList(boolean);  
    // cout <<"\n";
	// struct NameList *currgroupingAtts=groupingAtts;
	// cout << "Printing groupingAtts:\n ";
	// while(currgroupingAtts!=NULL){
	// 	cout << currgroupingAtts->name <<",";
	// 	currgroupingAtts=currgroupingAtts->next;
	// }
	// cout << "\n";
	// cout<< "Printing selection attributes: \n";
	// struct NameList *currattsTosel=attsToSelect;
	// while (currattsTosel!=NULL){
	// 	cout << currattsTosel->name<<",";
	// 	currattsTosel=currattsTosel->next;
	// }
	// cout << "\n";
	// cout << "DistinctAtts: " << distinctAtts << " DistinctFunctions: " << distinctFunc << "\n";
	
	QueryOptimizer myQO; 
	myQO.optimizeQuery();
	myQO.printQueryPlanTree();
	// myQO.PrintMaps(); 
	return 0;
}


