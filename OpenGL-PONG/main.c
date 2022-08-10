#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int last_frame_time = 0;

///////////////////////////////////////////////////////////////////////////////
// Declare two game objects for the ball and the paddle
///////////////////////////////////////////////////////////////////////////////
struct game_object {
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle, paddleTwo;

///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
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
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////
// Function to poll SDL events and process keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            game_is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) 
                game_is_running = false;
            // Set paddle velocity of first paddle based on A and D
            if (event.key.keysym.sym == SDLK_a) 
                paddle.vel_y = -600;
            if (event.key.keysym.sym == SDLK_d) 
                paddle.vel_y = 600;
            // set paddle velocity of second paddle based on J and L
            if (event.key.keysym.sym == SDLK_j)
                paddleTwo.vel_y = -600;
            if (event.key.keysym.sym == SDLK_l)
                paddleTwo.vel_y = 600;
            break;
        // when key is let go
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_a)
                paddle.vel_y = 0;
            if (event.key.keysym.sym == SDLK_d)
                paddle.vel_y = 0;
            if (event.key.keysym.sym == SDLK_j)
                paddleTwo.vel_y = 0;
            if (event.key.keysym.sym == SDLK_l)
                paddleTwo.vel_y = 0;
            break;
        default:
           break;
    }
}
///////////////////////////////////////////////////////////////////////////////
// Setup function that runs once at the beginning of our program
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    // Initialize the ball object moving down at a constant velocity
    ball.width = 15;
    ball.height = 15;
    ball.x = 500;
    ball.y = -100;
    ball.vel_x = -200;
    ball.vel_y = 200;
    // Initialize the paddle position at the bottom of the screen
    paddle.width = 100;
    paddle.height = 20;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
    paddle.y = WINDOW_HEIGHT - 40; // since -40 represents left-most width "border" paddle 2. 
    paddle.vel_x = 0;
    paddle.vel_y = 0;
    // Initialize second paddle
    paddleTwo.width = 20;
    paddleTwo.height = 100;
    paddleTwo.x = (WINDOW_WIDTH - 40); // since -40 represents left-most width "border" paddle 2. 
    paddleTwo.y = (WINDOW_HEIGHT / 2);
    paddleTwo.vel_x = 0;
    paddleTwo.vel_y = 0;
}
///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Calculate how much we have to wait until we reach the target frame time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    // Only delay if we are too fast too update this frame
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    // Get delta_time factor converted to seconds to be used to update objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    // Store the milliseconds of the current frame to be used in the next one
    last_frame_time = SDL_GetTicks();

    // Update ball position based on its velocity
    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    // Update paddle position based on its velocity
    paddle.x += paddle.vel_x * delta_time;
    paddle.y += paddle.vel_y * delta_time; // nearly forgot this one... why does the game still work when i remove this? 

    // Update paddleTwo position based on its velocity
    paddleTwo.x += paddleTwo.vel_x * delta_time;
    paddleTwo.y += paddleTwo.vel_y * delta_time; // nearly forgot this one... why does the game still work when i remove this? 

    // Check for ball collision with the walls
    if (ball.y + ball.height > WINDOW_HEIGHT || ball.y < 0) // collision with up-down walls
        ball.vel_y = (ball.vel_y * -1);


    // Check for ball collision with paddle 1
    if (ball.y > paddle.y && ball.y < paddle.y + paddle.height) {
        if (ball.x < paddle.x + paddle.width)
            ball.vel_x = ball.vel_x * -1; 
    }
    // Check for ball collision with paddle 2
    if (ball.y > paddleTwo.y && ball.y < paddleTwo.y + paddleTwo.height) {
        if (ball.x > paddleTwo.x - paddleTwo.width)
            ball.vel_x = ball.vel_x * -1;
    }


    // Prevent paddle 1 from moving outside the boundaries of the window
    if (paddle.y <= 0)
        paddle.vel_y = 600; //probably a better solution than adding the "offset"...
    if (paddle.y >= WINDOW_HEIGHT - 100)
        paddle.vel_y = -600;
    //Prevent paddle 2 from moving outside the boundaries of the window
    if (paddleTwo.y <= 0)
        paddleTwo.vel_y = 600; //probably a better solution than adding the "offset"...
    if (paddleTwo.y >= WINDOW_HEIGHT - 100)
        paddleTwo.vel_y = -600;


    // Check for game over when ball hits left-right part of screen
    if (ball.x + ball.width > WINDOW_WIDTH || ball.x < 0) {
        game_is_running = false;
    }
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle for the ball object
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    // Draw a rectangle for the paddle object
    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    // draw second paddle 
    SDL_Rect paddleTwo_rect = {
     (int)paddleTwo.x,
     (int)paddleTwo.y,
     (int)paddleTwo.width,
     (int)paddleTwo.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddleTwo_rect);

    SDL_RenderPresent(renderer);
}
///////////////////////////////////////////////////////////////////////////////
// Function to destroy SDL window and renderer
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]) {
    game_is_running = initialize_window();

    setup();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
