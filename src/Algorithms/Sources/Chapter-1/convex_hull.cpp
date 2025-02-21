#include "Convex_hull_setup.h"


namespace chap1_convex_hull
{
	typedef alg::Point_2<int> point;
	typedef std::vector<point> convex_hull;
	typedef std::vector<point> point_set;

	gr::Slide_show slides;
	gr::Figure fig_points;
	gr::Figure fig_hull;
	gr::Figure fig_line;

	bool left_turn(const convex_hull& hull, const point& p)
	{
		if(hull.size() < 2)
		{
			return false;
		}

		unsigned n = hull.size();
		return alg::point_left_line(p, hull[n - 2], hull[n - 1]);
	}

	convex_hull make_upper_hull(const point_set& P)
	{
		unsigned n = P.size();
		fig_line.add_vertical_line(P[0].x);
		slides.add_slide(fig_points, fig_line);

		convex_hull U;
		U.push_back(P[0]);
		U.push_back(P[1]);

		fig_hull.add_segment(P[0].x, P[0].y,
							 P[1].x, P[1].y, gr::YELLOW);
		fig_line.clear();
		fig_line.add_vertical_line(P[1].x);
		slides.add_slide(fig_points, fig_line, fig_hull);

		for(unsigned i = 2; i < n; ++i)
		{
			unsigned k = U.size();
			fig_hull.erase_last_plot();
			fig_hull.add_segment(U[k - 2].x, U[k - 2].y,
								 U[k - 1].x, U[k - 1].y, gr::YELLOW);
			fig_line.clear();
			fig_line.add_vertical_line(P[i].x);

			while(left_turn(U, P[i]))
			{
				fig_hull.add_segment(U.back().x, U.back().y,
									 P[i].x, P[i].y, gr::RED);
				slides.add_slide(fig_points, fig_line, fig_hull);
				fig_hull.erase_last_k_plots(2);
				U.pop_back();
			}
			fig_hull.add_segment(U.back().x, U.back().y,
								 P[i].x, P[i].y, gr::GREEN);
			U.push_back(P[i]);
			slides.add_slide(fig_points, fig_line, fig_hull);
		}

		unsigned k = U.size();
		fig_hull.erase_last_plot();
		fig_hull.add_segment(U[k - 2].x, U[k - 2].y,
							 U[k - 1].x, U[k - 1].y, gr::YELLOW);

		return U;
	}

	convex_hull make_lower_hull(const point_set& P)
	{
		unsigned n = P.size();
		slides.add_slide(fig_points, fig_line, fig_hull);

		convex_hull L;
		L.push_back(P[n - 1]);
		L.push_back(P[n - 2]);

		fig_hull.add_segment(P[n - 1].x, P[n - 1].y,
							 P[n - 2].x, P[n - 2].y, gr::YELLOW);
		fig_line.clear();
		fig_line.add_vertical_line(P[n - 2].x);
		slides.add_slide(fig_points, fig_line, fig_hull);

		for(unsigned i = n - 3; i < n; --i)
		{
			unsigned k = L.size();
			fig_hull.erase_last_plot();
			fig_hull.add_segment(L[k - 2].x, L[k - 2].y,
								 L[k - 1].x, L[k - 1].y, gr::YELLOW);
			fig_line.clear();
			fig_line.add_vertical_line(P[i].x);

			while(left_turn(L, P[i]))
			{
				fig_hull.add_segment(L.back().x, L.back().y,
									 P[i].x, P[i].y, gr::RED);
				slides.add_slide(fig_points, fig_line, fig_hull);
				fig_hull.erase_last_k_plots(2);
				L.pop_back();
			}
			fig_hull.add_segment(L.back().x, L.back().y,
								 P[i].x, P[i].y, gr::GREEN);
			L.push_back(P[i]);
			slides.add_slide(fig_points, fig_line, fig_hull);
		}

		unsigned k = L.size();
		fig_hull.erase_last_plot();
		fig_hull.add_segment(L[k - 2].x, L[k - 2].y,
							 L[k - 1].x, L[k - 1].y, gr::YELLOW);
		slides.add_slide(fig_points, fig_line, fig_hull);

		return L;
	}

	void make_convex_hull(point_set& P)
	{
		for(auto p : P)
		{
			fig_points.add_point(p.x, p.y);
		}
		slides.add_slide(fig_points);

		std::sort(P.begin(), P.end(), alg::point_left_point<int>);

		convex_hull U = make_upper_hull(P);
		convex_hull L = make_lower_hull(P);

		U.pop_back();
		L.pop_back();

		for(auto& p : L)
		{
			U.push_back(p);
		}

		gr::Polygon_shp plot_CH(gr::YELLOW);
		for(auto& v : U)
		{
			plot_CH.add_vertex(v.x, v.y);
		}
		fig_hull.clear();
		fig_hull.add_polygon(plot_CH);
		slides.add_slide(fig_points, fig_hull);
	}
}

int main(int argc, char** argv)
{
	using namespace chap1_convex_hull;

	chs::Convex_hull_option opt = chs::process_command_line(argc, argv);
	point_set P = chs::make_point_set(opt);

	alg::save_point_2_set("log/Chapter-1/convex_hull", P);
	make_convex_hull(P);

	gr::Display_canvas canvas;
	canvas.set_title("Convex hull - animation");
	canvas.display_slide_show(slides);

	return 0;
}