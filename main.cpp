#include "header/header.h"
#include "raylib.h"


// Available colors: beige, black, blue, brown, gold, gray, green, magenta,
// pink, purple, red, white, yellow
int main() 
{
    
    class Button 
    {
        public:
            const int radius = 15;
            int x;
            int y;  
            Color colors;
            Button(int x_input, int y_input, Color color_input) {
                x = x_input;
                y = y_input;
                colors = color_input;
            }
            
    };

    Button button_one(100, 100, RED);
    Button button_two(150, 150, BLUE);
    Button button_three(200, 200, BLACK);
    
    int windowWidth = 1200;
    int windowHeight = 800;

    InitWindow(windowWidth, windowHeight, "Push");

    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle(button_one.x, button_one.y, 25, button_one.colors);
        DrawCircle(button_two.x, button_two.y, 25, button_two.colors);
        DrawCircle(button_three.x, button_three.y, 25, button_three.colors);
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            std::cout << GetMousePosition().x << button_one.x << "\n";

            if(GetMousePosition().x <= button_one.x + 25 && GetMousePosition().x >= button_one.x - 25)
            {
                std::cout << "Hit!" << "\n";
            }
        }
        EndDrawing();
    }
}