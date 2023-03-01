#include "Renderer.h"

namespace ConsoleRenderer {
	constexpr char symbol_lookup[64] { //oreder of some symbols is incorrect
		' ', //1 bit end
		'\'', 
		'`',
		'\"', //2 bit end
		',',
		':',
		'^',
		'^', //3 bit end
		',',
		'^',
		'`',
		'^',
		'-',
		'j',
		'l',
		'=', //4 bit end
		'.',
		';',
		'\\',
		'7',
		'i',
		'|',
		'\\',
		'7',
		'<',
		'<',
		'l',
		'4',
		'>',
		'<',
		'l',
		'Q', //5 bit end
		'.',
		'/',
		':',
		'1',
		'-',
		'\\',
		'>',
		'7',
		'i',
		'\\',
		'|',
		'F',
		'<',
		'/',
		'>',
		'?',
		'_',
		'j',
		'=',
		'=',
		'>',
		'/',
		'b',
		']',
		'<',
		'd',
		'L',
		'[',
		'm',
		'G',
		'G',
		'#' //6 bit end
	};

	Renderer::Renderer(const int scr_width_, const int scr_height_):scr_width(scr_width_), scr_height(scr_height_) {
		scr_width_actual = scr_width /= 2;
		scr_height_actual = scr_height /= 3;
		render_buffer = new int[scr_width_actual*scr_height_actual];
		ClearRenderBuffer();

		display_str = new char[(scr_width_actual+1)*scr_height_actual];
		std::fill_n(display_str,(scr_width_actual+1)*scr_height_actual, ' ');
		for(int i=0;i<scr_height_actual-1;i++) {
			display_str[(scr_width_actual*(i+1)+i)] = '\n';
		}
		display_str[(scr_width_actual+1)*scr_height_actual-1] = '\0'; //without termination character it prints some garbage data

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE); //set reference to current console
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(hStdout,&cci);
		cci.bVisible = false; 
		SetConsoleCursorInfo(hStdout,&cci); //hide cursor
	}

	char Renderer::choose_symbol(const int x,const int y) const {
		return symbol_lookup[render_buffer[x+y*scr_width_actual]];
	}

	void Renderer::DrawPoint(const int x, const int y) const {
		const int pixel_value = (1<<(y%3)*2)<<(x%2);
		render_buffer[x/2+y/3*scr_width_actual] |= pixel_value;
	}
	void Renderer::DrawPointFull(const int x, const int y) const {
		render_buffer[x/2+y/3*scr_width_actual] = 63;
	}

	void Renderer::plot_line_low(const int x1, const int y1, const int x2, const int y2) const {
		int dx = x2 - x1;
		int dy = y2 - y1;
		int yi = 1;
		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}
		int D = (2 * dy) - dx;
		int y = y1;
		for (int x = x1; x<x2; x++) {
			DrawPoint(x, y);
			if (D > 0) {
				y = y + yi;
				D = D + (2 * (dy - dx));
			} else {
				D = D + 2*dy;
			}
		}
	}
	void Renderer::plot_line_high(const int x1, const int y1, const int x2, const int y2) const {
		int dx = x2 - x1;
		int dy = y2 - y1;
		int xi = 1;
		if (dx < 0){
			xi = -1;
			dx = -dx;
		}
		int D = (2 * dx) - dy;
		int x = x1;

		for (int y = y1; y<y2;y++) {
			DrawPoint(x, y);
			if (D > 0) {
				x = x + xi;
				D = D + (2 * (dx - dy));
			} else {
				D = D + 2*dx;
			}
		}
	}
	void Renderer::DrawLine(const int x1, const int y1, const int x2, const int y2) const {
		if (abs(y2 - y1) < abs(x2 - x1)) {
			if (x1 > x2) {
				plot_line_low(x2, y2, x1, y1);
			} else {
				plot_line_low(x1, y1, x2, y2);
			}
		} else {
			if (y1 > y2) {
				plot_line_high(x2, y2, x1, y1);
			} else {
				plot_line_high(x1, y1, x2, y2);
			}
		}
	}
	
	void Renderer::DrawRect(const int x, const int y, const int w, const int h) const {
		DrawLine(x  , y  , x+w,	y  );
		DrawLine(x  , y+h, x+w,	y+h);
		DrawLine(x  , y  , x  ,	y+h);
		DrawLine(x+w, y  , x+w,	y+h);
	}
	void Renderer::DrawRectFill(const int x, const int y, const int w, const int h) const {
		DrawRect(x,y,w,h);
		for(int y1 = y+1;y1<y+h-1;y1++) {
			for(int x1 = x+1;x1<x+w-1;x1++) {
				DrawPoint(x1,y1);
			}
		}
	}


	void Renderer::draw_circle(const int xc,const int yc,const int x,const int y) const {
		DrawPoint(xc+x,yc+y);
		DrawPoint(xc-x,yc+y);
		DrawPoint(xc+x,yc-y);
		DrawPoint(xc-x,yc-y);
		DrawPoint(xc+y,yc+x);
		DrawPoint(xc-y,yc+x);
		DrawPoint(xc+y,yc-x);
		DrawPoint(xc-y,yc-x);
	}
	void Renderer::DrawCircle(const int xc,const int yc,const int r) const {
		int x = 0;
		int y = r;
		int d = 3 - 2*r;
		draw_circle(xc,yc,x,y);
		while(y >= x){
			x++;
			if(d > 0){
				y--;
				d = d + 4 * (x - y) + 10;
			} else
				d = d + 4 * x + 6;
			draw_circle(xc,yc,x,y);
		}
	}


	void Renderer::Render() const {
		for(int y = 0;y<scr_height_actual;y++) {
			for(int x = 0;x<scr_width_actual;x++){
				display_str[x+y*(scr_width_actual+1)] = choose_symbol(x,y);

				//SetConsoleCursorPosition(hStdout,{static_cast<SHORT>(x),static_cast<SHORT>(y)});
				//std::cout<<choose_symbol(x,y);
			}
		}
		std::cout<<display_str;
	}

	void Renderer::ClearRenderBuffer() const {
		std::fill_n(render_buffer,scr_width_actual*scr_height_actual,0);
	}

	void Renderer::ClearScreen() const {
		ClearRenderBuffer();
		SetConsoleCursorPosition(hStdout,{0,0});
	}

}
