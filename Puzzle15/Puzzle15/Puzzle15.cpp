#include <iostream>
#include <numeric>
#include <random>
#include <chrono>

constexpr int g_consoleLines{ 25 };
constexpr int SIZE = 4;

class Tile {  // reprez un patratel
private:
    int m_number{};
public:
    Tile() = default;
    Tile(int number) : m_number{ number } { }

    friend std::ostream& operator<<(std::ostream& out, const Tile& tile) {
        
        if (tile.m_number > 9)
            out << ' ' << tile.m_number << ' ';
        else if (tile.m_number > 0)
            out << "  " << tile.m_number << ' ';
        else if(tile.m_number == 0)
            out  << "    ";

        return out;
    }

    friend bool operator==(const Tile& tile, int value) {
        return tile.m_number == value;
    }

    bool isEmpty() const{
        return m_number == 0;
    }

    int getNum() const{
        return m_number;
    }
};

class Direction {

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
       
        switch (dir.m_direction) { // vechiul showCommand()
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
    
    static Direction getRandDirect() {
        static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

        int rand{ mt() % (Type::max_directions) };
        Type rand_direction{ static_cast<Type>(rand) };   

        //rand_direction.showCommand();
        std::cout << '\n';
        return Direction{ rand_direction };
    }

private:
    Type m_direction{};

};

class Point {
private:
    int m_x{};     // 00 10 20 30  etc.. (inversare i cu j)
    int m_y{};     // 01 11 21 31  etc..  [i][j] -> [j][i]
                    
public:
    Point(int x, int y) : m_x{ x }, m_y{ y } {}

    //  copy constructor de facut (nah)
    //Point(const Point& point) :m_x{ point.m_x }, m_y{ point.m_y } {}

    friend bool operator==(const Point& p1, const Point& p2) {
        return (p1.m_x == p2.m_x && p1.m_y == p2.m_y);
    }

    friend bool operator!=(const Point& p1, const Point& p2) {
        return (p1.m_x != p2.m_x || p1.m_y != p2.m_y);
    }

    int getX() const { return m_x; }
    int getY() const { return m_y; }

    Point getAdjacentPoint(const Direction& dir) {

        switch (dir.getDirection()) {
        case Direction::Type::up: return Point{ m_x, m_y - 1 };
            break;
        case Direction::Type::down: return Point{ m_x, m_y + 1 };;
            break;
        case Direction::Type::left: return Point{ m_x - 1, m_y};
            break;
        case Direction::Type::right:return Point{ m_x + 1, m_y};
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
    Tile m_tiles[4][4]{
        Tile{ 1 }, Tile { 2 }, Tile { 3 } , Tile { 4 },
        Tile { 5 } , Tile { 6 }, Tile { 7 }, Tile { 8 },
        Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 },
        Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 }
    };

public:
    Board() = default;
        // generare board rezolvat (old way)
        /*int count{ 1 };
        for(int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++) {
                m_tiles[i][j] = count;
                count++;
            }
        }*/

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
        Point empty{ findEmpyTile() };
        //std::cout << empty.getX() << " " << empty.getY() << ' ';

        Point adjTile{ empty.getAdjacentPoint(-dir) }; // vechia fct modifica coord empty
        //std::cout << adjTile.getX() << " " << adjTile.getY();
        
        /*for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                std::cout << "M["  << i << "][" << j << "] = " << m_tiles[i][j] << '\n';
                
            }
        }*/

        if (adjTile.isValid()) {
            //std::cout << "\nYES!\n";
            //std::cout << m_tiles[empty.getX()][empty.getY()] << " , \n";
            // interschimbare tile-uri
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
            //std::cout << randDir << '\n';

            if (moveTile(randDir))
                i++;
        }
    }

    bool isSolved() {

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



Direction Direction::operator-() const {
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
        // fct pt a ignora orice caracter introdus dupa primul(idk cum fct)
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

    // convertim din c in Type 
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


void Test1() {
    Tile tile1{ 10 };
    Tile tile2{ 8 };
    Tile tile3{ 0 }; // the missing tile
    Tile tile4{ 1 };

    std::cout << "0123456789ABCDEF\n"; // to make it easy to see how many spaces are in the next line
    std::cout << tile1 << tile2 << tile3 << tile4 << '\n';

    std::cout << std::boolalpha << tile1.isEmpty() << ' ' << tile3.isEmpty() << '\n';
    std::cout << "Tile 2 has number: " << tile2.getNum() << "\nTile 4 has number: " << tile4.getNum() << '\n';
}
void Test2() {
    Board board{};
    std::cout << board;
}
void Test3() {
    std::cout << "Generating random direction... " << Direction::getRandDirect() << '\n';
    std::cout << "Generating random direction... " << Direction::getRandDirect() << '\n';
    std::cout << "Generating random direction... " << Direction::getRandDirect() << '\n';
    std::cout << "Generating random direction... " << Direction::getRandDirect() << '\n';
}
void Test4() {
    std::cout << std::boolalpha;
    std::cout << (Point{ 1, 1 }.getAdjacentPoint(Direction::up) == Point{ 1, 0 }) << '\n';
    std::cout << (Point{ 1, 1 }.getAdjacentPoint(Direction::down) == Point{ 1, 2 }) << '\n';
    std::cout << (Point{ 1, 1 }.getAdjacentPoint(Direction::left) == Point{ 0, 1 }) << '\n';
    std::cout << (Point{ 1, 1 }.getAdjacentPoint(Direction::right) == Point{ 2, 1 }) << '\n';
    std::cout << (Point{ 1, 1 } != Point{ 2, 1 }) << '\n';
    std::cout << (Point{ 1, 1 } != Point{ 1, 2 }) << '\n';
    std::cout << !(Point{ 1, 1 } != Point{ 1, 1 }) << '\n';
}

int main()
{
    playGame();
    
}

