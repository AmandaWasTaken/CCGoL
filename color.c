#include <stdio.h> // for printf

void change_color(const int color_index){
	printf("\033[%im", color_index);
	return;
}
