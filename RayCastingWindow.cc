/*
 * RayCastingWindow.cc
 *
 *  Created on: 24 РѕРєС‚. 2020 Рі.
 *      Author: unyuu
 */

#include <iostream>
#include "RayCastingWindow.h"
#include <cmath>
#include <cassert>

RayCastingWindow::RayCastingWindow(int width, int height)
: Window(width, height)
{
	_map = std::make_shared<Map>("map01.txt");
	_player = std::make_shared<Player>(_map);
	_wall_texture = std::make_shared<Texture>(_renderer, "wall.jpg");
}

void RayCastingWindow::render()
{
	SDL_Rect r_sky { 0, 0, width(), height() / 2 };
	SDL_Rect r_floor { 0, height() / 2, width(), height() / 2 };
	SDL_SetRenderDrawColor(_renderer.get(), 64, 128, 192, 255);
	SDL_RenderFillRect(_renderer.get(), &r_sky);
	SDL_SetRenderDrawColor(_renderer.get(), 0, 128, 0, 255);
	SDL_RenderFillRect(_renderer.get(), &r_floor);

	SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND);

	//Рисование стен
	double x = _player -> x();
	double y = _player -> y();

	double pi = 3.141592653589793;
	double FOV = pi/3;

	double alpha = _player -> dir(); //Направление взгляда

	double sd = double(width()) / (2. * tan(FOV / 2.));

	double epsilon = 0.00001;

	for (int col = 0; col < width(); ++col) {

		// Алгоритм
		double gamma = atan((col - (width() / 2)) / sd);
		double beta = alpha + gamma;

		double n = 0;
		//Горизнтальные линии
		if (sin(beta) > epsilon){
			dy = 1;
			ry = floor(y) + epsilon;
			dx = 1 / tan(beta);
			rx = x - (y - ry) * dx;

			cast_ray(rx, ry, dx, dy);

			Dh = sqrt(pow((rx + n * dx - x), 2) + pow((ry + n * dy - y), 2));
		} else if (sin(beta) < -epsilon){
			dy = -1;
			ry = ceil(y) - epsilon;
			dx  = 1 / tan(-beta);
			rx = x - (ry - y) * dx;

			cast_ray(rx, ry, dx, dy);

			Dh = sqrt(pow((rx + n * dx - x), 2) + pow((ry + n * dy - y), 2));
		} else
			Dh = 1000000;

		txh = rx - floor(rx);

		//Вертикальные линии
		if (cos(beta) > epsilon){
			dx = 1;
			rx = floor(x) + epsilon;
			dy = tan(beta);
			ry = y - (x - rx) * dy;

			cast_ray(rx, ry, dx, dy);

			Dv = sqrt(pow((rx + n * dx - x), 2) + pow((ry + n * dy - y), 2));
		} else if (cos(beta) < -epsilon){
			dx = -1;
			rx = ceil(x) - epsilon;
			dy = tan(-beta);
			ry = y - (rx - x) * dy;

			cast_ray(rx, ry, dx, dy);

			Dv = sqrt(pow((rx + n * dx - x), 2) + pow((ry + n * dy - y), 2));
		} else
			Dv = 1000000;

		txv = ry - floor(ry);

		D = std::min(Dh, Dv);

		if (D == Dh){
			tx = txh;
		} else {
			tx = txv;
		}

		h = ((sd / cos(gamma)) * 0.5) / D;

		if (_textured)
			draw_texture_col(col, h, tx);
		else
			draw_col(col, h);
	}

	// Рисование карты

	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 255, 64);
	for (int y = 0; y < _map->height(); ++y)
		for (int x = 0; x < _map->width(); ++x) {
			SDL_Rect r { x * 20, y * 20, 20, 20 };
			if (_map->wall(x, y))
				SDL_RenderFillRect(_renderer.get(), &r);
		}

	SDL_Rect r_player {
		int(_player->x() * 20)-10,
		int(_player->y() * 20)-10,
		10, 10
	};
	SDL_Rect r_player_eye {
		int(_player->x() * 20 + 10*cos(_player->dir()))-7,
		int(_player->y() * 20 + 10*sin(_player->dir()))-7,
		5, 5
	};

	SDL_SetRenderDrawColor(_renderer.get(), 255, 64, 64, 255);
	SDL_RenderFillRect(_renderer.get(), &r_player);
	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 0, 255);
	SDL_RenderFillRect(_renderer.get(), &r_player_eye);
}

void RayCastingWindow::draw_col(int col, int h)
{
	SDL_SetRenderDrawColor(_renderer.get(), 100, 100, 100, 255);
	int y1 = height() / 2 - h / 2;
	int y2 = height() / 2 + h / 2;
	SDL_RenderDrawLine(_renderer.get(), col, y1, col, y2);
}

void RayCastingWindow::handle_keys(const Uint8 *keys)
{
	if (keys[SDL_SCANCODE_W]) _player->walk_forward();
	if (keys[SDL_SCANCODE_S]) _player->walk_backward();
	if (keys[SDL_SCANCODE_D]) _player->shift_right();
	if (keys[SDL_SCANCODE_A]) _player->shift_left();
	if (keys[SDL_SCANCODE_E]) _player->turn_right();
	if (keys[SDL_SCANCODE_Q]) _player->turn_left();
	if (keys[SDL_SCANCODE_T]) _textured = false; else _textured = true;
}

void RayCastingWindow::draw_texture_col(int col, int h, double tx) {
	SDL_Rect what{
		int(floor(_wall_texture->getWidth() * tx)),
		0,
		1,
		_wall_texture->getHeight()
	};
	SDL_Rect where{ col, height()/2 - h/2, 1, h };
	_wall_texture->draw(&what, &where);
}

void RayCastingWindow::cast_ray(double &rx, double &ry, double dx, double dy){
	do {
		rx += dx;
		ry += dy;
	} while (not _map->wall(rx, ry));
}





