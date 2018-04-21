/* WhinningLottery.c
This programm checks the string sequnces of numbers if they suitable for lotto ticket pick
Created by Yuri Rootshtein
*/

#include "WinningLottery.h"

const int firstLottoNumber = 1;
const int lastLottoNumber = 59;
const int maxFileSize = 1500; //the supported file size (intentialy higher than 1K)
int winNumbers[MAX_TICKET_NUMBERS];

int main(int argc, char* argv[]) {
    char *filename = argv[1];
    FILE *file = openFile(filename);
    if (file == NULL) {
        return -1;
    }

    int numOfSequences;
    char **setOfSequences = readStoryFile(file, &numOfSequences);
    fclose(file);
    if (numOfSequences == 0) {
        printf("The file is empty\n");
        return -1;
    }
 
    NumbersTreeNode *numbersTreeRoot = sequentialNumbersToBST(firstLottoNumber, lastLottoNumber);
    findLottoSet(setOfSequences, numOfSequences, numbersTreeRoot);

    deleteTree(numbersTreeRoot);
    numbersTreeRoot = NULL;

    for (int i = 0; i < numOfSequences; i++) {
        if (setOfSequences[i] != NULL) {
            free(setOfSequences[i]);
        }
    }
    if (setOfSequences != NULL) {
        free(setOfSequences);
    }
    return 0;
}

int calculateAllCombinations(int numof1DigitNumbers, int numOf2DigitNumbers, int numOfSequences) {
    char template[MAX_TICKET_NUMBERS+1];
    int combinationsCounter = 0;
    int  i,j;

    for (i = 0 ; i < numof1DigitNumbers; i++) {
        template[i] = '1';        
    }

    for (j = 0; j < numOf2DigitNumbers; j++) {
        template[j+i] = '2';        
    }

    template[j+i] = '\0';    

    if ((numOf2DigitNumbers * numof1DigitNumbers) == 0) {
        combinationsCounter = 1;
        strcpy(combinationsBuffer[1], template);
    }

    findCombinations(template, 0, MAX_TICKET_NUMBERS - 1, &combinationsCounter);
    return combinationsCounter;
}

void findCombinations(char *template, int start, int end, int *combinationsCounter) {
    if (start == end) {
        strcpy(combinationsBuffer[*combinationsCounter], template);
        (*combinationsCounter)++;
        return;
    } else { 

        for (int i = start; i <= end; i++) {
            swap((template + start), (template + i));
            findCombinations(template, start + 1, end, combinationsCounter);
            swap((template + start), (template + i));
            for(;template[i] == template[i+1]; i++) {} // skip duplications
        }
    }   
}

void swap(char *template, char *templateNextChar) {
    char temp = *template;
    *template = *templateNextChar;
    *templateNextChar = temp;
}

/* checks if thee current sequnce of the number can be lotto ticket pick */ 
void findLottoSet(char **setOfSequences, int numOfSequences, NumbersTreeNode *numbersTreeRoot) {
    for (int sequenceIndex = 0; sequenceIndex < numOfSequences; sequenceIndex++) {
        int arrayLength = strlen(setOfSequences[sequenceIndex]);
        int arrayOfNumbers[arrayLength];

        /*converts string of numbers to array of integers*/
        for (int i = 0; i < arrayLength; i++) {
            arrayOfNumbers[i] = setOfSequences[sequenceIndex][i] != '0' ? setOfSequences[sequenceIndex][i] - '0' : 0;
        }

        int numOf2DigitNumbers = arrayLength - MAX_TICKET_NUMBERS;
        int numOf1DigitNumbers = MAX_TICKET_NUMBERS * 2 - arrayLength;
        int numOfCombinations = calculateAllCombinations(numOf1DigitNumbers, numOf2DigitNumbers, numOfSequences);

        bool foundNumber = false;
        int j = 0, k = 0;

        for (int i = 0; i < numOfCombinations; i++) {
            bool isGoodNumber = true;
            for (j = 0, k = 0; j < MAX_TICKET_NUMBERS; j++, k++) {
                if (combinationsBuffer[i][j] == '1') {
                    winNumbers[j] = arrayOfNumbers[k]; 
                } else {
                    winNumbers[j] = arrayOfNumbers[k] * 10 + arrayOfNumbers[++k];
                }

                if (winNumbers[j] > lastLottoNumber || winNumbers[j] == 0 || isNumberAppearsTwice(numbersTreeRoot, winNumbers[j])) {
                    isGoodNumber = false;
                    break;
                }     
            }
            if (isGoodNumber == true) {
                foundNumber = true;
                printf("\n%s -> ", setOfSequences[sequenceIndex]);
                for (int i = 0; i < MAX_TICKET_NUMBERS; i++) {
                    printf("%d ", winNumbers[i]);
                }
                printf("\n");
            }
            
            if (j == 0 && winNumbers[j] == 0) {  // if the first number is 0 stop checking the rest combinations and continue to the next sequence
                break;
            }
            resetCounters(numbersTreeRoot);
        }
    }
    return;
}

char** readStoryFile(FILE *file, int *numOfSequences) {
    int positionCounter = 0;
    char buffer[MAX_ALLOWED_NUMBERS];
    int numOfSequencesCounter = 0;
    char **setOfSequences = NULL;

    int fileScanner = (char)fgetc(file);
    do {
        if (fileScanner > 47 && fileScanner < 58 && positionCounter < MAX_ALLOWED_NUMBERS) {   // ascii 0<=fileScanner<=9
            buffer[positionCounter] = fileScanner;
        } else if ((positionCounter > MAX_TICKET_NUMBERS - 1) &&  positionCounter < MAX_ALLOWED_NUMBERS) {
                
            if (numOfSequencesCounter == 0){
                setOfSequences = malloc(sizeof(char*));
            } else {
                setOfSequences = realloc(setOfSequences, sizeof(char*) * (numOfSequencesCounter + 1));
            }

            buffer[positionCounter] = '\0';                
            setOfSequences[numOfSequencesCounter] = malloc(sizeof(char) * (MAX_ALLOWED_NUMBERS));
            strcpy(setOfSequences[numOfSequencesCounter], buffer);
             
            numOfSequencesCounter++;
            positionCounter = 0;
        } else {
            positionCounter = 0;
            continue;
        }
 
        positionCounter++;
    } while ((fileScanner=fgetc(file)) != EOF);

    *numOfSequences = numOfSequencesCounter;
    return setOfSequences;
}

FILE *openFile(char *filename){
    FILE *file;  
    file = fopen(filename, "r");
    if (file == NULL) {
         printf("Failed to open file. Usage WhinningLottery <file>  (max file size 1K)\n");
         return NULL;       
    }

    fseek(file, 0L, SEEK_END);
    if (ftell(file) > maxFileSize) {
        printf("The file too big. The max file size is 1k\n");       
        return NULL;
    }

    fseek(file, 0L, SEEK_SET);

    return file;
}

/* this function checks if the number appears more than once in the sequence*/
bool isNumberAppearsTwice(NumbersTreeNode *numbersTreeRoot, int number) {
    if (numbersTreeRoot == NULL) { 
        return false;
    }

    if (number == numbersTreeRoot->number) {
        if (numbersTreeRoot->counter == 0) {
            numbersTreeRoot->counter++;
            return false;
        }
        return true;
    }

    if (number < numbersTreeRoot->number) {
        return isNumberAppearsTwice(numbersTreeRoot->leftNode, number);
    } else {
        return isNumberAppearsTwice(numbersTreeRoot->rightNode, number);            
    }
}

NumbersTreeNode *newNode(int lottoNumber) {    
    NumbersTreeNode *newNode = (NumbersTreeNode*) malloc(sizeof(NumbersTreeNode));
    newNode->number = lottoNumber;
    newNode->counter = 0;
    newNode->leftNode = NULL;
    newNode->rightNode = NULL;
    return newNode;
}

/* this function builts balanced BST on the app startup to store and count the number apeareances in each sequence
each node keeps a lotto number (from 1 to 59) and counter */

NumbersTreeNode *sequentialNumbersToBST(int start, int end) {
    if (start > end) {
        return NULL;
    }

    int middle = (start+end)/2;
    NumbersTreeNode *numbersTreeRoot = newNode(middle);
    numbersTreeRoot->leftNode = sequentialNumbersToBST(start, middle - 1);
    numbersTreeRoot->rightNode = sequentialNumbersToBST(middle + 1, end);

    return numbersTreeRoot;
}

void resetCounters(NumbersTreeNode *numbersTreeRoot) {
    if (numbersTreeRoot == NULL) {
        return;
    }

    if (numbersTreeRoot->leftNode != NULL) {
        numbersTreeRoot->leftNode->counter = 0;
        resetCounters(numbersTreeRoot->leftNode);
    }

    if (numbersTreeRoot->rightNode != NULL) {
        numbersTreeRoot->rightNode->counter = 0;
        resetCounters(numbersTreeRoot->rightNode);
    }
}

void deleteTree(NumbersTreeNode *numbersTreeRoot) 
{
    if (numbersTreeRoot == NULL) {
        return;
    }
    deleteTree(numbersTreeRoot->leftNode);
    deleteTree(numbersTreeRoot->rightNode);
    free(numbersTreeRoot);
} 