/*
		 _       _
	 ___| |_ ___|_|___
	|  _|  _|  _| |_ -|
	|___|_| |_| |_|___|

	ctris is a puzzle video game about falling tetrominoes in a rectangular
	playfield stacking into lines, written in ansi c.

	copyright (c) 2026 baiar tsy <124367691+bxrtsy@users.noreply.github.com>

	permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
	ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/select.h>

#define PF_WIDTH 10
#define PF_HEIGHT 20


const char *title =	"     _       _     \n"
					" ___| |_ ___|_|___ \n"
					"|  _|  _|  _| |_ -|\n"
					"|___|_| |_| |_|___|\n";


void tcsetraw(struct termios *oldt) {
	struct termios newt = *oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void tcreset(struct termios *oldt) {
	tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

void setnonblock(int enable) {
	int fl = fcntl(STDIN_FILENO, F_GETFL, 0);

	if (enable) fl |= O_NONBLOCK;
	else fl &= ~O_NONBLOCK;

	fcntl(STDIN_FILENO, F_SETFL, fl);
}


void usleep(long ms) {
	struct timeval tv;
	tv.tv_sec = ms / 1000000L;
	tv.tv_usec = ms % 1000000L;
	select(0, NULL, NULL, NULL, &tv);
}


const int TM_SHAPES[7][4][4][4] = {
	{
		{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	},
	{
		{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
		{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}
	},
	{
		{{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
		{{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}
	},
	{
		{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
		{{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
		{{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}
	},
	{
		{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
		{{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}
	},
	{
		{{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}
	},
	{
		{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}
	}
};

typedef struct {
	int x, y;
	int rot;
	int type;
} tm;

int tm_predict(int *pf, tm *t, int nx, int ny) {
	int x, y;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (TM_SHAPES[t->type][t->rot][x][y]) {
				int pf_x = nx + x;
				int pf_y = ny + y;

				if (pf_x < 0 || pf_x >= PF_WIDTH || pf_y >= PF_HEIGHT) {
					return 1;
				}

				if (pf_y >= 0 && pf[pf_y * PF_WIDTH + pf_x]) {
					return 1;
				}
			}
		}
	}
	return 0;
}

void tm_lock(int *pf, tm *t) {
	int x, y;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (TM_SHAPES[t->type][t->rot][x][y]) {
				int pf_x = t->x + x;
				int pf_y = t->y + y;

				if (pf_x >= 0 && pf_x < PF_WIDTH && pf_y >= 0 && pf_y < PF_HEIGHT) {
					pf[pf_y * PF_WIDTH + pf_x] = 1;
				}
			}
		}
	}
}


void pf_clear(int *pf, int *score) {
	int x, y;
	for (y = PF_HEIGHT - 1; y >= 0; y--) {
		int full = 1;

		for (x = 0; x < PF_WIDTH; x++) {
			if (!pf[y * PF_WIDTH + x]) {
				full = 0;
				break;
			}
		}

		if (full) {
			int x, sy;
			for (sy = y; sy > 0; sy--) {
				for (x = 0; x < PF_WIDTH; x++) {
					pf[sy * PF_WIDTH + x] = pf[(sy - 1) * PF_WIDTH + x];
				}
			}

			for (x = 0; x < PF_WIDTH; x++) {
				pf[0 * PF_WIDTH + x] = 0;
			}

			*score += 10;
			y++;
		}
	}
}

void pf_render(int *pf, tm *t, int *time, int *score) {
	printf("\033[H\033[32m");
	printf("%s\nTIME:\t%d\nSCORE:\t%d\n\n", title, *time, *score);

	int x, y;
	for (y = 0; y < PF_HEIGHT; y++) {
		printf("<!");
		for (x = 0; x < PF_WIDTH; x++) {
			int is_tm = 0;
			if (x >= t->x && x < t->x + 4 && y >= t->y && y < t->y + 4) {
				is_tm = TM_SHAPES[t->type][t->rot][x - t->x][y - t->y];
			}
			if (pf[y * PF_WIDTH + x] || is_tm) printf("[]");
			else printf(" .");
		}
		printf("!>\n");
	}
	printf("<!====================!>\n");
	printf("  \\/\\/\\/\\/\\/\\/\\/\\/\\/\\/");

	fflush(stdout);
}


int main(void) {
	int *pf = (int *)calloc(PF_WIDTH * PF_HEIGHT, sizeof(int));

	if (!pf) {
		perror("error: calloc failed");
		return 1;
	}

	int i;
	for (i = 0; i < PF_WIDTH * PF_HEIGHT; i++) {
		pf[i] = 0;
	}

	tm t;
	int tm_falling = 0;
	int time = 0;
	int score = 0;

	printf("\033[2J");

	struct termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	tcsetraw(&oldt);
	setnonblock(1);

	int g_timeout = 100;
	int g_time = 0;
	char ch;

	while (1) {
		if (read(STDIN_FILENO, &ch, 1) == 1) {
			if (ch == 'q') break;
			if (tm_falling) {
				if (ch == 'w' && !tm_predict(pf, &t, t.x, t.y + 1)) {
					t.rot = (t.rot + 1) % 4;
				}
				if (ch == 's' && !tm_predict(pf, &t, t.x, t.y + 1)) {
					t.y++;
				}
				if (ch == 'a' && !tm_predict(pf, &t, t.x - 1, t.y)) {
					t.x--;
				}
				if (ch == 'd' && !tm_predict(pf, &t, t.x + 1, t.y)) {
					t.x++;
				}
			}
		}

		if (!tm_falling) {
			t.x = PF_WIDTH / 2 - 2;
			t.y = 0;
			t.rot = 0;
			t.type = rand() % 7;
			tm_falling = 1;
			if (tm_predict(pf, &t, t.x, t.y)) {
				break;
			};
		}

		if (g_time >= g_timeout) {
			if (tm_falling) {
				if (!tm_predict(pf, &t, t.x, t.y + 1)) {
					t.y++;
				} else {
					tm_lock(pf, &t);
					pf_clear(pf, &score);
					tm_falling = 0;
				}
			}
			g_time = 0;
			time++;
		}

		pf_render(pf, &t, &time, &score);

		usleep(10000);

		g_time++;
	}

	setnonblock(0);
	tcreset(&oldt);

	printf("\033[2J\033[H");

	printf("GAME OVER! Press Enter to exit.");
	getchar();

	printf("\033[0m");

	free(pf);

	return 0;
}
