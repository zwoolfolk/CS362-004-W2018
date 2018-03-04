/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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
    int i;
    int numPlayer = 2;
    int p, sameOrderCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall};
    struct gameState G;


    printf("TESTING shuffle():\n");
    for (p = 0; p < numPlayer; p++)
    {
	    memset(&G, 23, sizeof(struct gameState));   // clear the game state
		//randomized game seed
		seed = rand() % 10000;
		initializeGame(numPlayer, k, seed, &G); // initialize a new game

		G.deckCount[p] = 0;
		printf("Initialized player %d to 0 deck count\n", p);
		//should not shuffle if player has 0 cards (i.e. should return -1)
		assertEqual(shuffle(p, &G), -1);

		printf("Setting player %d to 10 deck count\n", p);
		G.deckCount[p] = 10;
		shuffle(p, &G);
		//player should still have 10 cards after shuffling
		assertEqual(G.deckCount[p], 10);

		sameOrderCount = 0;

		for(i = 0; i < 10; i++){
			int first_card_order = G.deck[p][i];
			shuffle(p, &G);
			int shuffled_card_order = G.deck[p][i];
			if(first_card_order == shuffled_card_order)
				sameOrderCount++;
		}

		if(sameOrderCount == 10){
			printf("FAIL: None of the 10 cards were shuffled!\n");
			assertEqual(0,1);
		}
		else{
			printf("10 cards successfully shuffled!\n");
			assertEqual(0,0);
		}

		printf("Setting player %d to 100 deck count\n", p);
		G.deckCount[p] = 100;
		shuffle(p, &G);
		//player should still have 10 cards after shuffling
		assertEqual(G.deckCount[p], 100);

		sameOrderCount = 0;

		for(i = 0; i < 100; i++){
			int first_card_order = G.deck[p][i];
			shuffle(p, &G);
			int shuffled_card_order = G.deck[p][i];
			if(first_card_order == shuffled_card_order)
				sameOrderCount++;
		}

		if(sameOrderCount == 100){
			printf("FAIL: None of the 100 cards were shuffled!\n");
			assertEqual(0,1);
		}
		else{
			printf("100 cards successfully shuffled!\n");
			assertEqual(0,0);
		}
   	}





    finalTestResult();

    return 0;
}
