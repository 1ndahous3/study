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
    float z = 0;

    Pointf() {}

    Pointf(float x, float y)
    : x(x)
    , y(y) {}
};

class Bezier {

private:
	std::map<Pointf, std::vector<Pointf>> verts;

public:

	void calculateVert(Pointf p1, Pointf p2) {

		Point p3 = p2;

		std::vector<Pointf> points;

		for (int i = 0; i <= keyframes; i++) {
			Pointf p;

			float t = (float)i / (float)keyframes;
			float _t = 1.0f - t;

			p.x = _t * _t * p1.x + 2 * t * _t * p3.x + t * t * p2.x;
			p.y = _t * _t * p1.y + 2 * t * _t * p3.y + t * t * p2.y;
			p.z = _t * _t * p1.z + 2 * t * _t * p3.z + t * t * p2.z;

			points.push_back(p);
		}
		
		verts[p1] = points;
	};

	Pointf getPoint(Pointf p, int k) {

		return verts[p][k];
	}

	void clear() {
		
		verts.clear();
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
	int sectors = 64, slices = 5, currentframe = 0;
	std::vector<float> verts, norms;
	Bezier bezier;

	float* getVertsArray() {
		return &verts[0];
	}

	float* getNormsArray() {
		return &norms[0];
	}

	int getNel() {
		return nel;
	}

	virtual void changeFragm(int sectors, int slices, int currentframe) = 0;
	virtual void calculateVert(int keyframes, int currentframe) {
		verts.clear();
	};

	virtual void calculateNorms() {
		norms.clear();
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

	void changeFragm(int sectors, int slices, int currentframe) {
		this->slices = slices;
		this->sectors = sectors;
		this->currentframe = currentframe;
	}

	void calculateNorms(float z) {
		Shape3D::calculateNorms();


		for (int i = 0; i < 4 * sectors * slices; i++) {
			norms.push_back(0);
			norms.push_back(0);
			norms.push_back(z);
		}
	}

	void calculateVert(int keyframes, int current_frame) {
		Shape3D::calculateVert(keyframes, current_frame);
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

			// Pointf p1(transform(1, Pointf(cos(cir1), sin(cir1))));
			// Pointf p2(transform(1, Pointf(cos(cir2), sin(cir2))));

			Pointf p1_start = Pointf(cos(cir1), sin(cir1));
			Pointf p2_start = Pointf(cos(cir2), sin(cir2));

			Pointf p1_finish = transform(1, Pointf(cos(cir1), sin(cir1)));
			Pointf p2_finish = transform(1, Pointf(cos(cir2), sin(cir2)));

			Pointf p1, p2;

			p1.x = p1_start.x + ((p1_finish.x - p1_start.x) / (float)keyframes) * current_frame;
			p1.y = p1_start.y + ((p1_finish.y - p1_start.y) / (float)keyframes) * current_frame;
			p1.z = p1_start.z + ((p1_finish.z - p1_start.z) / (float)keyframes) * current_frame;
				
			p2.x = p2_start.x + ((p2_finish.x - p2_start.x) / (float)keyframes) * current_frame;
			p2.y = p2_start.y + ((p2_finish.y - p2_start.y) / (float)keyframes) * current_frame;
			p2.z = p2_start.z + ((p2_finish.z - p2_start.z) / (float)keyframes) * current_frame;

			for (int j = 1; j <= slices; j++) {
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
				from = j * (1.0f / slices);
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

		// 	for (int j = 1; j <= slices; j++) {
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
		// 		from = j * (1.0f / slices);
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

	void changeFragm(int sectors, int slices, int currentframe) {
		this->slices = slices;
		this->sectors = sectors;
		this->currentframe = currentframe;
	};

	void calculateVert(int keyframes, int current_frame) {
		Shape3D::calculateVert(keyframes, currentframe);
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

	void changeFragm(int sectors, int slices, int currentframe) {
		this->slices = slices;
		this->sectors = sectors;
		this->currentframe = currentframe;
		cir_top.changeFragm(sectors, slices, currentframe);
		cir_bot.changeFragm(sectors, slices, currentframe);
	};

	void calculateNorms() {
		Shape3D::calculateNorms();
		cir_top.calculateNorms(1.0f);
		cir_bot.calculateNorms(-1.0f);

		for (int i = 0; i < sectors; i++) {
			for (int j = 0; j < slices; j++) {
				Pointf n1, p1, p2, p3;
				glm::vec3 v1, v2, v3;

				p1.x = verts[4 * 3 * (i * slices + j) + 0];
				p1.y = verts[4 * 3 * (i * slices + j) + 1];
				p1.z = verts[4 * 3 * (i * slices + j) + 2];

				std::cout << "p1: " << p1.x << ' ' << p1.y << ' ' << p1.z << std::endl;

				//if (j != (slices - 1)) {
					p2.x = verts[4 * 3 * (i * slices + j) + 3 + 0];
					p2.y = verts[4 * 3 * (i * slices + j) + 3 + 1];
					p2.z = verts[4 * 3 * (i * slices + j) + 3 + 2];
				//} else {
				//	p2.x = verts[4 * 3 * (i * slices + j - 1) + 0];
				//	p2.y = verts[4 * 3 * (i * slices + j - 1) + 1]; // shit
				//	p2.z = verts[4 * 3 * (i * slices + j - 1) + 2];
				//}


				std::cout << "p2: " << p2.x << ' ' << p2.y << ' ' << p2.z << std::endl;

				p3 = p1;
				p3.z++;


				std::cout << "p3: " << p3.x << ' ' << p3.y << ' ' << p3.z << std::endl;

				v1 = glm::vec3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
				v2 = glm::vec3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
				v3 = glm::normalize(glm::cross(v2, v1));


				std::cout << "v1: " << v1.x << ' ' << v1.y << ' ' << v1.z << std::endl;
				std::cout << "v2: " << v2.x << ' ' << v2.y << ' ' << v2.z << std::endl;
				std::cout << "v3: " << v3.x << ' ' << v3.y << ' ' << v3.z << std::endl << std::endl;

				norms.push_back(v3.x);
				norms.push_back(v3.y);
				norms.push_back(v3.z);

				norms.push_back(v3.x);
				norms.push_back(v3.y);
				norms.push_back(v3.z);

				norms.push_back(v3.x);
				norms.push_back(v3.y);
				norms.push_back(v3.z);

				norms.push_back(v3.x);
				norms.push_back(v3.y);
				norms.push_back(v3.z);
			}
		}
	}

	void calculateVert(int keyframes, int current_frame) {
		Shape3D::calculateVert(keyframes, currentframe);
		cir_top.calculateVert(keyframes, currentframe);
		cir_bot.calculateVert(keyframes, currentframe);

		for (int i = 0; i < sectors; i++) {
			float cir1 = 2 * M_PI * i / sectors;
			float cir2 = 2 * M_PI * (i + 1) / sectors;
			float from = -1.0f;
			for (int j = 1; j  <= slices; j++) {

				Pointf p1_start = Pointf(cos(cir1), sin(cir1));
				Pointf p2_start = Pointf(cos(cir2), sin(cir2));

				Pointf p1_finish = transform(1, Pointf(cos(cir1), sin(cir1)));
				Pointf p2_finish = transform(1, Pointf(cos(cir2), sin(cir2)));

				Pointf p1, p2;

				p1.x = p1_start.x + ((p1_finish.x - p1_start.x) / (float)keyframes) * current_frame;
				p1.y = p1_start.y + ((p1_finish.y - p1_start.y) / (float)keyframes) * current_frame;
				p1.z = p1_start.z + ((p1_finish.z - p1_start.z) / (float)keyframes) * current_frame;
				
				p2.x = p2_start.x + ((p2_finish.x - p2_start.x) / (float)keyframes) * current_frame;
				p2.y = p2_start.y + ((p2_finish.y - p2_start.y) / (float)keyframes) * current_frame;
				p2.z = p2_start.z + ((p2_finish.z - p2_start.z) / (float)keyframes) * current_frame;

				// Pointf p1(transform(1, Pointf(cos(cir1), sin(cir1))));
				// Pointf p2(transform(1, Pointf(cos(cir2), sin(cir2))));

				// Pointf p1(cos(cir1), sin(cir1));
				// Pointf p2(cos(cir2), sin(cir2));

				verts.push_back(p1.x);
				verts.push_back(p1.y);
				verts.push_back(from);

				verts.push_back(p2.x);
				verts.push_back(p2.y);
				verts.push_back(from);

				from = -1.0f + j * (2.0f / slices);

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

	void changeFragm(int sectors, int slices, int currentframe) {
		this->slices = slices;
		this->sectors = sectors;
		squ_bot.changeFragm(sectors, slices, currentframe);
		squ_top.changeFragm(sectors, slices, currentframe);
		squ_left.changeFragm(sectors, slices, currentframe);
		squ_right.changeFragm(sectors, slices, currentframe);
		squ_back.changeFragm(sectors, slices, currentframe);
		squ_front.changeFragm(sectors, slices, currentframe);
	};

	void calculateVert(int keyframes, int currentframe) {
		Shape3D::calculateVert(keyframes, currentframe);
		squ_bot.calculateVert(keyframes, currentframe);
		squ_top.calculateVert(keyframes, currentframe);
		squ_left.calculateVert(keyframes, currentframe);
		squ_right.calculateVert(keyframes, currentframe);
		squ_back.calculateVert(keyframes, currentframe);
		squ_front.calculateVert(keyframes, currentframe);

		nel = verts.size() / 3;
	};
};
