


#include "Shape.h"

/********************************/
//Shape Implementation
Shape::Shape(shapeType type, Point loc1, Point loc2, Point Vel,uint16_t Color){
	if(type == RECT){
		this->_loc1 = loc1;
		this->_loc2 = loc2;
		this->_type = type;
		this->_vel =Vel;
		this->_color =Color;
	}
}

Shape::Shape(shapeType type, Point loc, Point Vel,uint8_t radius,uint16_t Color){
	if(type == CIRCLE){
		this->_loc1 = loc;
		this->_type = type;
		this->_vel =Vel;
		this->_radius = radius;
		this->_color =Color;
	}
}

void Shape::UpdatePos(){
	this->_loc1.x +=_vel.x;
	this->_loc1.y +=_vel.y;
};

shapeType Shape::getType(){
	return this->_type;
}
Point Shape::getLoc1(){
	return this->_loc1;
}
Point Shape::getLoc2(){
	return this->_loc2;
}
Point Shape::getVel(){
	return this->_vel;;
}
uint8_t Shape::getRadius(){
	return this->_radius;
}
uint16_t Shape::getColor(){
	return _color;
}
void Shape::setType(shapeType type){
	_type=type;
}
void Shape::setLoc1(Point loc){
	_loc1=loc;
}
void Shape::setLoc2(Point loc){
	_loc2=loc;
}
void Shape::setVel(Point vel){
	_vel=vel;
}
void Shape::setRadius(uint8_t radius){
	_radius=radius;
}
void Shape::setColor(uint16_t color){
	_color=color;
}