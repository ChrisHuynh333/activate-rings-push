#include "header/header.h"
#include "raylib.h"
#include <vector>
#include <cstdlib> // this is where srand() is defined
#include <ctime> // this is where time() is defined
#include <algorithm>
#include <random>
#include <iterator>
#include <chrono>
// Available colors: beige, black, blue, brown, gold, gray, green, magenta,
// pink, purple, red, white, yellow

const std::vector<Color> color_array {BLUE, GRAY, GREEN, PURPLE, YELLOW, RED};

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
        
        void set_button_attributes(Color &color_one_input, Color &color_two_input, int &color_one_position, int &color_two_position)
        {
            color_one = color_one_input;
            color_two = color_two_input;
            color_one_pos = color_one_position;
            color_two_pos = color_two_position;
        }
        void set_coords(int &x_input, int &y_input)
        {
            x = x_input;
            y = y_input;
        }

        void set_colors(Color color_one_input, Color color_two_input)
        {
            color_one = color_one_input;
            color_two = color_two_input;
        }
        
};

class Target_Button:public Button
{
    public:
        bool found = false;
        int x_in_game;
        int y_in_game;

        void set_found()
        {
            found = true;
        }

        void set_in_game_coords(int &x_input, int &y_input)
        {
            x_in_game = x_input;
            y_in_game = y_input;
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
    
    for (int i = 0; i < 4; i ++)
    {   
        set_game_button_attributes(game_buttons.at(i));
        // ADD THESE AS POINTERS?
        target_buttons.at(i).set_button_attributes(game_buttons.at(i).color_one, game_buttons.at(i).color_two, game_buttons.at(i).color_one_pos, game_buttons.at(i).color_two_pos);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(game_buttons.begin(), game_buttons.end(), g);
    int windowWidth = 1200;
    int windowHeight = 800;

    int target_button_x_coord = int(150);
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
        if(!game_buttons.at(j).color_one_pos)
        {
            set_game_button_attributes(game_buttons.at(j));
            check_duplicate_colors_to_targets(target_buttons, game_buttons.at(j));
        }
        else
        {
            for(int a = 0; a < 4; a++)
            {
                if(target_buttons.at(a).color_one_pos == game_buttons.at(j).color_one_pos && target_buttons.at(a).color_two_pos == game_buttons.at(j).color_two_pos)
                {
                    target_buttons.at(a).set_in_game_coords(game_buttons.at(j).x, game_buttons.at(j).y);
                    break;
                }
            }
        }
        
        x_coord += 45;
        if (x_coord >= 1185)
        {
            x_coord = 15;
            y_coord += 45;
        }
    }
    
    InitWindow(windowWidth, windowHeight, "Push");
    const clock_t begin_time = std::clock();
    int past_timer = 0;
    int current_timer;
    int minutes = 0;
    int seconds = 5;
    bool game_over = false;
    while(!WindowShouldClose()) 
    {
        while(!game_over)
        {
            current_timer = float( clock () - begin_time );
            if (current_timer - past_timer > 1000)
            {
                past_timer = current_timer;
                if (seconds == 0) 
                {
                    if (minutes > 0)
                    {
                        seconds = 59;
                        minutes -= 1;
                    }
                    else
                    {
                        game_over = true;
                        DrawText("Game Over! Out Of Time", 320, 310, 50, RED);
                    }
                }
                else
                {
                    seconds -= 1;
                }
            }
            BeginDrawing();
            ClearBackground(WHITE);
            DrawText(TextFormat("%02d:%02d", minutes, seconds), 15, 15, 50, BLACK);
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
                for (int l = 0; l < 4; l++)
                {
                    if(GetMousePosition().x <= target_buttons.at(l).x_in_game + 15 && GetMousePosition().x >= target_buttons.at(l).x_in_game - 15 && GetMousePosition().y <= target_buttons.at(l).y_in_game + 15 && GetMousePosition().y >= target_buttons.at(l).y_in_game - 15)
                    {
                        for(int m = 0; m < num_of_buttons; m++)
                        {
                            if (game_buttons.at(m).x == target_buttons.at(l).x_in_game && game_buttons.at(m).y == target_buttons.at(l).y_in_game)
                            {
                                std::cout << game_buttons.at(m).x << "/" << game_buttons.at(m).y << "\n";
                                game_buttons.at(m).set_colors(WHITE, WHITE);
                                target_buttons.at(l).set_colors(WHITE, WHITE);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            
            EndDrawing();
        }
        if(game_over)
        {
            _sleep(5000);
            CloseWindow();
        }
    }
    if(!game_over)
    {   
        CloseWindow();
    }
    
}