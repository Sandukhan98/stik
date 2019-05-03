#include "list_players.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#define UNUSED(x) (void)x
enum color_t player__get_color(struct player_t const * player){
    return player->c;
}

// Return a new list_players dynamically allocated
struct list_players* list_players__empty(){
    struct list_players* lst_players = malloc(sizeof(struct list_players));
    lst_players->numb=0;
    return lst_players;
}

int list_players__numb(struct list_players const* lp){
    return lp->numb;
}

// Initialize each player of the list_players "lp" with the path of dynamic library corresponding to each player in "tab", return 1 if success and 0 if error
int list_players__initialize(struct list_players *lp, char** tab,int numb){
    struct player_t * p;
    char *error;
    if(numb>PL_MAX_PLAYERS)return 0;
    lp->numb=(size_t)numb;
    for(int i=0;i<numb;i++){
        p=&(lp->players[i]);
        p->c=i;
        p->next=&(lp->players[(i+1)%numb]);
        p->handler=dlopen(tab[i],RTLD_LAZY);
        if(!p->handler){
            return 0;
        }
        p->get_player_name=dlsym(p->handler,"get_player_name");
        p->propose_opening=dlsym(p->handler,"propose_opening");
        p->accept_opening=dlsym(p->handler,"accept_opening");
        p->initialize=dlsym(p->handler,"initialize");
        p->play=dlsym(p->handler,"play");
        p->finalize=dlsym(p->handler,"finalize");
        if((error=dlerror())!=NULL){
            return 0;
        }
    }
    return 1;
}

// Return the player who plays next according to the "actual_player". If "actual_player" is NULL, return first element of list_players "lp"
struct player_t* list_players__next(struct player_t const* actual_player, struct list_players* lp){
    if(actual_player==NULL)return &(lp->players[0]);
    return actual_player->next;
}

// Finalize each player of the list_players "lp"
void list_players__finalize(struct list_players* lp,int numb){
    struct player_t * p;
    for(int i=0;i<numb;i++){
        p=&(lp->players[i]);
        p->finalize();
        dlclose(p->handler);
    }
}
// Free allocated memory
void list_players__free(struct list_players* lp){
    free(lp);
}
