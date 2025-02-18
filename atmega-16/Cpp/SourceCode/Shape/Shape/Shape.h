
#ifndef SHAPE_H_
#define SHAPE_H_

#include <math.h>
#include <avr/io.h>

struct Point{
	int x;
	int y;
};
enum shapeType{CIRCLE,RECT};

/**********************************************************************************/
//Shape class
class Shape{
	public:
	Shape(shapeType type, Point loc1, Point loc2, Point Vel, uint16_t Color);

	Shape(shapeType type, Point loc, Point Vel,uint8_t radius,uint16_t Color);

	void UpdatePos();

	shapeType getType();
	Point getLoc1();
	Point getLoc2();
	Point getVel();
	uint8_t getRadius();
	uint16_t getColor();

	void setType(shapeType type);
	void setLoc1(Point loc);
	void setLoc2(Point loc);
	void setVel(Point vel);
	void setRadius(uint8_t radius);
	void setColor(uint16_t color);


	bool Draw(bool fill);

	private:
	Point _loc1;
	Point _loc2;
	Point _vel;
	shapeType _type;
	uint8_t _radius;
	uint16_t _color;
};

#endif /* SHAPE_H_ */