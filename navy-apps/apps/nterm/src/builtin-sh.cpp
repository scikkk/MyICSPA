#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

#include <string.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
	static char buf[256] = {};
	va_list ap;
	va_start(ap, format);
	int len = vsnprintf(buf, 256, format, ap);
	va_end(ap);
	term->write(buf, len);
}

static void sh_banner() {
	sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
	sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
	char buf[255];
	for(int k = 0; cmd[k] != '\n'; k++){
		if(cmd[k] == ' '){
			buf[k] = '\0';
		}
		else{
			buf[k] = cmd[k];
		}
		buf[k+1] = '\0';
	}
	char *order = &buf[0];
	char *argv[20]= {NULL};
	int argc = 0;
	for(int k = 0; cmd[k] != '\n'; k++) {
		if(cmd[k] == ' ') {
			argv[argc] =  &buf[k+1];
			argv[++argc] = NULL;	
		}
	}
	/* if (strcmp("echo", order) == 0){ */
	/* 	sh_printf("%s", &cmd[4]); */
	/* } */
	/* else */
	if(strcmp("exit", order) == 0){
		_exit(0);
	}
	else {
		execvp(order, argv);	
	}
}

void builtin_sh_run() {
	sh_banner();
	sh_prompt();
	setenv("PATH", "/bin:/usr/bin", 1);
	while (1) {
		SDL_Event ev;
		if (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
				const char *res = term->keypress(handle_key(&ev));
				if (res) {
					sh_handle_cmd(res);
					sh_prompt();
				} 
			}
		}
		refresh_terminal();
	}
}
