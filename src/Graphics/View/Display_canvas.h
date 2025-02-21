#pragma once

#include "Canvas.h"
#include "Graphics/Model/Slide_show.h"


namespace gr
{
	// +-----------------------------------------------------------------------+
	// |                             DECLARATIONS                              |
	// +-----------------------------------------------------------------------+

	/*!
	 *
	 */
	class Display_canvas : public Canvas
	{
	private:
		unsigned nb_slides = 0;
		unsigned slide_index = 0;

	public:
		Display_canvas() = default;
		~Display_canvas() = default;

		void display_figure(Figure& figure);
		void display_slide_show(Slide_show& slide_show);

	private:
		void handle_events();
		void next_slide();
		void prev_slide();
		void key_pressed_event(const sf::Event& event);
	};


	// +-----------------------------------------------------------------------+
	// |                              DEFINITIONS                              |
	// +-----------------------------------------------------------------------+

	void Display_canvas::display_figure(Figure& figure)
	{
		figure.make_bounding_box();
		bounding_box = figure.get_bounding_box();
		open();
		setup_view();
		while(window.isOpen())
		{
			handle_events();
			draw_figure(figure);
			window.display();
		}
	}

	void Display_canvas::display_slide_show(Slide_show& slide_show)
	{
		slide_show[0].make_bounding_box();
		bounding_box = slide_show[0].get_bounding_box();
		nb_slides = slide_show.nb_slides();
		open();
		setup_view();
		while(window.isOpen())
		{
			handle_events();
			draw_figure(slide_show[slide_index]);
			window.display();
		}

	}

	void Display_canvas::next_slide()
	{
		if(slide_index < nb_slides - 1)
		{
			slide_index++;
		}
	}

	void Display_canvas::prev_slide()
	{
		if(slide_index > 0)
		{
			slide_index--;
		}
	}
}