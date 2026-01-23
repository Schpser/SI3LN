// UI Component stubs
#include <SDL2/SDL.h>
#include <string>

namespace SI3LN
{
	namespace UI
	{

		class Button
		{
		public:
			Button([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int w, [[maybe_unused]] int h, [[maybe_unused]] const std::string &text) {}
			void render([[maybe_unused]] SDL_Renderer *renderer) {}
			bool isClicked([[maybe_unused]] int x, [[maybe_unused]] int y) { return false; }
		};

		class InputField
		{
		public:
			InputField([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int w, [[maybe_unused]] int h) {}
			void render([[maybe_unused]] SDL_Renderer *renderer) {}
			std::string getText() { return ""; }
		};

		class Panel
		{
		public:
			Panel([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int w, [[maybe_unused]] int h) {}
			void render([[maybe_unused]] SDL_Renderer *renderer) {}
		};

		class ProfileIcon
		{
		public:
			ProfileIcon([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int size) {}
			void render([[maybe_unused]] SDL_Renderer *renderer) {}
		};

	} // namespace UI
} // namespace SI3LN
