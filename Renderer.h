#pragma once

#include <iostream>
#include "windows.h"

namespace ConsoleRenderer {
	class Renderer {
	private:
		int scr_width;
		int scr_height;
		int scr_width_actual;
		int scr_height_actual;
		int* render_buffer;
		char* display_str;
		HANDLE hStdout;//reference to current console
		void plot_line_low(int x1, int y1, int x2, int y2) const;
		void plot_line_high(int x1, int y1, int x2, int y2) const;
		void draw_circle(int xc,int yc,int x,int y) const;
		char choose_symbol(int x, int y) const;
	public:
		Renderer(const int scr_width_, const int scr_height_);
		void DrawPoint(int x, int y) const;
		void DrawPointFull(int x, int y) const;
		void DrawLine(int x1, int y1, int x2, int y2) const;
		void DrawRect(int x, int y, int w, int h) const;
		void DrawRectFill(int x, int y, int w, int h) const;
		void DrawCircle(int xc,int yc,int r) const;
		void Render() const;
		void ClearRenderBuffer() const;
		void ClearScreen() const;
	};
}


