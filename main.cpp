#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// NOLINT(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)

inline void runMacro();

inline void doCommands();

std::vector<std::string> macro;  // NOLINT: don't care about that macro is
// non-const and globally accessible
// NOLINT: don't care about that coordinates is non-const and globally
// accessible
std::vector<std::pair<int, int>>
    coordinates =  // NOLINT: don't care about that coordinates is non-const and
    // global accessible
    {{0, 0}};
std::vector<std::pair<std::pair<int, int>, int>>
    walls;  // NOLINT: don't care about that walls is non-const and globally
            // accessible
int max_x = 0, max_y = 0;  // NOLINT: don't care about that max_x and max_y is
// non-const and globally accessible
int min_x = 0, min_y = 0;  // NOLINT: don't care about that min_x and min_y is
// non-const and globally accessible
std::string command;  // NOLINT: don't care about that command is non-const and
// globally accessible
int x = 0, y = 0;  // NOLINT: don't care about that x and y is non-const and
// globally accessible
int direction = 1;  // NOLINT: don't care about that direction is non-const and

// globally accessible

struct Direction {
    int dx, dy;
};

const Direction DIRECTIONS[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::map<std::string, int> Commands =  // NOLINT: don't care about that Commands
                                       // is non-const and globall accessible
    {{"quit", -1},        {"exit", -1},        {"sherlock-holmes", 11},
     {"sherlock", 11},    {"cat-jeoffry", 12}, {"cat", 12},
     {"forward", 21},     {"fwd", 21},         {"backward", 22},
     {"bwd", 22},         {"turn-right", 23},  {"right", 23},
     {"turn-left", 24},   {"left", 24},        {"show-map", 25},
     {"start-macro", 30}, {"stop-macro", 31},  {"end-macro", 31},
     {"run-macro", 32},   {"put-wall", 40},    {"remove-wall", 41}};

int min(int a, int b) {
    if (b < a) {
        return b;
    } else {
        return a;
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

bool ifThereIsAWall(int h, int w, const std::vector<std::pair<int, int>> &vec) {
    auto k = std::find(vec.begin(), vec.end(), std::make_pair(h, w));
    return (k != vec.end());
}

bool ifWeWereHere(
    int mi,
    int mj,
    int maxy,
    int miny,
    const std::vector<std::pair<int, int>> &cord
) {
    auto k = std::find(
        cord.begin(), cord.end(), std::make_pair(mj, (maxy - abs(mi - miny)))
    );
    return (k != cord.end());
}

void putTheWall(int dx, int dy, int dir) {
    walls.push_back({{dx, dy}, dir});
}

void wallDelete(int dx, int dy, int dir) {
    auto i = std::find(
        walls.begin(), walls.end(), std::make_pair(std::make_pair(dx, dy), dir)
    );
    if (i != walls.end()) {
        walls.erase(i);
    }
}

void removeTheWall(int dx, int dy, int dir) {
    if (dir == 1) {
        wallDelete(dx, dy, dir);
        wallDelete(dx, dy + 1, dir + 2);
    }
    if (dir == 3) {
        wallDelete(dx, dy, dir);
        wallDelete(dx, dy - 1, dir - 2);
    }
    if (dir == 2) {
        wallDelete(dx, dy, dir);
        wallDelete(dx - 1, dy, dir - 2);
    }
    if (dir == 0) {
        wallDelete(dx, dy, dir);
        wallDelete(dx + 1, dy, dir + 2);
    }
}

std::vector<std::pair<int, int>> fillTheCordsOfTheWall(
    int minx,
    int maxy,
    std::vector<std::pair<int, int>> cords
) {
    for (auto wallcord : walls) {
        int h = 0;
        int w = 0;
        int napr = wallcord.second;
        int ygr = wallcord.first.second;
        int iks = wallcord.first.first;
        switch (napr) {
            case 1:  // Вверх
                w = 2 * (abs(iks - minx + 1));
                h = 2 * (abs(ygr - maxy) + 1) - 1;
                cords.emplace_back(h, w);
                cords.emplace_back(h, w + 1);
                cords.emplace_back(h, w - 1);
                break;
            case 2:  // Влево
                w = 2 * (abs(iks - minx + 1)) - 1;
                h = 2 * (abs(ygr - maxy) + 1);
                cords.emplace_back(h, w);
                cords.emplace_back(h + 1, w);
                cords.emplace_back(h - 1, w);
                break;
            case 3:  // Вниз
                w = 2 * (abs(iks - minx + 1));
                h = 2 * (abs(ygr - maxy) + 1) + 1;
                cords.emplace_back(h, w);
                cords.emplace_back(h, w + 1);
                cords.emplace_back(h, w - 1);
                break;
            case 0:  // Вправо
                w = 2 * (abs(iks - minx + 1)) + 1;
                h = 2 * (abs(ygr - maxy) + 1);
                cords.emplace_back(h, w);
                cords.emplace_back(h + 1, w);
                cords.emplace_back(h - 1, w);
                break;
        }
    }
    return cords;
}

void writeAWall(int h, int w) {
    if ((h % 2 == 1) && (w % 2 == 1)) {
        std::cout << "+";
    } else if (w % 2 == 0) {
        std::cout << "-";
    } else {
        std::cout << "|";
    }
}

void showMap(
    int maxx,
    int maxy,
    int miny,
    int minx,
    int dx,
    int dy,
    int dir,
    const std::vector<std::pair<int, int>> &cord
) {
    std::vector<std::pair<int, int>> cordsOfTheWalls;
    cordsOfTheWalls = fillTheCordsOfTheWall(minx, maxy, cordsOfTheWalls);
    char c = 0;
    switch (dir) {
        case 0:
            c = '>';
            break;
        case 1:
            c = '^';
            break;
        case 2:
            c = '<';
            break;
        case 3:
            c = 'v';
            break;
        default:
            break;
    }
    int i = miny;
    int j = minx;
    for (int h = 1; h <= 2 * abs(maxy - miny + 1) + 1; h++) {
        for (int w = 1; w <= 2 * abs(maxx - minx + 1) + 1; w++) {
            if (ifThereIsAWall(h, w, cordsOfTheWalls)) {
                writeAWall(h, w);
                continue;
            }
            if (h % 2 == 1) {
                std::cout << " ";
                continue;
            }
            if (w % 2 == 1) {
                std::cout << " ";
                continue;
            }
            if ((i == (maxy - abs(dy - miny))) && (j == dx)) {
                std::cout << c;
            } else {
                if (ifWeWereHere(i, j, maxy, miny, cord)) {
                    std::cout << "o";
                } else {
                    std::cout << ".";
                }
            }
            j++;
        }
        std::cout << '\n';
        if (h % 2 == 0) {
            i++;
        }
        j = minx;
    }
}

void recordMacro() {
    macro = {};
    std::string comm;
    int c = 0;
    while (std::getline(std::cin, comm)) {
        if ((comm == "stop-macro") || (comm == "end-macro")) {
            std::cout << "New macro of " << c << " command(s)"
                      << "\n";
            return;
        }
        macro.push_back(comm);
        std::cout << "Recorded"
                  << "\n";
        c++;
    }
}

bool myFind(
    int dx,
    int dy,
    int dir,
    std::vector<std::pair<std::pair<int, int>, int>> vect
) {
    auto i = std::find(
        vect.begin(), vect.end(), std::make_pair(std::make_pair(dx, dy), dir)
    );
    return (i != vect.end());
}

void forward() {
    if (direction == 1) {
        if ((myFind(x, y, direction, walls)) ||
            myFind(x, y + 1, direction + 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    if (direction == 3) {
        if ((myFind(x, y, direction, walls)) ||
            myFind(x, y - 1, direction - 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    if (direction == 2) {
        if ((myFind(x, y, direction, walls)) ||
            myFind(x - 1, y, direction - 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    if (direction == 0) {
        if ((myFind(x, y, direction, walls)) ||
            (myFind(x + 1, y, direction + 2, walls))) {
            std::cout << "Bump!\n";
            return;
        }
    }
    x += DIRECTIONS[direction].dx;
    y += DIRECTIONS[direction].dy;
    coordinates.emplace_back(x, y);
    max_x = max(x, max_x);
    max_y = max(y, max_y);
    min_x = min(x, min_x);
    min_y = min(y, min_y);
}

void backward() {
    int tempd = (direction + 2) % 4;
    if (tempd == 1) {
        if ((myFind(x, y, tempd, walls)) ||
            myFind(x, y + 1, tempd + 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    if (tempd == 3) {
        if ((myFind(x, y, tempd, walls)) ||
            myFind(x, y - 1, tempd - 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    if (tempd == 2) {
        if ((myFind(x, y, tempd, walls)) ||
            myFind(x - 1, y, tempd - 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    if (tempd == 0) {
        if ((myFind(x, y, tempd, walls)) ||
            myFind(x + 1, y, tempd + 2, walls)) {
            std::cout << "Bump!\n";
            return;
        }
    }
    x -= DIRECTIONS[direction].dx;
    y -= DIRECTIONS[direction].dy;
    coordinates.emplace_back(x, y);
    max_x = max(x, max_x);
    max_y = max(y, max_y);
    min_x = min(x, min_x);
}

void doCommands(const std::string &s) {
    switch (Commands[s]) {
        case 11: {
            std::cout
                << "I had had so many reasons to believe in my friend's "
                   "subtle "
                   "powers of\n"
                   "reasoning and extraordinary energy in action that I "
                   "felt "
                   "that he must\n"
                   "have some solid grounds for the assured and easy "
                   "demeanour "
                   "with which\n"
                   "he treated the singular mystery which he had been "
                   "called "
                   "upon to\n"
                   "fathom. Once only had I known him to fail, in the case "
                   "of "
                   "the King of\n"
                   "Bohemia and of the Irene Adler photograph; but when I "
                   "looked back to\n"
                   "the weird business of the Sign of Four, and the "
                   "extraordinary\n"
                   "circumstances connected with the Study in Scarlet, I "
                   "felt "
                   "that it would\n"
                   "be a strange tangle indeed which he could not "
                   "unravel.";
            std::cout << "\n";
            return;
        }  //"sherlock" or "sherlock-holmes"
        case 12: {
            std::cout << "For he is of the tribe of Tiger.\n"
                         "For the Cherub Cat is a term of the Angel Tiger.\n"
                         "For he has the subtlety and hissing of a serpent, "
                         "which in goodness he suppresses.\n"
                         "For he will not do destruction, if he is well-fed, "
                         "neither will he spit without provocation.\n"
                         "For he purrs in thankfulness, when God tells him "
                         "he's a good Cat.\n"
                         "For he is an instrument for the children to learn "
                         "benevolence upon.\n"
                         "For every house is incomplete without him and a "
                         "blessing is lacking in the spirit.\n"
                         "For the Lord commanded Moses concerning the cats at "
                         "the departure of the Children of Israel from Egypt.\n"
                         "For every family had one cat at least in the bag.\n"
                         "For the English Cats are the best in Europe.";
            std::cout << "\n";
            return;
        }  //"cat" or "cat-jeoffry"
        case 21: {
            forward();
            return;
        }  //"forward" or "fwd"
        case 22: {
            backward();
            return;
        }  //"backward" or "bwd"
        case 23: {
            direction -= 1;
            direction = (direction + 4) % 4;
            return;
        }  //"right" or "turn-right"
        case 24: {
            direction += 1;
            direction = (direction + 4) % 4;
            return;
        }  //"left" or "turn-left"
        case 25: {
            showMap(max_x, max_y, min_y, min_x, x, y, direction, coordinates);
            return;
        }  //"show-map"
        case 40: {
            putTheWall(x, y, direction);
            return;
        }  //"put-wall"
        case 41: {
            removeTheWall(x, y, direction);
            return;
        }  //"remove-wall"
    }
}

void runMacro() {
    for (std::string &s : macro) {
        doCommands(s);
    }
}

int main() {
    std::string s;
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
    while (std::getline(std::cin, s)) {
        if ((s == "exit") || (s == "quit")) {
            return 0;
        }
        if (s == "run-macro") {
            runMacro();
        }
        if (s == "start-macro") {
            recordMacro();
        }
        doCommands(s);
    }
}
