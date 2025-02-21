#pragma once

/** @cond */
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
/** @endcond */
#include "Graphics/Model/Figure.h"


namespace gr
{
	// +-----------------------------------------------------------------------+
	// |                             DECLARATIONS                              |
	// +-----------------------------------------------------------------------+

	/*!
	 * The Canvas class will handle all the drawing of Figure\n
	 */
	class Canvas
	{
	protected:
		sf::RenderWindow window;
		Bounding_box bounding_box;
		sf::View view;

		Config config;

		float zoom = 1;
		float zoom_factor = 1.1;
		float offset_x = 0;
		float offset_y = 0;
		float hold_x = 0;
		float hold_y = 0;
		float hold_offset_x = 0;
		float hold_offset_y = 0;

		enum Mouse_button
		{
			NONE,
			LEFT,
			RIGHT,
			MIDDLE,
			PREV,
			NEXT
		};
		Mouse_button mouse_button = NONE;
		float mouse_x = 0;
		float mouse_y = 0;

	public:
		Canvas() = default;
		~Canvas() = default;

		std::string get_title() const;
		sf::Color get_color(Color col) const;
		sf::Color get_background_color() const;
		sf::Color get_default_plot_color() const;
		sf::Font get_font() const;
		unsigned get_width() const;
		unsigned get_height() const;
		unsigned get_margin() const;

		void set_title(const std::string& new_title);
		void set_color(Color col, const sf::Color& new_col);
		void set_background_color(const sf::Color& new_color);
		void set_default_plot_color(const sf::Color& new_color);
		void set_font(const sf::Font& new_font);
		void set_width(unsigned& new_width);
		void set_height(unsigned& new_height);
		void set_margin(unsigned& new_margin);

		void draw_figure(const Figure& figure);

	protected:
		void open();
		void setup_view();

//		void draw_point(const Point_shp& point);
//		void draw_segment(const Segment_shp& segment);
//		void draw_vector(const Vector_shp& vector);
//		void draw_polygon(const Polygon_shp& polygon);
//		void draw_circle(const Circle_shp& circle);
//		void draw_line(const Line_shp& line);
//		void draw_text(const Text_shp& text);

		void mouse_wheel_scrolled_event(const sf::Event& event);
		void mouse_button_pressed_event(const sf::Event& event);
		void mouse_button_released_event(const sf::Event& event);
		void mouse_moved_event(const sf::Event& event);

		friend void Point_shp::draw(Canvas& canvas) const;
		friend void Segment_shp::draw(Canvas& canvas) const;
		friend void Line_shp::draw(Canvas& canvas) const;
		friend void Polygon_shp::draw(Canvas& canvas) const;
		friend void Circle_shp::draw(Canvas& canvas) const;
		friend void Vector_shp::draw(Canvas& canvas) const;
		friend void Text_shp::draw(Canvas& canvas) const;
	};


	// +-----------------------------------------------------------------------+
	// |                              DEFINITIONS                              |
	// +-----------------------------------------------------------------------+

	std::string Canvas::get_title() const
	{
		return config.window_title;
	}

	sf::Color Canvas::get_color(Color col) const
	{
		return config.get_color(col);
	}

	sf::Color Canvas::get_background_color() const
	{
		return config.colors[BACKGROUND_COLOR];
	}

	sf::Color Canvas::get_default_plot_color() const
	{
		return config.colors[DEFAULT_PLOT_COLOR];
	}

	sf::Font Canvas::get_font() const
	{
		return config.font;
	}

	unsigned Canvas::get_width() const
	{
		return config.width;
	}

	unsigned Canvas::get_height() const
	{
		return config.height;
	}

	unsigned Canvas::get_margin() const
	{
		return config.margin;
	}

	void Canvas::set_title(const std::string& new_title)
	{
		config.window_title = new_title;
	}

	void Canvas::set_color(Color col, const sf::Color& new_col)
	{
		config.colors[col] = new_col;
	}

	void Canvas::set_background_color(const sf::Color& new_color)
	{
		config.colors[BACKGROUND_COLOR] = new_color;
	}

	void Canvas::set_default_plot_color(const sf::Color& new_color)
	{
		config.colors[DEFAULT_PLOT_COLOR] = new_color;
	}

	void Canvas::set_font(const sf::Font& new_font)
	{
		config.font = new_font;
	}

	void Canvas::set_width(unsigned& new_width)
	{
		config.width = new_width;
	}

	void Canvas::set_height(unsigned& new_height)
	{
		config.height = new_height;
	}

	void Canvas::set_margin(unsigned& new_margin)
	{
		config.margin = new_margin;
	}

	void Canvas::open()
	{
		window.clear(config.colors[BACKGROUND_COLOR]);
		if(window.isOpen())
		{
			return;
		}
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;

		if(config.width == -1 || config.height == -1)
		{
			window.create(sf::VideoMode::getDesktopMode(),
						  config.window_title,
						  sf::Style::Default, settings);
		}
		else
		{
			window.create(sf::VideoMode(config.width, config.height),
						  config.window_title,
						  sf::Style::Default, settings);
		}
	}

	void Canvas::setup_view()
	{
		Coordinate xm = bounding_box.get_min_abscissa();
		Coordinate xM = bounding_box.get_max_abscissa();
		Coordinate ym = -bounding_box.get_max_ordinate();
		Coordinate yM = -bounding_box.get_min_ordinate();

		if(xm == xM && ym == yM)
		{
			xm -= 1;
			xM += 1;
			ym -= 1;
			yM += 1;
		}

		float window_format = float(config.width) / float(config.height);
		float figure_format = (xM - xm) / (yM - ym);
		if(window_format < figure_format)
		{
			float margin_offset = 2 * config.margin / float(config.width);
			margin_offset /= (1 - margin_offset);
			margin_offset *= (xM - xm) / zoom;
			view.setSize(margin_offset + (xM - xm) / zoom,
						 margin_offset + (xM - xm) / window_format / zoom);
		}
		else
		{
			float margin_offset = 2 * config.margin / float(config.height);
			margin_offset /= (1 - margin_offset);
			margin_offset *= (xM - xm) / zoom;
			view.setSize(margin_offset + (yM - ym) * window_format / zoom,
						 margin_offset + (yM - ym) / zoom);
		}
		float ratio = view.getSize().x / float(config.width);
		view.setCenter(-ratio * (offset_x + hold_offset_x) + (xm + xM) / 2,
					   -ratio * (offset_y + hold_offset_y) + (ym + yM) / 2);

		window.setView(view);
	}
}