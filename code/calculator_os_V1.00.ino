#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
//libs for the keypad
#include <Keypad.h>
//include libs for internal eeprom
#include <EEPROM.h>
//backlight pin
#define backlight 5
#define buzzer 9
//some usefful defines for calculator
#define screen_w  84
#define screen_h  48
#define char_w    6
#define char_h    8
#define func_pos_x 2
#define func_pos_y 28
#define char_offset 48
#define first_num_pos_y 12
#define second_num_pos_y 20
#define third_num_pos_y 39
#define len_of_num 8

int8_t num1[len_of_num - 1];
int8_t num2[len_of_num - 1];

//selected function
int8_t selected_func;
//first number and length
long first_num = 0L;
int8_t first_num_len = 0;
//second number and length
long second_num = 0L;
int8_t second_num_len = 0;
//result and length
float result = 0;
int8_t result_len = 0;
//the current selected number (1 or 2)
int8_t selected_number = 1;
//boolean if we got result
bool result_flag = false;
//the state of the devise
int8_t state = 3;
//the main menu
int8_t menuitem = 1;
//the snake menu
int8_t snakemenuitem = 1;
//the snake diff_lvl
int8_t diff_lvl = 3;
int snakeSpeed = 175;
int8_t snakegamestate = 1;
int score = 0;
int lastScore = 0;
int highScore = 0;
//settings
int8_t brightness;
boolean sound;
boolean beep;
int8_t default_App;
static const unsigned char PROGMEM snake_splash[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x30, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x41, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x8d, 0x30, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x8d, 0xb0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x81, 0xc1, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x06, 0xe2, 0x16, 0xb0, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x0c, 0x00, 0x08, 0x70, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x06, 0x00, 0x00, 0x20, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x21, 0xe6, 0xe3, 0xc6, 0x63, 0xc0, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x23, 0xe7, 0xe7, 0xe6, 0xc7, 0xe0, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x07, 0x07, 0x6e, 0x67, 0x8c, 0xe0, 0x00,
  0x00, 0x02, 0x0c, 0xc0, 0x43, 0x8e, 0x6c, 0xcf, 0x8f, 0xc0, 0x00,
  0x00, 0x01, 0x04, 0x40, 0x81, 0xec, 0x78, 0xcf, 0xcf, 0x00, 0x00,
  0x00, 0x00, 0x80, 0x00, 0x80, 0x6c, 0x78, 0xcc, 0xcc, 0x00, 0x00,
  0x00, 0x00, 0x67, 0x03, 0x0f, 0xec, 0x6f, 0xcc, 0xef, 0xe0, 0x00,
  0x00, 0x00, 0x1c, 0x06, 0x07, 0x8c, 0x4f, 0x48, 0x67, 0xc0, 0x00,
  0x00, 0x00, 0x77, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc2, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x06, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3e, 0x04, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x04, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x08, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x08, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x78, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00,
  0x00, 0x00, 0x30, 0xd0, 0x00, 0x00, 0x00, 0x00, 0xf6, 0x00, 0x00,
  0x00, 0x00, 0x20, 0x90, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x20, 0xd0, 0x00, 0x7f, 0x80, 0x00, 0x18, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x01, 0x98, 0xf0, 0x00, 0x1e, 0x00, 0x00,
  0x00, 0x00, 0x40, 0x10, 0x05, 0x8f, 0x88, 0x00, 0x19, 0x00, 0x00,
  0x00, 0x00, 0x40, 0x10, 0x11, 0x82, 0x1c, 0x00, 0x38, 0x00, 0x00,
  0x00, 0x00, 0x40, 0x3e, 0xf3, 0x00, 0x12, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x20, 0x21, 0x0e, 0x00, 0x19, 0x80, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x20, 0x33, 0x00, 0x06, 0x0e, 0x61, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x0e, 0x00, 0x71, 0xc0, 0x23, 0x60, 0x00, 0x00,
  0x00, 0x00, 0x08, 0x00, 0x01, 0xc0, 0x20, 0x36, 0x40, 0x00, 0x00,
  0x00, 0x00, 0x04, 0x00, 0x07, 0x00, 0x18, 0x08, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x00, 0x1c, 0x00, 0x06, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xe3, 0xe0, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const unsigned char PROGMEM snake_game_over[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0x80, 0x70, 0x3c, 0x7c, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0xc0, 0xf8, 0x3e, 0x7c, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0xf1, 0xe0, 0xf8, 0x3e, 0x7c, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xe0, 0x01, 0xd8, 0x3e, 0x7c, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0x01, 0xdc, 0x3f, 0xfc, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x01, 0xc7, 0xe1, 0xdc, 0x3f, 0xfc, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x01, 0xc7, 0xe3, 0xfe, 0x3f, 0xfc, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xe0, 0xe3, 0xfe, 0x3f, 0xfc, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0xe7, 0x8f, 0x3b, 0xdc, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0xe7, 0x07, 0x3b, 0xdc, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0x87, 0x07, 0x3b, 0xdc, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0x07, 0x07, 0x7f, 0xc7, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xc7, 0x07, 0x7f, 0xc7, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe3, 0xc7, 0x8f, 0x70, 0x07, 0x0e, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0xe3, 0x8e, 0x70, 0x07, 0x0e, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0xe3, 0x8e, 0x7f, 0xc7, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0xe1, 0xdc, 0x7f, 0xc7, 0xf8, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0xe1, 0xdc, 0x70, 0x07, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xe1, 0xe1, 0xfc, 0x70, 0x07, 0x3c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf3, 0xc0, 0xf8, 0x70, 0x07, 0x1e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xc0, 0xf8, 0x7f, 0xc7, 0x0e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0x00, 0x70, 0x7f, 0xe7, 0x0e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'D', 'c', '%', '/'},
  {'7', '8', '9', 'x'},
  {'4', '5', '6', '-'},
  {'1', '2', '3', '+'},
  {'n', '0', '^', '='}
};
//the pins of the matrix
byte rowPins[ROWS] = {A5, 0, 2, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A3, A2, A1}; //connect to the column pinouts of the keypad
// object for the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
// object for the display with h/w SPI bus dc,cs,rst
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 6);
//simple class for x,y points
class point {
  public:
    int8_t x;
    int8_t y;
    int8_t prevx;
    int8_t prevy;

    point(int8_t x, int8_t y) {
      this->x = x;
      this->y = y;
      this->prevx = x;
      this->prevy = y;
    }
    point() {
      this->x = 0;
      this->y = 0;
      this->prevx = 0;
      this->prevy = 0;
    }
    setX(int8_t  x) {
      this->x = x;
    }
    setY(int8_t y) {
      this->y = y;
    }
    getX() {
      return this->x;
    }
    getY() {
      return this->y;
    }

    setPrevX(int8_t prevx) {
      this->prevx = prevx;
    }
    setPrevY(int8_t prevy) {
      this->prevy = prevy;
    }
    getPrevX() {
      return this->prevx;
    }
    getPrevY() {
      return this->prevy;
    }
};
//the food class spawns food in the map clears the old
class food {
  public:
    //the size the cell offset and the x,y location
    int8_t food_size = 4;
    int8_t cell_offset = 4;
    int8_t x;
    int8_t y;

    //default constructor
    food() {
    }

    //set the new food into a random location
    //but check if over tail
    newFood(point p[] , int8_t len) {
      x = random(19) + 1;
      y = random(10) + 1;
      int8_t i = 0;
      while (1) {
        if (p[i].getX() == x & p[i].getY() == y) {
          newFood(p, len);
        }
        if (i == len) {
          break;
        }
        i++;
      }
    }
    //draw the food
    drawFood() {
      display.fillRect((x * cell_offset), (y * cell_offset), 4, 4, BLACK);
      display.display();
    }
    //clear the old food
    clearFood() {
      display.fillRect((x * cell_offset), (y * cell_offset), 4, 4, WHITE);
      display.display();
    }

    //getters
    getX() {
      return this->x * cell_offset;
    }
    getY() {
      return this->y * cell_offset;
    }
};
//the snake class
class snake {
    //the variables for the snake
  public:
    int8_t snake_len = 1;
    int8_t part_size = 4;
    int8_t xdir = 1;
    int8_t ydir = 0;
    boolean gameOver = false;
    //the main body of the snake
    point body[47];

    //initialize the head somewhere in the middle
    snake() {
      body[0] = point(28, 20);
    }

    //function to set snakes direction
    setDirection(int8_t xdir, int8_t ydir) {
      if (!((this->xdir == 1 & xdir == -1) || (this->xdir == -1 & xdir == 1))) {
        this->xdir = xdir;
      }
      if (!((this->ydir == 1 & ydir == -1) || (this->ydir == -1 & ydir == 1))) {
        this->ydir = ydir;
      }

    }
    //update the snake
    updateSnake() {
      //if the head is inside the wall
      if (insideWall())
      {
        //move the head
        body[0].setX(body[0].getX() + (xdir * part_size));
        body[0].setY(body[0].getY() + (ydir * part_size));
        //now if the head didnt collide with the tail
        if (!headToTail()) {
          //go over every part
          for (int8_t i = snake_len; i > 0; i--) {
            //if we are in the last part of the snake
            //set its previous x,y so it can be
            //erased as it is the last part of the tail
            if (i == snake_len) {
              body[i].setPrevX(body[i].getX());
              body[i].setPrevY(body[i].getY());
              body[i].setX(body[i - 1].getX());
              body[i].setY(body[i - 1].getY());
            }
            //else just shift everything by 1 place
            body[i].setX(body[i - 1].getX());
            body[i].setY(body[i - 1].getY());
          }
          //else set game over flag
        } else {
          gameOver = true;
        }
      } else {
        gameOver = true;
      }
    }
    //check if the head hits the tail
    headToTail() {
      //if we have a snake with length > 1
      if (snake_len > 1) {
        //check every part against head
        for (int8_t i = 1; i <= snake_len; i++) {
          if (body[0].getX() == body[i].getX() &&
              body[0].getY() == body[i].getY())
            return true;
        }
      }
      return false;
    }
    //check if the head is inside the wall
    insideWall() {
      if (body[0].getX() + (xdir * part_size) > 3 & body[0].getX() + (xdir * part_size) < 78 &
          body[0].getY() + (ydir * part_size) > 3 & body[0].getY() + (ydir * part_size) < 42)
      {
        return true;
      } else {
        return false;
      }
    }
    //eat the food and grow
    snakeEatFood(food fd) {
      if (body[0].getX() == fd.getX() & body[0].getY() == fd.getY()) {
        this->grow();
        score += 20;
        return true;
      }
      return false;
    }
    //check the game over status
    checkGameOver() {
      return this->gameOver;
    }
    //grow the tail
    grow() {
      snake_len++;
    }

    //print the snake
    draw() {
      //go over its part
      for (int8_t i = 0; i <= snake_len; i++) {
        //if its the last one draw the part and erase the old position
        if (i == snake_len)
        {
          display.fillRect(body[i].getX(), body[i].getY(), part_size, part_size, BLACK);
          display.fillRect(body[i].getPrevX(), body[i].getPrevY(), part_size, part_size, WHITE);
          display.display();
          //else draw the part
        } else
          display.fillRect(body[i].getX(), body[i].getY(), part_size, part_size, BLACK);
        display.display();
      }
    }
};
//draw info about the handheld
void drawInfo() {
  display.setCursor(0, 0);
  display.print("     INFO     ");
  display.drawFastHLine(2, 8, 79, BLACK);
  display.setCursor(0, 10);
  display.print("Rom used:  82%");
  display.setCursor(0, 18);
  display.print("Ram used:  73%");
  display.setCursor(0, 26);
  display.print("Lines of code:");
  display.setCursor(0, 34);
  display.print("     1630     ");
  display.display();
}
//function to draw sqrt
void drawSqrt() {
  //clear old symbol
  display.fillRect(func_pos_x, func_pos_y, 6, 8, WHITE);
  //draw new
  display.drawPixel(func_pos_x, func_pos_y + 5, BLACK);
  display.drawPixel(func_pos_x + 1, func_pos_y + 6, BLACK);
  display.drawPixel(func_pos_x + 2, func_pos_y + 7, BLACK);
  display.drawLine(func_pos_x + 3, func_pos_y + 1, func_pos_x + 3, func_pos_y + 7, BLACK);
  display.drawLine(func_pos_x + 3, func_pos_y + 1, func_pos_x + 5, func_pos_y + 1, BLACK);
}
//function to clear the spot of the number
void clearNumber(int8_t spot) {
  if (spot == 1) {
    first_num = 0L;
    first_num_len = 0;
    display.fillRect(1, first_num_pos_y, 82, 8, WHITE);
  } else if (spot == 2) {
    second_num = 0L;
    second_num_len = 0;
    display.fillRect(1, second_num_pos_y, 82, 8, WHITE);
  }
  display.display();
}
//figure out the length of the result number
void resultLength(float resultnum) {
  int8_t num_of_dig = 0;
  //if the number isnt negative
  if (resultnum > 0) {
    //figure out the digits
    while (resultnum != 0) {
      resultnum = resultnum / 10;
      num_of_dig++;
    }
    //if its negative
  } else if (resultnum < 0) {
    //make it positve
    resultnum = resultnum * -1;
    //figure out the digits
    while (resultnum != 0) {
      resultnum = resultnum / 10;
      num_of_dig++;
    }
    num_of_dig++;
    //make it positve
    resultnum = resultnum * -1;
  } else {
    num_of_dig = 1;
  }
  //set the length of the result
  result_len  = num_of_dig;
}
//function to clear everything
void clearAll() {
  //reset numbers reset result
  //clear screen
  //be ready for another input
  first_num = 0L;
  first_num_len = 0;
  second_num = 0L;
  second_num_len = 0;
  result = 0;
  result_len = 0;
  //clear old symbol
  display.fillRect(func_pos_x, func_pos_y, 6, 8, WHITE);
  //clear numbers and result
  display.fillRect(1, first_num_pos_y, 82, 8, WHITE);
  display.fillRect(1, second_num_pos_y, 82, 8, WHITE);
  display.fillRect(1, third_num_pos_y, 82, 8, WHITE);
  //reset the flag
  result_flag = false;
  //selected_number
  selected_number = 1;
  display.display();
}
//this will print the result
void printResult(long first_num , long second_num , int8_t theFunction) {
  switch (theFunction) {
    case 1:
      result = first_num + second_num;
      break;
    case 2:
      result = first_num - second_num;
      break;
    case 3:
      result = first_num / second_num;
      break;
    case 4:
      result = first_num * second_num;
      break;
    case 5:
      result = first_num % second_num;
      break;
    case 6:
      result = pow(first_num, second_num);
      break;
    case 7:
      result = sqrt(first_num);
      break;
  }
  resultLength(result);
  displayNum(result, result, result_len, 3, 2);
}
//this function will print the selected symbol// 1 +// 2 -// 3 /// 4 *// 5 %// 6 ^
void printFuncSymbol(int8_t symbol) {
  //clear old symbol
  display.fillRect(func_pos_x, func_pos_y, 6, 8, WHITE);
  //set the cursor for the new one
  display.setCursor(func_pos_x, func_pos_y);
  //based on the selection display it
  switch (symbol) {
    case 1:
      display.print("+");
      break;
    case 2:
      display.print("-");
      break;
    case 3:
      display.print("/");
      break;
    case 4:
      display.print("*");
      break;
    case 5:
      display.print("%");
      break;
    case 6:
      display.print("^");
      break;
    default:
      break;
  }
  display.display();
}
//this function will print a number at a location
void displayNum(long theNumber, float theNumber2, int8_t lengthOfNumber, int8_t positionOfNumber, int8_t type) {
  //calculate the offset that the nymber needs to be printed
  int8_t offset = 84 - ((lengthOfNumber * char_w) + 1);
  //if we are printing the first number
  if (positionOfNumber == 1) {
    display.fillRect(1, first_num_pos_y, 82, 8, WHITE);
    display.display();
    display.setCursor(offset , first_num_pos_y);
  } else  if (positionOfNumber == 2) {
    display.fillRect(1, second_num_pos_y, 82, 8, WHITE);
    display.display();
    display.setCursor(offset , second_num_pos_y);
  } else if (positionOfNumber == 3) {
    display.fillRect(1, third_num_pos_y, 82, 8, WHITE);
    display.display();
    display.setCursor(offset , third_num_pos_y);
  }
  if (type == 2) {
    //print the numbres
    display.print((String)theNumber2);
    display.display();
  } else {
    //print the numbres
    display.print((String)theNumber);
    display.display();
  }
}
//this function will draw the calculator face when calculator mode is enabled
void drawCalculatorFace() {
  //here draw a full rectangle of the screen
  display.drawRect(0, 0, screen_w, screen_h, BLACK);
  //draw the top box for the logo
  display.drawRect(0, 0, screen_w, 11, BLACK);
  //set cursor and draw the logo
  display.setCursor(6, 2);
  display.print("*CALCULATOR*");
  //also draw the line above the result
  display.drawFastHLine(2, (screen_h - char_h) - 3, 79, BLACK);
}
//this will draw the snake menu face
void drawSnakeMenuFace() {
  display.setCursor(13, 0);
  display.print("SNAKE MENU");
}
//this will be called many times to get updated
void drawSnakeMenu() {
  display.setCursor(0, 8);
  display.print(" Start Game!");
  display.setCursor(0, 16);
  display.print(" Last Score");
  display.setCursor(0, 24);
  display.print(" High Score");
  display.setCursor(0, 40);
  display.print(" Diff Lvl: ");
  display.display();
  switch (snakemenuitem) {
    case 1:
      display.setCursor(0, 8);
      display.print(">Start Game!");
      break;
    case 2:
      display.setCursor(0, 16);
      display.print(">Last Score");
      break;
    case 3:
      display.setCursor(0, 24);
      display.print(">High Score");
      break;
    case 4:
      display.setCursor(0, 40);
      display.print(">Diff Lvl: ");
      break;
    default:
      break;
  }
  display.setCursor(66, 40);
  display.print(diff_lvl);
  display.display();
}
//this function will draw the menu face
void drawMenuFace() {
  display.setCursor(15, 0);
  display.print("MAIN MENU");
}
//this will be called many times to actualy draw the inners of the menu
void drawMenu() {
  display.setCursor(0, 8);
  display.print(" Bright: ");
  display.setCursor(0, 16);
  display.print(" Sound: ");
  display.setCursor(0, 24);
  display.print(" KeyBeep: ");
  display.setCursor(0, 32);
  display.print(" DefApp: ");
  display.setCursor(0, 40);
  display.print(" Reset    Save");
  display.display();
  switch (menuitem) {
    case 1:
      display.setCursor(0, 8);
      display.print(">Bright: ");
      break;
    case 2:
      display.setCursor(0, 16);
      display.print(">Sound: ");
      break;
    case 3:
      display.setCursor(0, 24);
      display.print(">KeyBeep: ");
      break;
    case 4:
      display.setCursor(0, 32);
      display.print(">DefApp: ");
      break;
    case 5:
      display.setCursor(0, 40);
      display.print(">Reset    Save");
      break;
    case 6:
      display.setCursor(0, 40);
      display.print(" Reset   >Save");
      break;
    default:
      break;
  }
  //draw all the values
  display.setCursor(66, 8);
  display.print(brightness);
  display.setCursor(66, 16);
  display.fillRect(66, 16, 18, 8, WHITE);
  display.print(sound ? "ON" : "OFF");
  display.setCursor(66, 24);
  display.fillRect(66, 24, 18, 8, WHITE);
  display.print(beep ? "ON" : "OFF");
  display.setCursor(66, 32);
  display.print(default_App);
  display.display();
}
//read if any button was pressed and set the state
void setState() {
  delay(125);
  char customKey = customKeypad.getKey();
  if (customKey == 'n') {
    state  = 1;
  } else if (customKey == '0') {
    state = 2;
  } else if (customKey == '^') {
    state = 3;
  } else if (customKey == '=') {
    state = 4;
  }
}
//set the brightnes
void setLcdBrightnes() {
  display.fillRect(66, 8, 18, 8, WHITE);
  display.display();
  analogWrite(backlight, map(brightness, 0, 100, 255, 0));
}
//this function will set all the settings
void setSettings() {
  EEPROM.write(0, brightness);
  EEPROM.write(1, sound);
  EEPROM.write(2, beep);
  EEPROM.write(3, default_App);
}
//this function will read all the settings upon boot up and set them
void readSettings() {
  brightness = EEPROM.read(0);
  sound = EEPROM.read(1);
  beep = EEPROM.read(2);
  default_App = EEPROM.read(3);
}
//function to read int form eeprom
int readIntFromEEPROM(int address) {
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);

  return (byte1 << 8) + byte2;
}
//function to store int to eeprom
void writeIntIntoEEPROM(int address, int number) {
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}
//functions to save and read score for snake
void readScores() {
  lastScore = readIntFromEEPROM(4);
  highScore = readIntFromEEPROM(6);
}
void storeScores() {
  //the last score
  writeIntIntoEEPROM(4, score);
  if (readIntFromEEPROM(6) < score) {
    writeIntIntoEEPROM(6, score);
  }
}
snake theSnake;
food theFood;
//timmer for frames
long lastFrame;
void setup() {
  //set the backlight pin output
  pinMode(backlight, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //begin the lcd display
  display.begin();
  //set rotation
  display.setRotation(2);
  //read settings from eeprom
  readSettings();
  //set the default state
  state = default_App;
  //set the backlight
  analogWrite(backlight, map(brightness, 0, 100, 255, 0));
  //set the text size
  display.setTextSize(1);
  //set this for letters to overlay
  display.setTextColor(BLACK, WHITE);
  //clear the screen buffer
  display.clearDisplay();
  display.display();
  //upon bootup check if any key was pressed
  //if yes switch the state
  setState();
  switch (state) {
    //here we will have the snake
    case 1:
      display.drawBitmap(0, 0, snake_splash, 84, 48, BLACK);
      display.display();
      delay(1500);
      display.clearDisplay();
      display.display();
      drawSnakeMenuFace();
      drawSnakeMenu();
      display.display();
      break;
    //here we will have the calculator
    case 2:
      //drawCalculator face
      drawCalculatorFace();
      //display it
      display.display();
      break;
    //here will be the settings
    case 3:
      //construct the screen and print it
      drawMenuFace();
      drawMenu();
      display.display();
      break;
    //in case 4 just some info
    case 4:
      drawInfo();
      break;
    default:
      break;
  }

}
void loop() {
  char customKey = customKeypad.getKey();
  switch (state) {
    //we are in the snake game
    case 1:
      //based on the state of the game
      switch (snakegamestate) {
        //menu
        case 1:
          if (customKey) {
            if (beep) {
              tone(buzzer, 500, 20);
            }
            //go up one menu item
            if (customKey == '8') {
              if (snakemenuitem > 1) {
                snakemenuitem--;
                drawSnakeMenu();
              }
              //go down one
            } else if (customKey == '2') {
              if (snakemenuitem < 4) {
                snakemenuitem++;
                drawSnakeMenu();
              }
            } else if (customKey == '5') {
              switch (snakemenuitem) {
                case 1:
                  theSnake = snake();
                  theFood = food();
                  score = 0;
                  display.clearDisplay();
                  theFood.newFood(theSnake.body, theSnake.snake_len);
                  randomSeed(analogRead(0));
                  display.drawRect(3, 3, 78, 42, BLACK);
                  display.display();
                  snakegamestate = 2;
                  break;
                //dispaly the last score
                case 2:
                  readScores();
                  display.setTextSize(2);
                  display.clearDisplay();
                  display.display();
                  display.setCursor(0, 0);
                  display.print("LAST");
                  display.setCursor(0, 17);
                  display.print("SCORE");
                  display.setCursor(0, 32);
                  display.print(lastScore);
                  display.setTextSize(1);
                  display.display();
                  delay(1500);
                  display.clearDisplay();
                  display.display();
                  drawMenuFace();
                  drawSnakeMenu();
                  break;
                //dispaly the last score
                case 3:
                  readScores();
                  display.setTextSize(2);
                  display.clearDisplay();
                  display.display();
                  display.setCursor(0, 0);
                  display.print("HIGH");
                  display.setCursor(0, 17);
                  display.print("SCORE");
                  display.setCursor(0, 32);
                  display.print(highScore);
                  display.display();
                  display.setTextSize(1);
                  delay(1500);
                  display.clearDisplay();
                  display.display();
                  drawMenuFace();
                  drawSnakeMenu();
                  break;
              }
              //if the 4 key pressed (left button or -)
            } else if (customKey == '4') {
              if (snakemenuitem == 4) {
                if (diff_lvl > 1) {
                  diff_lvl--;
                  snakeSpeed += 25;
                  drawSnakeMenu();
                }
              }
              //if the 6 key pressed (right button or +)
            } else if (customKey == '6') {
              if (snakemenuitem == 4) {
                if (diff_lvl < 6) {
                  diff_lvl++;
                  snakeSpeed -= 25;
                  drawSnakeMenu();
                }
              }
            }
          }
          break;
        //we are runing the game
        case 2:
          //frame timer
          if ((millis() - lastFrame) >= snakeSpeed) {
            //move snake
            theSnake.updateSnake();
            //check if game over
            if (theSnake.checkGameOver()) {
              snakegamestate = 3;
            }
            //draw the snake
            theSnake.draw();
            //draw food
            theFood.drawFood();
            //check if food eaten
            if (theSnake.snakeEatFood(theFood)) {
              if (sound) {
                tone(buzzer, 500, 20);
              }//make new food
              theFood.clearFood();
              theFood.newFood(theSnake.body, theSnake.snake_len);
            }
            //reset the timer
            lastFrame = millis();
          }
          //user ipnut for movment
          if (customKey) {
            if (customKey == '8') {
              theSnake.setDirection(0, -1);
            } else if (customKey == '2') {
              theSnake.setDirection(0, 1);
            } else if (customKey == '4') {
              theSnake.setDirection(-1, 0);
            } else if (customKey == '6') {
              theSnake.setDirection(1, 0);
            }
          }
          break;
        //game over state
        case 3:
          delay(500);
          //clear display
          display.clearDisplay();
          display.display();
          //sad tone
          if (sound) {
            tone(buzzer, 200, 300);
            delay(300);
            tone(buzzer, 250, 200);
            delay(200);
            tone(buzzer, 300, 300);
            delay(300);
          }
          //game over screen
          display.drawBitmap(0, 0, snake_game_over, 84, 48, BLACK);
          display.display();
          display.clearDisplay();
          delay(1500);
          //display score
          display.setTextSize(2);
          display.setCursor(0, 0);
          display.print(" SCORE ");
          display.setCursor(0, 17);
          display.print(score);
          display.setTextSize(1);
          display.display();
          delay(1500);
          display.clearDisplay();
          display.display();
          //save the score
          storeScores();
          drawSnakeMenuFace();
          drawSnakeMenu();
          display.display();
          snakegamestate = 1;
          break;
      }
      break;
    //we are in the calculator
    case 2:
      if (customKey) {
        if (beep) {
          tone(buzzer, 500, 20);
        }
        //if we havent pressed the result button still change numbers
        if (!result_flag) {
          int8_t theNum;
          //check over witch number is pressed
          if (customKey == '0' & first_num_len != 0) {
            if (selected_number == 1 & first_num_len < len_of_num) {
              //multiply by 10 and then add the number
              first_num = first_num * 10 + theNum ;
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //multiply by 10 and then add the number
              second_num = second_num * 10 + theNum ;
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '1') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          }
          else if (customKey == '2') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          }
          else if (customKey == '3') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '4') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '5') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '6') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '7') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '8') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == '9') {
            theNum = (int)customKey - char_offset;
            if (selected_number == 1 & first_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (first_num_len == 0) {
                //just make the number
                first_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                first_num = first_num * 10 + theNum ;
              }
              num1[first_num_len] = theNum;
              first_num_len++;
              displayNum(first_num, first_num, first_num_len, 1, 1);
            } else if (selected_number == 2 & second_num_len < len_of_num) {
              //if this is the first time pressing a number
              if (second_num_len == 0) {
                //just make the number
                second_num = theNum;
              }
              //else multiply by 10 and then add the number
              else {
                second_num = second_num * 10 + theNum ;
              }
              num2[second_num_len] = theNum;
              second_num_len++;
              displayNum(second_num, second_num, second_num_len, 2, 1);
            }
          } else if (customKey == 'c') {
            if (selected_number == 1) {
              first_num = first_num - num1[first_num_len - 1];
              if (first_num == 0) {
                first_num_len = 0;
                clearNumber(1);
              } else {
                first_num = first_num / 10;
                first_num_len--;
                displayNum(first_num, first_num, first_num_len, 1, 1);
              }

            } else if (selected_number == 2 ) {
              second_num = second_num - num1[second_num_len - 1];
              if (second_num == 0) {
                second_num_len = 0;
                clearNumber(2);
              } else {
                second_num = second_num / 10;
                second_num_len--;
                displayNum(second_num, second_num, second_num_len, 2, 1);
              }
            }
          }
        }
        //set the selected function based on the function choosen
        //and change to the second number
        //if = was pressed show result
        if (!result_flag & first_num_len > 0) {
          if (customKey == '+') {
            printFuncSymbol(1);
            selected_func = 1;
            selected_number = 2;
          } else if (customKey == '-') {
            printFuncSymbol(2);
            selected_func = 2;
            selected_number = 2;
          } else if (customKey == '/') {
            printFuncSymbol(3);
            selected_func = 3;
            selected_number = 2;
          } else if (customKey == 'x') {
            printFuncSymbol(4);
            selected_func = 4;
            selected_number = 2;
          } else if (customKey == '%') {
            printFuncSymbol(5);
            selected_func = 5;
            selected_number = 2;
          } else if (customKey == '^') {
            printFuncSymbol(6);
            selected_func = 6;
            selected_number = 2;
          } else if (customKey == 'n') {
            drawSqrt();
            selected_func = 7;
            printResult(first_num, second_num, selected_func);
            result_flag = true;

          } else if (customKey == '=') {
            printResult(first_num, second_num, selected_func);
            result_flag = true;
          }
        }
        //if we got the result
        if (result_flag) {
          // and we pressed D (delet all)
          if (customKey == 'D') {
            //clear everything
            clearAll();
          }
        }
      }
      break;
    //we are in the settings menu
    case 3:
      if (customKey) {
        if (beep) {
          tone(buzzer, 500, 20);
        }
        //go up one menu item
        if (customKey == '8') {
          if (menuitem > 1) {
            menuitem--;
            drawMenu();
          }
          //go down one
        } else if (customKey == '2') {
          if (menuitem < 6) {
            menuitem++;
            drawMenu();
          }
          //if the e key pressed (left button or -)
        } else  if (customKey == '4') {
          switch (menuitem) {
            //if we are in the brightness menu decreas brightness
            case 1:
              if (brightness > 0) {
                brightness -= 5;
                setLcdBrightnes();
                drawMenu();
              }
              break;
            //if we are in the default app change the default app
            case 4:
              if (default_App > 1) {
                default_App -= 1;
                drawMenu();
              }
              break;
          }
          //if the 6 key pressed (right button or +)
        } else  if (customKey == '6') {
          switch (menuitem) {
            //if we are in the brightness menu increas brightness
            case 1:
              if (brightness < 100) {
                brightness += 5;
                setLcdBrightnes();
                drawMenu();
              }
              break;
            //if we are in the default app change the default app
            case 4:
              if (default_App < 3) {
                default_App += 1;
                drawMenu();
              }
              break;
          }
          //if the middle "ok" button pressed
        } else if (customKey == '5') {
          switch (menuitem) {
            //sound on/off
            case 2:
              if (sound) {
                sound = false;
                drawMenu();
              } else if (!sound) {
                sound = true;
                drawMenu();
              }
              break;
            //beep on/off
            case 3:
              if (beep) {
                beep = false;
                drawMenu();
              } else if (!beep) {
                beep = true;
                drawMenu();
              }
              break;
            //reset settings
            case 5:
              brightness = 50;
              setLcdBrightnes();
              beep = false;
              sound = false;
              default_App = 1;
              drawMenu();
              break;
            //save settings
            case 6:
              setSettings();
              break;
            //failsafe
            default:
              break;
          }
        }
      }
      break;
    default:
      break;
  }
}
