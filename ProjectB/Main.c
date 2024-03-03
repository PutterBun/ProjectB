#include <stdio.h>
#include <SDL.h>
#include "./constant.h"
#include <SDL_image.h>


int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* ball_texture = NULL;



int last_frame_time = 0;

int move_up = FALSE;
int move_down = FALSE;
int move_left = FALSE;
int move_right = FALSE;
float movement_speed = 300.0f; // Pixels per second, adjust as needed


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
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer( window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return FALSE;
	}

	if (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG != IMG_INIT_PNG) {
		fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
		return FALSE;
	}


	return TRUE;
}



void process_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				game_is_running = FALSE;
			}
			move_up = event.key.keysym.sym == SDLK_w;
			move_left = event.key.keysym.sym == SDLK_a;
			move_down = event.key.keysym.sym == SDLK_s;
			move_right = event.key.keysym.sym == SDLK_d;
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_w) move_up = FALSE;
			if (event.key.keysym.sym == SDLK_a) move_left = FALSE;
			if (event.key.keysym.sym == SDLK_s) move_down = FALSE;
			if (event.key.keysym.sym == SDLK_d) move_right = FALSE;
			break;
		}
	}
}



SDL_Texture* load_texture(const char* filename, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
	}
	else {
		// Set the blend mode for the texture
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}

	SDL_FreeSurface(surface); // No longer need the surface

	return texture;
}



void setup() {
	ball.x = 20;
	ball.y = 20;
	ball.width = 83;
	ball.height = 97;

	ball_texture = load_texture("C:/Users/Kanta/source/repos/ProjectB/ProjectB/Picture/MC2.png", renderer);

}

void update() {
	//TODO: logic to keep a fixed timestep, waste sometime until we reach time target.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME)); //lock execution to delay.

	//get delta time factor converted to secont to be updated in pixel per sec
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();


	if (move_up) ball.y -= movement_speed * delta_time;
	if (move_down) ball.y += movement_speed * delta_time;
	if (move_left) ball.x -= movement_speed * delta_time;
	if (move_right) ball.x += movement_speed * delta_time;
}

void render() {
	SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Background color
	SDL_RenderClear(renderer);

	if (ball_texture) {
		SDL_Rect ball_rect = {
			(int)ball.x,
			(int)ball.y,
			(int)ball.width,
			(int)ball.height
		};
		SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
	}

	SDL_RenderPresent(renderer);
}



void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	
	printf("Game is running...\n");
	game_is_running = initialize_window();

	setup();

	while (game_is_running){
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
