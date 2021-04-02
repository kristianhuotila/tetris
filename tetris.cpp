#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
/* 
Gör ett tetrisobject med följande metoder: rotate(), reinit(), move() och typ copy(& ..[4])
Lista ut hur man kan få det att åka ner med jämna intervall samtidigt som man kan flytta det ett diskret avstånd med knapptryck
TODO! Konfigurera om alla tetrisar till den enligt officiella, sen rotation på det

Gameklassen ska innehålla nån typ av 2d array av rektangelobject som håller koll på var alla tetrisar hamnar sen, inita den i 
konstruktorn för ett gameobjekt, med alla fält satt till svart färg, rita den före själva tetrisobjektet

KVAR: Gör koden vettig med game-klass och allt, lös själva detect-grejen, bara ned ska trigga reinit, inte höger och vänster också
fixa droppet och själva tetrisen, kolla även efter en bättre random-seed
*/


//----------------------CONSTANTS-------------------------------------------------------//
//stores all the start positions, the first square in each sub-array is the pivot square

    const int BLOCK_SIZE = 30; 
    const int NROF_ROWS = 20; 
    const int NROF_COLS = 10; 

    const float block_config[7][4][2] = {
                                    {{4.0f, 0.0f}, //bar
                                    {3.0f, 0.0f},
                                    {5.0f, 0.0f},
                                    {6.0f, 0.0f}},

                                    {{5.0f, 0.0f}, //box
                                    {5.0f, 1.0f},
                                    {4.0f, 0.0f},
                                    {4.0f, 1.0f}},

                                    {{5.0f, 1.0f}, //right hook
                                    {4.0f, 1.0f},
                                    {6.0f, 1.0f},
                                    {6.0f, 0.0f}},

                                    {{5.0f, 1.0f}, //left hook
                                    {4.0f, 0.0f},
                                    {4.0f, 1.0f},
                                    {6.0f, 1.0f}},

                                    {{5.0f, 1.0f}, //right romb
                                    {4.0f, 1.0f},
                                    {5.0f, 0.0f},
                                    {6.0f, 0.0f}},

                                    {{5.0f, 1.0f}, //left romb
                                    {4.0f, 0.0f},
                                    {5.0f, 0.0f},
                                    {6.0f, 1.0f}},

                                    {{5.0f, 1.0f}, //camera
                                    {4.0f, 1.0f},
                                    {5.0f, 0.0f},
                                    {6.0f, 1.0f}}


                                     
                                    };

///--------------------------TETRIS CLASS-----------------------------------------------///                                    

class Tetris {
    public: Tetris();  
    void rotation(); 
    void reinit(); 
    void move_tetris(sf::Keyboard::Key key);
    void revert(); 
    //void getBlock(sf::RectangleShape& block, int index); 
    sf::RectangleShape getBlock(int index); 

    public: 
    std::vector<sf::RectangleShape> blocks; 

    private:
    int rot_number = 0; 
    int block_num; 
    float old_position[4][2]; 


    

};

Tetris::Tetris()
{
   
    sf::RectangleShape sq1(sf::Vector2f(BLOCK_SIZE * 1.0f, BLOCK_SIZE * 1.0f));
    sf::RectangleShape sq2(sf::Vector2f(BLOCK_SIZE * 1.0f, BLOCK_SIZE * 1.0f));
    sf::RectangleShape sq3(sf::Vector2f(BLOCK_SIZE * 1.0f, BLOCK_SIZE * 1.0f));
    sf::RectangleShape sq4(sf::Vector2f(BLOCK_SIZE * 1.0f, BLOCK_SIZE * 1.0f));
    blocks.push_back(sq1); 
    blocks.push_back(sq2); 
    blocks.push_back(sq3); 
    blocks.push_back(sq4); 
 
    reinit(); 

}

void Tetris::reinit() {
    block_num = rand() % 7; 
    //stores all the start positions, the first squared in each sub-array is the pivot square, 

    
    for (int i = 0; i < 4; i++) {
        blocks[i].setPosition(block_config[block_num][i][0] * BLOCK_SIZE, block_config[block_num][i][1] * BLOCK_SIZE);
    }

    switch (block_num)
    {
    case 0:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color::Cyan); 
        }
        break;
    
    case 1:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color::Yellow); 
        }
        break;

    case 2:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color(255,69,0)); 
        }
        break;

    case 3:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color::Blue); 
        }
        break;

    case 4:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color::Green); 
        }
        break;

    case 5:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color::Red); 
        }
        break;

    case 6:
        for (int i = 0; i < 4; i++) {
            blocks[i].setFillColor(sf::Color(75,0,130)); 
        }
        break;
    }
    
} //reinit()

void Tetris::rotation() {

    if (block_num != 1) {

        float v_prime_x; 
        float v_prime_y; 
        old_position[0][0] = blocks[0].getPosition().x; 
        old_position[0][1] = blocks[0].getPosition().y; 

        for (int i = 1; i < 4; i++) {
            v_prime_x = blocks[0].getPosition().x - blocks[i].getPosition().x; 
            v_prime_y = blocks[0].getPosition().y - blocks[i].getPosition().y; 
            //store the old block state
            old_position[i][0] = blocks[i].getPosition().x; 
            old_position[i][1] = blocks[i].getPosition().y; 

            blocks[i].setPosition(blocks[0].getPosition().x + v_prime_y, blocks[0].getPosition().y - v_prime_x); 
        }
    }
} //rotation


void Tetris::move_tetris(sf::Keyboard::Key key) {
       sf::Vector2f movement(0.0f, 0.0f); 
    if (key == sf::Keyboard::S) {
        movement.y += BLOCK_SIZE;
    }
    if (key == sf::Keyboard::A) {
        movement.x -= BLOCK_SIZE; 
    }
    if (key == sf::Keyboard::D) {
        movement.x += BLOCK_SIZE; 
    }


    for (int i = 0; i < blocks.size(); i++) {
        old_position[i][0] = blocks[i].getPosition().x;
        old_position[i][1] = blocks[i].getPosition().y;
        blocks[i].move(movement); 
    }

    
} // move_tetris 


void Tetris::revert() {
    for (int i = 0; i < blocks.size(); i++) {
        blocks[i].setPosition(old_position[i][0], old_position[i][1]); 
    }
} //revert

sf::RectangleShape Tetris::getBlock(int index) {
    return blocks[index]; 
}


//--------------------------------------GAME CLASS---------------------------------------------//

class Game {
    public: 
    Game(); 
    void run(); 
    void handlePlayerInput(sf::Keyboard::Key key); 
    void render(); 
    bool isFullRow(int row); 
    bool hitDetection(); 
    void processEvents(); 
    void drop(); 
    void add();
    void updateGrid(int row); 
    int updateScore(int lines);



    private: 
    sf::RenderWindow window; 
    sf::RectangleShape black_shape; 
    Tetris tetris; 
    bool occupied[10][20];
    sf::RectangleShape grid[10][20]; 
    int score = 0; 
    bool running = true;
    sf::Text text; 
    sf::Font font; 
    std::string point = "Score: ";
    int level = 1; 
    int row_count = 0; 
    bool isEmpty[NROF_ROWS]; 
    int conseq_rows = 0; 
    int row_max = 0; 
    int row_min = NROF_ROWS - 1;


    
    



};

Game::Game() 
: window(sf::VideoMode(NROF_COLS * BLOCK_SIZE, NROF_ROWS * BLOCK_SIZE), "TETRIS")
, black_shape(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE))

{
    tetris.reinit(); 
    for (int rows = 0; rows < NROF_ROWS; rows++) {
        for (int cols = 0; cols <  NROF_COLS; cols++) {
            grid[cols][rows] = black_shape; 
            grid[cols][rows].setFillColor(sf::Color::Black); 
            occupied[cols][rows] = false; 

        }
        isEmpty[rows] = true; 

    }

    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Error loading font" << std::endl; 
    }
    text.setFont(font); 
    point = point + std::to_string(score) + "/n Level: " + std::to_string(level); 

    text.setString(point);


    // set the character size
    text.setCharacterSize(BLOCK_SIZE); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition(BLOCK_SIZE * 3 * 1.0f, 0.0f); 

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}


void Game::run() {
    sf::Clock clock; 
    sf::Time timeSinceLastUpdate = sf::Time::Zero; 
    sf::Time TimePerFrame = sf::seconds(1.0f/(1.0f + 0.5f * (level - 1)));
    while (window.isOpen()) {
        while (running) {
            processEvents(); 
            timeSinceLastUpdate += clock.restart(); 

            while (timeSinceLastUpdate > sf::seconds(1.0f/level)) {
                timeSinceLastUpdate -= sf::seconds(1.0f/level);
                tetris.move_tetris(sf::Keyboard::Key::S); 
                
                if (hitDetection()) {
                    tetris.revert();
                    add();
                    score += updateScore(conseq_rows); 
                    tetris.reinit();
                    if (hitDetection()) {
                        running = false; 
                    }
                }
                
            }

            render(); 
        }
        std::string end = "GAME OVER \n Score: ";
        end += std::to_string(score); 
        text.setString(end); 
        text.setPosition(0.0f, NROF_ROWS * 0.5 * BLOCK_SIZE * 1.0f); 
        render();
        processEvents(); 

    }
    
}

void Game::processEvents()  {
    sf::Event event; 
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code);
            break;  
        case sf::Event::Closed:
            running = false; 
            window.close(); 
            break; 

        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::W) {
        tetris.rotation(); 
        if (hitDetection()) {
            tetris.revert(); 
        }
    }
    if (key == sf::Keyboard::S) {
        tetris.move_tetris(key); 
        if (hitDetection()) {
            tetris.revert();
        }
    }
    if (key == sf::Keyboard::A || key == sf::Keyboard::D) {
        tetris.move_tetris(key); 
        if (hitDetection()) {
            tetris.revert();
        }
    }
    if (key == sf::Keyboard::Space) {  //DROP
        while (!hitDetection()) {
            tetris.move_tetris(sf::Keyboard::S); 
        }
        tetris.revert(); 
        add();
        score += updateScore(conseq_rows); 
        tetris.reinit();
        if (hitDetection()) {
            running = false; 
        }

    }
}

void Game::render() {

    window.clear(); 
        
    for (int rows = 0; rows < NROF_ROWS; rows++) {
        for (int cols = 0; cols < NROF_COLS; cols++) {
            window.draw(grid[cols][rows]);
        }
    }
        
    for (int i = 0; i < 4; i++) {
        window.draw(tetris.blocks[i]); 
    }

    if (running) {
    point = point + std::to_string(score) + "\n Level: " + std::to_string(level); 
    text.setString(point); 
    }

    window.draw(text); 
    window.display(); 

    point = "Score: ";

}

bool Game::hitDetection() {
    bool hit = false; 
    for (int i = 0; i < 4; i++) {
        if (tetris.blocks[i].getPosition().x < 0 || tetris.blocks[i].getPosition().x >= NROF_COLS * BLOCK_SIZE || tetris.blocks[i].getPosition().y > ((NROF_ROWS - 1) * BLOCK_SIZE )) {
            hit = true; 
            break; 
        }
        if (occupied[(int)(tetris.blocks[i].getPosition().x/BLOCK_SIZE)][(int)(tetris.blocks[i].getPosition().y/BLOCK_SIZE)] == true) {
            hit = true; 
            break; 
        }
    }
        

    return hit; 
}

void Game::add() {
    for (int i = 0; i < 4; i++) {
        int pos_x = tetris.blocks[i].getPosition().x/BLOCK_SIZE;
        int pos_y = tetris.blocks[i].getPosition().y/BLOCK_SIZE;
        grid[pos_x][pos_y] = tetris.blocks[i]; 
        occupied[pos_x][pos_y] = true; 
        isEmpty[pos_y] = false; 

        if (pos_y > row_max) {
            row_max = tetris.blocks[i].getPosition().y/BLOCK_SIZE;
        }
        if (pos_y < row_min) {
            row_min = tetris.blocks[i].getPosition().y/BLOCK_SIZE;
        }
                       
    }
    for (int i = row_min; i <= row_max; i++) {
        if (isFullRow(i)) {
            updateGrid(i); 
        }
    }

}

bool Game::isFullRow(int row) {
    bool full_row = false; 
    int full_count = 0; 
    for (int i = 0; i < NROF_COLS; i++) {
        if (occupied[i][row] == false) {
            break; 
        } 
        if (occupied[i][row] == true) {
            full_count++; 
        }
    }
    if (full_count == 10) {
        full_row = true; 
        conseq_rows++;
    }
    full_count = 0; 
    return full_row; 
}

void Game::updateGrid(int row) {
    int index = 0; 
    for (int i = row; i > 0; i--) {
        isEmpty[i] = isEmpty[i - 1];
        for (int cols = 0; cols < NROF_COLS; cols++) {
            grid[cols][i].setFillColor(grid[cols][i - 1].getFillColor());
            occupied[cols][i] = occupied[cols][i - 1];
        }
        if (isEmpty[i]) {
            index = i; 
            break; 
        }
    }
    /*
    for (int i = 0; i < NROF_COLS; i++) {
        grid[i][index - 1] = black_shape; 
        grid[i][index - 1].setFillColor(sf::Color::Black); 
        occupied[i][index - 1] = false; 
    }
    */
    row_count++;
    if (row_count % 10 == 0) {
        level++;
    }
}

int Game::updateScore(int lines) {
    int out = 0; 
    std::cout << "lines: " << lines << std::endl;
    conseq_rows = 0; 
    switch (lines)
    {
    case 1:
        out += 40;        
        break;
    case 2:
        out += 100; 
        break;
    case 3: 
        out += 300; 
        break;
    case 4: 
        out += 1200;
        break; 
    }
    conseq_rows = 0;
    row_max = 0;
    row_min = NROF_ROWS - 1;
    return out; 
}
 

int main() {
    Game game; 
    game.run(); 
    return 0; 
}




