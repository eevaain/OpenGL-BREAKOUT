#include <stdio.h>
#include <SDL.h>
#include "./constants.h" // local folder "./"

// doest matter where you put the pointer -> compiler interprets same
// pointer is the TYPE
// declaration consists of TYPE followed by VARIABLE
// pointer is a variable which stores the address of another variable
//  window is the NAME of the POINTER variable
// window POINTS to the address of the return TYPE SDL_Window
// NULL is the variable declaration
// SDL_Window* window is the pointer variable declaration
// we set NULL value to pointer variable because we dont have an exact address to assign
// this is called a NULL pointer, therefore points to nothing
// since SDL_Window is the type, in the line below, we're saying "return window
// in C we initialize our variables first so we understand WHEN and HOW they are being used
// window is a POINTER to SDL_Window. SDL_Window is a type. 

// window is a variable that holds a value... the value is an address. by using asterik
// we are saying "value in address" -> and we are setting THAT to NULL. 

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL; 

// whenever a function doesnt recieve anything, speficy void in parameters
int initialize_window(void) {
	// initializes everything cus we're lazy
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		// just a formatted printf, outputs to stderr
		fprintf(stderr, "Error initializing SDL.\n");
		// return 0 if something went bad
		return FALSE;
	}
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		// write to default error file where i can write error messages
		fprintf(stderr, "Error creating SDL window .\n");
		return 0;
	}
	// -1 means default display driver, 0 means no special way of rendering
	renderer = SDL_CreateRenderer(window, -1, 0);
		if (!renderer) {
			fprintf(stderr, "Error creating an SDL renderer.\n");
			return FALSE;
		}

	return TRUE;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) { // does event inherit .type from SDL_Event or SDL_PollEvent? 
	case SDL_QUIT:
		game_is_running = FALSE;// x button of window
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			game_is_running = FALSE; 
		break; 
	}
}

void update() {

}

void render() {

}

void destory_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


int main() {
	int game_is_running = initialize_window();

	setup();

	while (game_is_running) {
		process_input();
		update();
		render(); //draw 
	}

	destroy_window();

	return 0;
}































// in old systems, rendering pixels was easy -> you "poke" pixels into memory
// addresses