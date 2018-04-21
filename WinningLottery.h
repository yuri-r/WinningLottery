#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct NumbersTree {
    int number;
    int counter;
    struct NumbersTree* leftNode;
    struct NumbersTree* rightNode;
} NumbersTreeNode;

char** readStoryFile(FILE *file, int *numOfSequences);
FILE* openFile(char *filename);
NumbersTreeNode *newNode(int numberOfNodes);
NumbersTreeNode *sequentialNumbersToBST(int start, int end);
void resetCounters(NumbersTreeNode *numbersTreeRoot);
void deleteTree(NumbersTreeNode *numbersTreeRoot);
bool isNumberAppearsTwice(NumbersTreeNode *numbersTreeRoot, int number);
int calculateAllCombinations(int numof1DigitNumbers, int numOf2DigitNumbers, int numOfSequences);
void findCombinations(char *numberSpots, int start, int end, int *combinationsCounter);
void swap(char *numberSpots, char *numberSpotsNext);
void findLottoSet(char **setOfSequences, int numOfSequences, NumbersTreeNode *numbersTreeRoot);
void print2DUtil(NumbersTreeNode *numbersTreeRoot, int space);
void inorder(NumbersTreeNode *numbersTreeRoot);

#define MAX_ALLOWED_NUMBERS 14
#define MAX_TICKET_NUMBERS 7


static char combinationsBuffer[35][MAX_TICKET_NUMBERS+1];

