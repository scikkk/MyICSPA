#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[32];
  struct watchpoint *prev;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *tail = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */



void new_wp(const char* expr){
	if (free_ == NULL){printf("%s\n", "No more free space!\0");assert(0);}
	WP *nwp = free_;
	free_ = free_->next;
	nwp->next = NULL;
	nwp->prev = tail;
	if (head == NULL){
		head = nwp;
		tail = nwp;
		head->NO = 1;
	}
	else {
		nwp->NO = tail->NO + 1;
		tail->next = nwp;
		tail = tail->next;
	}
	strcpy(tail->expr,expr);
	return ;
}
void free_wp(int wp_no){
if (head==NULL){printf("no watchpoint.");}
	WP *wp = NULL;
	for (wp=head; wp->NO != wp_no; wp = wp->next) {
		if (wp == NULL) {printf("0000/n");return;}
	}
	if (wp->next == wp->prev){
	head = NULL;
	head = NULL;
	}
	else if (wp->prev==NULL){
		head = wp->next;
		head->prev = NULL;
	} 
	else if(wp->next==NULL){
	tail = wp->prev;
	tail->next=NULL;
	}
	else{
		wp->prev->next = wp->next;
		wp->next->prev = wp->prev;
		wp->next=free_;
	}
	wp->next = free_;
		free_->prev=wp;
		free_=free_->prev;
	return ;	
}
