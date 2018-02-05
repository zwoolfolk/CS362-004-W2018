/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
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
    int p;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};
    struct gameState G;


    printf("TESTING gainCard():\n");

    for(p = 0; p < numPlayer; p++){
	    memset(&G, 23, sizeof(struct gameState));   // clear the game state
		//randomized game seed
		seed = rand() % 10000;
		initializeGame(numPlayer, k, seed, &G); // initialize a new game

		G.supplyCount[curse] = 0;
		printf("Initialized curse card supply to 0\n");
		//player, deck, and discard should not be able to gain cards when there are none in the supply
		assertEqual(gainCard(curse, &G, 0, p), -1);
		assertEqual(gainCard(curse, &G, 1, p), -1);
		assertEqual(gainCard(curse, &G, 2, p), -1);

		printf("Set curse card supply to 10\n");
		G.supplyCount[curse] = 10;
		int expectedCurseCount = 10;

		int currentDeckCount = G.deckCount[p];
		printf("Testing add card to deck\n");
		gainCard(curse, &G, 1, p);
		expectedCurseCount--;
		//deck should gain 1 card
		assertEqual(currentDeckCount+1, G.deckCount[p]);

		int currentHandCount = G.handCount[p];
		printf("Testing add card to hand\n");
		gainCard(curse, &G, 2, p);
		expectedCurseCount--;
		//hand should gain 1 card
		assertEqual(currentHandCount+1, G.handCount[p]);

		int currentDiscardCount = G.discardCount[p];
		printf("Testing add card to discard\n");
		gainCard(curse, &G, 0, p);
		expectedCurseCount--;
		//discard should gain 1 card
		assertEqual(currentDiscardCount+1, G.discardCount[p]);

		printf("Checking expected final curse supply count\n");
		//check final curse supply count
		assertEqual(expectedCurseCount, G.supplyCount[curse]);
	}


    finalTestResult();

    return 0;
}
