#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "listlib.c"
#include "vectorlib.c"

//DEBUG PRINTFLAG
void printFlag(int flag[4][13]) {
    printf("%d %d %d %d %d %d %d %d %d %d %d %d %d || %d %d %d %d %d %d %d %d %d %d %d %d"
           " %d || %d %d %d %d %d %d %d %d %d %d %d %d %d || %d %d %d %d %d %d %d %d %d %d %d %d %d\n\n",
           flag[0][0], flag[0][1], flag[0][2], flag[0][3], flag[0][4], flag[0][5], flag[0][6], flag[0][7], flag[0][8],
           flag[0][9], flag[0][10], flag[0][11], flag[0][12],
           flag[1][0], flag[1][1], flag[1][2], flag[1][3], flag[1][4], flag[1][5], flag[1][6], flag[1][7], flag[1][8],
           flag[1][9], flag[1][10], flag[1][11], flag[1][12],
           flag[2][0], flag[2][1], flag[2][2], flag[2][3], flag[2][4], flag[2][5], flag[2][6], flag[2][7], flag[2][8],
           flag[2][9], flag[2][10], flag[2][11], flag[2][12],
           flag[3][0], flag[3][1], flag[3][2], flag[3][3], flag[3][4], flag[3][5], flag[3][6], flag[3][7], flag[3][8],
           flag[3][9], flag[3][10], flag[3][11], flag[3][12]);
}

//RULES FUNCTION
void rules() {
    printf("\n\nThis is a game of poker, in the '5 Card Draw' variant.\n"
           "There can be up to 6 players, each one receives 5 cards, there are no community cards in this version.\n"
           "If you don't like some or all of your cards, you can change them once.\n"
           "Each player also receives 100 fiches to begin with, when you run out of fiches, you're out.\n"
           "Commands to play the game:\n"
           "c --> check\n"
           "f --> fold\n"
           "r --> raise\n"
           "The rest of the rules are like classic poker. Enjoy yourselves!!\n\n");
}

//region variables
int flag[4][13] = {0};
int const zero[4][13] = {0};
int rng_seed;
int rng_value;
int nSeed = 4;
int nCardsPerSeed = 13;
char *seedNames[4] = {"Spades", "Hearts", "Clubs", "Diamonds"};
int nplayers;
int call = 1;
int raise = 0;
int tableBet;
//endregion

//region basic functions
void assignCards(int deck[4][13], struct node_player *player, int nplayers) {
    for (int l = 0; l < nplayers; l++) {
        for (int i = 0, j = 0; i < 5; i++) {
            rng_seed = rand() % nSeed;
            rng_value = rand() % nCardsPerSeed;
            while (flag[rng_seed][rng_value] != 0) {
                rng_seed = rand() % nSeed;
                rng_value = rand() % nCardsPerSeed;
            }
            flag[rng_seed][rng_value] = 1;
            player->p.cardSeed[j] = rng_seed + 1;
            player->p.cardValue[j] = rng_value + 1;
            j++;
        }
        player = player->nextNodo;
    }
}

void printcards(struct node_player nodePlayer, int position) {
    while (nodePlayer.previousNodo != NULL) {
        nodePlayer = *nodePlayer.previousNodo;
    }
    for (int j = 1; j < position; j++) {
        if (nodePlayer.nextNodo != NULL) {
            nodePlayer = *nodePlayer.nextNodo;
        }
    }
    printf("Player %d, your cards are:\n\n", nodePlayer.p.value);
    for (int i = 0; i < 5; i++) {
        if (nodePlayer.p.cardValue[i] > 1 && nodePlayer.p.cardValue[i] < 11) {
            printf("%d of %s\n", nodePlayer.p.cardValue[i], seedNames[nodePlayer.p.cardSeed[i] - 1]);
        } else if (nodePlayer.p.cardValue[i] == 1) {
            printf("Ace of %s\n", seedNames[nodePlayer.p.cardSeed[i] - 1]);
        } else if (nodePlayer.p.cardValue[i] == 11) {
            printf("Jack of %s\n", seedNames[nodePlayer.p.cardSeed[i] - 1]);
        } else if (nodePlayer.p.cardValue[i] == 12) {
            printf("Queen of %s\n", seedNames[nodePlayer.p.cardSeed[i] - 1]);
        } else {
            printf("King of %s\n", seedNames[nodePlayer.p.cardSeed[i] - 1]);
        }
    }
    printf("\n");
}

void createList(struct node_player *player1, struct node_player *player2, struct node_player *player3,
                struct node_player *player4,
                struct node_player *player5, struct node_player *player6, struct node_player *dummy, int nplayers) {
    if (nplayers == 2) {
        player1->previousNodo = NULL;
        player1->nextNodo = player2;
        player2->previousNodo = player1;
        player2->nextNodo = player3;
        player3->previousNodo = player2;
        player3->nextNodo = NULL;
    } else if (nplayers == 3) {
        player1->previousNodo = NULL;
        player1->nextNodo = player2;
        player2->previousNodo = player1;
        player2->nextNodo = player3;
        player3->previousNodo = player2;
        player3->nextNodo = player4;
        player4->previousNodo = player3;
        player4->nextNodo = NULL;
    } else if (nplayers == 4) {
        player1->previousNodo = NULL;
        player1->nextNodo = player2;
        player2->previousNodo = player1;
        player2->nextNodo = player3;
        player3->previousNodo = player2;
        player3->nextNodo = player4;
        player4->previousNodo = player3;
        player4->nextNodo = player5;
        player5->previousNodo = player4;
        player5->nextNodo = NULL;
    } else if (nplayers == 5) {
        player1->previousNodo = NULL;
        player1->nextNodo = player2;
        player2->previousNodo = player1;
        player2->nextNodo = player3;
        player3->previousNodo = player2;
        player3->nextNodo = player4;
        player4->previousNodo = player3;
        player4->nextNodo = player5;
        player5->previousNodo = player4;
        player5->nextNodo = player6;
        player6->previousNodo = player5;
        player6->nextNodo = NULL;
    } else if (nplayers == 6) {
        player1->previousNodo = NULL;
        player1->nextNodo = player2;
        player2->previousNodo = player1;
        player2->nextNodo = player3;
        player3->previousNodo = player2;
        player3->nextNodo = player4;
        player4->previousNodo = player3;
        player4->nextNodo = player5;
        player5->previousNodo = player4;
        player5->nextNodo = player6;
        player6->previousNodo = player5;
        player6->nextNodo = dummy;
        dummy->previousNodo = player6;
        dummy->nextNodo = NULL;
    }
}

void ante(struct node_player *player, int nplayers, int *tableBet) {
    for (int i = 0; i < nplayers; i++) {
        player->p.money--;
        player->p.currentBet++;
        player = player->nextNodo;
    }
    *tableBet = nplayers;
}

void shuffleDeck(int deck[4][13]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck[i][j] = 0;
        }
    }
}

void nextNode(struct node_player *player, int *isLastPlayer) {
    while (player->nextNodo != NULL && player->p.hasFolded == 1) {
        player = player->nextNodo;
    }
    if (player->nextNodo == NULL) {
        *isLastPlayer = 1;
    }
}

void betting(struct node_player *player, int nplayers, int *tableBet, int *call, int *raise) {
    int isLastPlayer = 0;
    while (player != NULL && isLastPlayer == 0) {
        char answer;
        if (player->p.currentBet < *call && player->p.hasFolded == 0) {
            printf("-------------------------PLAYER %d-------------------------\n", player->p.value);
            printf("Player info:\nMoney:  %d\nCurrent Bet: %d\n", player->p.money, player->p.currentBet);
            printf("Call: %d\n", *call);
            printcards(*player, player->p.value);
            printf("Do you want to call(c), raise(r) or fold (f)?\n");
            scanf("%c*c", &answer);
            fflush(stdin);
            while (answer != 'c' && answer != 'C' && answer != 'r' && answer != 'R' && answer != 'f' && answer != 'F') {
                printf("Incorrect input: type 'c' for call, 'r' for raise or 'f' for fold.\n");
                scanf("%c*c", &answer);
                fflush(stdin);
            }
            if (answer == 'c' || answer == 'C') {
                player->p.currentBet = *call;
                *tableBet = *tableBet + *raise;
                player->p.money = player->p.money - *raise;
                if (player->nextNodo != NULL) {
                    player = player->nextNodo;
                } else { isLastPlayer = 1; }
            } else if (answer == 'r' || answer == 'R') {
                *tableBet = *tableBet + *raise;
                player->p.money = player->p.money - *raise;
                if (player->p.money == 0) {
                    printf("You can't raise while all-in\n");
                    player->p.money = player->p.money + *raise;
                    *tableBet = *tableBet - *raise;
                } else {
                    printf("By how much do you want to raise?\n");
                    scanf("%d*d", raise);
                    fflush(stdin);
                    while (*raise > player->p.money || *raise < 1) {
                        printf("Incorrect input: please enter a positive amount lower than your current money: %d fiches.\n",
                               player->p.money);
                        scanf("%d*d", raise);
                        fflush(stdin);
                    }
                    player->p.money = player->p.money - *raise;
                    int tmpBet = player->p.currentBet;
                    player->p.currentBet = *call + *raise;
                    *call = player->p.currentBet;
                    *tableBet = *tableBet + *raise;
                    *raise = *call - tmpBet;
                    if (player->nextNodo != NULL) {
                        player = player->nextNodo;
                    } else { isLastPlayer = 1; }
                }
            } else {
                player->p.hasFolded = 1;
                if (player->nextNodo != NULL) {
                    player = player->nextNodo;
                } else isLastPlayer = 1;
            }
        } else if (player->p.hasFolded == 0) {
            printf("-------------------------PLAYER %d-------------------------\n", player->p.value);
            printf("Player info:\nMoney:  %d\nCurrent Bet: %d\n", player->p.money, player->p.currentBet);
            printf("Call: %d\n", *call);
            printcards(*player, player->p.value);
            printf("Do you want to check (c) or raise(r)?\n");
            scanf("%c*c", &answer);
            fflush(stdin);
            while (answer != 'c' && answer != 'C' && answer != 'r' && answer != 'R') {
                printf("Incorrect input: type 'c' for check or 'r' for raise.\n");
                scanf("%c*c", &answer);
                fflush(stdin);
            }
            if (answer == 'c' || answer == 'C') {
                if (player->nextNodo != NULL) {
                    player = player->nextNodo;
                } else { isLastPlayer = 1; }
            } else {
                printf("By how much do you want to raise?\n");
                scanf("%d*d", raise);
                fflush(stdin);
                while (*raise > player->p.money || *raise < 1) {
                    printf("Incorrect input: please enter an amount lower than your current money: %d fiches.\n",
                           player->p.money);
                    scanf("%d*d", raise);
                    fflush(stdin);
                }
                player->p.money = player->p.money - *raise;
                player->p.currentBet = player->p.currentBet + *raise;
                *call = player->p.currentBet;
                *tableBet = *tableBet + *raise;
                if (player->nextNodo != NULL) {
                    player = player->nextNodo;
                } else { isLastPlayer = 1; }
            }
        } else {
            if (player->nextNodo != NULL) {
                player = player->nextNodo;
            } else { isLastPlayer = 1; }
        }
    }
    isLastPlayer = 0;
    //BET SECOND PART
    while (player->previousNodo != NULL) {
        player = player->previousNodo;
    }
    while (isLastPlayer == 0) {
        if (player->p.currentBet < *call && player->p.hasFolded == 0) {
            printf("-------------------------PLAYER %d-------------------------\n", player->p.value);
            printf("Player info:\nMoney:  %d\nCurrent Bet: %d\n", player->p.money, player->p.currentBet);
            printf("Call: %d\n", *call);
            printcards(*player, player->p.value);
            char answer;
            printf("Do you want to call (c) or fold(f)?\n");
            scanf("%c*c", &answer);
            fflush(stdin);
            while (answer != 'c' && answer != 'C' && answer != 'f' && answer != 'F') {
                printf("Incorrect input: type 'c' for call or 'r' for raise.\n");
                scanf("%c*c", &answer);
                fflush(stdin);
            }
            if (answer == 'c' || answer == 'C') {
                player->p.money = player->p.money + player->p.currentBet;
                *tableBet = *tableBet + (*call - player->p.currentBet);
                player->p.currentBet = *call;
                player->p.money = player->p.money - *call;
            } else {
                player->p.hasFolded = 1;
            }
            nextNode(player, &isLastPlayer);
        }
        if (player->nextNodo != NULL) {
            player = player->nextNodo;
        } else {
            isLastPlayer = 1;
        }
    }
}

void swap(struct node_player *player, int nplayers, int flag[4][13]) {
    //FUNZIONE SWAP: scambia una o più carte ai giocatori secondo quanto richiesto
    for (int i = 0; i < nplayers; i++) {
        if (player->p.hasFolded == 0) {
            printcards(*player, player->p.value);
            int swaps;
            char answer;
            struct vector selCards;
            printf("Player %d, do you want to change your cards? Answer y/n\n", player->p.value);
            scanf("%c*c", &answer);
            fflush(stdin);
            while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') {
                printf("Incorrect input: please type 'y' if you want to swap cards, or 'n' if you don't want to.\n");
                scanf("%c*c", &answer);
                fflush(stdin);
            }
            if (answer == 'n' || answer == 'Y') {
                player = player->nextNodo;
            }
            if (answer == 'y' || answer == 'N') {
                create(&selCards, 1);
                printf("How many cards do you want to change?\n");
                scanf("%d*d", &swaps);
                fflush(stdin);
                while (swaps > 5 || swaps < 1) {
                    printf("Please enter an amount between 1 and 5.\n");
                    scanf("%d*d", &swaps);
                    fflush(stdin);
                }
                for (int j = 0; j < swaps; j++) {
                    insertLast(&selCards, j + 1);
                }
                printcards(*player, player->p.value);
                printf("Which cards do you want to change?\nAnswer by typing the order of the cards\n(ex: the first nodeFirstPlayer->p.cardValue is 1, the second is 2 and so on).\n");
                int array[5] = {0};
                for (int j = 0; j < swaps; j++) {
                    scanf("%d*d", &selCards.array[j]);
                    fflush(stdin);
                    while (selCards.array[j] > 5 || selCards.array[j] < 1) {
                        printf("Please enter an amount between 1 and 5.\n");
                        scanf("%d*d", &selCards.array[j]);
                        fflush(stdin);
                    }
                    while (array[selCards.array[j] - 1] == 1) {
                        printf("You already changed this nodeFirstPlayer->p.cardValue, please select another.\n");
                        scanf("%d*d", &selCards.array[j]);
                        fflush(stdin);
                    }
                    array[selCards.array[j] - 1] = 1;
                }
                for (int j = 0; j < swaps; j++) {
                    rng_seed = rand() % nSeed;
                    rng_value = rand() % nCardsPerSeed;
                    while (flag[rng_seed][rng_value] != 0) {
                        rng_seed = rand() % nSeed;
                        rng_value = rand() % nCardsPerSeed;
                    }
                    flag[player->p.cardSeed[selCards.array[j] - 1] - 1][player->p.cardValue[selCards.array[j] - 1] -
                                                                        1] = 0;
                    flag[rng_seed][rng_value] = 1;
                    player->p.cardSeed[selCards.array[j] - 1] = rng_seed + 1;
                    player->p.cardValue[selCards.array[j] - 1] = rng_value + 1;
                }
                printcards(*player, player->p.value);
                player = player->nextNodo;
            }
        } else {
            player = player->nextNodo;
        }
    }
}

//endregion
//region point calculator
int checkRoyalFlush(int card[], int cardSeed[], int royalFlush) {
    if (cardSeed[0] == cardSeed[1] && cardSeed[0] == cardSeed[2] && cardSeed[0] == cardSeed[3] &&
        cardSeed[0] == cardSeed[4]) {
        if (card[0] + card[1] + card[2] + card[3] + card[4] == 47) {
            if (card[0] == 1 || card[1] == 1 || card[2] == 1 || card[3] == 1 || card[4] == 1) {
                royalFlush = 1;
            } else {}
        } else {}
    } else {}
    return royalFlush;
}

int checkStraightFlush(int card[], int cardSeed[], int straightFlush, int *straightValue) {
    if (cardSeed[0] == cardSeed[1] && cardSeed[0] == cardSeed[2] && cardSeed[0] == cardSeed[3] &&
        cardSeed[0] == cardSeed[4]) {
        if ((card[0] + card[1] + card[2] + card[3] + card[4]) % 5 == 0) {
            if (card[0] + card[1] + card[2] + card[3] + card[4] == 15 &&
                card[0] * card[1] * card[2] * card[3] * card[4] == 120) {
                straightFlush = 1;
                *straightValue = 1;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 20 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 720) {
                straightFlush = 1;
                *straightValue = 2;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 25 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 2520) {
                straightFlush = 1;
                *straightValue = 3;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 30 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 6720) {
                straightFlush = 1;
                *straightValue = 4;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 35 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 15120) {
                straightFlush = 1;
                *straightValue = 5;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 40 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 30240) {
                straightFlush = 1;
                *straightValue = 6;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 45 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 55440) {
                straightFlush = 1;
                *straightValue = 7;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 50 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 95040) {
                straightFlush = 1;
                *straightValue = 8;
            } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 55 &&
                       card[0] * card[1] * card[2] * card[3] * card[4] == 154440) {
                straightFlush = 1;
                *straightValue = 9;
            } else {}
        }
    }
    return straightFlush;
}

int checkPoker(int card[], int poker, int *pokerValue) {
    int j;
    for (int i = 1; i < 5; i++) {
        if (card[0] == card[i]) {
            j++;
        } else {}
    }
    if (j == 3) {
        poker = 1;
        *pokerValue = card[0];
    } else if (j == 0) {
        for (int i = 2; i < 5; i++) {
            if (card[1] == card[i]) {
                j++;
            } else {}
        }
        if (j == 3) {
            poker = 1;
            pokerValue = card[1];
        } else {}
    } else {}
    if (*pokerValue == 1) {
        *pokerValue = 15;
    }
    return poker;
}

int checkHouse(int card[], int house, int *tris) {
    int i, j;
    for (i = 1, j = 0; i < 5; i++) {
        if (card[0] == card[i]) {
            j++;
        } else {}
    }
    if (j == 2) {
        if (card[0] != card[1]) {
            for (i = 2, j = 0; i < 5; i++) {
                if (card[1] == card[i]) {
                    j++;
                }
            }
            if (j == 1) {
                house = 1;
                *tris = card[0];
            }
        } else if (card[2] == card[3] || card[2] == card[4]) {
            house = 1;
            *tris = card[0];
        } else if (card[3] == card[4]) {
            house = 1;
            *tris = card[0];
        }
    } else if (j == 1) {
    }
    if (card[0] != card[1]) {
        for (i = 2, j = 0; i < 5; i++) {
            if (card[1] == card[i]) {
                j++;
            }
        }
        if (j == 2 || j == 1) {
        }
    } else {
        for (i = 3, j = 0; i < 5; i++) {
            if (card[2] == card[i]) {
                j++;
            }
        }
        if (j == 2) {
            house = 1;
            *tris = card[2];
        } else if (j == 1) {
        }
    }
    if (*tris == 1) {
        *tris = 15;
    }
    return house;
}

int checkFlush(int cardSeed[], int flush) {
    if (cardSeed[0] == cardSeed[1] && cardSeed[0] == cardSeed[2] && cardSeed[0] == cardSeed[3] &&
        cardSeed[0] == cardSeed[4]) {
        flush = 1;
    }
    return flush;
}

int checkStraight(int card[], int straight, int *straightValue) {
    if (card[0] + card[1] + card[2] + card[3] + card[4] == 47) {
        if (card[0] == 1 || card[1] == 1 || card[2] == 1 || card[3] == 1 || card[4] == 1) {
            if (card[0] * card[1] * card[2] * card[3] * card[4] == 17160) {
                straight = 1;
                *straightValue = 15;
            }
        } else {}
    } else if ((card[0] + card[1] + card[2] + card[3] + card[4]) % 5 == 0) {
        if (card[0] + card[1] + card[2] + card[3] + card[4] == 15 &&
            card[0] * card[1] * card[2] * card[3] * card[4] == 120) {
            straight = 1;
            *straightValue = 1;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 20 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 720) {
            straight = 1;
            *straightValue = 2;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 25 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 2520) {
            straight = 1;
            *straightValue = 3;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 30 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 6720) {
            straight = 1;
            *straightValue = 4;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 35 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 15120) {
            straight = 1;
            *straightValue = 5;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 40 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 30240) {
            straight = 1;
            *straightValue = 6;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 45 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 55440) {
            straight = 1;
            *straightValue = 7;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 50 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 95040) {
            straight = 1;
            *straightValue = 8;
        } else if (card[0] + card[1] + card[2] + card[3] + card[4] == 55 &&
                   card[0] * card[1] * card[2] * card[3] * card[4] == 154440) {
            straight = 1;
            *straightValue = 9;
        }
    }
    return straight;
}

int checkThreeOfAKind(int card[], int threeOfAKind, int *tris) {
    int i, j;
    for (i = 1, j = 0; i < 5; i++) {
        if (card[0] == card[i]) {
            j++;
        }
    }
    if (j == 2) {
        threeOfAKind = 1;
        *tris = card[0];
    } else if (j == 1) {
    } else if (j == 0) {
        for (i = 2, j = 0; i < 5; i++) {
            if (card[1] == card[i]) {
                j++;
            }
        }
        if (j == 2) {
            threeOfAKind = 1;
            *tris = card[1];
        } else if (j == 1) {
        } else if (j == 0) {
            if (card[2] == card[3] && card[2] == card[4]) {
                threeOfAKind = 1;
                *tris = card[2];
            }
        }
    }
    if (*tris == 1) {
        *tris = 15;
    }
    return threeOfAKind;
}

int checkTwoPair(int card[], int twoPair, int *pair_1, int *pair_2) {
    int i, j;
    for (i = 1, j = 0; i < 5; i++) {
        if (card[0] == card[i]) {
            j++;
        }
    }
    if (j == 1) {
        *pair_1 = card[0];
        if (card[0] == card[1]) {
            if (card[2] == card[3] || card[2] == card[4]) {
                *pair_2 = card[2];
                twoPair = 1;
            } else if (card[3] == card[4]) {
                *pair_2 = card[3];
                twoPair = 1;
            }
        } else if (card[0] == card[2]) {
            if (card[1] == card[3] || card[1] == card[4]) {
                *pair_2 = card[1];
                twoPair = 1;
            } else if (card[3] == card[4]) {
                *pair_2 = card[3];
                twoPair = 1;
            }
        } else if (card[0] == card[3]) {
            if (card[1] == card[2] || card[1] == card[4]) {
                *pair_2 = card[1];
                twoPair = 1;
            } else if (card[2] == card[4]) {
                *pair_2 = card[2];
                twoPair = 1;
            }
        } else {
            if (card[1] == card[2] || card[1] == card[3]) {
                *pair_2 = card[1];
                twoPair = 1;
            } else if (card[2] == card[3]) {
                *pair_2 = card[2];
                twoPair = 1;
            }
        }
    } else if (j == 0) {
        for (i = 2, j = 0; i < 5; i++) {
            if (card[1] == card[i]) {
                j++;
            }
        }
        if (j == 1) {
            *pair_1 = card[1];
            if (card[1] == card[2] && card[3] == card[4] || card[1] == card[3] && card[2] == card[4]) {
                *pair_2 = card[4];
                twoPair = 1;
            } else if (card[1] == card[4] && card[2] == card[3]) {
                *pair_2 = card[2];
                twoPair = 1;
            }
        }
    }
    if (*pair_2 > *pair_1) {
        int tmp_pair = *pair_1;
        *pair_1 = *pair_2;
        *pair_2 = tmp_pair;
    }
    return twoPair;
}

int checkPair(int card[], int pair, int *pairValue) {
    if (card[0] == card[1] || card[0] == card[2] || card[0] == card[3] || card[0] == card[4]) {
        pair = 1;
        *pairValue = card[0];
    } else if (card[1] == card[2] || card[1] == card[3] || card[1] == card[4]) {
        pair = 1;
        *pairValue = card[1];
    } else if (card[2] == card[3] || card[2] == card[4]) {
        pair = 1;
        *pairValue = card[2];
    } else if (card[3] == card[4]) {
        pair = 1;
        *pairValue = card[3];
    } else {
    }
    return pair;
}

int checkHighCard(int card[], int *highCard) {
    if (card[0] == 1 || card[1] == 1 || card[2] == 1 || card[3] == 1 || card[4] == 1) {
        *highCard = 15;
    } else if (card[0] > card[1] && card[0] > card[2] && card[0] > card[3] && card[0] > card[4]) {
        *highCard = card[0];
    } else if (card[1] > card[2] && card[1] > card[3] && card[1] > card[4]) {
        *highCard = card[1];
    } else if (card[2] > card[3] && card[2] > card[4]) {
        *highCard = card[2];
    } else if (card[3] > card[4]) {
        *highCard = card[3];
    } else {
        *highCard = card[4];
    }
    printf("\n%d\n", *highCard);
    return *highCard;
}
//endregion

int main() {
    srand(time(0));
    rules();
    //Check number of players

    printf("How many players are there?\n");
    scanf("%d", &nplayers);
    fflush(stdin);
    while (nplayers != 2 && nplayers != 3 && nplayers != 4 && nplayers != 5 && nplayers != 6) {
        printf("Incorrect input, please choose a number between 2 and 6\n");
        scanf("%d", &nplayers);
        fflush(stdin);
    }

    struct node_player *nodeFirstPlayer = initList(nplayers);

    //ante: each player pays 1 fiche to enter the game
    ante(nodeFirstPlayer, nplayers, &tableBet);
    printf("All players paid 1 fiche for ante.\n");

    //cards are assigned to each player
    int feedCards = 1;
    //Debug Mode
    if (feedCards == 1) {
//-----------------------1---------------------------
        nodeFirstPlayer->p.cardSeed[0] = 2;
        nodeFirstPlayer->p.cardValue[0] = 4;

        nodeFirstPlayer->p.cardSeed[1] = 1;
        nodeFirstPlayer->p.cardValue[1] = 4;

        nodeFirstPlayer->p.cardSeed[2] = 3;
        nodeFirstPlayer->p.cardValue[2] = 4;

        nodeFirstPlayer->p.cardSeed[3] = 1;
        nodeFirstPlayer->p.cardValue[3] = 10;

        nodeFirstPlayer->p.cardSeed[4] = 1;
        nodeFirstPlayer->p.cardValue[4] = 4;

        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//--------------------------2----------------------
        nodeFirstPlayer->p.cardSeed[0] = 1;
        nodeFirstPlayer->p.cardValue[0] = 1;

        nodeFirstPlayer->p.cardSeed[1] = 1;
        nodeFirstPlayer->p.cardValue[1] = 1;

        nodeFirstPlayer->p.cardSeed[2] = 1;
        nodeFirstPlayer->p.cardValue[2] = 1;

        nodeFirstPlayer->p.cardSeed[3] = 1;
        nodeFirstPlayer->p.cardValue[3] = 1;

        nodeFirstPlayer->p.cardSeed[4] = 1;
        nodeFirstPlayer->p.cardValue[4] = 4;

        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//------------------------3----------------------
        nodeFirstPlayer->p.cardSeed[0] = 1;
        nodeFirstPlayer->p.cardValue[0] = 4;

        nodeFirstPlayer->p.cardSeed[1] = 1;
        nodeFirstPlayer->p.cardValue[1] = 4;

        nodeFirstPlayer->p.cardSeed[2] = 1;
        nodeFirstPlayer->p.cardValue[2] = 4;

        nodeFirstPlayer->p.cardSeed[3] = 1;
        nodeFirstPlayer->p.cardValue[3] = 3;

        nodeFirstPlayer->p.cardSeed[4] = 1;
        nodeFirstPlayer->p.cardValue[4] = 4;

        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//------------------------4--------------------------
        nodeFirstPlayer->p.cardSeed[0] = 1;
        nodeFirstPlayer->p.cardValue[0] = 1;

        nodeFirstPlayer->p.cardSeed[1] = 1;
        nodeFirstPlayer->p.cardValue[1] = 1;

        nodeFirstPlayer->p.cardSeed[2] = 1;
        nodeFirstPlayer->p.cardValue[2] = 1;

        nodeFirstPlayer->p.cardSeed[3] = 1;
        nodeFirstPlayer->p.cardValue[3] = 1;

        nodeFirstPlayer->p.cardSeed[4] = 1;
        nodeFirstPlayer->p.cardValue[4] = 1;

        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//--------------------------5-----------------------------
        nodeFirstPlayer->p.cardSeed[0] = 1;
        nodeFirstPlayer->p.cardValue[0] = 1;

        nodeFirstPlayer->p.cardSeed[1] = 1;
        nodeFirstPlayer->p.cardValue[1] = 1;

        nodeFirstPlayer->p.cardSeed[2] = 1;
        nodeFirstPlayer->p.cardValue[2] = 1;

        nodeFirstPlayer->p.cardSeed[3] = 1;
        nodeFirstPlayer->p.cardValue[3] = 1;

        nodeFirstPlayer->p.cardSeed[4] = 1;
        nodeFirstPlayer->p.cardValue[4] = 5;

        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//------------------------6---------------------------
        nodeFirstPlayer->p.cardSeed[0] = 1;
        nodeFirstPlayer->p.cardValue[0] = 1;

        nodeFirstPlayer->p.cardSeed[1] = 1;
        nodeFirstPlayer->p.cardValue[1] = 1;

        nodeFirstPlayer->p.cardSeed[2] = 1;
        nodeFirstPlayer->p.cardValue[2] = 1;

        nodeFirstPlayer->p.cardSeed[3] = 1;
        nodeFirstPlayer->p.cardValue[3] = 1;

        nodeFirstPlayer->p.cardSeed[4] = 1;
        nodeFirstPlayer->p.cardValue[4] = 1;

        while (nodeFirstPlayer->previousNodo != NULL) {
            nodeFirstPlayer = nodeFirstPlayer->previousNodo;
        }
    }
    //Normal Mode
    if (feedCards == 0) {
        assignCards(&flag, nodeFirstPlayer, nplayers);
    }
/*
    //first round of bets
    betting (nodeFirstPlayer, nplayers, &tableBet, &call, &raise);
    raise = 0;

    //swapping cards
    swap(nodeFirstPlayer, nplayers, flag);

    //second round of bets
    betting (nodeFirstPlayer, nplayers, &tableBet, &call, &raise);
*/
    // ------------ CALCOLO PUNTI --------------
    while (nodeFirstPlayer->previousNodo != NULL) {
        nodeFirstPlayer = nodeFirstPlayer->previousNodo;
    }
    for (int i = 0; i < nplayers; i++) {
        int royalFlush = 0;
        int straightFlush = 0;
        int poker = 0;
        int house = 0;
        int flush = 0;
        int straight = 0;
        int threeOfAKind = 0;
        int twoPair = 0;
        int pair = 0;
        int highCard = 0;
        int pairValue = 0;
        int pair_1, pair_2;
        int tris;
        int straightValue;
        int pokerValue;
        int isFinished = 0;
        printf("Player %d:\n", nodeFirstPlayer->p.value);
        if (nodeFirstPlayer->p.isDead == 0 && nodeFirstPlayer->p.hasFolded == 0) {
            royalFlush = checkRoyalFlush(nodeFirstPlayer->p.cardValue, nodeFirstPlayer->p.cardSeed, royalFlush);
            if (royalFlush == 1) {
                isFinished = 1;
                nodeFirstPlayer->p.score = 900;
            }
            if (isFinished == 0) {
                straightFlush = checkStraightFlush(nodeFirstPlayer->p.cardValue, nodeFirstPlayer->p.cardSeed,
                                                   straightFlush, &straightValue);
                if (straightFlush == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 800 + straightValue;
                }
            }
            if (isFinished == 0) {
                poker = checkPoker(nodeFirstPlayer->p.cardValue, poker, &pokerValue);
                if (poker == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 700 + pokerValue;
                }
            }
            if (isFinished == 0) {
                house = checkHouse(nodeFirstPlayer->p.cardValue, house, &tris);
                if (house == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 600 + tris;
                }
            }
            if (isFinished == 0) {
                flush = checkFlush(nodeFirstPlayer->p.cardSeed, flush);
                if (flush == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 500;
                }
            }
            if (isFinished == 0) {
                straight = checkStraight(nodeFirstPlayer->p.cardValue, straight, &straightValue);
                if (straight == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 400 + straightValue;
                }
            }
            if (isFinished == 0) {
                threeOfAKind = checkThreeOfAKind(nodeFirstPlayer->p.cardValue, threeOfAKind, &tris);
                if (threeOfAKind == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 300 + tris;
                }
            }
            if (isFinished == 0) {
                twoPair = checkTwoPair(nodeFirstPlayer->p.cardValue, twoPair, &pair_1, &pair_2);
                if (twoPair == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 200 + pair_1;
                }
            }
            if (isFinished == 0) {
                pair = checkPair(nodeFirstPlayer->p.cardValue, pair, &pairValue);
                if (pair == 1) {
                    isFinished = 1;
                    if (pairValue == 1) {
                        nodeFirstPlayer->p.score = 115;
                    } else {
                        nodeFirstPlayer->p.score = 100 + pairValue;
                    }
                }
            }
            if (isFinished == 0) {
                highCard = checkHighCard(nodeFirstPlayer->p.cardValue, &highCard);
                nodeFirstPlayer->p.score = highCard;
            }

            //PRINT
            if (royalFlush == 1) {
                printf("You did a Royal Flush?!!!!!!!! Lmao\n");
            } else {
            }
            if (straightFlush == 1) {
                printf("You did: \n\nStraight Flush\n");
            }
            if (poker == 1) {
                if (nodeFirstPlayer->p.cardValue[0] == nodeFirstPlayer->p.cardValue[1]) {
                    if (nodeFirstPlayer->p.cardValue[0] > 1 && nodeFirstPlayer->p.cardValue[0] < 11) {
                        printf("You did:\n\nPoker of %ds\n", nodeFirstPlayer->p.cardValue[0]);
                    } else if (nodeFirstPlayer->p.cardValue[0] == 1) {
                        printf("You did:\n\nPoker of Aces\n");
                    } else if (nodeFirstPlayer->p.cardValue[0] == 11) {
                        printf("You did:\n\nPoker of Jacks\n");
                    } else if (nodeFirstPlayer->p.cardValue[0] == 12) {
                        printf("You did:\n\nPoker of Queens\n");
                    } else if (nodeFirstPlayer->p.cardValue[0] == 13) {
                        printf("You did:\n\nPoker of Kings\n");
                    }
                } else {
                    if (pokerValue < 11) {
                        printf("You did:\n\nPoker of %ds\n", pokerValue);
                    } else if (pokerValue == 15) {
                        printf("You did:\n\nPoker of Aces\n");
                    } else if (pokerValue == 11) {
                        printf("You did:\n\nPoker of Jacks\n");
                    } else if (pokerValue == 12) {
                        printf("You did:\n\nPoker of Queens\n");
                    } else if (pokerValue == 13) {
                        printf("You did:\n\nPoker of Kings\n");
                    }
                }
            }
            if (house == 1) {
                if (tris < 11) {
                    printf("You did: \n\nHouse (%d)\n", tris);
                } else if (tris == 15) {
                    printf("You did: \n\nHouse (Ace)\n");
                } else if (tris == 11) {
                    printf("You did: \n\nHouse (Jack)");
                } else if (tris == 12) {
                    printf("You did: \n\nHouse (Queen)");
                } else {
                    printf("You did: \n\nHouse (King)");
                }
            }
            if (flush == 1) {
                printf("You did:\n\nFlush\n");
            }
            if (straight == 1) {
                printf("You did:\n\nStraight\n");
                printf("Straight Value: %d\n", straightValue);
            }
            if (threeOfAKind == 1) {
                if (tris < 11) {
                    printf("You did: \n\nThree of a Kind (%d)\n", tris);
                } else if (tris == 15) {
                    printf("You did: \n\nThree of a Kind (Ace)\n");
                } else if (tris == 11) {
                    printf("You did: \n\nThree of a Kind (Jack)\n");
                } else if (tris == 12) {
                    printf("You did: \n\nThree of a Kind (Queen)\n");
                }
                else {
                    printf("You did: \n\nThree of a Kind (King)\n");
                }
            }
            if (twoPair == 1) {
                if (pair_1 > 1 && pair_1 < 11) {
                    printf("You did: \n\nTwo Pair (%d)\n", pair_1);
                } else if (pair_1 == 1) {
                    printf("You did: \n\nTwo Pair (Ace)\n");
                } else if (pair_1 == 11) {
                    printf("You did: \n\nTwo Pair (Jack)\n");
                } else if (pair_1 == 12) {
                    printf("You did: \n\nTwo Pair (Queen)\n");
                } else {
                    printf("You did: \n\nTwo Pair (King)\n");
                }
            }
            if (pair == 1) {
                if (pairValue > 1 && pairValue < 11) {
                    printf("You did: \n\nPair (%d)\n", pairValue);
                } else if (pairValue == 1) {
                    printf("You did: \n\nPair (Ace)\n");
                } else if (pairValue == 11) {
                    printf("You did: \n\nPair (Jack)\n");
                } else if (pairValue == 12) {
                    printf("You did: \n\nPair (Queen)\n");
                } else {
                    printf("You did: \n\nPair (King)\n");
                }
            } else {
            }
            if (isFinished == 0) {
                if (highCard < 11) {
                    printf("You did: \n\nHigh card with %d\n", highCard);
                } else if (highCard == 11) {
                    printf("You did: \n\nHigh card with Jack\n");
                } else if (highCard == 12) {
                    printf("You did: \n\nHigh card with Queen\n");
                } else if (highCard == 13) {
                    printf("You did: \n\nHigh card with King\n");
                } else {
                    printf("You did: \n\nHigh card with Ace\n");
                }
            }
            printf("Player %d, your score is: %d\n", nodeFirstPlayer->p.value, nodeFirstPlayer->p.score);
        }
        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
    }

    return 0;
}
