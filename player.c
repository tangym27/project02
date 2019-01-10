#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deck.h"

#define MAX_GAME_SIZE 6
//max game size currently 6: change later if desired

struct team { int points; struct player * p_0; struct player * p_1; };
struct player { char * username; int hand[4]; int partner; struct team * p_team; int is_turn;};
struct player * players[MAX_GAME_SIZE]; 
struct team * teams[MAX_GAME_SIZE/2];

/* Given a username, creates a player and returns their player number.
 * This player can be accessed by players[ PLAYER_NO]
 */
int create_player(char * usrname) {
    for (int i=0; i<MAX_GAME_SIZE; i++){
        if (!players[i]) {
            players[i] = malloc( sizeof(struct player));
            players[i]->username = usrname;
            players[i]->is_turn = 0;
            return i;
        }
    }
    return -1; //full
}

/* Given two int player_nos, forms a team composed of those two players and returns their team number.
 * This team can be accessed by teams[ TEAM_NO]
 */
int form_team( int player_no0, int player_no1) {
    if (!(players[player_no0] && players[player_no1])) return -1;
    for (int i=0; i<MAX_GAME_SIZE/2; i++){
        if (!teams[i]) {
            teams[i] = malloc( sizeof(struct team));
            players[ player_no0]->p_team = teams[i];
            players[ player_no1]->p_team = teams[i];
            
            teams[i]->points =0;
            teams[i]->p_0 = players[ player_no0];
            teams[i]->p_1 = players[ player_no1];
            
            players[ player_no0]->partner = player_no1;
            players[ player_no1]->partner = player_no0;
        
            return i;
        }
    }
    return 0;
}

/* Makes a hand by drawing 4 cards.
 * If the deck is out of cards, returns -1.
 * Else, returns 0.
 */
int make_hand( int player_no) {
    struct player * plyr = players[player_no];
    for (int i = 0; i<4; i++) {
        int new_card = draw_card();
        if (new_card == -1) return -1;
        plyr->hand[i] = new_card;
    }
    return 0;
}

/* Given a player_no, adds a point to the player's team.
 * Returns -1 if the player has no team.
 * Else, returns 0.
 */
int add_point( int player_no) {
    struct player * plyr = players[player_no];
    if (plyr->p_team) plyr->p_team->points++;
    else return -1;
    return 0;
}

/* Given a player_no, returns that player's points.
 */
int get_points( int player_no){
    if (!players[ player_no]) return -1;
    return players[player_no]->p_team->points;
}

/* Given a player_no, checks if the player's hand contains crabs.
 * Returns 0 if no crabs. Returns 1 if crabs.
 */
int check_crabs( int player_no) {
    struct player * plyr = players[player_no];
    int card_check = plyr->hand[0];
    for (int i=1; i<4; i++){
        if ( card_check != plyr->hand[i] ) return 0;
    }
    return 1;
}

/* Given a player_no, prints the player's hand. */
int print_hand( int player_no) {
    struct player * plyr = players[player_no];
    if (plyr) {
        printf("Cards in %s's hand: \n", plyr->username);
        for (int i=0; i<3; i++){
            printf("[%d] %s, ", i, cards[ plyr->hand[i]]);
        }
        printf("and [3] %s\n", cards[ plyr->hand[3]]);
        if (check_crabs(player_no)) printf("Psst! Congratulations. You've got crabs!\n");
        else printf("No crabs here. Maybe you should trade some cards?\n");
        return 0;        
    }
    return -1;
}

/* Given an int player_no, the index of the player's card to be switched, and the index of the table's card to be switched.
 * Returns -1 if no player with that player_no was found, and -2 if it is not currently the player's turn.
 * Else, returns 0. 
 */
int swap_cards( int player_no, int player_card_index, int deck_card_index) {
    if (! players[ player_card_index]) return -1; 
    if (! players[ player_card_index]->is_turn) return -2;
    
    int deck_card = current_table->table_cards[deck_card_index];
    int player_card = players[ player_no]->hand[ player_card_index];
    
    players[ player_no]->hand[player_card_index] = deck_card;
    current_table->table_cards[deck_card_index] = player_card;
    
    return 0;
}


/* Given a team_no, switches turns.
 * If both players on a team were turnless, it becomes player 0's turn.
 * If no team was found with that team_no, -1 is returned.
 */
int switch_turns( int team_no) {
    if (! teams[team_no]) return -1;
    
    if (teams[team_no]->p_0->is_turn) {
        teams[team_no]->p_0->is_turn = 0;
        teams[team_no]->p_1->is_turn = 1;
    }
    else {
        teams[team_no]->p_0->is_turn = 1;
        teams[team_no]->p_1->is_turn = 0;
    }
    
    return 0;
}

int main() {
    srand( time(NULL));

    int my_player = create_player("crabby_laddy_2008");
    int my_teammate = create_player("X_lobster_d00d_X");
    
    make_deck();
    
    printf("YOUR HAND:\n");
    make_hand( my_player);
    print_hand( my_player);
    
    printf("\n\nYOUR TEAMMATE'S HAND:\n");
    make_hand( my_teammate);
    print_hand( my_teammate);
    
    int my_team = form_team( my_player, my_teammate);

    add_point(my_player);
    printf("\ncrabby team points: %d\n", get_points( my_player));
    add_point(my_teammate);
    printf("crabby team points: %d\n", get_points( my_teammate));
    
    
    create_table();
    print_table();
    
    printf("\n");
    if (swap_cards(0, 1, 1) == -2) printf("Hey! It's not your turn. No switch was made.\n");
    switch_turns( my_team);
    
    if (swap_cards(1, 1, 1) == -2) printf("Hey! It's not your turn. No switch was made.\n");
        
        
    print_hand( my_player);
    printf("\n");
    print_table();
}