/* bug:
 * 1. 窗口过小，容不下一段文字时文字不显示
 * 2. 窗口由大变小时光标位置不对
 */
#include "game.h"
#include "str.h"    // String
#include <clocale>  // setlocale()
#include <cctype>   // isspace()
#include <cstring>  // strcmp()
#include <ctime>    // rand()
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>

using namespace std;

//ofstream logger;

class Article {

    vector<String> text;
    vector<int> widths;

public:
    void read_stream(const string &filename, ifstream &ifs) {
        ifs.open(filename, ios::in);
        if (!ifs) {
            cerr << "error: cannot open file '" << filename << "'. "
                "does this file exist?\n";
            exit(1);
        }
    }

    // 载入
    void read(const string &filename, int begin = 1, int end = 0x7fffffff) {
        ifstream ifs;
        read_stream(filename, ifs);
        String line;
        text.clear();
        for (int i = 1; getline(ifs, line); ++i) {
            if (i < begin) continue;
            else if (i > end) break;
            text.push_back(line);
            // 计算一行文字的宽度并缓存
            int w = 0;
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i].is_ascii())
                    w += 1;
                else
                    w += 2;
            }
            if (line.size() == 0)
                w = 1; // 空行也有一格的高度
            widths.push_back(w);
        }
        ifs.close();
    }

    // 随机文本
    void rand(const string &filename, int rand_cnt = 10) {
        ifstream ifs;
        read_stream(filename, ifs);
        Char ch;
        vector<Char> str;
        while (ifs >> ch) {
            if (!ch.is_ascii()) {
                str.push_back(ch);
            }
        }

        // 均匀随机 k 排列: shuffle
        int cnt = 0, n = str.size();
        rand_cnt = std::min(n, rand_cnt);
        for (int i = 0; i < rand_cnt; ++i) {
            int j = i + std::rand() % (n-i);
            std::swap(str[i], str[j]);
        }
        vector<Char> slice = vector<Char>(str.begin(), str.begin() + rand_cnt);
        text.clear();
        text.push_back(String(slice));
        widths.push_back(rand_cnt * 2);
        ifs.close();
    }

    int size() const {
        return text.size();
    }

    const String &get(int line) const {
        return text[line];
    }

    // 第 line 行第 col 字
    Char get(size_t line, size_t col) const {
        if (line < text.size() && col < text[line].size())
            return text[line][col];
        return Char();
    }

    int width(int line) {
        return widths[line];
    }

    bool is_end(int line, int col) const {
        return line == size() - 1 && col == get(line).size();
    }

};

enum Status {
    C1, // 英文正确
    C2, // 中文正确
    E1, // 英文错误
    E2, // 中文错误
    E3, // 误在英文处输入中文
    E4, // 误在中文处输入英文
    CR  // 回车
};
enum Ctrl {NONE, CONTINUE, BREAK, RETURN};

class Game {
    const char *filename;
    int begin, end, rand_cnt;
    bool need_restart;
    int line, col, x;
    int loaded_cnt;
    struct { int correct = 0, error = 0, backspace = 0; } statistics;
    stack<Status> undo;
    Article article;

public:
    Game(const char *filename, int begin = 1, int end = 0x7fffffff, int rand_cnt = -1):
        filename(filename), begin(begin), end(end), rand_cnt(rand_cnt)
    {
        do {
            restart();
        } while (need_restart);
    }

private:
    void restart() {
        this->line = 0;
        this->col = 0;
        this->x = 0;
        this->loaded_cnt = 0;
        this->need_restart = false;
        this->statistics = { 0, 0, 0 };

        setlocale(LC_ALL, "");      // 使用系统 locale
        if (rand_cnt == -1) {
            article.read(filename, begin, end);
        } else {
            article.rand(filename, rand_cnt);
        }

        print();
        play();
    }

    // 在画面范围内打印
    void print() {
        screen_clear();
        get_ttysize();
        cout << STYLE_DIM;
        if (loaded_cnt < article.size()) {
            int h = height(loaded_cnt);
            while (loaded_cnt < article.size() && h < tty.ws_row) {
                cout << article.get(loaded_cnt++) << '\n';
                h += height(loaded_cnt);
            }
            cursor_up(h - height(loaded_cnt));
        }
        cout << STYLE_RESET;
    }

    void play() {
        toggle_flush();
        int c = getchar();
        long begin = time_ms();
        control(c);
        if (!need_restart) {
            summary(begin);
        }
        toggle_flush();
    }

    static int width(const Char &c) {
        return c.is_ascii() ? 1 : 2;
    }

    int width(int line) {
        return article.width(line);
    }

    int height(int line) {
        return width(line) / tty.ws_col + 1; // 记得刷新终端尺寸
    }

    void update() {
        // 用户已打完的行数距离已打印行数小于 4 时更新一行
        if (loaded_cnt < line + 4 && loaded_cnt < article.size()) {
            get_ttysize();
            cout << STYLE_DIM;
            // 光标下移三行
            int k = 0;
            while (k < 3) cursor_down(height(line+(k++)));
            // 打印新的一行
            const int h = height(loaded_cnt);
            cout << article.get(loaded_cnt++) << endl;
            // 光标归位
            cursor_up(h);
            while (k) cursor_up(height(line+(--k)));
            cout << STYLE_RESET;
        }
    }

    void summary(long begin)
    {
        int s = time_ms() - begin;
        int mm = s / 60000;
        int ss = s / 1000;
        int ms = s % 1000;
        int typed = statistics.correct + statistics.error;

        // 滚动到页尾
        get_ttysize();
        int sum = 0;
        while (line < article.size() && sum < tty.ws_row) {
            int h = height(line++);
            cursor_down(h);
            sum += h;
        }

        int count = 0;
	int len = 0;
	const int recent_count = 20;
        float recent_speed[recent_count];
        float avg = 0.0f;
        fstream fs("typing.log", ios::in);
        if (fs) {
            // 存在则读取, 然后清空文件重新打开
            fs >> count;
            len = count < recent_count ? count : recent_count;
            for (int i = 0; i < len; ++i) {
                fs >> recent_speed[i];
            }
            fs.close();
            fs.open("typing.log", ios::out | ios::trunc);
        } else {
            // 不存在则创建
            fs.close();
            fs.open("typing.log", ios::out);
        }

        float accuracy = 100.0 * statistics.correct / typed;
        float speed = 600.0 * statistics.correct * accuracy / s;
        fs << count + 1 << '\n';
        for (int i = 0; i < len; ++i) {
            avg += recent_speed[i];
	    // 最多只保留最近 recent_count 条速度记录, 然后取平均
            if (i > 0 || len < recent_count) {
                fs << recent_speed[i] << '\n';
            }
        }
        avg = (avg + speed) / (len + 1);
        fs << speed << endl;
        fs.close();
        ++count;

        printf(
            "\r--------------------\n"
            "lesson:    %d\n"
            "time:      %dm %ds %dms\n"
            "speed:     %.2fkpm\n"
            "avg_speed: %.2fkpm\n"
            "accuracy:  %.2f%%\n"
            "typed:     %d\n"
            "correct:   %d\n"
            "backspace: %d\n",
            count,
            mm, ss, ms,
            speed,
            avg,
            accuracy,
            typed,
            statistics.correct,
            statistics.backspace
        );
    }

    void control(int key) {
        string buf;
        Ctrl ctrl = NONE;
        // ctrl-d to exit
        while (key != 4) {
            if (key == 127) {
                ctrl = onBackspace();
            } else if (key == 21) { // ctrl-u
                need_restart = true;
                return;
            } else if (key == '\n') {
                ctrl = onReturn();
            } else {
                Char answer = article.get(line, col);
                if (answer) {
                    ctrl = onInput(key, buf, answer);
                } else if (article.is_end(line, col)) {
                    // 跟打结束, 按任意键退出
                    ctrl = BREAK;
                }
//              else
//                  cout << "ignore: " << key << endl;
            }

            if (ctrl == CONTINUE) continue;
            else if (ctrl == BREAK) break;
            else if (ctrl == RETURN) return;
            key = getchar();
        }
    }

    Ctrl onBackspace() {
        get_ttysize();
        if (!undo.empty()) {
            ++statistics.backspace;
            enum Status stat = undo.top(); undo.pop();
            switch (stat) {
                case C1: remove(true, 1);  break;
                case C2: remove(true, 2);  break;
                case E1: remove(false, 1); break;
                case E2: case E4: remove(false, 2); break;
                case E3: remove(false, 2, true); break;
                case CR:
                    col = article.get(--line).size();
                    x = article.width(line);
                    cursor_up(1);
                    cursor_right(x % tty.ws_col);
                    break;
                default: break;
            }
        }
        return NONE;
    }

    Ctrl onReturn() {
        if (!article.get(line, col)) {
            undo.push(CR);
            ++statistics.correct;
            ++line;
            col = 0;
            x = 0;
            putchar('\n');
            update();
            if (line == article.size())
                return BREAK;
        }
        return NONE;
    }

    Ctrl onInput(char key, string &buf, const Char &answer) {
        buf += key;
        istringstream is(buf);
        Char c;
        if (is >> c) {
            buf = "";
            if (c == answer) {
                cout << c;
                add_correct(width(c));
            } else {
                cout << COLOR(FG_RED) << c << STYLE_RESET;
//              cout << answer.ord;
                add_error(width(c), width(answer));
            }
            x += width(c);
            ++col;
        }
        return NONE;
    }

    void add_correct(int w) {
        if (w == 1)
            undo.push(C1); // 英文正确
        else
            undo.push(C2); // 中文正确
        ++statistics.correct;
    }

    void add_error(int w, int w_ans) {
        if (w == 1) {
            if (w_ans == 1) {
                undo.push(E1); // 英文错误
            } else {
                undo.push(E4); // 误在中文处输入英文
                cout << ' '; //! 补齐中文宽度
            }
        } else {
            if (w_ans == 1) {
                undo.push(E3); // 误在英文处输入中文
                ++col; //! 额外多加1
            } else {
                undo.push(E2); // 中文错误
            }
        }
        ++statistics.error;
    }

    void remove(bool correct, int w, bool additional=false) {
        if (correct)
            --statistics.correct;
        else
            --statistics.error;
        --col;
        x -= w;
        cursor_left(w);
        cout << STYLE_DIM;
        if (additional) {
            cout << article.get(line, col-1) << article.get(line, col);
            --col; //! 额外多减1
        } else {
            cout << article.get(line, col);
        }
        cout << STYLE_RESET;
        cursor_left(w);
    }

};

int main(int argc, char *argv[])
{
    if (argc == 1) {
        cout << "Typing practice. Choose your favorite article!\n"
            "usage: typing [filename] [begin line] [end line]\n";
        return 0;
    }
    //logger.open("typing.log", ios::out);

    srand(time(NULL));
    if (argc == 2) {
        Game game(argv[1]);
    } else if (argc == 3) {
        if (strcmp(argv[2], "-r") == 0) {
            Game game(argv[1], 1, 0x7fffffff, 10);
        } else {
            Game game(argv[1], atoi(argv[2]));
        }
    } else if (argc == 4) {
        if (strcmp(argv[2], "-r") == 0) {
            Game game(argv[1], 1, 0x7fffffff, atoi(argv[3]));
        } else {
            Game game(argv[1], atoi(argv[2]), atoi(argv[3]));
        }
    }
    return 0;
}

