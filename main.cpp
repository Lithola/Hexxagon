#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::pair;
using std::stoi;

/**
 *enum z możliwymi stanami pól na planszy
 */
enum FIELD_STATE {
    INACTIVE, ACTIVE, Ruby, Pearl
};

/**
 *struct dla pojedynczego pola
 * @field_state opisany przez enum
 * @x, @y będą służyć do późniejszego zapisywania indeksów z wektora wektorów
 */
struct Field {
    FIELD_STATE field_state;
    int field_number;
    int x, y;

/**Wyrysowuje pole na planszy w zależności od jego stanu
 * @return string describing Field
 */
    string drawField() {
        string str = "";
        if (field_number < 10) {
            str.append(" ");
        }
        str.append(std::to_string(field_number));
        str.append("-");
        if (field_state == ACTIVE) {
            str.append("O");
        }
        if (field_state == Pearl) {
            str.append("@");
        }
        if (field_state == Ruby) {
            str.append("#");
        }

        return str;
    }

};


/**struct Game dla przebiegu całej rozgrywki włącznie z niezbędnymi metodami
 * @basic_board
 */
struct Game {
    vector<vector<Field>> basic_board;
    pair<int, int> wynik;


/**metoda do odnajdywania pola po wpisaniu jego numeru
 * @param const integer
 * @return field found by number
 */
    auto find_Field_By_Number(const int a) {
        for (auto vec: basic_board) {
            for (auto field: vec)
                if (field.field_number == a)
                    return field;
        }
    }


/**metoda do zmiany stanu pola
 * @param const integer
 * @param const FIELD_STATE reference
 * @return void
 */

    auto changeState(const int nr, const FIELD_STATE &stateNew) {
        basic_board[find_Field_By_Number(nr).y][find_Field_By_Number(nr).x].field_state = stateNew;
    }

    /**metoda do ustawiania startowego stanu pionków na planszy
 * @see changeState()
 * @return void
 */

    auto startingPos() {
        changeState(11, Ruby);
        changeState(15, Ruby);
        changeState(61, Ruby);
        changeState(1, Pearl);
        changeState(47, Pearl);
        changeState(51, Pearl);
    }

    /**metoda do tworzenia stanów pol dla planszy w grę Hexxagon
 * @see startingPos()
 * @return void
 */

    auto create_Basic_Board() {
        basic_board = vector<vector<Field>>();
        wynik.first = 3;
        wynik.second = 3;
        int field_counter_left = 4;
        int field_counter_right = 4;
        int counter = 1; //numery pól
        basic_board.resize(17);
        for (int i = 0; i < 17; ++i) {
            basic_board[i] = vector<Field>();
            basic_board[i].resize(9);
            for (int j = 0; j < 9; ++j) {
                basic_board[i][j] = *new Field{INACTIVE};
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 9; ++j) {
                //tu zrobic ogolny if do i = 5
                if (i % 2 == 0 && j % 2 == 0) { //jesli linijka parzysta (wtedy łącznie ze srodkiem, czyli indeksem 4
                    if ((j >= field_counter_left && j <= 4) || (j > 4 && j <= field_counter_right)) {
                        basic_board[i][j].field_state = ACTIVE;
                        basic_board[i][j].field_number = counter++;
                    }
                } else if (i % 2 != 0 && j % 2 != 0) { //jesli nieparzysta
                    if ((j >= field_counter_left && j < 4) || (j > 4 && j <= field_counter_right)) {
                        basic_board[i][j].field_state = ACTIVE;
                        basic_board[i][j].field_number = counter++;
                    }
                }

            }
            field_counter_left--; //na koncy petli powinno sie rownac 0
            field_counter_right++; // na koncu petli powinno wynosic 8
        }
        //=================== 2 petla for dla i>=5 %% i<14
        for (int i = 4; i < 13; ++i) {
            for (int j = 0; j < 9; ++j) {
                if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)) {
                    basic_board[i][j].field_state = ACTIVE;
                    basic_board[i][j].field_number = counter++;
                }
            }
        }
        // ============ 3 pętla for
        for (int i = 13; i < 17; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (i % 2 == 0 && j % 2 == 0) { //jesli linijka parzysta (wtedy łącznie ze srodkiem, czyli indeksem 4
                    if ((j >= field_counter_left && j <= 4) || (j > 4 && j <= field_counter_right)) {
                        basic_board[i][j].field_state = ACTIVE;
                        basic_board[i][j].field_number = counter++;
                    }
                } else if (i % 2 != 0 && j % 2 != 0) { //jesli nieparzysta
                    if ((j >= field_counter_left && j < 4) || (j > 4 && j <= field_counter_right)) {
                        basic_board[i][j].field_state = ACTIVE;
                        basic_board[i][j].field_number = counter++;
                    }
                }
            }
            field_counter_left++;
            field_counter_right--;
        }
        for (int i = 0; i < basic_board.size(); ++i) {
            for (int j = 0; j < basic_board[i].size(); ++j) {
                basic_board[i][j].x = j;
                basic_board[i][j].y = i;
                if (basic_board[i][j].field_number == 22 || basic_board[i][j].field_number == 35 ||
                    basic_board[i][j].field_number == 36) {
                    basic_board[i][j].field_state = INACTIVE; // dla pól X
                }
            }
        }
        startingPos();
    }

    /**metoda do odnajdywania pola po jego współrzędnych w wektorze wektorów Field
   * @param const integer
   * @param const integer
   * @return field found
   */
    auto find_Field_By_XY(const int y, const int x) {
        for (auto vec: basic_board) {
            for (auto field: vec)
                if (field.x == x && field.y == y)
                    return field;
        }
    }

    /**metoda do wyrysowania planszy w konsoli
* @see drawField()
* @return void
*/

    auto drawTable() {
        for (int i = 0; i < basic_board.size(); ++i) {
            for (int j = 0; j < basic_board[i].size(); ++j) {
                if (basic_board[i][j].field_state == INACTIVE) {
                    cout << " " << '\t';
                } else cout << basic_board[i][j].drawField() << '\t';
            }
            cout << endl;
        }
    }

    /**metoda do odnajdywania granic pola
* @param const Field reference
* @return vector of bordering fields
*/

    auto find_Borders(const Field &field) {
        vector<Field> borders_group;
        int x = field.x, y = field.y;
        if (y > 1)
            borders_group.push_back(find_Field_By_XY(y - 2, x));
        if (y > 0 && x < basic_board[y].size() - 1)
            borders_group.push_back(find_Field_By_XY(y - 1, x + 1));

        if (y > 0 && x > 0)
            borders_group.push_back(find_Field_By_XY(y - 1, x - 1));

        if (x > 0 && y < basic_board.size() - 1)
            borders_group.push_back(find_Field_By_XY(y + 1, x - 1));

        if (y < basic_board.size() - 1 && x < basic_board[y].size() - 1)
            borders_group.push_back(find_Field_By_XY(y + 1, x + 1));

        if (y < basic_board.size() - 2)
            borders_group.push_back(find_Field_By_XY(y + 2, x));

        return borders_group;
    }

    /**metoda do odnajdywania całkowitego zasięgu ruchu pionka
* @param const Field reference
     * @see find_Borders()
* @return vector of fields in range
*/

    auto find_possible_moves(const Field &field) {
        vector<Field> basic_borders;
        basic_borders = find_Borders(field);
        int max = basic_borders.size();
        auto advanced_borders = vector<Field>();
        for (int i = 0; i < max; ++i) {
            advanced_borders = find_Borders(basic_borders[i]);
            basic_borders.insert(basic_borders.end(), advanced_borders.begin(), advanced_borders.end());
            advanced_borders.clear();
        }
        return basic_borders;
    }


    /**metoda sprawdzająca czy pionek ma ruchy
* @param const Field reference
     * @see find_possible_moves()
* @return true if moves found
*/

    auto check_for_pos_moves(const Field &field) {
        auto pos_Moves = find_possible_moves(field);
        for (auto move: pos_Moves) {
            if (move.field_state == ACTIVE) {
                return true;
            }
        }
        return false;
    }

    /**metoda sprawdzająca czy gracz ma ruch
* @param const FIELD_STATE reference
     * @see check_for_pos_moves()
* @return true if player can move
*/

    auto check_Player(const FIELD_STATE &state) {
        for (auto vect: basic_board) {
            for (auto field: vect) {
                if (field.field_state == state && check_for_pos_moves(field)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**metoda sprawdzająca czy string jest liczbą
* @param const string reference
* @return true if string is a number
*/


    auto isNumber(const string &str) {
        for (auto ch: str)
            if (!std::isdigit(ch)) {
                return false;
            }
        return true;
    }

    /**metoda sprawdzająca czy wybrano istniejący numer pola
* @param const string reference
     * @see isNumber()
* @return true if valid field
*/

    auto validateInput(const string &str) {
        if (isNumber(str))
            if (std::stoi(str) >= 1 && std::stoi(str) <= 61)
                return true;
        cout << "Enter number from 1 to 61.\n";
        return false;
    }

    /**metoda wyświetlająca aktualny wynik
* @return void
*/


    auto print_Score() {
        cout << "Pearl: " << wynik.first << endl;
        cout << "Ruby: " << wynik.second << endl;
    }


    /**metoda aktualizująca wynik
* @return void
*/

    auto update_Score() {
        wynik.first = 0;
        wynik.second = 0;
        for (auto vect: basic_board) {
            for (auto field: vect) {
                if (field.field_state == Pearl) {
                    wynik.first++;
                }
                if (field.field_state == Ruby) {
                    wynik.second++;
                }

            }

        }
    }


    /**metoda wykonująca ruch dla pionka
* @param const integer
* @param const integer
* @param const FIELD_STATE reference
* @see find_possible_moves()
* @see find_Borders()
* @see find_Field_By_Number()
* @see changeState()
* @return void
*/


    auto make_move(const int nrTo, const int nrFrom, const FIELD_STATE &state) {
        auto borders = find_Borders(find_Field_By_Number(nrFrom));
        bool found = false;
        for (auto field: borders) {
            if (field.field_number == nrTo) {
                changeState(nrTo, state);
                found = true;
            }
        }
        if (!found) {
            borders.clear();
            borders = find_possible_moves(find_Field_By_Number(nrFrom));
            for (auto pole: borders) {
                if (pole.field_number == nrTo) {
                    changeState(nrTo, state);
                    changeState(nrFrom, ACTIVE);
                }
            }
        }
        borders.clear();
        borders = find_Borders
                (find_Field_By_Number(nrTo));
        for (auto pole: borders) {
            if (pole.field_state != state && pole.field_state != ACTIVE && pole.field_state != INACTIVE)
                changeState(pole.field_number, state);
        }
    }
    /**metoda opisująca turę gracza
 * @param const FIELD_STATE reference
 * @see validateInput()
 * @see validateFieldChoice()
 * @see find_Field_By_Number()
 * @see changeState()
 * @return void
 */
    auto turn(const FIELD_STATE &state) {
        int chosenPawn = 1;
        int chosenField = 1;
        string playerInput;
        bool validated = false;

        do {
            cout << (state == Pearl ? "@" : "#") << "Player's turn. Choose your pawn" << endl;
            cin >> playerInput;
            validated = validateInput(playerInput);
            if (validated) {
                chosenPawn = stoi(playerInput);
            }

        } while (!validateFieldChoice(chosenPawn, state) || !validated);
        validated = false;

        do {
            cout << "Choose field number to move" << endl;
            cin >> playerInput;
            validated = validateInput(playerInput);
            if (validated) {
                chosenField = stoi(playerInput);
            }

        } while (!validateMove(chosenField, chosenPawn) || !validated);
        make_move(chosenField, chosenPawn, state);
        update_Score();
        drawTable();
        print_Score();
    }
    /**metoda sprawdzająca poprawność ruchu
 * @param const integer
 * @param const integer
 * @see find_possible_moves()
 * @see find_Field_By_Number()
 * @return true if move is valid
 */
    bool validateMove(const int nrTo, const int nrFrom) {
        auto places = find_possible_moves(find_Field_By_Number(nrFrom));
        for (auto field: places) {
            if (field.field_state == ACTIVE && field.field_number == nrTo)
                return true;
        }
        cout << "You chose wrong place to move." << "\t";
        return false;
    }
    /**metoda sprawdzająca wyboru pola
    * @param const integer
    * @param const FIELD_STATE reference
    * @see find_possible_moves()
    * @see find_Field_By_Number()
    * @return true if chosen field is valid
    */
    bool validateFieldChoice(const int chosenFieldNum, const FIELD_STATE &state) { //
        Field chosenField = find_Field_By_Number(chosenFieldNum);
        if (chosenField.field_state != state) {
            return false;
        }

        auto borders = find_possible_moves(chosenField);
        for (auto field: borders) {
            if (field.field_state == ACTIVE) {
                return true;
            }
        }
        return false;
    }

    /**metoda sprawdzająca spełnienie warunków zwycięstwa
    * @see check_Player()
    * @see check_for_pos_moves()
    * @return true if game goes on
    */


    auto checkWinCon() {
        if ((wynik.first > wynik.second) && (!check_Player(Ruby)))
            return false;
        if ((wynik.first < wynik.second) && (!check_Player(Pearl)))
            return false;
        for (auto v: basic_board)
            for (auto field: v)
                if (check_for_pos_moves(field))
                    return true;
        return false;
    }

    /**metoda pomagająca ai znaleźć najlepszy pionek do wyboru
    * @param const Field reference
    * @see find_Borders()
    * @see find_possible_moves()
    * @return integer score for best move
    */

    auto checkForBestPawn(const Field &field) {
        int score = 0;
        int tmpScore = 0;
        auto borders = find_Borders(field);
        for (auto somsiad: borders) {
            if (somsiad.field_state == ACTIVE) {
                auto borderborders = find_Borders(somsiad);
                tmpScore = 1;
                for (auto somsomsiad: borderborders) {
                    if (somsomsiad.field_state == Ruby)
                        tmpScore++;
                }
                score = score > tmpScore ? score : tmpScore;
            }
        }
        borders.clear();
        borders = find_possible_moves(field);
        for (auto somsiad: borders) {
            if (somsiad.field_state == ACTIVE) {
                auto borderborders = find_Borders(somsiad);
                tmpScore = 0;
                for (auto somsomsiad: borderborders) {
                    if (somsomsiad.field_state == Ruby)
                        tmpScore++;
                }
                score = score > tmpScore ? score : tmpScore;
            }
        }
        return score;
    }
    /**metoda wywołująca ruch dla ai
    * @param const Field reference
    * @see find_Borders()
    * @see find_possible_moves()
    * @return void
    */
    auto bestMove(const Field &field) {
        int bestmove = 0;
        int score = 0;
        int tmpScore = 0;
        auto borders = find_Borders(field);
        for (auto somsiad: borders) {
            if (somsiad.field_state == ACTIVE) {
                auto borderborders = find_Borders(somsiad);
                tmpScore = 1;
                for (auto somsomsiad: borderborders) {
                    if (somsomsiad.field_state == Ruby)
                        tmpScore++;
                }
                if (tmpScore >= score) {
                    bestmove = somsiad.field_number;
                    score = tmpScore;
                }
            }
        }
        borders.clear();
        borders = find_possible_moves(field);
        for (auto somsiad: borders) {
            if (somsiad.field_state == ACTIVE) {
                auto borderborders = find_Borders(somsiad);
                tmpScore = 0;
                for (auto somsomsiad: borderborders) {
                    if (somsomsiad.field_state == Ruby)
                        tmpScore++;
                }
                if (tmpScore >= score) {
                    bestmove = somsiad.field_number;
                    score = tmpScore;
                }
            }
        }
        make_move(bestmove,
                  field.field_number,
                  Pearl);

    }
    /**metoda kontrolująca ai
    * @see check_for_pos_moves()
    * @see checkForBestPawn()
    * @see bestMove()
    * @see update_Score()
    * @see drawTable()()
    * @see print_Score()
    * @return void
    */
    auto ai() {
        Field chosenPawn;
        int score = 0;
        for (auto v: basic_board)
            for (auto field: v) {
                if (field.field_state == Pearl &&
                    check_for_pos_moves(field) && score <= checkForBestPawn(field)) {
                    score = checkForBestPawn(field);
                    chosenPawn = field;
                }
            }
        bestMove(chosenPawn);
        update_Score();
        drawTable();
        print_Score();
    }


    auto firstArg(const string &str) {
        string out = "";
        for (auto ch: str) {
            if (ch == ':') {
                break;
            }
            out.push_back(ch);
        }
        return stoi(out);
    }

    auto secArg(const string &str) {
        string out = "";
        bool bul = false;
        for (auto ch: str) {
            if (bul) {
                out.push_back(ch);
            }
            if (ch == ':') {
                bul = true;
            }
        }

        return stoi(out);
    }

    auto preparePair(const string &str) {
        return pair<string, int>(str, abs(firstArg(str) - secArg(str)));
    }

    void saveResults() {
        auto scores = vector<pair<string, int>>();
        string str = "";
        str.append(std::to_string(wynik.first));
        str.append(":");
        str.append(std::to_string(wynik.second));
        scores.push_back(pair<string, int>(str, abs(wynik.first - wynik.second)));


        fs::path f{"../wyniki.txt"};
        if (fs::exists(f)) {
            std::ifstream stream(f);
            while (std::getline(stream, str)) {
                scores.push_back(preparePair(str));
            }
        }
        std::sort(scores.begin(), scores.end(),
                  [](pair<string, int> pair1, pair<string, int> pair2) { return pair1.second > pair2.second; });
        std::ofstream output("../wyniki.txt");

        for (int i = 0; i < scores.size() && i < 5; ++i) {
            output << scores[i].first << "\n";


        }


    }

    auto game() {
        create_Basic_Board();
        string str = "";
        while (str != "1" && str != "2") {
            cout << "Write 1 for SINGLEPLAYER or 2 FOR 2 PLAYERS\n";
            std::cin >> str;
        }
        bool singlePlayer = (str == "1");
        create_Basic_Board();
        saveResults();
        drawTable();
        while (checkWinCon()) {
            if (check_Player(Ruby))
                turn(Ruby);
            if (check_Player(Pearl))
                singlePlayer ? ai() : turn(Pearl);
        }
        cout << "Game ended with a Score: \n";
        print_Score();
        cout << ((wynik.first == wynik.second) ? "DRAW" :
                 (wynik.first > wynik.second) ? "PEARLS WON" : "RUBIES WON") << "\n";
        saveResults();
    }


};


int main() {
    Game game;
    game.game();


}

//WARUNKi ZWYCIESTWA:
//zapelnione pola
//jedna ze stron nie ma mozliwosci ruchu!!
// -> funkcja sprawdzajaca ruch sprawdza czy pionki aktywnego gracza maja mozliwosc ruchu
