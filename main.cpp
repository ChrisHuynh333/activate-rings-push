#include "header/header.h"
#include "raylib.h"
#include <vector>
#include <cstdlib> // this is where srand() is defined
#include <ctime> // this is where time() is defined
#include <algorithm>
#include <random>
#include <iterator>
// Available colors: beige, black, blue, brown, gold, gray, green, magenta,
// pink, purple, red, white, yellow

const std::vector<Color> color_array {BLUE, GRAY, WHITE, GREEN, PURPLE, YELLOW, RED};

class Button 
{
    public:
        int radius = 15;
        Color color_one;
        Color color_two;
        int color_one_pos;
        int color_two_pos;
        int x;
        int y;
        
        void set_button_attributes(Color color_one_input, Color color_two_input, int color_one_position, int color_two_position)
        {
            color_one = color_one_input;
            color_two = color_two_input;
            color_one_pos = color_one_position;
            color_two_pos = color_two_position;
        }
        void set_coords(int x_input, int y_input)
        {
            x = x_input;
            y = y_input;
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
int get_random_pos_num()
{
    
    int number = rand() % color_array.size();
    return number;
}

Color get_color(int &position, const std::vector<Color> &colors)
{
    Color color = colors.at(position);
    return color;
}
void set_game_button_attributes(Button &button)
{
    int color_pos_one = get_random_pos_num();
    int color_pos_two = get_random_pos_num();
    Color get_color_one = get_color(color_pos_one, color_array);
    Color get_color_two = get_color(color_pos_two, color_array);
    button.set_button_attributes(get_color_one, get_color_two, color_pos_one, color_pos_two);
}

void check_duplicate_colors_to_targets(std::vector<Target_Button> &targets, Button &test_button)
{
    bool break_bool = true;
    for (int i = 0; i < 4; i++)
    {
        if (targets.at(i).color_one_pos == test_button.color_one_pos && targets.at(i).color_two_pos == test_button.color_two_pos)
        {
            break_bool = false;
            break;
        }
    }
    if (!break_bool)
    {
        set_game_button_attributes(test_button);
        check_duplicate_colors_to_targets(targets, test_button);
    }
}

int main() 
{
    srand (time(NULL));
    int num_of_buttons = 182;
    std::vector<Button> game_buttons(num_of_buttons);
    std::vector<Target_Button> target_buttons(4);
    
    for (int i = 0; i < num_of_buttons; i ++)
    {   
        set_game_button_attributes(game_buttons.at(i));
        if (i < 4)
        {
            // ADD THESE AS POINTERS?
            target_buttons.at(i).set_button_attributes(game_buttons.at(i).color_one, game_buttons.at(i).color_two, game_buttons.at(i).color_one_pos, game_buttons.at(i).color_two_pos);
        }
        else
        {
            check_duplicate_colors_to_targets(target_buttons, game_buttons.at(i));
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(game_buttons.begin(), game_buttons.end(), g);
    int windowWidth = 1200;
    int windowHeight = 800;

    int target_button_x_coord = 150;
    int target_button_y_coord = 400;
    int x_coord = 15;
    int y_coord = 500;
    for (int j = 0; j < num_of_buttons; j++)
    {
        if(j < 4)
        {
            target_buttons.at(j).set_coords(target_button_x_coord, target_button_y_coord);
            target_button_x_coord += 300;
        }
        game_buttons.at(j).set_coords(x_coord, y_coord);
        x_coord += 45;
        if (x_coord >= 1185)
        {
            x_coord = 15;
            y_coord += 45;
        }
    }

    InitWindow(windowWidth, windowHeight, "Push");
    bool initialize_game = false;
    
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(WHITE);
            
        for (int k = 0; k < num_of_buttons; k++)
        {
            if (k < 4)
            {
                
                DrawCircleGradient(target_buttons.at(k).x, target_buttons.at(k).y, 30, target_buttons.at(k).color_one, target_buttons.at(k).color_two);
                
            }
            
            DrawCircleGradient(game_buttons[k].x, game_buttons[k].y, game_buttons[k].radius, game_buttons[k].color_one, game_buttons[k].color_two);
            
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
    CloseWindow();
}