#include "header/header.h"
#include "raylib.h"
#include <vector>
// Available colors: beige, black, blue, brown, gold, gray, green, magenta,
// pink, purple, red, white, yellow

const std::vector<Color> color_array {BEIGE, BLACK, BLUE, BROWN, GOLD, GRAY, GREEN, MAGENTA, PINK, PURPLE, RED, YELLOW};

class Button 
{
    public:
        const int radius = 15;
        int x;
        int y;  
        Color color_one;
        Color color_two;
        int color_one_pos;
        int color_two_pos;
        
        void set_button_attributes(int x_coord, int y_coord, Color color_one_input, Color color_two_input, int color_one_position, int color_two_position)
        {
            x = x_coord;
            y = y_coord;
            color_one = color_one_input;
            color_two = color_two_input;
            color_one_pos = color_one_position;
            color_two_pos = color_two_position;
        }
        
};

class Target_Button:public Button
{
    public:
        bool found = false;

        void set_found()
        {
            found = true;
        }
};
int get_random_pos_num(const std::vector<Color> &colors)
{
    return rand() % sizeof(colors);
}

Color get_color(int &position, const std::vector<Color> &colors)
{
    Color color = colors.at(position);
    return color;
}
void set_game_button_attributes(Button &button, int &x, int &y)
{
    int color_pos_one = get_random_pos_num(color_array);
    int color_pos_two = get_random_pos_num(color_array);
    Color get_color_one = get_color(color_pos_one, color_array);
    Color get_color_two = get_color(color_pos_two, color_array);
    button.set_button_attributes(x, y, get_color_one, get_color_two, color_pos_one, color_pos_two);
}

void check_duplicate_colors_to_targets(std::vector<Button> &targets, Button &test_button)
{
    bool break_bool = true;
    for (int i = 0; i < 4; i++)
    {
        std::cout << i << "\n";
        std::cout << targets.at(i).color_one_pos << "/" << test_button.color_one_pos << "/" << targets.at(i).color_two_pos << "/" << test_button.color_two_pos << "\n";
        if (targets.at(i).color_one_pos == test_button.color_one_pos && targets.at(i).color_two_pos == test_button.color_two_pos)
        {
            std::cout << "HIIHGIOAHGOFADHOGA" << "\n";
            break_bool = false;
            break;
        }
    }
    if (!break_bool)
    {
        std::cout << "WHY" << "\n";
        set_game_button_attributes(test_button, test_button.x, test_button.y);
        check_duplicate_colors_to_targets(targets, test_button);
    }
}

int main() 
{
    int x_coord = 15;
    int y_coord = 500;
    int num_of_buttons = 40;
    std::vector<Button> game_buttons(num_of_buttons);
    std::vector<Button> target_buttons(4);
    for (int i = 0; i < num_of_buttons; i ++)
    {   
        set_game_button_attributes(game_buttons.at(i), x_coord, y_coord);
        if (i < 4)
        {
            // ADD THESE AS POINTERS?
            target_buttons.at(i).set_button_attributes(game_buttons.at(i).x, game_buttons.at(i).y, game_buttons.at(i).color_one, game_buttons.at(i).color_two, game_buttons.at(i).color_one_pos, game_buttons.at(i).color_two_pos);
        }
        else
        {
            check_duplicate_colors_to_targets(target_buttons, game_buttons.at(i));
        }

        x_coord += 45;
        if (x_coord >= 1185)
        {
            x_coord = 15;
            y_coord += 45;
        }
    }
    
    int windowWidth = 1200;
    int windowHeight = 800;

    InitWindow(windowWidth, windowHeight, "Push");

    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(WHITE);
        for (int i = 0; i < num_of_buttons; i ++)
        {
            DrawCircleGradient(game_buttons[i].x, game_buttons[i].y, game_buttons[i].radius, game_buttons[i].color_one, game_buttons[i].color_two);
        }
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {

            // if(GetMousePosition().x <= button_one.x + 25 && GetMousePosition().x >= button_one.x - 25)
            // {
            //     std::cout << "Hit!" << "\n";
            // }
        }
        EndDrawing();
    }
}