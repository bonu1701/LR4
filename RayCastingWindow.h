/*
 * RayCastingWindow.h
 *
 *  Created on: 24 окт. 2020 г.
 *      Author: unyuu
 */

#ifndef RAYCASTINGWINDOW_H_
#define RAYCASTINGWINDOW_H_

#include <memory>

#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Texture.h"

class RayCastingWindow: public Window
{
protected:
	std::shared_ptr<Map> _map;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Texture> _wall_texture;

	bool _textured { true };

	double dx, dy;
	double rx, ry;
	double Dh, Dv, D;
	double h, tx, txh, txv;

	void draw_col(int col, int h);
	void draw_texture_col(int col, int h, double tx);
public:
	RayCastingWindow(
			int width = DEFAULT_WIDTH,
			int height = DEFAULT_HEIGHT);
	virtual ~RayCastingWindow() = default;
	RayCastingWindow(const RayCastingWindow &other) = delete;
	RayCastingWindow(RayCastingWindow &&other) = default;

	virtual void render() override;
	virtual void handle_keys(const Uint8*) override;

	void cast_ray(double &rx, double &ry, double dx, double dy);
};

#endif /* RAYCASTINGWINDOW_H_ */
