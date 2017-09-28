#pragma once

#include <cmath>
#include <iostream>
#include <vector>

struct Point {

    int x = 0;
    int y = 0;

    Point() {}

    Point(int x, int y) :
            x(x), y(y) {
    }
};

struct Pointf {

    float x = 0;
    float y = 0;

    Pointf() {}

    Pointf(float x, float y) :
            x(x), y(y) {
    }
};

Pointf transform(int k, Pointf p) {
	
		Pointf p_out;
		float d;

		d = (1.0f/ k) - std::max(std::abs(p.x), std::abs(p.y));

		p_out.x = p.x + std::copysign(d, p.x);
		p_out.y = p.y + std::copysign(d, p.y);

		std::cout << "was: " << p.x << ' ' << p.y << std::endl;
		std::cout << "now: " << p_out.x << ' ' << p_out.y << std::endl;

		return p_out;
	}

class Shape3D {
protected:
	int nel = 0;
public:
	int sectors = 64, sections = 5;
	std::vector<float> verts;

	float* getArray() {
		return &verts[0];
	}


	int getNel() {
		return nel;
	}

	virtual void changeFragm(int sectors, int sections) = 0;
	virtual void calculateVert() {
		std::cout << verts.size() << std::endl;
		verts.clear();
	};
	virtual ~Shape3D() {};
};

class Shape2D : public Shape3D {
public:
	enum Plane { XY, YZ, XZ };
	Plane plane = Plane::XY;
	float pos = 0.0f;
};

class Circle : public Shape2D {
public:
	Circle(Plane plane, float pos) {
		this->plane = plane;
		this->pos = pos;
	}
	void changeFragm(int sectors, int sections) {
		this->sections = sections;
		this->sectors = sectors;
	};

	void calculateVert() {
		Shape3D::calculateVert();
		float a, b, c, *d, *e;
		switch (plane) {
		case Plane::XY:
			c = pos;
			d = &a;
			e = &b;
			break;
		case Plane::YZ:
			a = pos;
			d = &b;
			e = &c;
			break;
		case Plane::XZ:
			b = pos;
			d = &a;
			e = &c;
			break;
		}



		for (int i = 0; i < sectors; i++) {
			float cir1 = 2 * M_PI * i / sectors;
			float cir2 = 2 * M_PI * (i + 1) / sectors;
			float from = 0.0f;

			Pointf p1(transform(1, Pointf(cos(cir1), sin(cir1))));
			Pointf p2(transform(1, Pointf(cos(cir2), sin(cir2))));

			for (int j = 1; j <= sections; j++) {
				*d = from * p1.x;
				*e = from * p1.y;
				verts.push_back(a);
				verts.push_back(b);
				verts.push_back(c);
				*d = from * p2.x;
				*e = from * p2.y;
				verts.push_back(a);
				verts.push_back(b);
				verts.push_back(c);
				from = j * (1.0f / sections);
				*d = from * p2.x;
				*e = from * p2.y;
				verts.push_back(a);
				verts.push_back(b);
				verts.push_back(c);
				*d = from * p1.x;
				*e = from * p1.y;
				verts.push_back(a);
				verts.push_back(b);
				verts.push_back(c);
			}
		}

		// 		for (int i = 0; i < sectors; i++) {
		// 	float cir1 = 2 * M_PI * i / sectors;
		// 	float cir2 = 2 * M_PI * (i + 1) / sectors;
		// 	float from = 0.0f;

		// 	for (int j = 1; j <= sections; j++) {
		// 		*d = from * cos(cir1);
		// 		*e = from * sin(cir1);
		// 		verts.push_back(a);
		// 		verts.push_back(b);
		// 		verts.push_back(c);
		// 		*d = from * cos(cir2);
		// 		*e = from * sin(cir2);
		// 		verts.push_back(a);
		// 		verts.push_back(b);
		// 		verts.push_back(c);
		// 		from = j * (1.0f / sections);
		// 		*d = from * cos(cir2);
		// 		*e = from * sin(cir2);
		// 		verts.push_back(a);
		// 		verts.push_back(b);
		// 		verts.push_back(c);
		// 		*d = from * cos(cir1);
		// 		*e = from * sin(cir1);
		// 		verts.push_back(a);
		// 		verts.push_back(b);
		// 		verts.push_back(c);
		// 	}
		// }

		nel = verts.size() / 3;
	}
	~Circle() {};

};




class Square : public Shape2D {
public:
	Square(Plane plane, float pos) {
		this->plane = plane;
		this->pos = pos;
	};

	void changeFragm(int sectors, int sections) {
		this->sections = sections;
		this->sectors = sectors;
	};

	void calculateVert() {
		Shape3D::calculateVert();
		float a, b, c, *d, *e;
		switch (plane) {
		case Plane::XY:
			c = pos;
			d = &a;
			e = &b;
			break;
		case Plane::YZ:
			a = pos;
			d = &b;
			e = &c;
			break;
		case Plane::XZ:
			b = pos;
			d = &a;
			e = &c;
			break;
		}

		*d = 1.0f;
		*e = 1.0f;
		verts.push_back(a);
		verts.push_back(b);
		verts.push_back(c);
		*d = -1.0f;
		verts.push_back(a);
		verts.push_back(b);
		verts.push_back(c);
		*e = -1.0f;
		verts.push_back(a);
		verts.push_back(b);
		verts.push_back(c);
		*d = 1.0f;
		verts.push_back(a);
		verts.push_back(b);
		verts.push_back(c);

		nel = verts.size() / 3;
	}

};



class Cylinder : public Shape3D {
public:
	Circle cir_top, cir_bot;
	Cylinder() : cir_top(Shape2D::XY, -1.0f), cir_bot(Shape2D::XY, 1.0f) {}

	void changeFragm(int sectors, int sections) {
		this->sections = sections;
		this->sectors = sectors;
		cir_top.changeFragm(sectors, sections);
		cir_bot.changeFragm(sectors, sections);
	};

	

	void calculateVert() {
		Shape3D::calculateVert();
		cir_top.calculateVert();
		cir_bot.calculateVert();

		for (int i = 0; i < sectors; i++) {
			float cir1 = 2 * M_PI * i / sectors;
			float cir2 = 2 * M_PI * (i + 1) / sectors;
			float from = -1.0f;
			for (int j = 1; j  <= sections; j++) {

				Pointf p1(transform(1, Pointf(cos(cir1), sin(cir1))));
				Pointf p2(transform(1, Pointf(cos(cir2), sin(cir2))));

				// Pointf p1(cos(cir1), sin(cir1));
				// Pointf p2(cos(cir2), sin(cir2));

				verts.push_back(p1.x);
				verts.push_back(p1.y);
				verts.push_back(from);

				verts.push_back(p2.x);
				verts.push_back(p2.y);
				verts.push_back(from);

				from = -1.0f + j * (2.0f / sections);

				verts.push_back(p2.x);
				verts.push_back(p2.y);
				verts.push_back(from);

				verts.push_back(p1.x);
				verts.push_back(p1.y);
				verts.push_back(from);
			}
		}

		nel = verts.size() / 3;
	};
};


class Cube : public Shape3D {
public:
	Square squ_bot, squ_top, squ_left, squ_right, squ_back, squ_front;
	Cube() :
		squ_bot(Shape2D::XZ,-1.0f),
		squ_top(Shape2D::XZ, 1.0f),
		squ_left(Shape2D::YZ,-1.0f),
		squ_right(Shape2D::YZ,1.0f),
		squ_back(Shape2D::XY,-1.0f),
		squ_front(Shape2D::XY, 1.0f) {};

	void changeFragm(int sectors, int sections) {
		this->sections = sections;
		this->sectors = sectors;
		squ_bot.changeFragm(sectors, sections);
		squ_top.changeFragm(sectors, sections);
		squ_left.changeFragm(sectors, sections);
		squ_right.changeFragm(sectors, sections);
		squ_back.changeFragm(sectors, sections);
		squ_front.changeFragm(sectors, sections);
	};

	void calculateVert() {
		Shape3D::calculateVert();
		squ_bot.calculateVert();
		squ_top.calculateVert();
		squ_left.calculateVert();
		squ_right.calculateVert();
		squ_back.calculateVert();
		squ_front.calculateVert();

		nel = verts.size() / 3;
	};
};
