/*
	Tony's Snake 2.2 Remastered
	Programmed by Tony
	2020.9.13
	
	upgrade: cursor is now invisible
*/

#include <time.h>
#include "snake.h"

const int FREQUENCY = 100;

int high_score = 0;

void playGame(bool con);
void quitGame();
void tired();
void help();
int random(int lower, int upper);   //return a number in interval [lower, upper]
void load();
void saveScore(int score);
void saveStatus(Snake* snake, int fx, int fy, int score, int delay);
bool loadStatus(Snake* snake, int* fx, int *fy, int* score, int* delay);
bool loadStatus();
bool saveGame();
int main()
{
    Snake egg;  //Ester egg snake
    int i;
    char cmd;
    bool showContinue = false;

    //initialize
    setTextFont(10, 10, L"Fixedsys");
    hideCursor();
    load();
    srand((unsigned) time (NULL));
    showContinue = loadStatus();

    generate();
    menuInterface(showContinue);
    egg.setPosition(21, 6);
    for (i = 1; i <= 100; i++)
        egg.grow();
    while (1)
    {
        cmd = getCommand();
        switch(cmd)
        {
        case 'P':
            playGame(false);
            egg.grow();
            showContinue = loadStatus();
            menuInterface(showContinue);
            egg.printWhole();
            break;
        case 'C':
            playGame(true);
            egg.grow();
            showContinue = loadStatus();
            menuInterface(showContinue);
            egg.printWhole();
            break;
        case 'H':
            help();
            menuInterface(showContinue);
            egg.printWhole();
            break;
        case ESC:
            quitGame();
            return 0;
        default:
            break;
        }
        if (egg.getx() == 21 && egg.gety() == 6)
            egg.changeDirection(3);
        else if (egg.getx() == 81 && egg.gety() == 6)
            egg.changeDirection(2);
        else if (egg.getx() == 81 && egg.gety() == 36)
            egg.changeDirection(1);
        else if (egg.getx() == 21 && egg.gety() == 36)
            egg.changeDirection(0);
        egg.move();
        egg.print();
        if (egg.biteSelf())
        {
            tired();
            quitGame();
            return 0;
        }
    }

    return 0;
}

void playGame(bool con)
{
    Snake* snake;
    int snake_x, snake_y, length;
    int food_x, food_y;
    int i, j;
    int delay = FREQUENCY;
    int score = 0;
    char cmd;
    bool quit = false;

    gameInterface();

    snake = new Snake();
    if (!con)
    {
        //generate snake
        snake_x = random(2, COLUMN - 1);
        snake_y = random(2, ROW - 1);
        snake->setPosition(snake_x, snake_y);
        length = 2;	//bug fixed!-2019.10.13
        //generate first food
        food_x = random(2, COLUMN - 1);
        food_y = random(2, ROW - 1);
        while (snake->onSnake(food_x, food_y))
        {
            food_x = random(2, COLUMN - 1);
            food_y = random(2, ROW - 1);
        }
    }
    else
    {
        FILE * fp;

        fp = fopen("save.dat", "r");
        if (!loadStatus(snake, &food_x, &food_y, &score, &delay))
        {
            delete snake;
            fclose(fp);
            errorInterface();
            return;
        }
        fclose(fp);
        snake_x = snake->getx();
        snake_y = snake->gety();
    }
    //print snake
    snake->printWhole();
    //print food
    gotoPosition(food_x, food_y);
    putchar(FOOD);
    //data module
    printScore(snake->getLength() - 2, high_score);
    printData(snake->getx(), snake->gety(), snake->getLength(), food_x, food_y, delay);
    getCommand();
    while (!snake->biteSelf())
    {
        if (_kbhit())
        {
            cmd = toupper(getch());
            switch(cmd)
            {
            case UP:
                snake->changeDirection(0);
                break;
            case DOWN:
                snake->changeDirection(2);
                break;
            case LEFT:
                snake->changeDirection(1);
                break;
            case RIGHT:
                snake->changeDirection(3);
                break;
            case STOP:
                if (saveGame())
                    saveStatus(snake, food_x, food_y, score, delay);
                //clear data module
                for (i = COLUMN + 1; i < LENGTH - 1; i++)
                {
                    for (j = 2; j < MIDDLE; j++)
                    {
                        gotoPosition(i, j);
                        putchar(SPACE);
                    }
                }
                break;
            case ESC:
                quit = true;
                break;
            case UPGRADE:
                snake->grow();
                score++;
                if ((score % 10 == 0) && (delay > 10))  //level up
                    delay -= 10;
                break;
            case ACCELERATE:
                if (delay > 10)
                    delay -= 10;
                else if (delay > 1)
                    delay--;
                break;
            case DECELERATE:
                if (delay < 10)
                    delay++; 
                else if (delay < 1000)
                    delay += 10;
                break;
            default:
                continue;
                break;
            }
            if (quit)
                break;
        }
        snake->move();
        snake_x = snake->getx();
        snake_y = snake->gety();
        if (snake_x <= 1)
            snake_x = COLUMN - 1;
        if (snake_x >= COLUMN)
            snake_x = 2;
        if (snake_y <= 1)
            snake_y = ROW - 1;
        if (snake_y >= ROW)
            snake_y = 2;
        snake->changePosition(snake_x, snake_y);
        snake->print();
        if ((food_x == snake_x) && (food_y == snake_y)) //gets the food
        {
            snake->grow();
            score++;
            if ((score % 10 == 0) && (delay > 10))  //level up
                delay -= 10;
            //generate new food
            food_x = random(2, COLUMN - 1);
            food_y = random(2, ROW - 1);
            while (snake->onSnake(food_x, food_y))
            {
                food_x = random(2, COLUMN - 1);
                food_y = random(2, ROW - 1);
            }
            gotoPosition(food_x, food_y);
            putchar(FOOD);
        }

        printScore(score, high_score);
        printData(snake_x, snake_y, snake->getLength(), food_x, food_y, delay);
        absDelay(delay);
    }
    //dead
    setTextColor(0x4);
    snake->printWhole();
    setTextColor(0x7);
    //clear data module
    for (i = COLUMN + 1; i < LENGTH - 1; i++)
    {
        for (j = 2; j < MIDDLE; j++)
        {
            gotoPosition(i, j);
            putchar(SPACE);
        }
    }
    //game over
    i = COLUMN + (LENGTH - COLUMN - strlen(OVER)) / 2;
    j = MIDDLE / 2;
    gotoPosition(i, j - 5);
    setTextColor(0x4);
    printf("%s", OVER);
    setTextColor(0x7);
    score = snake->getLength() - 2;
    i = COLUMN + (LENGTH - COLUMN - 20) / 2;
    if (score <= high_score)
    {
        gotoPosition(i, j - 1);
        printf("Final score:\t %d", score);
        gotoPosition(i, j + 1);
        printf("High score:\t %d", high_score);
    }
    else
    {
        gotoPosition(i, j);
        setTextColor(0xA);
        printf("New High Score: %d !", score);
        setTextColor(0x7);
        high_score = score;
        saveScore(score);
    }

    i = COLUMN + (LENGTH - COLUMN - strlen(NOTE4)) / 2 + 3;
    j += 5;
    gotoPosition(i, j);
    while (_kbhit())
        getCommand();
    printf("%s", NOTE4);
    getCommand();

    delete snake;
}
void quitGame()
{
    int i, j;

    //clear snake's area
    for (i = 2; i < COLUMN; i++)
    {
        for (j = 2; j < ROW; j++)
        {
            gotoPosition(i, j);
            putchar(SPACE);
        }
    }
    i = (COLUMN - strlen(NOTE2)) / 2;
    j = ROW / 2;
    gotoPosition(i, j);
    setTextColor(0x9);
    printf("%s", NOTE2);
    setTextColor(0x7);
    absDelay(2000);
    clearScreen();

    return ;
}
void tired()
{
    int i, j;

    //clear snake's area
    for (i = 2; i < COLUMN; i++)
    {
        for (j = 2; j < ROW; j++)
        {
            gotoPosition(i, j);
            putchar(SPACE);
        }
    }
    i = (COLUMN - strlen(TIRED1)) / 2;
    j = ROW - MIDDLE;
    gotoPosition(i, j - 1);
    printf("%s", TIRED1);
    i = (COLUMN - strlen(TIRED2)) / 2;
    gotoPosition(i, j + 1);
    printf("%s", TIRED2);
    i = (COLUMN - strlen(NOTE4)) / 2;
    gotoPosition(i, j + 5);
    printf("%s", NOTE4);
    while (_kbhit())
        getCommand();
    getCommand();

    return ;
}
void help()
{
    FILE * fp;
    int i, j;
    int line, column;
    char ch;

    //clear snake's area
    for (i = 2; i < COLUMN; i++)
    {
        for (j = 2; j < ROW; j++)
        {
            gotoPosition(i, j);
            putchar(SPACE);
        }
    }

    fp = fopen("help.txt", "r");
    if (fp == NULL)
    {
        i = (COLUMN - strlen(NOTE3)) / 2;
        j = ROW - MIDDLE;
        gotoPosition(i, j);
        setTextColor(0x4);
        printf("%s", NOTE3);
        setTextColor(0x7);
        i = (COLUMN - strlen(NOTE4)) / 2;
        j += 2;
        gotoPosition(i, j);
        while (_kbhit())
            getCommand();
    }
    else
    {
        line = 5;
        column = 5;
        gotoPosition(column, line);
        setTextColor(0xB);
        while (fscanf(fp, "%c", &ch) != EOF)
        {
            putchar(ch);
            if (ch == '\n')
                gotoPosition(column, ++line);
            else if (!isalpha(ch))
                continue;
            else
                absDelay(5);
        }
        setTextColor(0x7);
        line += 2;
        gotoPosition(column, line);
        while (_kbhit())
            getCommand();
    }
    gotoPosition(5, 36);
    printf("%s", NOTE4);
    getCommand();

    return ;
}
int random(int lower, int upper)    //return a number in interval [lower, upper]
{
    return lower + rand() % (upper - lower + 1);
}
void load()
{
    FILE * fp;

    system("CLS");
    printf("Loading..\n");
    fp = fopen("score.dat", "r");
    if (fp == NULL)
    {
        setTextColor(0x4);
        printf("Warning: Cannot find the file \"score.dat\" !\n");
        setTextColor(0x7);
        absDelay(500);
    }
    else
    {
        fscanf(fp, "%d", &high_score);
        fclose(fp);
    }
    fp = fopen("help.txt", "r");
    if (fp == NULL)
    {
        setTextColor(0x4);
        printf("Warning: Cannot find the file \"help.txt\" !\n");
        setTextColor(0x7);
        absDelay(500);
    }
    else
        fclose(fp);

    return ;
}
void saveScore(int score)
{
    FILE * fp;

    fp = fopen("score.dat", "w");
    fprintf(fp, "%d\n", score);

    fclose(fp);
    return ;
}
void saveStatus(Snake* snake, int fx, int fy, int score, int delay)
{
    FILE * fp;

    fp = fopen("save.dat", "w");
    snake->exportData(fp);
    fprintf(fp, "[food info]\n");
    fprintf(fp, "%d %d\n", fx, fy);
    fprintf(fp, "[game info]\n");
    fprintf(fp, "%d %d\n", score, delay);

    fclose(fp);
}
bool loadStatus(Snake* snake, int* fx, int* fy, int* score, int* delay)
{
    FILE* fp;

    fp = fopen("save.dat", "r");
    if (fp == NULL)
        return false;
    else
    {
        if (!snake->loadData(fp))
            return false;
        fscanf(fp, "%*[^0123456789] %d %d", fx, fy);
        fscanf(fp, "%*[^0123456789] %d %d", score, delay);
        fclose(fp);
    }

    return true;
}
bool loadStatus()
{
    FILE* fp;
    fp = fopen("save.dat", "r");
    if (fp == NULL)
        return false;
    else
        fclose(fp);

    return true;
}
bool saveGame()
{
    int i, j;
    char cmd;

    //clear data module
    for (i = COLUMN + 1; i < LENGTH - 1; i++)
    {
        for (j = 2; j < MIDDLE; j++)
        {
            gotoPosition(i, j);
            putchar(SPACE);
        }
    }

    while (_kbhit())
        getCommand();
    i = COLUMN + (LENGTH - COLUMN - strlen(SAVE)) / 2;
    j = MIDDLE / 2;
    gotoPosition(i, j);
    printf("%s", SAVE);
    i = COLUMN + (LENGTH - COLUMN - 12) / 2;
    gotoPosition(i, j + 2);
    printf("(Y) or (Esc)");
    while (1)
    {
        cmd = getCommand();
        switch (cmd)
        {
        case 'Y':
            return true;
            break;
        case ESC:
            return false;
            break;
        default:
            break;
        }
    }

    return false;
}