#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct ball {
	float x;
	float y;
	float width;
	float height;
} ball;

int initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED, // window centered horiz.
		SDL_WINDOWPOS_CENTERED,	// window centered vert.
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if(!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return FALSE;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return FALSE;
	}
	return TRUE;
}

void setup(void) {
	ball.x = 20;
	ball.y = 20;
	ball.width = 15;
	ball.height = 15;
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				game_is_running = FALSE;
			break;
	}
}

void update(void) {
	// TODO: waste some time / sleep until we reach the target time
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	// get a delta time factor converted to seconds to be used to update my objects
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();	

	ball.x += 70 * delta_time;
	ball.y += 50 * delta_time;
}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// TODO: here is where we can start drawing our game objects
	SDL_Rect ball_rect = {
		(int)ball.x,
		(int)ball.y,
		(int)ball.width,
		(int)ball.height
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_RenderPresent(renderer);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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
