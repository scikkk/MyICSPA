#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char expr[32];
	struct watchpoint *prev;
	char type[12];
	uint32_t value;
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



void new_wp(char* expr_s,const char* wp_type){
	bool* success = (bool*)malloc(sizeof(bool));
	*success = true;
	uint32_t res = expr(expr_s,success);
	if (!*success) {
		printf("Invalid expression！\n");
		free(success);
		return;
	}
	free(success);
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
	strcpy(tail->expr,expr_s);
	strcpy(tail->type,wp_type);
	if (strcmp(wp_type,"Breakpoint")==0){
		sprintf(tail->expr,"$pc==%s",expr_s);
		bool* success = (bool*)malloc(sizeof(bool));
		*success = true;
		tail->value = expr(tail->expr,success);
		assert(*success);	
		free(success);
	}
	else {
		tail->value = res;
	}
	printf("%s %d: %s(%u)\n",tail->type, tail->NO, tail->expr,tail->value);
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
	WP *wp = head;
	if (wp == NULL){
		printf("No watchpoints.\n");
	}
	else {
		printf("%-9s%-16s%-20s%s\n", "Num", "Type", "What", "Value");
		while(wp != NULL){
			printf("%-9d%-16s%-20s%u\n",wp->NO,wp->type,wp->expr,wp->value);
			wp=wp->next;
		}
	}
	return;
}

bool wp_change(){
	bool return_value = false;
	WP *wp = head;
	if (wp != NULL) {
		uint32_t res; 
		bool* success = (bool*)malloc(sizeof(bool));
		*success = true;
		while(wp != NULL){
			res = expr(wp->expr,success);
			assert(*success);
			if (res != wp->value){
				printf("\n%s %d: %-16s\n\n",wp->type,wp->NO,wp->expr);
				printf("Old value = %-12d0x%x\n", wp->value, wp->value);
				printf("New value = %-12d0x%x\n\n", res, res);
				wp->value = res;
				return_value = true;
			}
			wp=wp->next;
		}
		free(success);
	}
	return return_value;
}
