#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

class Rasterizer
{
	public:
		void loadData();
		void clearBuffer();
		void drawPoint(int x, int y, float r, float g, float b);
		static void displayCallback();
};
#endif