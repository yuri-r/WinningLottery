# WinningLottery

Solution for “Winning Ticket!” implemented in C

To compile on Linux:

	cd  WinningLottery
	gcc -o WinningLottery WinningLottery.c


The solution consist of 4 sections:

	1. Loading files and building data structures.
	2. Reading series of numbers from a text file.
	3. Calculating permutations for all possible positions of 2-digits and 1-digit numbers in the set of 7 lottery ticket pick and building template of numbers 
   	   positions.
	4. Iterating over the list of series of numbers and applying all the templates per each number 


Loading files and building data structures.

	On the startup the function    NumbersTreeNode *sequentialNumbersToBST(int start, int end)
	builds balanced BST when each node consists of number (from 1 to 59), the counter wich set to 
	0, and two pointers (left, right). The BST serves for detection of number duplications
	
  
Reading numbers from file 

	The executable WinningLottery gets the text file as a cli parameter (up to 1KB) :
	
		./WinningLottery story.txt 

	The text file may contain any sequence of any characters (the test file is included). 
            
	The function     char** readStoryFile(FILE *file, int *numOfSequences) 
	extracts the subsequent numbers characters while the 6 < length < 15 and builds dynamic array 	 of numbers sequences. 

	
Calculating permutations

	The function  int calculateAllCombinations(int numof1DigitNumbers, int 
                                               numOf2DigitNumbers,  
                                               int numOfSequences, int *numOfCombinations)

	calculates all possible permutation of 1 and 2 (for 1-digit and 2-digits numbers accordingly)
	for each numbers sequence in 7 positions and excludes duplication of the templates. 


	For example for sequence 12345678 the function returns array of combinations:

	1111112
	1111121
	1111211
	1112111
	1121111
	1211111
	2111111

	
Iterating over the list of series of numbers and applying all the templates

	The function   void findLottoSet(char **setOfSequences, int numOfSequences, NumbersTreeNode *numbersTreeRoot)

  applies all the templates on each numbers sequence and check the validity as a lottery ticket pick.
	Each number is checked for duplication in BST, if counter of the node is 0, than the counter is incremented otherwise
  there is duplication and the numbers sequence is not a lottery pick.
	Each number is checked for if it’s not 0.
	Each 2-digit number is checked if it’s < 60.

