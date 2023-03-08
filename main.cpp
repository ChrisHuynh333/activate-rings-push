#include "header/header.h"
#include "raylib.h"
#include <vector>
#include <cstdlib> // this is where srand() is defined
#include <ctime> // this is where time() is defined
#include <algorithm>
#include <random>
#include <iterator>
#include <chrono>
#include <thread>
// Available colors: beige, black, blue, brown, gold, gray, green, magenta,
// pink, purple, red, white, yellow

std::vector<Color> color_array {BLACK, BLUE, GRAY, GREEN, PURPLE, YELLOW, RED};

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

class Lives
{
    public:
        Color color;
        std::string color_str;
        int x;

        void set_attributes(Color color_input, std::string &color_string, int &x_input)
        {
            color = color_input;
            color_str = color_string;
            x = x_input;
        }
};
int get_random_pos_num()
{
    // A bug occurs when index 0 is selected, so I added "BLACK" at index 0 (I don't want to use black) and shifted the vector over.
    // Was unable to figure out why the bug occurred. If a target button had a color_one with index 0, the game_button had a different
    // circle color scheme even though all the button attributes were the same.
    int number = rand() % (color_array.size() - 1) + 1;
    return number;
}

void set_game_button_attributes(Button &button)
{
    int color_pos_one = get_random_pos_num();
    int color_pos_two = get_random_pos_num();
    Color get_color_one = color_array.at(color_pos_one);
    Color get_color_two = color_array.at(color_pos_two);
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
    int num_of_buttons = 8;
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
                if(!target_buttons.at(a).x_in_game && target_buttons.at(a).color_one_pos == game_buttons.at(j).color_one_pos && target_buttons.at(a).color_two_pos == game_buttons.at(j).color_two_pos)
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
    
    
    const clock_t begin_time = std::clock();
    int past_timer_clock = 0;
    int current_timer;
    int minutes = 2;
    int seconds = 0;
    int score = 1000;
    bool game_over = false;
    int past_timer_score = 0;

    std::vector<Lives> lives(5);
    int lives_x = 237;
    for(int i = 0; i < 5; i++)
    {
        std::string color = "green";
        lives.at(i).set_attributes(GREEN, color, lives_x);
        lives_x += 30;
    }


    InitWindow(windowWidth, windowHeight, "Push");
    while(!WindowShouldClose()) 
    {
        while(!game_over)
        {
            BeginDrawing();
            ClearBackground(WHITE);
            // current_timer = float( clock () - begin_time );
            if (current_timer - past_timer_clock > 1000)
            {
                past_timer_clock = current_timer;
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
            if (current_timer - past_timer_score > 120 && score > 0)
            {
                past_timer_score = current_timer;
                score -= 1;
            }
            
            DrawText("Time", 17, 15, 35, BLACK);
            DrawText(TextFormat("%02d:%02d", minutes, seconds), 15, 40, 50, BLACK);
            DrawText("Lives", 250, 15, 35, BLACK);
            for(int i = 0; i < 5; i++)
            {
                DrawCircle(lives.at(i).x, 65, 10, lives.at(i).color);
            }
            DrawText("Score", 550, 15, 35, BLACK);
            DrawText(TextFormat("%d", score), 550, 40, 50, BLACK);

            if (lives.at(0).color_str == "red")
            {
                game_over = true;
                DrawText("Game Over! Out Of Lives", 320, 310, 50, RED);
            }

            for (int k = 0; k < num_of_buttons; k++)
            {
                if (k < 4)
                {
                    
                    DrawCircleGradient(target_buttons.at(k).x, target_buttons.at(k).y, 30, target_buttons.at(k).color_one, target_buttons.at(k).color_two);
                    
                }
                
                DrawCircleGradient(game_buttons.at(k).x, game_buttons.at(k).y, game_buttons.at(k).radius, game_buttons.at(k).color_one, game_buttons.at(k).color_two);
                
            }
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if(GetMousePosition().y >= 485)
                {
                
                    for (int l = 0; l < 4; l++)
                    {
                        if(GetMousePosition().x <= target_buttons.at(l).x_in_game + 15 && GetMousePosition().x >= target_buttons.at(l).x_in_game - 15 && GetMousePosition().y <= target_buttons.at(l).y_in_game + 15 && GetMousePosition().y >= target_buttons.at(l).y_in_game - 15)
                        {
                            for(int m = 0; m < num_of_buttons; m++)
                            {
                                if (game_buttons.at(m).x == target_buttons.at(l).x_in_game && game_buttons.at(m).y == target_buttons.at(l).y_in_game)
                                {
                                    game_buttons.at(m).set_colors(WHITE, WHITE);
                                    target_buttons.at(l).set_colors(WHITE, WHITE);
                                    break;
                                }
                            }
                            break;
                        }
                        else if (l == 3)
                        {
                            for (int i = 4; i > -1; i--)
                            {
                                if(lives.at(i).color_str == "green")
                                {
                                    std::string color = "red";
                                    lives.at(i).set_attributes(RED, color, lives.at(i).x);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            EndDrawing();

            if(IsKeyPressed(KEY_ESCAPE))
            {
                CloseWindow();
                break;
            }
        }
        if(game_over)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            CloseWindow();
        }
    }    
}