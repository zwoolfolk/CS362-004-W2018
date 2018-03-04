/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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
    int i;
    int total_card_types = 27;
	int all_cards[27] = {curse,estate,duchy,province,copper,silver,gold,adventurer,council_room,feast,gardens,mine,remodel,smithy,village,baron,great_hall,minion,steward,tribute,ambassador,cutpurse,embargo,outpost,salvager,sea_hag,treasure_map};
	int expected_card_cost[27] = {0, 2, 5, 8, 0, 3, 6, 6, 5, 4, 4, 5, 4, 4, 3, 4, 3, 5, 3, 5, 3, 4, 2, 5, 4, 4, 4};


    printf("TESTING getCost():\n");
    printf("Testing getCost() vs expected cost for every possible card:\n");
   	for(i = 0; i < total_card_types; i++){
   		int cost = getCost(all_cards[i]);
   		assertEqual(cost, expected_card_cost[i]);
   		printf("getCost() for enum #%d = %d, expected cost = %d\n", i, cost, expected_card_cost[i]);
   	}

    printf("Testing getCost() for invalid card type numbers:\n");
   	for(i = 0; i < 100; i++){
   		int r = rand() % 100000 + total_card_types;
   		int cost = getCost(r);
   		assertEqual(cost, -1);
   		printf("getCost() for enum #%d = %d, expected cost = -1\n", r, cost);
   	}





    finalTestResult();

    return 0;
}
