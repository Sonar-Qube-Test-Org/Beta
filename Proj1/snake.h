#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "graph.h"

const int DIRECTION[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

struct Body
{
	int x;
	int y;
	Body* next;
	Body* prev;

	Body()
	{
		x = 0;
		y = 0;
		next = NULL;
		prev = NULL;
	}
	void print()
	{
		printf("(%d, %d)", x, y);
	}
};

class Snake
{
private:
	Body *head;
	Body *tail;
	int dir;	//0 ~ 3
	int length;
public:
	Snake();
	~Snake();
	int getx() const;
	int gety() const;
	int getLength() const;
	int getDirection() const;
	void move();
	void grow();
	void setPosition(int x, int y);	//initialize
	void changePosition(int x, int y);	//out of range
	void setDirection(int d);
	void changeDirection(int d);
	void print();	//print snake head and clear tailf 
	void printWhole();	//print whole snake
	void clear();	//clear graph on cruiser
	bool biteSelf();
	bool onSnake(int x, int y);
	bool exportData(FILE* fp) const;
	bool loadData(FILE* fp);
};

Snake::Snake()
{
	Body* body = new(Body);
	head = new(Body);
	tail = new(Body);
	//start length is 2
	//head->body->[tail(hidden)]
	head->next = body;
	body->prev = head;
	body->next = tail;
	tail->prev = body;
	dir = 3;	//right
	length = 2;
}
Snake::~Snake()
{
	Body* body = this->tail;

	do
	{
		body = body->prev;
		delete this->tail;
		this->tail = body;
	} while (body != NULL);
}
int Snake::getx() const
{
	return this->head->x;
}
int Snake::gety() const
{
	return this->head->y;
}
int Snake::getLength() const
{
	return this->length;
}
int Snake::getDirection() const
{
	return this->dir;
}
void Snake::move()
{
	Body* body = tail;
	while (body != head)
	{
		body->x = body->prev->x;
		body->y = body->prev->y;
		body = body->prev;
	}
	head->x += DIRECTION[dir][0];
	head->y += DIRECTION[dir][1];
}
void Snake::grow()
{
	Body* extra = new(Body);

	this->tail->prev->next = extra;
	extra->prev = this->tail->prev;
	extra->next = this->tail;
	this->tail->prev = extra;
	extra->x = tail->x;
	extra->y = tail->y;
	length++;
}
void Snake::setPosition(int x, int y)
{
	Body* body = this->head;

	while (body != NULL)
	{
		body->x = x;
		body->y = y;
		body = body->next;
	}
}
void Snake::changePosition(int x, int y)	//out of range
{
	this->head->x = x;
	this->head->y = y;

	return ;
}
void Snake::setDirection(int d)
{
	this->dir = d;
}
void Snake::changeDirection(int d)
{
	
	if ((this->dir + 6) % 4 != d)
		this->dir = d;
}
void Snake::print()	//print the whole snake
{	
	gotoPosition(this->tail->x, this->tail->y);
	putchar(SPACE);
	gotoPosition(this->head->x, this->head->y);
	putchar(SNAKE);
}
void Snake::printWhole()
{
	Body* body;

	for (body = this->head; body != this->tail; body = body->next)
	{
		gotoPosition(body->x, body->y);
		putchar(SNAKE);
	}
}
void Snake::clear()	//clear the whole snake
{
	Body* body = this->head;
	
	while (body != NULL)
	{
		gotoPosition(body->x, body->y);
		putchar(SPACE);
		body = body->next;
	}
}
bool Snake::biteSelf()
{
	Body* body = this->head;

	if ((this->head->x == this->head->next->x) && (this->head->y == this->head->next->y))	//original form
		return false;

	body = body->next;
	while (body != tail)
	{
		if ((body->x == this->head->x) && (body->y == this->head->y))
			return true;
		body = body->next;
	}
	return false;
}
bool Snake::onSnake(int x, int y)
{
	Body* body = this->head;

	while (body != tail)
	{
		if ((body->x == x) && (body->y == y))
			return true;
		body = body->next;
	}
	return false;
}
bool Snake::exportData(FILE* fp) const
{
	Body* body;

	fprintf(fp, "[snake info]\n");
	fprintf(fp, "%d %d\n", dir, length);
	for (body = this->head; body != this->tail; body = body->next)
		fprintf(fp, "%d %d\n", body->x, body->y);

	return true;
}
bool Snake::loadData(FILE* fp)
{
	int x, y;
	int len;
	Body* body;

	if (fscanf(fp, "%*[^0123456789] %d %d", &this->dir, &len) != 2)
		return false;
	for (int i = 2; i < len; i++)
		grow();
	for (body = this->head; body != this->tail; body = body->next)
	{
		if (fscanf(fp, "%d %d", &x, &y) != 2)
			return false;
		body->x = x;
		body->y = y;
	}

	return true;
}

#endif