#pragma once

#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <stack>
#include <stdio.h>



#include <string.h>

#include <GL/glew.h>


extern int width, height;

struct Point {
	int x = 0;
	int y = 0;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class Line {
protected:
	int nel = 0;
	Point a, b;

public:
	std::vector<Point> points;

	Line(Point &a, Point &b) : a(a), b(b) {};

	std::vector<Point> &getPoints() {
		return points;
	}

	int getNel() {
		return nel;
	}

	void calculateVert() {

		Point p1(a), p2(b);
		int iter1, iter2, *ptr_x, *ptr_y;
		short sign1, sign2;
		float err, d;

		float dx = b.x - a.x;
		float dy = b.y - a.y;

		if (std::abs(dx) >= std::abs(dy) ) {
			ptr_x = &iter1;
			ptr_y = &iter2;
		} else {
			ptr_x = &iter2;
			ptr_y = &iter1;
			std::swap(p1.x, p1.y);
			std::swap(p2.x, p2.y);
			std::swap(dx, dy);
		}

		iter1 = p1.x;
		iter2 = p1.y;
		sign1 = std::copysign(1, dx);
		sign2 = std::copysign(1, dy);

		d = abs(dy / dx);
		err = -0.5f;

		while (iter1 != p2.x) {
			points.emplace_back(*ptr_x, *ptr_y);
			err += d;
			if (err >= 0) {
				err -= 1;
				iter2 += sign2;
			}
			iter1 += sign1;
		}
	}

	/*void calculateVert() {
		points.clear();
		nel = 0;

		//std::cout << "a: " << a.x << ' ' << a.y << " b: " << b.x << ' '  << b.y << std::endl;

		float dx = b.x - a.x;
		float dy = b.y - a.y;

		//std::cout << "dx: " << dx << " dy: " << dy << std::endl;

		float j = a.y;
		float i = a.x;
		int k = 1;
		while (std::abs(i - a.x) < std::abs(dx)) {
			//std::cout << "i: " << i;
			//std::cout << "| j: ";

			if (std::abs(j - a.y) > std::abs(dy * k / dx)) {
				j -= std::copysign(1, dy);
			}

			while (std::abs(j - a.y) <= std::abs(dy * k / dx)) {
				//std::cout << j << " ";
				points.push_back(Point(i, j));
				j += std::copysign(1, dy);
				nel++;
			}
			k++;

			//std::cout << std::endl;
			i += std::copysign(1, dx);
		}
	}*/

};

class Shape {
protected:
	int nel = 0;
	int width, height;
public:
	std::vector<Point> points;
	std::vector<float> verts;
	GLubyte *mtrx;

	Shape() {
		this->height = ::height;
		this->width = ::width;
		mtrx = (GLubyte *) calloc(width * height * 3, sizeof(GLubyte));
	}

	void resize() {

		//if (::width > width || ::height > height) {

			GLubyte * mtrx_tmp = (GLubyte *) calloc(::width * ::height * 3,
					sizeof(GLubyte));

			for (int i = 0; i < std::min(height, ::height); i++) {
				memcpy(mtrx_tmp + i * ::width * 3, mtrx + i * width * 3,
						std::min(width, ::width) * 3);
			};


			this->height = ::height;
			this->width = ::width;

			std::swap(mtrx, mtrx_tmp);

			free(mtrx_tmp);
//		}
	}

	void addPoint(int x, int y) {
		points.push_back(Point(x, y));
		if (points.size() > 1) {
			calculateVert();
		}
	}

	void clear() {
		points.clear();
	}

	void calculateVert() {
		memset(mtrx, 0, width * height * 3);
		points.push_back(points.front());
		std::cout << points.size() << std::endl;
		for (unsigned int i = 0; i < points.size() - 1; i++) {
			Line line(points[i], points[i + 1]);
//			std::cout << "i: " << i << std::endl;
//			std::cout <<"i, i+1: " << points[i].x << ' ' << points[i].y << ' ' << points[i + 1].x << ' ' << points[i + 1].y << std::endl;
			line.calculateVert();
//			std::cout << "line: ";

/*
			for (auto &foo : line.getPoints()) {
				std::cout << foo.x << ' ' << foo.y << std::endl;
			}
*/

			for (const auto &foo : line.getPoints()) {
				if (foo.y < height && foo.x < width) {
//					std::cout << foo.x << ' ' << foo.y << std::endl;

					mtrx[foo.y * width * 3 + foo.x * 3] = 255;
					mtrx[foo.y * width * 3 + foo.x * 3 + 1] = 255;
					mtrx[foo.y * width * 3 + foo.x * 3 + 2] = 255;
				}
			}
		}

		points.erase(points.end() - 1);
	}

	void render() {
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mtrx);
	}

	bool isBorder (Point p) {
		return (mtrx[p.y * width * 3 + p.x * 3] == 255 &&
				mtrx[p.y * width * 3 + p.x * 3 + 1] == 255 &&
				mtrx[p.y * width * 3 + p.x * 3 + 2] == 255);
	}

	bool isFilled(Point p) {
		return (mtrx[p.y * width * 3 + p.x * 3] == 100 &&
				mtrx[p.y * width * 3 + p.x * 3 + 1] == 100 &&
				mtrx[p.y * width * 3 + p.x * 3 + 2] == 100);
	}

	void fill(Point p) {
		mtrx[p.y * width * 3 + p.x * 3] = 100;
		mtrx[p.y * width * 3 + p.x * 3 + 1] = 100;
		mtrx[p.y * width * 3 + p.x * 3 + 2] = 100;
	}

	void scanLine(std::stack<Point> &st, int x_l, int x_r, int y) {
		int x(x_l);
		while (x <= x_r) {
			bool flag(false);
			while (!isFilled(Point(x, y)) &&
					!isBorder(Point(x, y)) &&
					x < x_r) {
				flag = true;
				x++;
			}
			if (flag) {
				st.push(Point((x == x_r && !isBorder(Point(x, y)) && !isFilled(Point(x, y)) ) ?
						x : x - 1,
						y));
				flag = false;
			}
			int x_tmp(x);

			while ((isBorder(Point(x, y)) || isFilled(Point(x, y))) && x < x_r) {
				x++;
			}

			if (x == x_tmp) {
				x++;
			}
		}
	}


	void fillShape(int x, int y) {
		std::stack<Point> st;

		st.emplace(x, y);

		while (!st.empty()) {

			Point p(st.top());
			st.pop();

			std::cout << "point: " << p.x << ' ' << p.y << std::endl;
			fill(p);

			Point p_cur(p);

			p_cur.x++;

			while (!isBorder(p_cur)) {
				fill(p_cur);
				p_cur.x++;
			}

			int x_r(p_cur.x - 1);

			p_cur = p;

			p_cur.x--;

			while (!isBorder(p_cur)) {
				fill(p_cur);
				p_cur.x--;
			}

			int x_l(p_cur.x + 1);

			scanLine(st, x_l, x_r, p.y + 1);

			scanLine(st, x_l, x_r, p.y - 1);
		}
	}

};
