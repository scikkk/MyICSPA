#include <cpu/cpu.h>
#include <cpu/exec.h>
#include <cpu/difftest.h>
#include <isa-all-instr.h>
#include <locale.h>
#include <isa-exec.h>

// wk 2.2 ftrace
#ifdef CONFIG_FTRACE 
void ftrace_write(paddr_t, paddr_t, bool);
void ftrace_display();
void func_display();
#endif 
// wk 2.2 ftrace


// wk add
bool wp_change();
// wk add

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

CPU_state cpu = {};
uint64_t g_nr_guest_instr = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;
const rtlreg_t rzero = 0;
rtlreg_t tmp_reg[4];

void device_update();
void fetch_decode(Decode *s, vaddr_t pc);

// wk 2.2 iringbuf
char iringbuf[16][128];
short iringbuf_idx = 0;
// wk 2.2 iringbuf

static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_ITRACE_COND
	if (ITRACE_COND) log_write("%s\n", _this->logbuf);
#endif
	if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
	IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
	if (wp_change()){
		nemu_state.state=NEMU_STOP;
		return;
	}
}






#define FILL_EXEC_TABLE(name) [concat(EXEC_ID_, name)] = concat(exec_, name),
static const void* g_exec_table[TOTAL_INSTR] = {
	MAP(INSTR_LIST, FILL_EXEC_TABLE)
};

static void fetch_decode_exec_updatepc(Decode *s) {
	fetch_decode(s, cpu.pc);
	s->EHelper(s);
	cpu.pc = s->dnpc;
}

static void statistic() {
	IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%ld", "%'ld")
	Log("host time spent = " NUMBERIC_FMT " us", g_timer);
	Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_instr);
	if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " instr/s", g_nr_guest_instr * 1000000 / g_timer);
	else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
#ifdef CONFIG_FTRACE_FILE_COND
	if(!FTRACE_FILE_COND)	ftrace_display();
#endif 
}

// wk 2.2 iringbuf display
void iringbuf_display(){
	printf("\n");
	for(int k = 0; k < 16; k++){
		if(k == iringbuf_idx){
			printf("--> %s\n", iringbuf[k]);
		}
		else{
			printf("    %s\n", iringbuf[k]);
		}
	}
	printf("\n");
}
// wk 2.2 iringbuf display

void assert_fail_msg() {
	isa_reg_display();
	// wk 2.2 iringbuf
#ifdef CONFIG_ITRACE
	iringbuf_display();
#endif
	// wk 2.2 iringbuf
	statistic();
}

void fetch_decode(Decode *s, vaddr_t pc) {
	s->pc = pc;
	s->snpc = pc;
	int idx = isa_fetch_decode(s);
	s->dnpc = s->snpc;
	s->EHelper = g_exec_table[idx];
#ifdef CONFIG_ITRACE
	char *p = s->logbuf;
	p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
	int ilen = s->snpc - s->pc;
	int i;
	uint8_t *instr = (uint8_t *)&s->isa.instr.val;
	for (i = 0; i < ilen; i ++) {
		p += snprintf(p, 4, " %02x", instr[i]);
	}
	int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
	int space_len = ilen_max - ilen;
	if (space_len < 0) space_len = 0;
	space_len = space_len * 3 + 1;
	memset(p, ' ', space_len);
	p += space_len;

	void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
	disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
			MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.instr.val, ilen);
	// wk 2.2
	iringbuf_idx++;
	iringbuf_idx %= 16;
	strcpy(iringbuf[iringbuf_idx], s->logbuf);
	// wk 2.2
#endif

	// wk 2.2 ftrace
#ifdef CONFIG_FTRACE
	// jal
	if ((s->isa.instr.val&0x7f)==0x6f){
		paddr_t cur = cpu.pc;
		paddr_t dst = cpu.pc+s->src1.simm;
		ftrace_write(cur, dst,1);
		/* printf("%-10x-> %x\n",cur, dst); */
		/* func_display(); */
	}
	// jalr
	if ((s->isa.instr.val&0x7f)==0x67){
		paddr_t cur = cpu.pc;
		paddr_t dst = *(s->src1.preg) + s->src2.simm;
		if (s->isa.instr.val==0x00008067){

			ftrace_write(cur, dst,0);
		}
		else{

			ftrace_write(cur, dst,1);
		}
		/* printf("%-10x-> %x\n",cur, dst); */
		/* func_display(); */
	}
#endif 
	// wk 2.2 ftrace

}


/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
	g_print_step = (n < MAX_INSTR_TO_PRINT);
	switch (nemu_state.state) {
		case NEMU_END: case NEMU_ABORT:
			printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
			return;
		default: nemu_state.state = NEMU_RUNNING;
	}

	uint64_t timer_start = get_time();

	Decode s;
	// wk 2.2
	iringbuf_idx = 0;
	// wk 2.2
	for (;n > 0; n --) {
		fetch_decode_exec_updatepc(&s);
		g_nr_guest_instr ++;
		trace_and_difftest(&s, cpu.pc);
		if (nemu_state.state != NEMU_RUNNING) break;
		IFDEF(CONFIG_DEVICE, device_update());
		// wk 4.3
		word_t intr = isa_query_intr();
		if (intr != INTR_EMPTY) {
			/* printf("cpu exec recieved intr!!!\n"); */
			cpu.pc = isa_raise_intr(intr, cpu.pc);
			/* printf("INTR cpu.pc=0x%x\n", cpu.pc); */
		}
		// wk 4.3
	}

	uint64_t timer_end = get_time();
	g_timer += timer_end - timer_start;

	switch (nemu_state.state) {
		case NEMU_RUNNING: nemu_state.state = NEMU_STOP; break;
		case NEMU_END: 	case NEMU_ABORT: Log("nemu: %s at pc = " FMT_WORD,
												 (nemu_state.state == NEMU_ABORT ? ASNI_FMT("ABORT", ASNI_FG_RED) :
												  (nemu_state.halt_ret == 0 ? ASNI_FMT("HIT GOOD TRAP", ASNI_FG_GREEN) :
												   ASNI_FMT("HIT BAD TRAP", ASNI_FG_RED))),
												 nemu_state.halt_pc);
										 // fall through
		case NEMU_QUIT: statistic();
	}
}
