#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "listlib.c"
#include "vectorlib.c"
#include "sortingAlgorithm.c"

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
int winScore;
int deadPlayers = 0;
int remainPlayers;
int nTurns = 1;
//endregion

//region basic functions
void payday(int tableBet, int nWinners, struct node_player *nodeFirstPlayer, int winnersPosition[]) {
    tableBet = tableBet / nWinners;
    for (int i = 0; i < nWinners; i++) {
        get_player(nodeFirstPlayer, winnersPosition[i] + 1)->p.money += tableBet;
        printf("Player %d, you won %d fiche/s!!\n", get_player(nodeFirstPlayer, winnersPosition[i] + 1)->p.value,
               tableBet);
    }
}

void determineWinners(struct node_int *winners, struct node_player *nodeFirstPlayer, int winnersPosition[], int *nWinners, int losers[], int nPlayers) {
    while (winners->previousNodo != NULL) {
        *winners = *winners->previousNodo;
    }
    for (int i = 0; i < *nWinners; i++) {
        losers[winners->value - 1] = 0;
        if (winners->nextNodo != NULL) {
            *winners = *winners->nextNodo;
        }
    }
    while (winners->previousNodo != NULL) {
        *winners = *winners->previousNodo;
    }
    while (winners->nextNodo != NULL) {
        for (int j = 0; j < nplayers; j++) {
            if (get_player(nodeFirstPlayer, winners->value)->p.cardValue[4 - j] == 1 ||
                get_player(nodeFirstPlayer, winners->nextNodo->value)->p.cardValue[4 - j] == 1) {
                printf("At least one has Ace\n");
                if (get_player(nodeFirstPlayer, winners->value)->p.cardValue[4 - j] >
                    get_player(nodeFirstPlayer, winners->value + 1)->p.cardValue[4 - j]) {
                    j = nPlayers;
                    losers[winners->value - 1] = 9;
                    printf("Player %d in losers array\n", get_player(nodeFirstPlayer, winners->value)->p.value);
                } else if (get_player(nodeFirstPlayer, winners->value)->p.cardValue[4 - j] <
                           get_player(nodeFirstPlayer, winners->nextNodo->value)->p.cardValue[4 - j]) {
                    j = nPlayers;
                    losers[winners->nextNodo->value - 1] = 9;
                    printf("Player %d in losers array\n", get_player(nodeFirstPlayer, winners->value + 1)->p.value);
                } else {
                    printf("both have ace\n");
                }
            } else if (get_player(nodeFirstPlayer, winners->value)->p.cardValue[j] >
                       get_player(nodeFirstPlayer, winners->nextNodo->value)->p.cardValue[j]) {
                printf("No one has Ace\n");
                j = nPlayers;
                losers[winners->nextNodo->value - 1] = 9;
                printf("Player %d in losers array\n", get_player(nodeFirstPlayer, winners->value + 1)->p.value);
            } else if (get_player(nodeFirstPlayer, winners->value)->p.cardValue[j] <
                       get_player(nodeFirstPlayer, winners->nextNodo->value)->p.cardValue[j]) {
                j = nPlayers;
                losers[winners->value - 1] = 9;
                printf("Player %d in losers array\n", get_player(nodeFirstPlayer, winners->value)->p.value);
            }
        }
        if (winners->nextNodo != NULL) {
            *winners = *winners->nextNodo;
        }
    }
    while (winners->previousNodo != NULL) {
        *winners = *winners->previousNodo;
    }
    *nWinners = 0;
    int j = 0;
    for (int i = 0; i < 6; i++) {
        if (losers[i] == 0) {
            *nWinners = *nWinners + 1;
            winnersPosition[j] = i;
            j++;
        }
    }
}

void sortWinnerCards(struct node_player *nodeFirstPlayer, int nPlayers) {
    for (int i = 0; i < nplayers; i++) {
        sortArrayDesc(nodeFirstPlayer->p.cardValue, 5);
        if (nodeFirstPlayer->nextNodo != NULL) {
            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
        }
    }
}

int remainingPlayers(struct node_player *_nodo, int nplayers) {
    int remainingPlayers = nplayers;
    for (int i = 0; i < nplayers; i++) {
        if (_nodo->p.hasFolded || _nodo->p.isDead != 0) {
            remainingPlayers--;
        }
        _nodo = _nodo->nextNodo;
    }
    return remainingPlayers;
}

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

void printPlayerInfo(struct node_player *node, int *call) {
    printf("-------------------------PLAYER %d-------------------------\n", node->p.value);
    printf("Player info:\nMoney:  %d\nCurrent Bet: %d\n", node->p.money, node->p.currentBet);
    printf("Call: %d\n", *call);
    printf("DEBUG INFO:\nRAISE: %d\nTABLEBET: %d\nHASFOLDED: %d\nISDEAD: %d\nALLIN: %d\n", raise, tableBet, node->p.hasFolded, node->p.isDead, node->p.allIn);
    printcards(*node, node->p.value);
}

char CRF(char answer) {
    printf("Do you want to call(c), raise(r) or fold (f)?\n");
    scanf("%c*c", &answer);
    fflush(stdin);
    while (answer != 'c' && answer != 'C' && answer != 'r' && answer != 'R' && answer != 'f' && answer != 'F') {
        printf("Incorrect input: type 'c' for call, 'r' for raise or 'f' for fold.\n");
        scanf("%c*c", &answer);
        fflush(stdin);
    }
    return answer;
}

char CR(char answer) {
    printf("Do you want to check (c) or raise(r)?\n");
    scanf("%c*c", &answer);
    fflush(stdin);
    while (answer != 'c' && answer != 'C' && answer != 'r' && answer != 'R') {
        printf("Incorrect input: type 'c' for check or 'r' for raise.\n");
        scanf("%c*c", &answer);
        fflush(stdin);
    }
    return answer;
}

char CF(char answer) {
    printf("Do you want to call (c) or fold(f)?\n");
    scanf("%c*c", &answer);
    fflush(stdin);
    while (answer != 'c' && answer != 'C' && answer != 'f' && answer != 'F') {
        printf("Incorrect input: type 'c' for call or 'r' for raise.\n");
        scanf("%c*c", &answer);
        fflush(stdin);
    }
    return answer;
}

void ante(struct node_player *player, int nplayers, int *tableBet) {
    int j = 0;
    for (int i = 0; i < nplayers; i++) {
        if (player->p.isDead == 0) {
            player->p.money--;
            player->p.currentBet++;
            if (player->p.money == 0) {
                printf("Player %d, due to ante you went all-in\n", player->p.value);
                player->p.allIn = 1;
            }
            j++;
        }
        player = player->nextNodo;
    }
    *tableBet = j;
}

void shuffleDeck(int deck[4][13]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck[i][j] = 0;
        }
    }
}

void InitializeValues(struct node_player *nodeFirstPlayer, int nplayers, int *deadPlayers, int flag, int *nTurns,
                      int *losers, int *call, int *raise, int *tableBet, int *winScore) {
    *call = 1;
    *raise = 0;
    *tableBet = 0;
    *winScore = 0;
    losers[0] = 9;
    losers[1] = 9;
    losers[2] = 9;
    losers[3] = 9;
    losers[4] = 9;
    losers[5] = 9;
    while (nodeFirstPlayer->previousNodo != NULL) {
        nodeFirstPlayer = nodeFirstPlayer->previousNodo;
    }
    for (int i = 0; i < nplayers; i++) {
        if (nodeFirstPlayer->p.money < 1 && nodeFirstPlayer->p.isDead == 0) {
            printf("Player %d, you ran out of money and lost\n", nodeFirstPlayer->p.value);
            nodeFirstPlayer->p.isDead = 1;
            *deadPlayers += 1;
        }
        nodeFirstPlayer->p.hasFolded = 0;
        nodeFirstPlayer->p.score = 0;
        nodeFirstPlayer->p.currentBet = 0;
        nodeFirstPlayer->p.allIn = 0;
        if (nodeFirstPlayer->nextNodo != NULL) {
            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
        }
    }
}

void betting(struct node_player *player, int nplayers, int *tableBet, int *call, int *raise) {
    int isLastPlayer = 0;
    while (player != NULL && isLastPlayer == 0) {
        char answer = 'p';
        if (player->p.currentBet < *call && player->p.hasFolded == 0 && player->p.isDead == 0 && player->p.allIn == 0) {
            printPlayerInfo(player, call);
            answer = CRF(answer);
            if (answer == 'c' || answer == 'C') {
                if (*call - player->p.currentBet >= player->p.money) {
                    printf("You went all-in\n");
                    player->p.allIn = 1;
                    player->p.currentBet += player->p.money;
                    *tableBet += player->p.money;
                    player->p.money = 0;
                } else {
                    player->p.currentBet = *call;
                    *tableBet = *tableBet + *raise;
                    player->p.money = player->p.money - *raise;
                }
            } else if (answer == 'r' || answer == 'R') {
                *tableBet = *tableBet + *raise;
                player->p.money = player->p.money - *raise;
                if (player->p.money == 0) {
                    player->p.allIn = 1;
                    printf("You can't raise while all-in\n");
                    player->p.money += *raise;
                    player->p.currentBet += player->p.money;
                    player->p.money = 0;
                    *tableBet = *tableBet - *raise;
                } else if (player->p.money < 0) {
                    player->p.allIn = 1;
                    printf("You can't raise more, you went all-in\n");
                    player->p.money += *raise;
                    player->p.currentBet += player->p.money;
                    tableBet = tableBet - *raise;
                    tableBet += player->p.money;
                    player->p.money = 0;
                } else {
                    printf("By how much do you want to raise?\n");
                    scanf("%d*d", raise);
                    fflush(stdin);
                    if (*raise == player->p.money){
                        printf("You went all-in\n");
                        player->p.allIn = 1;
                    }
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
                }
            } else {
                player->p.hasFolded = 1;
            }
        } else if (player->p.hasFolded == 0 && player->p.isDead == 0 && player->p.allIn == 0) {
            printPlayerInfo(player, call);
            answer = CR(answer);
            if (answer == 'c' || answer == 'C') {
            } else {
                if (player->p.money == 0) {
                    printf("You can't raise while all-in\n");
                    player->p.money = player->p.money + *raise;
                    *tableBet = *tableBet - *raise;
                } else {
                    printf("By how much do you want to raise?\n");
                    scanf("%d*d", raise);
                    fflush(stdin);
                    if (*raise == player->p.money){
                        printf("You went all-in\n");
                        player->p.allIn = 1;
                    }
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
                }
            }
        }
        if (player->nextNodo != NULL) {
            player = player->nextNodo;
        } else { isLastPlayer = 1; }
    }
    isLastPlayer = 0;
    //BET SECOND PART
    while (player->previousNodo != NULL) {
        player = player->previousNodo;
    }
    while (isLastPlayer == 0) {
        if (player->p.currentBet < *call && player->p.hasFolded == 0 && player->p.isDead == 0 && player->p.allIn == 0) {
            printPlayerInfo(player, call);
            char answer = 'p';
            answer = CF(answer);
            if (answer == 'c' || answer == 'C') {
                if (*call - player->p.currentBet >= player->p.money) {
                    printf("You went all-in\n");
                    player->p.allIn = 1;
                    player->p.currentBet += player->p.money;
                    *tableBet += player->p.money;
                    player->p.money = 0;
                } else {
                    player->p.money = player->p.money + player->p.currentBet;
                    *tableBet = *tableBet + (*call - player->p.currentBet);
                    player->p.currentBet = *call;
                    player->p.money = player->p.money - *call;
                }
            } else {
                player->p.hasFolded = 1;
            }
        }
        if (player->nextNodo != NULL) {
            player = player->nextNodo;
        } else {
            isLastPlayer = 1;
        }
    }
}

void swap(struct node_player *player, int nplayers, int flag[4][13]) {
    //FUNZIONE SWAP: scambia una o pi?? carte ai giocatori secondo quanto richiesto
    for (int i = 0; i < nplayers; i++) {
        if (player->p.hasFolded == 0 && player->p.isDead == 0) {
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
            if (answer == 'n' || answer == 'N') {
                player = player->nextNodo;
            }
            if (answer == 'y' || answer == 'Y') {
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
                int array[5] = {0};
                if (swaps != 5) {
                    printf("Which cards do you want to change?\nAnswer by typing the order of the cards\n(ex: the first card is 1, the second is 2 and so on).\n");
                    for (int j = 0; j < swaps; j++) {
                        scanf("%d*d", &selCards.array[j]);
                        fflush(stdin);
                        while (selCards.array[j] > 5 || selCards.array[j] < 1) {
                            printf("Please enter an amount between 1 and 5.\n");
                            scanf("%d*d", &selCards.array[j]);
                            fflush(stdin);
                        }
                        while (array[selCards.array[j] - 1] == 1) {
                            printf("You already changed this card, please select another.\n");
                            scanf("%d*d", &selCards.array[j]);
                            fflush(stdin);
                        }
                        array[selCards.array[j] - 1] = 1;
                    }
                } else {
                    array[0] = 1;
                    array[1] = 2;
                    array[2] = 3;
                    array[3] = 4;
                    array[4] = 5;
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
//region point calculator internal functions
int checkRoyalFlush(int card[], int cardSeed[], int royalFlush) {
    if (cardSeed[0] == cardSeed[1] && cardSeed[0] == cardSeed[2] && cardSeed[0] == cardSeed[3] &&
        cardSeed[0] == cardSeed[4]) {
        if (card[0] + card[1] + card[2] + card[3] + card[4] == 47) {
            if (card[0] == 1 || card[1] == 1 || card[2] == 1 || card[3] == 1 || card[4] == 1) {
                royalFlush = 1;
            }
        }
    }
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
        }
    }
    if (j == 3) {
        poker = 1;
        *pokerValue = card[0];
    } else if (j == 0) {
        for (int i = 2; i < 5; i++) {
            if (card[1] == card[i]) {
                j++;
            }
        }
        if (j == 3) {
            poker = 1;
            *pokerValue = card[1];
        }
    }
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
        }
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
        }
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
    return *highCard;
}
//endregion
void pointCalculator(struct node_player *nodeFirstPlayer, int nplayers) {
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
        if (nodeFirstPlayer->p.isDead == 0 && nodeFirstPlayer->p.hasFolded == 0) {
            printf("-------------------------PLAYER %d-------------------------\n", nodeFirstPlayer->p.value);
            royalFlush = checkRoyalFlush(nodeFirstPlayer->p.cardValue, nodeFirstPlayer->p.cardSeed, royalFlush);
            if (royalFlush == 1) {
                isFinished = 1;
                nodeFirstPlayer->p.score = 900;
                printf("You did a Royal Flush?!!!!!!!! Lmao\n");
            }
            if (isFinished == 0) {
                straightFlush = checkStraightFlush(nodeFirstPlayer->p.cardValue, nodeFirstPlayer->p.cardSeed,
                                                   straightFlush, &straightValue);
                if (straightFlush == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 800 + straightValue;
                    printf("You did:\nStraight Flush\n");
                }
            }
            if (isFinished == 0) {
                poker = checkPoker(nodeFirstPlayer->p.cardValue, poker, &pokerValue);
                if (poker == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 700 + pokerValue;
                    if (pokerValue < 11) {
                        printf("You did:\nPoker of %ds\n", pokerValue);
                    } else if (pokerValue == 15) {
                        printf("You did:\nPoker of Aces\n");
                    } else if (pokerValue == 11) {
                        printf("You did:\nPoker of Jacks\n");
                    } else if (pokerValue == 12) {
                        printf("You did:\nPoker of Queens\n");
                    } else if (pokerValue == 13) {
                        printf("You did:\nPoker of Kings\n");
                    }
                }
            }
            if (isFinished == 0) {
                house = checkHouse(nodeFirstPlayer->p.cardValue, house, &tris);
                if (house == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 600 + tris;
                    if (tris < 11) {
                        printf("You did:\nHouse (%d)\n", tris);
                    } else if (tris == 15) {
                        printf("You did:\nHouse (Ace)\n");
                    } else if (tris == 11) {
                        printf("You did:\nHouse (Jack)");
                    } else if (tris == 12) {
                        printf("You did:\nHouse (Queen)");
                    } else {
                        printf("You did:\nHouse (King)");
                    }
                }
            }
            if (isFinished == 0) {
                flush = checkFlush(nodeFirstPlayer->p.cardSeed, flush);
                if (flush == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 500;
                    printf("You did:\nFlush\n");
                }
            }
            if (isFinished == 0) {
                straight = checkStraight(nodeFirstPlayer->p.cardValue, straight, &straightValue);
                if (straight == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 400 + straightValue;
                    printf("You did:\nStraight\n");
                    printf("Straight Value: %d\n", straightValue);
                }
            }
            if (isFinished == 0) {
                threeOfAKind = checkThreeOfAKind(nodeFirstPlayer->p.cardValue, threeOfAKind, &tris);
                if (threeOfAKind == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 300 + tris;
                    if (tris < 11) {
                        printf("You did:\nThree of a Kind (%d)\n", tris);
                    } else if (tris == 15) {
                        printf("You did:\nThree of a Kind (Ace)\n");
                    } else if (tris == 11) {
                        printf("You did:\nThree of a Kind (Jack)\n");
                    } else if (tris == 12) {
                        printf("You did:\nThree of a Kind (Queen)\n");
                    } else {
                        printf("You did:\nThree of a Kind (King)\n");
                    }
                }
            }
            if (isFinished == 0) {
                twoPair = checkTwoPair(nodeFirstPlayer->p.cardValue, twoPair, &pair_1, &pair_2);
                if (twoPair == 1) {
                    isFinished = 1;
                    nodeFirstPlayer->p.score = 200 + pair_1;
                    if (pair_1 > 1 && pair_1 < 11) {
                        printf("You did:\nTwo Pair (%d)\n", pair_1);
                    } else if (pair_1 == 1) {
                        printf("You did:\nTwo Pair (Ace)\n");
                    } else if (pair_1 == 11) {
                        printf("You did:\nTwo Pair (Jack)\n");
                    } else if (pair_1 == 12) {
                        printf("You did:\nTwo Pair (Queen)\n");
                    } else {
                        printf("You did:\nTwo Pair (King)\n");
                    }
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
                    if (pairValue > 1 && pairValue < 11) {
                        printf("You did:\nPair (%d)\n", pairValue);
                    } else if (pairValue == 1) {
                        printf("You did:\nPair (Ace)\n");
                    } else if (pairValue == 11) {
                        printf("You did:\nPair (Jack)\n");
                    } else if (pairValue == 12) {
                        printf("You did:\nPair (Queen)\n");
                    } else {
                        printf("You did:\nPair (King)\n");
                    }
                }
            }
            if (isFinished == 0) {
                highCard = checkHighCard(nodeFirstPlayer->p.cardValue, &highCard);
                nodeFirstPlayer->p.score = highCard;
                if (highCard < 11) {
                    printf("You did:\nHigh card with %d\n", highCard);
                } else if (highCard == 11) {
                    printf("You did:\nHigh card with Jack\n");
                } else if (highCard == 12) {
                    printf("You did:\nHigh card with Queen\n");
                } else if (highCard == 13) {
                    printf("You did:\nHigh card with King\n");
                } else {
                    printf("You did:\nHigh card with Ace\n");
                }
            }
        } else { nodeFirstPlayer->p.score = 0; }
        if (nodeFirstPlayer->nextNodo != NULL) {
            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
        }
    }

    while (nodeFirstPlayer->previousNodo != NULL) {
        nodeFirstPlayer = nodeFirstPlayer->previousNodo;
    }
}

// Check Winner/s
int winnerScore(struct node_player *nodeFirstPlayer, int nplayers) {
    struct node_int winners;
    winners.value = 0;
    winners.nextNodo = NULL;
    winners.previousNodo = NULL;
    for (int i = 1; i < nplayers; i++) {
        if (get_player(nodeFirstPlayer, i + 1)->p.score >
            get_player(nodeFirstPlayer, winners.value + 1)->p.score) {
            insertLast_int(&winners, i);
            while (winners.nextNodo != NULL) {
                removeFirst_int(&winners);
                while (winners.nextNodo != NULL) {
                    winners = *winners.nextNodo;
                }
            }
        }
    }
    winScore = get_player(nodeFirstPlayer, winners.value + 1)->p.score;
    return winScore;
}
// Create Winners List
int winnerList(struct node_player *nodeFirstPlayer, struct node_int *winners, int nplayers) {
    int j = 0;
    int nWinners;
    for (int i = 0; i < nplayers; i++) {
        if (nodeFirstPlayer->p.score == winScore) {
            j++;
            insertLast_int(winners, i + 1);
        }
        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
    }
    removeFirst_int(winners);
    nWinners = j;
    return nWinners;
}

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
    printf("------------GAME START------------\n");
    struct node_player *nodeFirstPlayer = initList(nplayers);

    while (deadPlayers != nplayers - 1) {
        printf("------------TURN %d START------------\n", nTurns);
        while (nodeFirstPlayer->previousNodo != NULL) {
            nodeFirstPlayer = nodeFirstPlayer->previousNodo;
        }
        ante(nodeFirstPlayer, nplayers, &tableBet);
        printf("All players paid 1 fiche for ante.\n");
        //cards are assigned to each player
        int feedCards = 1;
        //Debug Mode
        if (feedCards == 1) {
//-----------------------1---------------------------
            nodeFirstPlayer->p.cardSeed[0] = 1;
            nodeFirstPlayer->p.cardValue[0] = 5;

            nodeFirstPlayer->p.cardSeed[1] = 3;
            nodeFirstPlayer->p.cardValue[1] = 5;

            nodeFirstPlayer->p.cardSeed[2] = 1;
            nodeFirstPlayer->p.cardValue[2] = 5;

            nodeFirstPlayer->p.cardSeed[3] = 2;
            nodeFirstPlayer->p.cardValue[3] = 6;

            nodeFirstPlayer->p.cardSeed[4] = 1;
            nodeFirstPlayer->p.cardValue[4] = 13;

            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//--------------------------2----------------------
            nodeFirstPlayer->p.cardSeed[0] = 4;
            nodeFirstPlayer->p.cardValue[0] = 5;

            nodeFirstPlayer->p.cardSeed[1] = 1;
            nodeFirstPlayer->p.cardValue[1] = 7;

            nodeFirstPlayer->p.cardSeed[2] = 4;
            nodeFirstPlayer->p.cardValue[2] = 5;

            nodeFirstPlayer->p.cardSeed[3] = 3;
            nodeFirstPlayer->p.cardValue[3] = 6;

            nodeFirstPlayer->p.cardSeed[4] = 3;
            nodeFirstPlayer->p.cardValue[4] = 9;

            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//------------------------3----------------------
            nodeFirstPlayer->p.cardSeed[0] = 2;
            nodeFirstPlayer->p.cardValue[0] = 5;

            nodeFirstPlayer->p.cardSeed[1] = 4;
            nodeFirstPlayer->p.cardValue[1] = 5;

            nodeFirstPlayer->p.cardSeed[2] = 3;
            nodeFirstPlayer->p.cardValue[2] = 5;

            nodeFirstPlayer->p.cardSeed[3] = 3;
            nodeFirstPlayer->p.cardValue[3] = 6;

            nodeFirstPlayer->p.cardSeed[4] = 2;
            nodeFirstPlayer->p.cardValue[4] = 1;

            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//------------------------4--------------------------
            nodeFirstPlayer->p.cardSeed[0] = 1;
            nodeFirstPlayer->p.cardValue[0] = 10;

            nodeFirstPlayer->p.cardSeed[1] = 2;
            nodeFirstPlayer->p.cardValue[1] = 1;

            nodeFirstPlayer->p.cardSeed[2] = 1;
            nodeFirstPlayer->p.cardValue[2] = 10;

            nodeFirstPlayer->p.cardSeed[3] = 1;
            nodeFirstPlayer->p.cardValue[3] = 11;

            nodeFirstPlayer->p.cardSeed[4] = 4;
            nodeFirstPlayer->p.cardValue[4] = 6;

            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//--------------------------5-----------------------------
            nodeFirstPlayer->p.cardSeed[0] = 3;
            nodeFirstPlayer->p.cardValue[0] = 9;

            nodeFirstPlayer->p.cardSeed[1] = 1;
            nodeFirstPlayer->p.cardValue[1] = 1;

            nodeFirstPlayer->p.cardSeed[2] = 3;
            nodeFirstPlayer->p.cardValue[2] = 7;

            nodeFirstPlayer->p.cardSeed[3] = 4;
            nodeFirstPlayer->p.cardValue[3] = 1;

            nodeFirstPlayer->p.cardSeed[4] = 3;
            nodeFirstPlayer->p.cardValue[4] = 5;

            nodeFirstPlayer = nodeFirstPlayer->nextNodo;
//------------------------6---------------------------
            nodeFirstPlayer->p.cardSeed[0] = 2;
            nodeFirstPlayer->p.cardValue[0] = 5;

            nodeFirstPlayer->p.cardSeed[1] = 3;
            nodeFirstPlayer->p.cardValue[1] = 5;

            nodeFirstPlayer->p.cardSeed[2] = 1;
            nodeFirstPlayer->p.cardValue[2] = 5;

            nodeFirstPlayer->p.cardSeed[3] = 3;
            nodeFirstPlayer->p.cardValue[3] = 6;

            nodeFirstPlayer->p.cardSeed[4] = 1;
            nodeFirstPlayer->p.cardValue[4] = 10;

            while (nodeFirstPlayer->previousNodo != NULL) {
                nodeFirstPlayer = nodeFirstPlayer->previousNodo;
            }
        }
        //Normal Mode
        if (feedCards == 0) {
            assignCards(flag, nodeFirstPlayer, nplayers);
            printf("The dealer assigned five cards to each player.\n");
        }
        //TURN
        remainPlayers = remainingPlayers(nodeFirstPlayer, nplayers);
        if (remainPlayers != 1) {
            printf("The first round of bets starts\n");
            betting(nodeFirstPlayer, nplayers, &tableBet, &call, &raise);
        }
        raise = 0;
        remainPlayers = remainingPlayers(nodeFirstPlayer, nplayers);
        if (remainPlayers != 1) {
            printf("Now each player can change their cards\n");
            swap(nodeFirstPlayer, nplayers, flag);
        }
        remainPlayers = remainingPlayers(nodeFirstPlayer, nplayers);
        if (remainPlayers != 1) {
            printf("The second round of bets starts\n");
            betting(nodeFirstPlayer, nplayers, &tableBet, &call, &raise);
        }

        // ------------ CALCOLO PUNTI --------------
        pointCalculator(nodeFirstPlayer, nplayers);

        printf("----------------------------------------------------------\n");

        winScore = winnerScore(nodeFirstPlayer, nplayers);
        struct node_int winners;
        int nWinners;
        int winnersPosition[6] = {-1, -1, -1, -1, -1, -1};
        int losers[6] = {9, 9, 9, 9, 9, 9};
        winners.value = 0;
        winners.nextNodo = NULL;
        winners.previousNodo = NULL;
        nWinners = winnerList(nodeFirstPlayer, &winners, nplayers);
        winners = *winners.nextNodo;

        sortWinnerCards(nodeFirstPlayer, nplayers);

        determineWinners(&winners, nodeFirstPlayer, winnersPosition, &nWinners, losers, nplayers);

        payday(tableBet, nWinners, nodeFirstPlayer, winnersPosition);

        InitializeValues(nodeFirstPlayer, nplayers, &deadPlayers, flag, &nTurns, &losers, &call, &raise, &tableBet,
                         &winScore);
        shuffleDeck(flag);
        nTurns++;
    }
    while (nodeFirstPlayer->previousNodo != NULL) {
        nodeFirstPlayer = nodeFirstPlayer->previousNodo;
    }
    while (nodeFirstPlayer->p.isDead != 0) {
        nodeFirstPlayer = nodeFirstPlayer->nextNodo;
    }
    printf("Congrats Player %d, you won the game!!!\n\n", nodeFirstPlayer->p.value);
    return 0;
}
