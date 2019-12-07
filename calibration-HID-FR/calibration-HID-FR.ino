
/*
    calibration-HID-FR.ino (azerty keyboard) calibrate the HID keyvalues of the keyboard
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

/* 
    compile and upload this ino to your keyboard
    open KeyValue.ino in your favorite IDE
    and press the keys of your keyboard
    //004 q 004
    004 is the HID value
    q is the letter "pressed"

    if you don't want to assign an HID value just press the first key = q    
*/
#include <KeyboardHID.h>

const int numCols = 10;
const int numRows = 10;
const int colPins[numCols] = {0, 1, 2, 3, 4, 5, 6, 7, 11, 13};
const int rowPins[numRows] = {A0, A1, A2, A3, A4, A5, A8, A9, A10, A11};

//                     0   1   2   3   4   5   6   7   8   9
const int toto[10] = {98, 89, 90, 91, 92, 93, 94, 95, 96, 97};

int n1;
int n2;
int n3;

int key;
int sensorValue;

int calibr = 4;

int first = 0;
int firstKey = 999;
int firstColumn = 999;
int firstRow = 999;


void setup() {
  KeyboardHID.begin();

  Serial.begin(9600);

  for (int column = 0; column < numCols; column++)
  {
    pinMode(colPins[column], OUTPUT);
    digitalWrite(colPins[column], LOW);
  }

  for (int row = 0; row < numRows; row++)
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
      delay(1);      // delay in between reads for stability
      sensorValue = analogRead(rowPins[row]);

      key = 0;

      if (sensorValue < 104)
      {
        key = 0;
      }
      else if (sensorValue < 271)
      {
        key = 1;
      }
      else if (sensorValue < 371)
      {
        key = 2;
      }
      else if (sensorValue < 715)
      {
        key = 3;
      }


      if (key > 0)
      {

        n1 = calibr / 100;
        n2 = (calibr - (100 * n1)) / 10;
        n3 = (calibr - (100 * n1) - (10 * n2 ));

        KeyboardHID.write(84);
        KeyboardHID.write(84);

        KeyboardHID.write(toto[n1]);
        KeyboardHID.write(toto[n2]);
        KeyboardHID.write(toto[n3]);

        KeyboardHID.write(44);

        KeyboardHID.write(calibr);

        KeyboardHID.write(44);


        KeyboardHID.write(toto[n1]);
        KeyboardHID.write(toto[n2]);
        KeyboardHID.write(toto[n3]);

        KeyboardHID.write(40); // new line

        if ( ((key - 1) == firstKey) && (column == firstColumn) && (row == firstRow))
        {
          key = 0;
        }
        else
        {

          KeyboardHID.write(14);
          KeyboardHID.write(8);
          KeyboardHID.write(28);

          KeyboardHID.press(KEY_LEFT_SHIFT);
          KeyboardHID.write(25);
          KeyboardHID.release(KEY_LEFT_SHIFT);
          KeyboardHID.write(20);
          KeyboardHID.write(15);
          KeyboardHID.write(24);
          KeyboardHID.write(8);

          KeyboardHID.press(KEY_LEFT_ALT);
          KeyboardHID.write(34);
          KeyboardHID.release(KEY_LEFT_ALT);

          KeyboardHID.write(toto[key - 1]);

          KeyboardHID.press(KEY_LEFT_ALT);
          KeyboardHID.write(45);
          KeyboardHID.release(KEY_LEFT_ALT);

          KeyboardHID.press(KEY_LEFT_ALT);
          KeyboardHID.write(34);
          KeyboardHID.release(KEY_LEFT_ALT);

          KeyboardHID.write(toto[column]);

          KeyboardHID.press(KEY_LEFT_ALT);
          KeyboardHID.write(45);
          KeyboardHID.release(KEY_LEFT_ALT);

          KeyboardHID.press(KEY_LEFT_ALT);
          KeyboardHID.write(34);
          KeyboardHID.release(KEY_LEFT_ALT);

          KeyboardHID.write(toto[row]);

          KeyboardHID.press(KEY_LEFT_ALT);
          KeyboardHID.write(45);
          KeyboardHID.release(KEY_LEFT_ALT);


          KeyboardHID.write(44);
          KeyboardHID.write(46);
          KeyboardHID.write(44);

          KeyboardHID.write(toto[n1]);
          KeyboardHID.write(toto[n2]);
          KeyboardHID.write(toto[n3]);

          KeyboardHID.write(54);
          KeyboardHID.write(40); // new line
          KeyboardHID.write(40); // new line
        }

        calibr++;

        if (first == 0)
        {
          first = 1;
          firstKey = (key - 1);
          firstColumn = (column);
          firstRow = (row);
        }

      }
    }

    digitalWrite(colPins[column], LOW);

  }
}
