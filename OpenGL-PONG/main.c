#include <stdio.h>
#include <SDL.h>
#include "./constants.h" // local folder "./"
#include <stdbool.h>

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

int game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = false; 

int last_frame_time = 0; // milliseconds of the last frame called

struct ball {
	float x;
	float y;
	float width;
	float height;
} ball; //already declare ball

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

		// TODO: set paddle velocity based on left / right arrow keys
		break; 
	case SDL_KEYUP:


		// reset paddle velocity based on left / right arrow keys
		break;
	}
}



void setup() {
	ball.x = 20;
	ball.y = 20;
	ball.width = 15;
	ball.height = 15;


}


void update() {
	// WASTE time/sleep until we reach frame target time
	Uint32 timeout = last_frame_time + FRAME_TARGET_TIME;
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
		/* ... do work until timeout has elapsed */
	}

	// get delta time factor converted to seconds to update objects
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f; //float div

	last_frame_time = SDL_GetTicks();

	ball.x += 70 * delta_time;
	ball.y += 50 * delta_time; 






}

void render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//draw rec (SDL_Rect is a type from SDL) 
	SDL_Rect ball_rect = {
		(int)ball.x, // (int) is called a "cast" -> casting float to int
		(int)ball.y,
		(int)ball.width,
		(int)ball.height
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// we pass in the address of the struct
	// instead of passing all the values of the struct one by one,
	// we just pass in a pointer of where the object is located
	// by doing this, we AVOID copying values in memory. 
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_RenderPresent(renderer); 

	// here is where we an start drawing our game objs
	// SDL use double buffering 
	//we see front buffer -> back buffer populating.. when its done
	// buffers "swap"
	// swaps "virtual" screen buffers
	SDL_RenderPresent(renderer);

}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


int main() {
	int game_is_running = initialize_window(); //#1 initialize window

	setup(); // #2 variables of game

	while (game_is_running) { // #3 game loop 
		process_input(); //#3.1 process keyboard input into game state changes
		update();
		render(); //clears screen, draws pixels, double buffer
	}
	destroy_window(); // when game_is_running is false this runs!
	return 0;
}































// in old systems, rendering pixels was easy -> you "poke" pixels into memory
// addresses