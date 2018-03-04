/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <time.h>
#include <stdlib.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

static int failCount = 0;

void assertEqual(int a, int b){
	static int caseNum = 1;
	if(a == b){
		printf("Test case #%d: PASS\n", caseNum);
	}
	else{
		printf("Test case #%d: FAIL\n", caseNum);
		failCount++;
	}
	caseNum++;
}


void finalTestResult(){
	if(failCount == 0){
		printf("TEST SUCCESSFULLY COMPLETED\n");
	}
	else{
		printf("TEST FAILED with %d cases failing\n", failCount);
	}
}


int main() {
	srand(time(NULL));
	int seed;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};
    struct gameState G1, G2;
    int handPos = 0;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int bonus = 0;
    int p;


    printf("TESTING village card:\n");

    for(p = 0; p < numPlayer; p++){
	    memset(&G1, 23, sizeof(struct gameState));   // clear the game state
	    memset(&G2, 23, sizeof(struct gameState));   // clear the game state
		//randomized game seed
		seed = rand() % 10000;
		initializeGame(numPlayer, k, seed, &G1); // initialize a new game
		memcpy(&G2, &G1, sizeof(struct gameState)); // make a copy of the game state for later comparison

		int curPlayer = whoseTurn(&G1);
		int otherPlayer;
		if(curPlayer == 0)
			otherPlayer = 1;
		else
			otherPlayer = 0;

		printf("Playing village for player %d\n", curPlayer);
		cardEffect(village, choice1, choice2, choice3, &G1, handPos, &bonus);

		printf("Player %d should gain 1 card\n", curPlayer);
		assertEqual(G1.handCount[curPlayer], G2.handCount[curPlayer]); //+0 because village is discarded too

		printf("Player %d should also gain 2 actions\n", curPlayer);
		assertEqual(G1.numActions, G2.numActions+2);
		

		printf("Check if the 1 card came from the right deck\n");
		assertEqual(G1.deckCount[curPlayer], G2.deckCount[curPlayer]-1);
		assertEqual(G1.deckCount[otherPlayer], G2.deckCount[otherPlayer]);
		assertEqual(G1.supplyCount[province], G2.supplyCount[province]);
		assertEqual(G1.supplyCount[duchy], G2.supplyCount[duchy]);
		assertEqual(G1.supplyCount[estate], G2.supplyCount[estate]);
		assertEqual(G1.supplyCount[curse], G2.supplyCount[curse]);
		assertEqual(G1.supplyCount[adventurer], G2.supplyCount[adventurer]);
		assertEqual(G1.supplyCount[council_room], G2.supplyCount[council_room]);
		assertEqual(G1.supplyCount[feast], G2.supplyCount[feast]);
		assertEqual(G1.supplyCount[gardens], G2.supplyCount[gardens]);
		assertEqual(G1.supplyCount[mine], G2.supplyCount[mine]);
		assertEqual(G1.supplyCount[remodel], G2.supplyCount[remodel]);
		assertEqual(G1.supplyCount[smithy], G2.supplyCount[smithy]);
		assertEqual(G1.supplyCount[village], G2.supplyCount[village]);
		assertEqual(G1.supplyCount[baron], G2.supplyCount[baron]);
		assertEqual(G1.supplyCount[great_hall], G2.supplyCount[great_hall]);

		printf("Make sure other player didn't also gain cards\n");
		assertEqual(G1.handCount[otherPlayer], G2.handCount[otherPlayer]);
	}




    finalTestResult();

    return 0;
}
