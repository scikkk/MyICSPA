#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char expr[32];
	struct watchpoint *prev;
	char type[12];
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



void new_wp(const char* expr,const char* wp_type){
	static int wp_no=1;
	if (free_ == NULL){printf("%s\n", "No more free space!\0");assert(0);}
	WP *nwp = free_;
	free_ = free_->next;
	nwp->next = NULL;
	nwp->prev = tail;
	if (head == NULL){
		head = nwp;
		tail = nwp;
	}
	else {
		tail->next = nwp;
		tail = tail->next;
	}
	tail->NO = wp_no;
	wp_no++;
	strcpy(tail->expr,expr);
	strcpy(tail->type,wp_type);
	printf("Watchpoint %d: %s\n",tail->NO, tail->expr);
	return ;
}
void free_wp(int wp_no){
	if (head==NULL){printf("No watchpoint.\n");return;}
	WP *wp = head;
	if (wp_no == -1){
		while(wp != NULL){
			printf("Successfully delete watchpoint number %d: %s.\n",wp->NO,wp->expr);
			wp = wp->next;
		}
		tail->next = free_;
		free_->prev = tail;
		free_ = head;
		head = NULL;
		tail = NULL;
		return ;
	}
	while ( wp->NO != wp_no) {

		wp = wp->next;
		if (wp == NULL) {printf("No watchpoint number %d.\n",wp_no);return;}
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
	printf("Successfully delete watchpoint number %d: %s.\n",wp_no,wp->expr);
	return ;	
}

void wp_display(){
	printf("%-5s%-12s%s", "Num\0", "Type\0", "What\0");
	WP *wp = head;
	if (wp == NULL){
		printf("No watchpoints.\n");
	}
	else {
		while(wp != NULL){
			printf("%-5d%-12s%s",wp->NO,wp->type,wp->expr);
			wp=wp->next;
		}
	}
	return;
}
