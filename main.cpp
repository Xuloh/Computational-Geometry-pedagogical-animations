#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "src/Graphics/View/View.h"

int main()
{
    gr::Figure figure;
    gr::Circle circle(3, 4, 1);
    figure.add_circle(circle);
    gr::Point a (0, 0);
    gr::Point b (0, 5);
    gr::Point c (5, 5);
    gr::Point d (5, 0);
    gr::Polygon P ({a, b, c, d});
    figure.add_polygon(P);

    gr::Canvas canvas;
    canvas.display_figure(figure);

    return 0;
}

/*
    gr::Point a (0, 0);
    gr::Point b (0, 1);
    gr::Point c (1, 0);

    gr::Figure figure;
    figure.add_point(a);
    figure.add_point(b);
    figure.add_point(c);

    gr::Slide_show slide_show;
    slide_show.add_figure(figure);
    for(unsigned i = 1; i < 5; ++i)
    {
        gr::Point point(float(i) / 5, float(i) / 5);
        figure.add_point(point);
        slide_show.add_figure(figure);
    }

    gr::Canvas canvas;
    canvas.display_slide_show(slide_show);

    return 0;

    ******************************************************

    gr::Point a(100, 100);
    gr::Point b(200, 200);
    gr::Point c(180, 180);
    gr::Point d(180, 200);
    gr::Point e(150, 150);
    gr::Polygon P({a, b, c, d, e});

    gr::Figure figure;
    figure.add_polygon(P);
    gr::Canvas canvas;
    canvas.display_figure(figure);
*/