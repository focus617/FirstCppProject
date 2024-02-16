#pragma once

namespace xuzy::UI::Types
{
	/**
	* @brief Data structur that contains color information 
	* in a 0.f to 1.f float format
	*/
	struct Color
	{
		Color(float p_r = 1.0f, float p_g = 1.0f, float p_b = 1.0f, float p_a = 1.0f);

		float r;
		float g;
		float b;
		float a;

		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
		static const Color Black;
		static const Color Grey;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;

		/**
		* @brief Compares two colors, returns true if they are identical
		* @param p_other
		*/
		bool operator==(const Color& p_other);

		/**
		* @brief Compares two colors, returns true if they are different
		* @param p_other
		*/
		bool operator!=(const Color& p_other);
	};

} // namespace xuzy::UI::Types