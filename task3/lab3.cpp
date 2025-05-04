#include "C:\Users\User\OneDrive\Desktop\labengine\labengine.h"
#pragma warning(disable : 4996)
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define FIELD_SIZE 8
#define CELL_SIZE 50
#define COLORS_COUNT 5
#define BONUS_CHANCE 30 

enum BonusType { NONE, RECOLOR, BOMB };

struct Cell {
    int color;
    BonusType bonus;
    int x, y;
};

Cell field[FIELD_SIZE][FIELD_SIZE];
int cursorX = 0, cursorY = 0;
int selectedX = -1, selectedY = -1; 
int score = 0;


labcolor_t colors[COLORS_COUNT] = {
    LABCOLOR_RED,
    LABCOLOR_GREEN,
    LABCOLOR_BLUE,
    LABCOLOR_YELLOW,
    LABCOLOR_MAGENTA
};

void initField() {
    srand(time(NULL));
    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE; x++) {
            field[y][x].color = rand() % COLORS_COUNT;
            field[y][x].bonus = NONE;
            field[y][x].x = x * CELL_SIZE + 50;
            field[y][x].y = y * CELL_SIZE + 50;
        }
    }
}

void drawField() {
    LabClear(); 

    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE; x++) {
            
            LabSetColor(colors[field[y][x].color]);

            
            for (int i = 0; i < CELL_SIZE; i++) {
                LabDrawLine(field[y][x].x, field[y][x].y + i,
                    field[y][x].x + CELL_SIZE, field[y][x].y + i);
            }

            // курсор (белая рамка)
            if (x == cursorX && y == cursorY) {
                LabSetColor(LABCOLOR_WHITE);
                LabDrawRectangle(field[y][x].x, field[y][x].y,
                    field[y][x].x + CELL_SIZE, field[y][x].y + CELL_SIZE);
            }

            // выбранная ячейка (желтая рамка)
            if (x == selectedX && y == selectedY) {
                LabSetColor(LABCOLOR_YELLOW);
                LabDrawRectangle(field[y][x].x, field[y][x].y,
                    field[y][x].x + CELL_SIZE, field[y][x].y + CELL_SIZE);
            }

            // бонусы
            if (field[y][x].bonus != NONE) {
                LabSetColor(LABCOLOR_BLACK);
                if (field[y][x].bonus == RECOLOR) {
                     // "+"
                    LabSetColorRGB(0, 0, 0); 
                    LabDrawLine(field[y][x].x + 15, field[y][x].y + 20, field[y][x].x + 35, field[y][x].y + 20);
                    LabDrawLine(field[y][x].x + 25, field[y][x].y + 10, field[y][x].x + 25, field[y][x].y + 30);
                }
                else if (field[y][x].bonus == BOMB) {
                    // B
                    LabDrawLine(field[y][x].x + 20, field[y][x].y + 20, field[y][x].x + 20, field[y][x].y + 24);
                    LabDrawLine(field[y][x].x + 20, field[y][x].y + 20, field[y][x].x + 23, field[y][x].y + 20);
                    LabDrawLine(field[y][x].x + 20, field[y][x].y + 22, field[y][x].x + 23, field[y][x].y + 22);
                    LabDrawLine(field[y][x].x + 20, field[y][x].y + 24, field[y][x].x + 23, field[y][x].y + 24);
                    LabDrawLine(field[y][x].x + 24, field[y][x].y + 21, field[y][x].x + 24, field[y][x].y + 22);
                    LabDrawLine(field[y][x].x + 24, field[y][x].y + 23, field[y][x].x + 24, field[y][x].y + 24);
                }
            }
        }
    }

    //счет
    LabSetColor(LABCOLOR_WHITE);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);

    int startX = 10;
    int startY = 10;

    for (int i = 0; scoreText[i] != '\0'; i++) {
        char c = scoreText[i];
        if (c == 'S') {
            LabDrawLine(startX + i * 6, startY, startX + i * 6 + 4, startY);
            LabDrawLine(startX + i * 6, startY, startX + i * 6, startY + 2);
            LabDrawLine(startX + i * 6, startY + 2, startX + i * 6 + 4, startY + 2);
            LabDrawLine(startX + i * 6 + 4, startY + 2, startX + i * 6 + 4, startY + 4);
            LabDrawLine(startX + i * 6, startY + 4, startX + i * 6 + 4, startY + 4);
        }
        else if (c == ':') {
            LabDrawPoint(startX + i * 6 + 2, startY + 1);
            LabDrawPoint(startX + i * 6 + 2, startY + 3);
        }
        else if (c >= '0' && c <= '9') {
            int n = c - '0';
            // Упрощенное отображение цифр
            if (n != 1 && n != 4) LabDrawLine(startX + i * 6, startY, startX + i * 6 + 4, startY);
            if (n != 5 && n != 6) LabDrawLine(startX + i * 6 + 4, startY, startX + i * 6 + 4, startY + 2);
            if (n != 0 && n != 1 && n != 7) LabDrawLine(startX + i * 6, startY + 2, startX + i * 6 + 4, startY + 2);
            if (n != 2) LabDrawLine(startX + i * 6, startY + 2, startX + i * 6, startY + 4);
            if (n != 1 && n != 4 && n != 7) LabDrawLine(startX + i * 6 + 4, startY + 2, startX + i * 6 + 4, startY + 4);
            if (n != 1 && n != 4 && n != 7) LabDrawLine(startX + i * 6, startY + 4, startX + i * 6 + 4, startY + 4);
        }
    }

    LabDrawFlush(); 
}

bool isValidCell(int x, int y) {
    return x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE;
}

void swapCells(int x1, int y1, int x2, int y2) {
    std::swap(field[y1][x1].color, field[y2][x2].color);
    std::swap(field[y1][x1].bonus, field[y2][x2].bonus);
}

bool checkMatches() {
    bool matchesFound = false;
    bool marked[FIELD_SIZE][FIELD_SIZE] = { false };
    bool bonusDropped = false; 

    // Проверка совпадений
    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE; x++) {
            if (field[y][x].color == -1) continue;

            // Проверка горизонтальных совпадений
            if (x < FIELD_SIZE - 2 &&
                field[y][x].color == field[y][x + 1].color &&
                field[y][x].color == field[y][x + 2].color) {
                int endX = x + 2;
                while (endX + 1 < FIELD_SIZE && field[y][endX + 1].color == field[y][x].color)
                    endX++;

                for (int i = x; i <= endX; i++)
                    marked[y][i] = true;

                matchesFound = true;
            }

            // Проверка вертикальных совпадений
            if (y < FIELD_SIZE - 2 &&
                field[y][x].color == field[y + 1][x].color &&
                field[y][x].color == field[y + 2][x].color) {
                int endY = y + 2;
                while (endY + 1 < FIELD_SIZE && field[endY + 1][x].color == field[y][x].color)
                    endY++;

                for (int i = y; i <= endY; i++)
                    marked[i][x] = true;

                matchesFound = true;
            }

            // Проверка квадратов 2x2
            if (x < FIELD_SIZE - 1 && y < FIELD_SIZE - 1 &&
                field[y][x].color == field[y][x + 1].color &&
                field[y][x].color == field[y + 1][x].color &&
                field[y][x].color == field[y + 1][x + 1].color) {
                marked[y][x] = marked[y][x + 1] = marked[y + 1][x] = marked[y + 1][x + 1] = true;
                matchesFound = true;
            }
        }
    }

    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE - 2; x++) {
            if (field[y][x].color == field[y][x + 1].color &&
                field[y][x].color == field[y][x + 2].color) {
                marked[y][x] = marked[y][x + 1] = marked[y][x + 2] = true;
                matchesFound = true;
            }
        }
    }

    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE - 2; y++) {
            if (field[y][x].color == field[y + 1][x].color &&
                field[y][x].color == field[y + 2][x].color) {
                marked[y][x] = marked[y + 1][x] = marked[y + 2][x] = true;
                matchesFound = true;
            }
        }
    }

    // Обработка совпадений
    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE; x++) {
            if (marked[y][x]) {
                score += 10;

                int originalColor = field[y][x].color;

                // Выпадение бонуса
                if (!bonusDropped && rand() % 100 < BONUS_CHANCE) {
                    int attempts = 0;
                    while (attempts < 20) {
                        int dx = (rand() % 7) - 3;
                        int dy = (rand() % 7) - 3;
                        int bx = x + dx;
                        int by = y + dy;
                        if (isValidCell(bx, by) && field[by][bx].bonus == NONE) {
                            field[by][bx].bonus = (rand() % 10 < 7) ? RECOLOR : BOMB;
                            field[by][bx].color = originalColor; 
                            bonusDropped = true;
                            break;
                        }
                        attempts++;
                    }
                }

                // Уничтожаем ячейку 
                field[y][x].color = -1;
                field[y][x].bonus = NONE; 
            }
              
        }
    }

    return matchesFound;
}

void applyGravity() {
    for (int x = 0; x < FIELD_SIZE; x++) {
        int writePos = FIELD_SIZE - 1;
        for (int y = FIELD_SIZE - 1; y >= 0; y--) {
            if (field[y][x].color != -1) {
                if (y != writePos) {
                    field[writePos][x].color = field[y][x].color;
                    field[writePos][x].bonus = field[y][x].bonus;
                }
                writePos--;
            }
        }
       
        for (; writePos >= 0; writePos--) {
            field[writePos][x].color = rand() % COLORS_COUNT;
            field[writePos][x].bonus = NONE; 
        }
    }
}

void applyBonus(int x, int y) {
    if (!isValidCell(x, y) || field[y][x].bonus == NONE) return;

    int sourceColor = field[y][x].color; 
    BonusType bonusType = field[y][x].bonus;
    field[y][x].color = -1;
    field[y][x].bonus = NONE; 

    switch (bonusType) {
    case RECOLOR: {
        // 1. Перекрашиваем текущий квадрат
        field[y][x].color = sourceColor;

        // 2. Находим 2 случайных НЕсоседа в радиусе 3
        int recolored = 0;
        int attempts = 0;
        const int maxAttempts = 50; 

        while (recolored < 2 && attempts < maxAttempts) {
            attempts++;

        
            int dx = (rand() % 7) - 3; // от -3 до +3
            int dy = (rand() % 7) - 3; // от -3 до +3

            int nx = x + dx;
            int ny = y + dy;

            
            if (isValidCell(nx, ny) &&
                !(nx == x && ny == y) && // не текущая ячейка
                (abs(dx) > 1 || abs(dy) > 1) && // не сосед по горизонтали/вертикали
                !(abs(dx) == 1 && abs(dy) == 1)) { // исключаем диагональных соседей
                field[ny][nx].color = sourceColor;
                recolored++;
            }
        }
        break;
    }

    case BOMB: {
        
        field[y][x].color = -1;
        int destroyed = 1; 

       
        while (destroyed < 5) {
            int rx = rand() % FIELD_SIZE;
            int ry = rand() % FIELD_SIZE;
            if (field[ry][rx].color != -1) {
                field[ry][rx].color = -1;
                destroyed++;
            }
        }
        field[y][x].bonus = NONE; 
        break;
    }
    }
}


void processGameLogic() {
    while (checkMatches()) {
        applyGravity();
        drawField();
        LabDelay(500); 
    }
}

int main() {
    if (LabInit()) {
        initField();
        labkey_t key;

        while (1) {
            drawField();

            
            if (LabInputKeyReady()) {
                key = LabInputKey();

                if (key == LABKEY_ESC) break;

                // Управление курсором
                switch (key) {
                case LABKEY_LEFT:
                    if (cursorX > 0) cursorX--;
                    break;
                case LABKEY_RIGHT:
                    if (cursorX < FIELD_SIZE - 1) cursorX++;
                    break;
                case LABKEY_UP:
                    if (cursorY > 0) cursorY--;
                    break;
                case LABKEY_DOWN:
                    if (cursorY < FIELD_SIZE - 1) cursorY++;
                    break;
                case LABKEY_ENTER: {
                    if (selectedX == -1) {
                        selectedX = cursorX;
                        selectedY = cursorY;
                    }
                    else {
                        if ((abs(selectedX - cursorX) == 1 && selectedY == cursorY) ||
                            (abs(selectedY - cursorY) == 1 && selectedX == cursorX)) {
                            swapCells(selectedX, selectedY, cursorX, cursorY);

                      
                            if (field[cursorY][cursorX].bonus != NONE) {
                                applyBonus(cursorX, cursorY);
                            }
                            if (field[selectedY][selectedX].bonus != NONE) {
                                applyBonus(selectedX, selectedY);
                            }

                            
                            if (checkMatches()) {
                                processGameLogic();
                            }
                            else {
                                swapCells(selectedX, selectedY, cursorX, cursorY);
                            }

                            selectedX = -1;
                            selectedY = -1;
                        }
                    }
                    break;
                }
                }
            }
        }
    }

    LabTerm();
    return 0;
}