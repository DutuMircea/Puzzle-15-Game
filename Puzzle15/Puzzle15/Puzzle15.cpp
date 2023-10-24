#include <iostream>
#include <numeric>
#include <random>
#include <chrono>

constexpr int g_consoleLines{ 25 };
constexpr int SIZE = 4;

class Tile {  // represent a single a tile that contains the number
private:
    int m_number{};
public:
    Tile() = default;
    Tile(int number) : m_number{ number } { }

    friend std::ostream& operator<<(std::ostream& out, const Tile& tile) { // overload to the << operator for printing
        
        if (tile.m_number > 9)
            out << ' ' << tile.m_number << ' ';
        else if (tile.m_number > 0)
            out << "  " << tile.m_number << ' ';
        else if(tile.m_number == 0)
            out  << "    ";

        return out;
    }

    friend bool operator==(const Tile& tile, int value) {  // overload to the == operator
        return tile.m_number == value;
    }

    bool isEmpty() const{
        return m_number == 0;
    }

    int getNum() const{
        return m_number;
    }
};

class Direction {  // the class  contains all of the directions in which we can move the tiles

public:
    enum Type{
        up,  // 0
        down,
        left,
        right,

        max_directions, // 4
    };

    Direction(Type direction) : m_direction{ direction } {}

    Direction operator-() const;

    friend std::ostream& operator<<(std::ostream& out, const Direction& dir) {
       
        switch (dir.m_direction) {
        case Type::up: return(out << "up");
            break;
        case Type::down: return(out << "down");
            break;
        case Type::left: return(out << "left");
            break;
        case Type::right: return(out << "right");
            break;
        }
        
    }

    Type getDirection() const{
        return m_direction;
    }
    
    static Direction getRandDirect() {  // function that returns a random direction
        static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

        int rand{ mt() % (Type::max_directions) };
        Type rand_direction{ static_cast<Type>(rand) };   

        std::cout << '\n';
        return Direction{ rand_direction };
    }

private:
    Type m_direction{};

};

class Point {  // the coordinate of a tile 
private:
    int m_x{};     
    int m_y{};     
                    
public:
    Point(int x, int y) : m_x{ x }, m_y{ y } {}

    

    friend bool operator==(const Point& p1, const Point& p2) {
        return (p1.m_x == p2.m_x && p1.m_y == p2.m_y);
    }

    friend bool operator!=(const Point& p1, const Point& p2) {
        return (p1.m_x != p2.m_x || p1.m_y != p2.m_y);
    }

    int getX() const { return m_x; }
    int getY() const { return m_y; }

    Point getAdjacentPoint(const Direction& dir) {  // class that returns that changes a point coordinate after a direction

        switch (dir.getDirection()) {
        case Direction::Type::up: return Point{ m_x, m_y - 1 };
            break;
        case Direction::Type::down: return Point{ m_x, m_y + 1 };;
            break;
        case Direction::Type::left: return Point{ m_x - 1, m_y};
            break;
        case Direction::Type::right: return Point{ m_x + 1, m_y};
            break;
        }
       // return Point{ m_x,m_y };
    }

    bool isValid() {
        return(m_x >= 0 && m_x < SIZE  && m_y >= 0 && m_y < SIZE);
    }

};

class Board {
private:
    Tile m_tiles[4][4]{  // our initial puzzle starts in a solved state
        Tile{ 1 }, Tile { 2 }, Tile { 3 } , Tile { 4 },
        Tile { 5 } , Tile { 6 }, Tile { 7 }, Tile { 8 },
        Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 },
        Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 }
    };

public:
    Board() = default;

    friend std::ostream& operator<<(std::ostream& out, const Board& board) {

       for (int i = 0; i < g_consoleLines; i++)
           out << '\n';

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++){             
                    out << board.m_tiles[i][j];
            }
            out << '\n';
        }

        return out;
    }

    Point findEmpyTile() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (m_tiles[i][j].isEmpty()) {
                    return { j, i };
                }
            }
        }
    }

    bool moveTile(Direction dir) {
        Point empty{ findEmpyTile() };  // finding the empty tile

        Point adjTile{ empty.getAdjacentPoint(-dir) }; // the valid adjacent tile that moves with the given direction
       

        if (adjTile.isValid()) {  // if the tile is valid we change the coordonates acording to the direction
            
            m_tiles[empty.getY()][empty.getX()] = m_tiles[adjTile.getY()][adjTile.getX()];
            m_tiles[adjTile.getY()][adjTile.getX()] = Tile{ 0 };

            return true;
        }
        else
            return false; 
    }

    void randomizeBoard() {

        int i{ 0 };
        while (i < 100) {
            Direction randDir{ Direction::getRandDirect() };

            if (moveTile(randDir))
                i++;
        }
    }

    bool isSolved() { // checking if the board is solved

        int count{ 1 };
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (m_tiles[i][j] == count) {
                    count++;
                }
                else
                    return false;

                if(count == (SIZE*SIZE - 1) && (m_tiles[SIZE - 1][SIZE - 1] == 0))
                    return true;
            }
        }
    }
   
};



Direction Direction::operator-() const {  // overload to the - operator  so that we can move the tile in the oposite direction
   // return static_cast<Direction::Type>(-m_direction);
    switch (m_direction) {
    case Type::up: return Type::down;
        break;
    case Type::down: return Type::up;
        break;
    case Type::left: return Type::right;
        break;
    case Type::right: return Type::left;
        break;
    }
}

namespace UserInput {

    void ignoreLine(){
        std::cin.ignore(std::numeric_limits < std::streamsize>::max(), '\n');
    }

    char getCharacter() {
        char character{};

        std::cin >> character;
        ignoreLine();
        return character;
    }

    void getCommand(char c) {

        switch (c) {
        case 'w':
            std::cout << "You entered direction: up\n";
            break;
        case 'a':
            std::cout << "You entered direction: left\n";
            break;
        case 's':
            std::cout << "You entered direction: down\n";
            break;
        case 'd':
            std::cout << "You entered direction: right\n";
            break;
        case 'q':
            std::cout << "\n\nBye!\n\n";
            break;
        default:
            std::cout << "Invalid command!\n";
            break;
        }
    }

    // converting the input from the keyboard to a  direction
    Direction charToCommand(char c) {
        switch (c) {
        case 'w': return Direction::Type::up;
            break;
        case 's': return Direction::Type::down;
            break;
        case 'a': return Direction::Type::left;
            break;
        case 'd': return Direction::Type::right;
            break;    
        }
    }
}

void randomizeBoard(Board& board) {  
    // we randomise the board by generating random directions
    // this way the board can always be solved
    
    int i{ 0 };
    while (i < 100) {
        Direction randDir{ Direction::getRandDirect() };
        //std::cout << randDir << '\n';

       if (board.moveTile(randDir)) 
            i++;    
    }
}

void playGame() {

    Board board{};
    //std::cout << board;
   
    board.randomizeBoard();
    std::cout << board;

    while (true) {
        char command{ UserInput::getCharacter()};
        
        Direction dir{UserInput::charToCommand(command)};
       // std::cout  << dir << '\n';
        UserInput::getCommand(command);

        if(board.moveTile(dir))
            std::cout << board;

        if (board.isSolved()){
            std::cout << "\n\nYou won!\n\n";
            return;
        }
        if (command == 'q')
            return;

    }
}


int main()
{
    playGame();   
}

