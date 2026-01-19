// UI Component stubs
#include <SDL2/SDL.h>
#include <string>

namespace SI3LN {
namespace UI {

class Button {
public:
    Button(int x, int y, int w, int h, const std::string& text) {}
    void render(SDL_Renderer* renderer) {}
    bool isClicked(int x, int y) { return false; }
};

class InputField {
public:
    InputField(int x, int y, int w, int h) {}
    void render(SDL_Renderer* renderer) {}
    std::string getText() { return ""; }
};

class Panel {
public:
    Panel(int x, int y, int w, int h) {}
    void render(SDL_Renderer* renderer) {}
};

class ProfileIcon {
public:
    ProfileIcon(int x, int y, int size) {}
    void render(SDL_Renderer* renderer) {}
};

} // namespace UI
} // namespace SI3LN
