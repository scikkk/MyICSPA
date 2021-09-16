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



WP* new_wp(const char* expr){
	if (free_ == NULL){printf("%s\n", "No more free space!\0");assert(0);}
	WP *new_w = free_;
	new_w->next = NULL;
	new_w->prev = tail;
	free_ = free_->next;
	if (head == NULL){
		head = new_w;
		tail = new_w;
		head->NO = 1;
	}
	else {
		new_w->NO = tail->NO + 1;
		tail->next = new_w;
		tail = tail->next;
	}
	strcpy(tail->expr,expr);
	return tail;
}
void free_wp(WP *wp){
	if (wp->prev==NULL){
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
		free_->prev=wp;
		free_=free_->prev;
	}
	return ;	
}
