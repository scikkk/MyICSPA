#include <isa.h>

// wk add
#include <memory/vaddr.h>
// wk add

/* We use the POSIX rege (functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
	/* TODO: Add more token types */
	TK_INT,
	TK_EQ,
	TK_NEQ,
	TK_AND,
	TK_OR,
	TK_POINTER,
	TK_NEGATIVE,
	TK_NOTYPE = 256, 
	TK_DIVIDE = '/',
	TK_MULTIPLY = '*',
	TK_PLUS = '+',
	TK_MINUS = '-',
	TK_LEFT_PARENTHESIS = '(',
	TK_RIGHT_PARENTHESIS = ')',
	TK_HEX = 'X',
	TK_REG = '$',
};

static struct rule {
	const char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", TK_NOTYPE},    // spaces
	{"\\(", TK_LEFT_PARENTHESIS},          // left parenthesis
	{"\\)", TK_RIGHT_PARENTHESIS},          // right parenthesis
	{"0[x|X][0-9a-fA-F]+",TK_HEX}, //hexadecimal
	{"\\$[A-Za-z0-9]+|\\$\\$0", TK_REG},
	{"==", TK_EQ},        // equal
	{"&&", TK_AND},
	{"\\|\\|",TK_OR},
	{"!=", TK_NEQ},
	{"\\+", TK_PLUS},         // plus
	{"-", TK_MINUS},           // minus
	{"\\*", TK_MULTIPLY},         // multiply
	{"/", TK_DIVIDE},           // divide
	{"[1-9][0-9]*|0", TK_INT},   // integral
	// 负号
	// wk
};

//wk add


#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;
// wk modify len for test, old: 32
static Token tokens[65535] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

bool is_num(Token tk) {
	if (tk.type==')'||tk.type == TK_INT||tk.type==TK_NEGATIVE||tk.type==TK_POINTER||tk.type==TK_REG||tk.type==TK_HEX) 
		return true;
	else return false;
}
// wk:  remove a token from tokens  
// void remove_token(int index) {
// 	for (int k = index; k < nr_token - 1; k++) {
// 		tokens[k].type = tokens[k+1].type;
// 		strncpy(tokens[k].str,tokens[k+1].str,32);
// 	}
// 	nr_token--;
// 	return;
// }
// wk: remove a token from tokens

//wk: find signedint
void find_signed_tokens(){
	for (int k = nr_token - 1; k >= 0; k--) {
		if (tokens[k].type == '-') {
			if ((k == 0)||!is_num(tokens[k-1])) {
				tokens[k].type = TK_NEGATIVE;
			}
		}
	}
}
//wk: find signedint


void find_pointer_tokens(){
	for (int k = nr_token - 1; k >= 0; k--) {
		if (tokens[k].type == '*') {
			if ((k == 0)||!is_num(tokens[k-1])) {
				tokens[k].type = TK_POINTER;
			}
		}
	} 
}

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0') {
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i ++) {
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				// comment out for quick test
				// Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);

				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				// wk
				switch (rules[i].token_type) {
					case TK_NOTYPE: break;
					case TK_HEX:strncpy(tokens[nr_token].str,substr_start+2,substr_len);
								tokens[nr_token].str[substr_len-2] = '\0';
								tokens[nr_token].type = TK_HEX;
								nr_token++;
								break;
					case TK_REG : strncpy(tokens[nr_token].str,substr_start+1,substr_len);
								  tokens[nr_token].str[substr_len-1] = '\0';
								  tokens[nr_token].type = TK_REG;
								  nr_token++;
								  break;

					case TK_INT: strncpy(tokens[nr_token].str,substr_start,substr_len);
								 tokens[nr_token].str[substr_len] = '\0';
								 tokens[nr_token].type = TK_INT;
								 nr_token++;
								 break;
					default :		 tokens[nr_token].type = rules[i].token_type;
									 nr_token++;
				}
				break;
			}
		}
		// wk
		if (i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}


// wk
bool pure_check_parenthesis(int p, int q) {
	char stack[10000];
	int top = -1;
	for (int k = p; k <= q; k++) {
		if (tokens[k].type == '(') { 
			top++;
			stack[top] = '(';
			stack[top+1] = '\0';
		}
		else if (tokens[k].type == ')') {
			if (stack[top] != '(') return false;
			stack[top] = '\0';
			top--;
		}
	} 
	if (top != -1) return false;
	else return true;
}
bool check_parentheses(int p, int q, bool *success) {
	if (tokens[p].type != '(' || tokens[q].type != ')') return false;
	*success = pure_check_parenthesis(p,q);
	return pure_check_parenthesis(p+1, q-1);
}

uint32_t eval(int p, int q, bool* success) {
	if (p > q) {
		/* Bad expression */
		*success = false;
		return 0;
	}
	else if (p == q) {
		/* Single token.
		 * For now this token should be a number.
		 * Return the value of the number.
		 */
		switch (tokens[p].type) {
			case TK_INT:  return atoi(tokens[p].str);
			case TK_NEGATIVE: return -atoi(tokens[p].str);break;
			case TK_HEX: {
							 char *rest;
							 uint32_t res =  strtoul(tokens[p].str,&rest,16);
							 assert(strlen(rest) == 0);
							 return res;
						 }
			case TK_REG: return isa_reg_str2val(tokens[p].str,success);
			case TK_POINTER: {
								 char *rest;
								 uint32_t res =  vaddr_read(strtoul(tokens[p].str,&rest,10),4);
								 assert(strlen(rest) == 0);
								 return res;
							 }
			default : *success = false; return 0;
		}
	}
	else if (check_parentheses(p, q, success) == true) {
		/* The expression is surrounded by a matched pair of parentheses.
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1, success);
	}
	else { 
		uint32_t op = p; //the position of 主运算符 in the token expression;
		for (int k = p; k <= q; k++) {
			if (tokens[k].type == '(') {
				int top = 1;
				while (top != 0) {
					k++;
					if (tokens[k].type == '(') top++;
					else if (tokens[k].type == ')') top--;
				}
				continue;
			}
			int optype = tokens[op].type;
			switch (tokens[k].type) {
				case TK_INT:case TK_REG:case TK_HEX: 
					break;
				case TK_OR: op = k; break;
				case TK_AND:if (optype != TK_OR) {op = k;}break;
				case TK_NEQ:case TK_EQ:
					if(optype!=TK_OR && optype!=TK_AND) {op = k;}
					break;
				case '+': case '-':
					if(optype!=TK_OR && optype!=TK_AND && optype!=TK_NEQ && optype!=TK_EQ) {op = k;}
					break;
				case '*':case '/': 
					if (optype!=TK_OR && optype!=TK_AND && optype!=TK_NEQ && optype!=TK_EQ && optype != '+' && tokens[op].type != '-') 
						{op = k;}
					break;
				case TK_NEGATIVE: case TK_POINTER:
					if (optype!=TK_OR && optype!=TK_AND && optype!=TK_NEQ && optype!=TK_EQ && optype != '+' && tokens[op].type != '-'&&optype!='*'&&optype!='/') 
						{op = k;}
					break;

				default : printf("bad do on %d: %d\n",k,tokens[k].type);assert(0);
			}
		}

		int optype = tokens[op].type;
		uint32_t val1 = (optype==TK_POINTER || optype==TK_NEGATIVE)?0:eval(p, op - 1,success); 
		uint32_t val2 = eval(op + 1, q,success);
		switch (optype) {
			case TK_EQ :return val1 == val2;
			case TK_NEQ:return val1 != val2;
			case TK_AND:return val1 && val2;
			case TK_OR:return val1 || val2;
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': if (val2 == 0) {*success = false; return 0;}
						  else return val1 / val2;
			case TK_NEGATIVE: return -val2;
			case TK_POINTER:return vaddr_read(val2,4);
			default: printf("op=%d\ntokens[op].str=%s\n",op,tokens[op].str);assert(0);
		}
	}
	if (!*success) return 0;
}
// wk




word_t expr(char *e, bool *success) {
	if (!make_token(e)) {
		*success = false;
		return 0;
	}
	find_signed_tokens();
	find_pointer_tokens();
	/* TODO: Insert codes to evaluate the expression. */
	// TODO();
	// wk
	return eval(0,nr_token-1 ,success);
	// wk
}

