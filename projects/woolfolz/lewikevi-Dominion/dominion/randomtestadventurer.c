/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
 *      gcc -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
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
#include <math.h>
#define TEST_ITERATIONS 1000

enum FAIL_TYPE{CARD_EFFECT_FAIL, CARD_GAIN_FAIL, GAINED_CARDS_ARE_TREASURE, DECK_COUNT_FAIL};
static int failTracker[4] = {0,0,0,0};
static int failCount = 0;
static int totalCases = 0;


void assertEqual(int a, int b, int failType){
	if(a != b){
		failCount++;
		failTracker[failType] = 1;
		totalCases++;
	}
	else{
		totalCases++;
	}
}


void finalTestResult(){
	if(failCount == 0){
		printf("TEST SUCCESSFULLY COMPLETED: ALL CASES IN %d ITERATIONS PASSED\n", TEST_ITERATIONS);
	}
	else{
		printf("TEST FAILED WITH %d CASES FAILED OF %d\n", failCount, totalCases);
		if(failTracker[CARD_EFFECT_FAIL] == 1)
			printf("CARD_EFFECT_FAIL was triggered!\n");
		if(failTracker[CARD_GAIN_FAIL] == 1)
			printf("CARD_GAIN_FAIL was triggered!\n");
		if(failTracker[GAINED_CARDS_ARE_TREASURE] == 1)
			printf("GAINED_CARDS_ARE_TREASURE was triggered!\n");
		if(failTracker[DECK_COUNT_FAIL] == 1)
			printf("DECK_COUNT_FAIL was triggered!\n");
	}
}


void adventurerTest(struct gameState* origG){
	struct gameState newG;
	memset(&newG, 23, sizeof(struct gameState));   // clear the game state
	int i;
	int bonus = 0;
	int card;
	int curPlayer = whoseTurn(origG);
	int otherPlayer;
	if(curPlayer == 0)
		otherPlayer = 1;
	else
		otherPlayer = 0;


	//make copy of original game state for later comparison
	memcpy(&newG, origG, sizeof(struct gameState));


	//call cardEffect and get result (nonzero return is error)
	int result = cardEffect(adventurer, 0, 0, 0, &newG, 0, &bonus);

	//check if card effect returned successfully
	assertEqual(result, 0, CARD_EFFECT_FAIL);

	//player should gain 2 cards
	assertEqual(newG.handCount[curPlayer], origG->handCount[curPlayer]+2, CARD_GAIN_FAIL);

	//The cards gained should be treasure
	int newTreasure = 0;
	for(i = 0; i < newG.handCount[curPlayer]; i++){
		card = newG.hand[curPlayer][i];
		if(card == copper || card == silver || card == gold){
			newTreasure++;
		}
	}
	int oldTreasure = 0;
	for(i = 0; i < origG->handCount[curPlayer]; i++){
		card = origG->hand[curPlayer][i];
		if(card == copper || card == silver || card == gold){
			oldTreasure++;
		}
	}
	assertEqual(newTreasure, oldTreasure+2, GAINED_CARDS_ARE_TREASURE);

	//check if the 2 cards came from the right deck
	assertEqual(origG->deckCount[curPlayer]-2, newG.deckCount[curPlayer], DECK_COUNT_FAIL);
	assertEqual(origG->deckCount[otherPlayer], newG.deckCount[otherPlayer], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[province], newG.supplyCount[province], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[duchy], newG.supplyCount[duchy], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[estate], newG.supplyCount[estate], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[curse], newG.supplyCount[curse], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[adventurer], newG.supplyCount[adventurer], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[council_room], newG.supplyCount[council_room], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[feast], newG.supplyCount[feast], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[gardens], newG.supplyCount[gardens], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[mine], newG.supplyCount[mine], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[remodel], newG.supplyCount[remodel], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[smithy], newG.supplyCount[smithy], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[village], newG.supplyCount[village], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[baron], newG.supplyCount[baron], DECK_COUNT_FAIL);
	assertEqual(origG->supplyCount[great_hall], newG.supplyCount[great_hall], DECK_COUNT_FAIL);

	//make sure other player didn't also gain cards
	assertEqual(origG->handCount[otherPlayer], newG.handCount[otherPlayer], DECK_COUNT_FAIL);

}


int main() {
	srand(time(NULL));
    struct gameState G;
    int i, n;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
        remodel, smithy, village, baron, great_hall};


    printf("RANDOM TESTING %d ITERATIONS OF adventurer card:\n", TEST_ITERATIONS);

    for(i = 0; i < TEST_ITERATIONS; i++){
    	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    	int seed = (rand() % 10000) + 1;

    	//initialize the game state
    	initializeGame(numPlayer, k, seed, &G); // initialize a new game


		//randomize player
        int player = floor(Random() * numPlayer);
        G.deckCount[player] = rand() % MAX_DECK;
        G.discardCount[player] = rand() % MAX_DECK;
        G.handCount[player] = 0;

        //make sure there are treasure cards in the deck
        for(n = 0; n < 5; n++){
        	int r = rand() % 3;
        	if(r == 0)
        		G.deck[player][n] = copper;
        	if(r == 1)
        		G.deck[player][n] = silver;
        	if(r == 2)
        		G.deck[player][n] = gold;
        }
		


		adventurerTest(&G);
    }



    finalTestResult();

    return 0;
}
