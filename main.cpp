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

class Round:public Lives{};

int get_random_pos_num(const std::vector<Color> &colors)
{
    // A bug occurs when index 0 is selected, so I added "BLACK" at index 0 (I don't want to use black) and shifted the vector over.
    // Was unable to figure out why the bug occurred. If a target button had a color_one with index 0, the game_button had a different
    // circle color scheme even though all the button attributes were the same.
    int number = rand() % (colors.size() - 1) + 1;
    return number;
}

void set_game_button_attributes(Button &button, const std::vector<Color> &colors)
{
    int color_pos_one = get_random_pos_num(colors);
    int color_pos_two = get_random_pos_num(colors);
    Color get_color_one = colors.at(color_pos_one);
    Color get_color_two = colors.at(color_pos_two);
    button.set_button_attributes(get_color_one, get_color_two, color_pos_one, color_pos_two);
}

void check_duplicate_colors_to_targets(std::vector<Target_Button> &targets, Button &test_button, const std::vector<Color> &colors)
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
        set_game_button_attributes(test_button, colors);
        check_duplicate_colors_to_targets(targets, test_button, colors);
    }
}

void initialize_game(std::vector<Target_Button> &target_btns, std::vector<Button> &game_btns, const int &num_of_btns, const std::vector<Color> &colors) 
{
    for (int i = 0; i < 4; i ++)
    {   
        set_game_button_attributes(game_btns.at(i), colors);
        // ADD THESE AS POINTERS?
        target_btns.at(i).set_button_attributes(game_btns.at(i).color_one, game_btns.at(i).color_two, game_btns.at(i).color_one_pos, game_btns.at(i).color_two_pos);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(game_btns.begin(), game_btns.end(), g);
    
    int target_button_x_coord = int(150);
    int target_button_y_coord = 400;
    int x_coord = 15;
    int y_coord = 500;
    for (int j = 0; j < num_of_btns; j++)
    {
        if(j < 4)
        {
            target_btns.at(j).set_coords(target_button_x_coord, target_button_y_coord);
            target_button_x_coord += 300;
        }
        game_btns.at(j).set_coords(x_coord, y_coord);
        if(!game_btns.at(j).color_one_pos)
        {
            set_game_button_attributes(game_btns.at(j), colors);
            check_duplicate_colors_to_targets(target_btns, game_btns.at(j), colors);
        }
        else
        {
            for(int a = 0; a < 4; a++)
            {
                if(!target_btns.at(a).x_in_game && target_btns.at(a).color_one_pos == game_btns.at(j).color_one_pos && target_btns.at(a).color_two_pos == game_btns.at(j).color_two_pos)
                {
                    target_btns.at(a).set_in_game_coords(game_btns.at(j).x, game_btns.at(j).y);
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
}
void new_round(const int &num_of_btns, std::vector<Target_Button> &target_btns, std::vector<Button> &game_btns, const std::vector<Color> &colors)
{
    for (int i = 0; i < num_of_btns; i++)
    {
        if(i < 4)
        {
            target_btns.at(i) = Target_Button();
        }
        game_btns.at(i) = Button();
    }
    initialize_game(target_btns, game_btns, num_of_btns, colors);
}
int main() 
{
    srand (time(NULL));
    const std::vector<Color> color_array {BLACK, BLUE, GRAY, GREEN, PURPLE, YELLOW, RED};
    const int num_of_buttons = 182;
    std::vector<Button> game_buttons(num_of_buttons);
    std::vector<Target_Button> target_buttons(4);
    const clock_t begin_time = std::clock();
    int past_timer_clock = 0;
    int current_timer;
    int minutes = 2;
    int seconds = 0;
    int score = 1000;
    bool game_over = false;
    int past_timer_score = 0;
    int correct_counter = 0;
    int round_counter = 1;
    std::vector<Lives> lives(5);
    int lives_counter = 0;
    std::vector<Round> rounds(3);
    int lives_x = 333;
    int rounds_x = 670;
    int windowWidth = 1200;
    int windowHeight = 800;
    for(int i = 0; i < 5; i++)
    {
        if(i < 3)
        {
            if(i < round_counter)
            {
                std::string color = "green";
                rounds.at(i).set_attributes(GREEN, color, rounds_x);
            }
            else
            {
                std::string color = "black";
                rounds.at(i).set_attributes(BLACK, color, rounds_x);
            }
        }
        std::string color = "green";
        lives.at(i).set_attributes(GREEN, color, lives_x);
        lives_x += 30;
        rounds_x += 30;
    }


    InitWindow(windowWidth, windowHeight, "Push");
    initialize_game(target_buttons, game_buttons, num_of_buttons, color_array);
    while(!WindowShouldClose() && !game_over) 
    {
        if (correct_counter == 4)
        {
            if(round_counter < 3)
            {
                if(round_counter < 3)
                {
                    new_round(num_of_buttons, target_buttons, game_buttons, color_array);
                    std::string color = "green";
                    rounds.at(round_counter).set_attributes(GREEN, color, rounds.at(round_counter).x);
                    correct_counter = 0;
                    round_counter ++;
                }
                else
                {
                    for(int i = 0; i < 5; i++)
                    {
                        if(lives.at(i).color_str == "green")
                        {
                            lives_counter ++;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                int lives_points = lives_counter * 50;
                DrawText("Congratulations, you won!", 270, 200, 50, GREEN);
                DrawText(TextFormat("Your score is %d plus %d more from lives.", score, lives_points), 50, 250, 50, GREEN);
                game_over = true;
            }
            
        }
        BeginDrawing();
        ClearBackground(WHITE);
        current_timer = float( clock () - begin_time );
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
        
        DrawText("Time", 100, 15, 35, BLACK);
        DrawText(TextFormat("%02d:%02d", minutes, seconds), 80, 50, 50, BLACK);
        DrawText("Lives", 350, 15, 35, BLACK);
        for(int i = 0; i < 5; i++)
        {
            DrawCircle(lives.at(i).x, 75, 10, lives.at(i).color);
        }
        DrawText("Round", 650, 15, 35, BLACK);
        for(int i = 0; i < 3; i++)
        {
            DrawCircle(rounds.at(i).x, 75, 10, rounds.at(i).color);
        }
        DrawText("Score", 950, 15, 35, BLACK);
        DrawText(TextFormat("%d", score), 960, 50, 50, BLACK);

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
                                correct_counter ++;
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