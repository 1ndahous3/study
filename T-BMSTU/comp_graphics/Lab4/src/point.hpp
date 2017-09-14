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
extern int blur_rate_sq, blur_rate;

struct Point {

    int x = 0;
    int y = 0;

    Point(int x, int y) :
            x(x), y(y) {
    }
};

class Line {

protected:
    int nel = 0;
    Point a, b;

public:
    std::vector<Point> points;

    Line(Point &a, Point &b) :
            a(a), b(b) {
    }

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

        int dx = b.x - a.x;
        int dy = b.y - a.y;

        if (std::abs(dx) >= std::abs(dy)) {
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

        dx = std::abs(dx);
        dy = std::abs(dy);

        d = 2 * dy;
        err = -dx;

        while (iter1 != p2.x) {
            points.emplace_back(*ptr_x, *ptr_y);
            err += d;
            if (err >= 0) {
                err -= 2 * dx;
                iter2 += sign2;
            }

            iter1 += sign1;
        }
    }

//    void calculateVert() {
//
//        Point p1(a), p2(b);
//        int iter1, iter2, *ptr_x, *ptr_y;
//        short sign1, sign2;
//        float err, d;
//
//        float dx = b.x - a.x;
//        float dy = b.y - a.y;
//
//        if (std::abs(dx) >= std::abs(dy)) {
//            ptr_x = &iter1;
//            ptr_y = &iter2;
//        } else {
//            ptr_x = &iter2;
//            ptr_y = &iter1;
//            std::swap(p1.x, p1.y);
//            std::swap(p2.x, p2.y);
//            std::swap(dx, dy);
//        }
//
//        iter1 = p1.x;
//        iter2 = p1.y;
//        sign1 = std::copysign(1, dx);
//        sign2 = std::copysign(1, dy);
//
//        d = abs(dy / dx);
//        err = -0.5f;
//
//        while (iter1 != p2.x) {
//            points.emplace_back(*ptr_x, *ptr_y);
//            err += d;
//            if (err >= 0) {
//                err -= 1;
//                iter2 += sign2;
//            }
//            iter1 += sign1;
//        }
//    }

//    void calculateVert() {
//        points.clear();
//        nel = 0;
//
//        //std::cout << "a: " << a.x << ' ' << a.y << " b: " << b.x << ' '  << b.y << std::endl;
//
//        float dx = b.x - a.x;
//        float dy = b.y - a.y;
//
//        //std::cout << "dx: " << dx << " dy: " << dy << std::endl;
//
//        float j = a.y;
//        float i = a.x;
//        int k = 1;
//        while (std::abs(i - a.x) < std::abs(dx)) {
//            //std::cout << "i: " << i;
//            //std::cout << "| j: ";
//
//            if (std::abs(j - a.y) > std::abs(dy * k / dx)) {
//                j -= std::copysign(1, dy);
//            }
//
//            while (std::abs(j - a.y) <= std::abs(dy * k / dx)) {
//                //std::cout << j << " ";
//                points.push_back(Point(i, j));
//                j += std::copysign(1, dy);
//                nel++;
//            }
//            k++;
//
//            //std::cout << std::endl;
//            i += std::copysign(1, dx);
//        }
//    }

};

class Shape {

protected:
    int nel = 0;
    int width, height;

public:
    std::vector<Point> points;
    std::vector<float> verts;
    GLubyte *mtrx, *accum_buff;

    Shape() :
            width(::width), height(::height) {

        mtrx = (GLubyte *) calloc(width * height * 3, sizeof(GLubyte));
        accum_buff = (GLubyte *) calloc(width * height * 3, sizeof(GLubyte));

    }

    void resize() {

        if (::width > width || ::height > height) {

            GLubyte *mtrx_tmp = (GLubyte *) calloc(::width * ::height * 3,
                    sizeof(GLubyte));

            for (int i = 0; i < std::min(height, ::height); i++) {
                memcpy(mtrx_tmp + i * ::width * 3, mtrx + i * width * 3,
                        std::min(width, ::width) * 3);
            };

            this->height = ::height;
            this->width = ::width;

            std::swap(mtrx, mtrx_tmp);

            free(mtrx_tmp);

            free(accum_buff);

            accum_buff = (GLubyte *) calloc(::width * ::height * 3,
                    sizeof(GLubyte));
        }
    }

    void addPoint(int x, int y) {

        points.emplace_back(x, y);

        if (points.size() > 1) {
            buffer();
        }
    }

    void clear() {

        points.clear();
    }

    void buffer() {

        memset(mtrx, 0, width * height * 3);
        memset(accum_buff, 0, width * height * 3);

        glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

        int n_curr = blur_rate_sq;

        points.push_back(points.front());

        while (n_curr > 0) {

            std::cout << n_curr << std::endl;

            //glClear(GL_COLOR_BUFFER_BIT);

            for (unsigned int i = 0; i < points.size() - 1; i++) {

                Line line(points[i], points[i + 1]);
                line.calculateVert();

                for (const auto &foo : line.getPoints()) {
                    if (foo.y < height && foo.x < width) {

                        bufferVert(foo, n_curr);
                    }
                }

            }

            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    mtrx[j * width * 3 + i * 3] += (accum_buff[j * width * 3
                            + i * 3] / (float) blur_rate_sq);

                    mtrx[j * width * 3 + i * 3 + 1] += (accum_buff[j * width * 3
                            + i * 3 + 1] / (float) blur_rate_sq);

                    mtrx[j * width * 3 + i * 3 + 2] += (accum_buff[j * width * 3
                            + i * 3 + 2] / (float) blur_rate_sq);

                }
            }

            glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mtrx);

//            std::cout << glGetError() << std::endl;
//            glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mtrx);
//            glAccum(GL_ACCUM, (1.0f / (float) blur_rate_sq));
//            std::cout << glGetError() << std::endl;

            n_curr--;
        }
        points.erase(points.end() - 1);

        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mtrx);
    }

    void bufferVert(const Point &point, int n) {

        std::cout << "- Point: " << point.x << ' ' << point.y << std::endl;
        std::cout << "- n: " << n << std::endl;
        int d = (blur_rate_sq - n) / blur_rate;
        std::cout << "d = " << d << std::endl;

        for (int i = point.x - d; i <= point.x + d; i++) {
            for (int j = point.y - d; j <= point.y + d; j++) {
                int foo = blur_rate - std::abs(point.x - i);
                int bar = blur_rate - std::abs(point.y - j);

                std::cout << "x_rev = " << foo << ", y_rev = " << bar
                        << std::endl;

                if (foo * bar >= n) {
                    std::cout << "Paint:" << i << ' ' << j << std::endl;
                    accum_buff[j * width * 3 + i * 3] = 255;
                    accum_buff[j * width * 3 + i * 3 + 1] = 255;
                    accum_buff[j * width * 3 + i * 3 + 2] = 255;
                }
            }
        }

    }

    void render() {

        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mtrx);
    }

    bool isBorder(Point p) {

        return (mtrx[p.y * width * 3 + p.x * 3] == 255
                && mtrx[p.y * width * 3 + p.x * 3 + 1] == 255
                && mtrx[p.y * width * 3 + p.x * 3 + 2] == 255);
    }

    bool isFilled(Point p) {

        return (mtrx[p.y * width * 3 + p.x * 3] == 100
                && mtrx[p.y * width * 3 + p.x * 3 + 1] == 100
                && mtrx[p.y * width * 3 + p.x * 3 + 2] == 100);
    }

    void fill(Point p) {

        mtrx[p.y * width * 3 + p.x * 3] = 100;
        mtrx[p.y * width * 3 + p.x * 3 + 1] = 100;
        mtrx[p.y * width * 3 + p.x * 3 + 2] = 100;
    }

    void scanLine(std::stack<Point> &st, int x_l, int x_r, int y) {

        bool flag = false;

        for (int x = x_l; x <= x_r; x++) {

            while (!isFilled(Point(x, y)) && !isBorder(Point(x, y)) && x < x_r) {
                flag = true;
                x++;
            }

            if (flag) {
                st.emplace(
                        (x == x_r && !isBorder(Point(x, y))
                                && !isFilled(Point(x, y)) ? x : x - 1), y);

                flag = false;
            }
        }
    }

    void fillShape(int x, int y) {

        std::stack<Point> st;

        st.emplace(x, y);

        while (!st.empty()) {

            Point p(st.top());
            st.pop();

            fill(p);

            Point p_cur(p);

            p_cur.x++;

            while (!isBorder(p_cur)) {
                fill(p_cur);
                p_cur.x++;
            }

            int x_r = p_cur.x - 1;

            p_cur = p;

            p_cur.x--;

            while (!isBorder(p_cur)) {
                fill(p_cur);
                p_cur.x--;
            }

            int x_l = p_cur.x + 1;

            scanLine(st, x_l, x_r, p.y + 1);

            scanLine(st, x_l, x_r, p.y - 1);
        }
    }

}
;
