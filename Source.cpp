
#include "Renderer.h"
#include <conio.h>
#include <iostream>

int main(){
	constexpr int val_scale = 50;
	int y_val = 0;
	int y_inc = 1;
	int y_val2 = 0;
	int y_inc2 = 1;
	int x_val = 0;
	int x_inc = 1;
	ConsoleRenderer::Renderer renderer(200,80);
	while(true){
		renderer.ClearScreen();
		renderer.DrawLine(x_val/val_scale,20-y_val/val_scale +y_val2/val_scale,20+x_val/val_scale,y_val/val_scale+y_val2/val_scale);
		renderer.DrawRectFill(20,50,50,20);
		renderer.Render();
		y_val+=y_inc;
		y_val2+=y_inc2;
		x_val+=x_inc;
		if(y_val>=30*val_scale || y_val<=0){
			y_inc*=-1;
		}
		if(y_val2>=60*val_scale || y_val2<=0){
			y_inc2*=-1;
		}
		if(x_val>=160*val_scale || x_val<=0){
			x_inc*=-1;
		}
		if(_kbhit() && _getch() == 27) {
			break;
		}
	}
	return 0;
}