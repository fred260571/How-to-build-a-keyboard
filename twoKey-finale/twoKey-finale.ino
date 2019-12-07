/*

    twoKey.ino Key multiplication for Arduino : allows you to have two keys instead of one
    Copyright (C) 2019  Frederic Pouchal.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

/* Allows Arduino micro to control 200 keys instead of 100 = (10*10)
   Can easily be modified for standard Arduino and Raspberry Pi
*/

#include "KeyboardHID.h"

const int numCols = 10;
const int numRows = 10;

const int colPins[numCols] = {0, 1, 2, 3, 4, 5, 6, 7, 11, 13};
const int rowPins[numRows] = {A0, A1, A2, A3, A4, A5, A8, A9, A10, A11};

char keyState[2][10][10];
int keyValue[2][10][10];
void (*Press[2][10][10])();
void (*Release[2][10][10])();
int key;
int sensorValue;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < 10; k++)
      {
        keyState[i][j][k] = 0;
      }
    }
  }

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < 10; k++)
      {
        keyValue[i][j][k] = 0;
      }
    }
  }

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < 10; k++)
      {
        Press[i][j][k] = nada;
      }
    }
  }

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < 10; k++)
      {
        Release[i][j][k] = nada;
      }
    }
  }


  keyValue[0][5][6] = KEY_LEFT_SHIFT;
  //keyValue[0][5][6] = 225;

  keyValue[1][5][6] = 4; //a

  keyValue[0][0][5] = 0;
  Press[0][0][5] = P005;
  Release[0][0][5] = R005;

  keyValue[1][0][5] = 0;
  Press[1][0][5] = P105;
  Release[1][0][5] = R105;

  for (int column = 0; column < numCols; column++)
  {
    pinMode(colPins[column], OUTPUT);
    digitalWrite(colPins[column], LOW);
  }
    
  for (row = 0; row < numRows; row++)
  {
    pinMode(rowPins[row], INPUT);
  }
        
}

void loop() {

  int row;
  int column;

  for (column = 0; column < numCols; column++)
  {
    digitalWrite(colPins[column], HIGH);

    for ( row = 0; row < numRows; row++)
    {
      delay(1);  // delay in between reads for stability
      sensorValue = analogRead(rowPins[row]);

      key = 0;

      if (sensorValue < 104)
      {
        key = 0;
        if (keyState[0][column][row] == 1)
        {
          keyState[0][column][row] = 0;
          if (keyValue[0][column][row])
          {
            KeyboardHID.release(keyValue[0][column][row]);
            //key1 release
          }
          else
          {
            Release[0][column][row]();
          }
        }
        if (keyState[1][column][row] == 1)
        {
          keyState[1][column][row] = 0;
          if (keyValue[1][column][row])
          {
            KeyboardHID.release(keyValue[1][column][row]);
            //key2 release
          }
          else
          {
            Release[1][column][row]();
          }
        }
      }

      else if (sensorValue < 271)
      {
        key = 1;
        if (keyState[0][column][row] == 0)
        {
          keyState[0][column][row] = 1;
          if (keyValue[0][column][row])
          {
            KeyboardHID.press(keyValue[0][column][row]);
            //key1 press
          }
          else
          {
            Press[0][column][row]();
          }

        }
        if (keyState[1][column][row] == 1)
        {
          keyState[1][column][row] = 0;
          if (keyValue[1][column][row])
          {
            KeyboardHID.release(keyValue[1][column][row]);
            //key2 release
          }
          else
          {
            Release[1][column][row]();
          }
        }
      }

      else if (sensorValue < 371)
      {
        key = 2;
        if (keyState[0][column][row] == 1)
        {
          keyState[0][column][row] = 0;
          if (keyValue[0][column][row])
          {
            KeyboardHID.release(keyValue[0][column][row]);
            //key1 release
          }
          else
          {
            Release[0][column][row]();
          }
        }
        if (keyState[1][column][row] == 0)
        {
          keyState[1][column][row] = 1;
          if (keyValue[1][column][row])
          {
            KeyboardHID.press(keyValue[1][column][row]);
            //key2 press
          }
          else
          {
            Press[1][column][row]();
          }
        }
      }

      else
      {
        key = 3;
        if (keyState[0][column][row] == 0)
        {
          keyState[0][column][row] = 1;
          if (keyValue[0][column][row])
          {
            KeyboardHID.press(keyValue[0][column][row]);
            //key1 press
          }
          else
          {
            Press[0][column][row]();
          }
        }
        if (keyState[1][column][row] == 0)
        {
          keyState[1][column][row] = 1;
          if (keyValue[1][column][row])
          {
            KeyboardHID.press(keyValue[1][column][row]);
            //key2 press
          }   else
          {
            Press[1][column][row]();
          }
        }
      }

      if (key > 0)
      {
        Serial.print(key - 1);
        Serial.print(column);
        Serial.println(row);
      }
    }

    digitalWrite(colPins[column], LOW);

  }
}

void P005() {
  if (keyState[0][5][6] == 0)
  {
    KeyboardHID.press(40);
    KeyboardHID.press(11);
    KeyboardHID.press(8);
    KeyboardHID.write(15);
    KeyboardHID.write(15);
    KeyboardHID.press(18);
    KeyboardHID.press(16);
  }
  else
  {
    keyState[0][5][6] = 0;
    KeyboardHID.release(keyValue[0][5][6]);
    KeyboardHID.press(44);
    KeyboardHID.press(29);
    KeyboardHID.write(18);
    KeyboardHID.write(21);
    KeyboardHID.press(15);
    KeyboardHID.press(7);
    KeyboardHID.press(37);
    KeyboardHID.press(40);
  }
}

void R005() {
  if (keyState[0][5][6] == 0)
  {
    KeyboardHID.release(40);
    KeyboardHID.release(11);
    KeyboardHID.release(8);
    KeyboardHID.release(18);
    KeyboardHID.release(16);
  }
  else
  {
    keyState[0][5][6] = 0;
    KeyboardHID.release(keyValue[0][5][6]);
    KeyboardHID.release(44);
    KeyboardHID.release(29);
    KeyboardHID.release(15);
    KeyboardHID.release(7);
    KeyboardHID.release(37);
    KeyboardHID.release(40);
  }
}

void P105() {  // CTRL-ALT-DEL Keyboard logout
  KeyboardHID.press(KEY_LEFT_CTRL);
  KeyboardHID.press(KEY_LEFT_ALT);
  KeyboardHID.press(KEY_DELETE);
}

void R105() {
  KeyboardHID.release(KEY_LEFT_CTRL);
  KeyboardHID.release(KEY_LEFT_ALT);
  KeyboardHID.release(KEY_DELETE);
}

void nada()
{

}
