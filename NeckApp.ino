//import ax12 library to send DYNAMIXEL commands
#include <ax12.h>

int s = 6;
int CENTER = 512;
int engine1 = CENTER;
int engine2 = CENTER;

void setup() {
   Serial.begin(9600);
   center();
}

void loop() {
   if (Serial.available() == 0) {
      SetPosition(1, engine1);
      SetPosition(2, engine2);
      delay(10);
      return;
   }
   char c = Serial.read();
   if (c == '\0')
      return;
   if (c == 'n' || c == 'N') {
      nod(3, (c == 'N' ? 55 : 13), (c == 'N' ? 54 : 13));
   } else if (c == 's' || c == 'S') {
      shake(3, (c == 'S' ? 55 : 13), (c == 'S' ? 54 : 13));
   } else if (c == 'c') {
      center();
   } else if (c == 'r' || c == 'R') {
      engine1 = down_right(1, c == 'R' ? 54 : 13);
   } else if (c == 'l' || c == 'L') {
      engine1 = up_left(1, c == 'L' ?  54 : 13);
   } else if (c == 'u' || c == 'U') {
      engine2 = up_left(2, c == 'U' ? 54 : 13);
   } else if (c == 'd' || c == 'D') {
      engine2 = down_right(2, c == 'D' ? 54 : 13);
   } else if (c == 'e' || c == 'E') {
      ne(c == 'E' ? 54 : 13);
   } else if (c == 'q' || c == 'Q') {
      nw(c == 'Q' ? 54 : 13);
   } else if (c == 'x' || c == 'X') {
      se(c == 'X' ? 54 : 13);
   } else if (c == 'z' || c == 'Z') {
      sw(c == 'Z' ? 54 : 13);
   } else if (c == 'h') {
      char code[] = {
         'c',
         'r',
         'l',
         'u',
         'd',
         'R',
         'L',
         'U',
         'D',
         'n',
         'N',
         's',
         'S',
         'e',
         'E',
         'q',
         'Q',
         'x',
         'X',
         'z',
         'Z'
      };
      char * symbol[] = {
         "olarr",
         "rarr",
         "larr",
         "uarr",
         "darr",
         "rrarr",
         "llarr",
         "uuarr",
         "ddarr",
         "varr",
         "udarr",
         "harr",
         "lrarr",
         "nearr",
         "neArr",
         "nwarr",
         "nwArr",
         "searr",
         "seArr",
         "swarr",
         "swArr"
      };
      String codes = "[";
      for (int i = 0; i < sizeof(code) - 1; i++) {
         codes = codes + "{\"code\":\"" + code[i] + "\",\"symbol\":\"&" + symbol[i] + ";\"},";
      }
      int j = sizeof(code) - 1;
      codes = codes + "{\"code\":\"" + code[j] + "\",\"symbol\":\"&" + symbol[j] + ";\"}]";
      Serial.println(codes);
   }
}

// 1 step = 0.29º
int up_left(int engine, int top) {
   int j = (engine == 1 ? engine1 : engine2);
   top = top + j;
   while (j <= top) {
      j++;
      SetPosition(engine, j);
      delay(s);
   }
   return j;
}

int down_right(int engine, int top) {
   int j = (engine == 1 ? engine1 : engine2);
   top = j - top;
   while (j != top) {
      j--;
      SetPosition(engine, j);
      delay(s);
   }
   return j;
}

void center() {
   while (engine1 != CENTER || engine2 != CENTER) {
      if (engine1 != CENTER) {
         if (engine1 > CENTER) {
            engine1--;
         } else {
            engine1++;
         }
         SetPosition(1, engine1);
      }
      if (engine2 != CENTER) {
         if (engine2 > CENTER) {
            engine2--;
         } else {
            engine2++;
         }
         SetPosition(2, engine2);
      }
      delay(s);
   }
}

void nod(int t, int top, int down) {
   int temp = engine2;
   engine2 = up_left(2, top);
   for (int i = 0; i < t; i++) {
      engine2 = down_right(2, down * 2);
      engine2 = up_left(2, top * 2);
   }
   engine2 = down_right(2, down);
}

void shake(int t, int top, int down) {
   int temp = engine1;
   engine1 = down_right(1, down);
   for (int i = 0; i < t; i++) {
      engine1 = up_left(1, top * 2);
      engine1 = down_right(1, down * 2);
   }
   engine1 = up_left(1, top);
}

void ne(int steps) {
   for (int i = 0; i < steps; i++) {
      engine1 = down_right(1, 1);
      engine2 = up_left(2, 1);
   }
}

void nw(int steps) {
   for (int i = 0; i < steps; i++) {
      engine1 = up_left(1, 1);
      engine2 = up_left(2, 1);
   }
}

void se(int steps) {
   for (int i = 0; i < steps; i++) {
      engine1 = down_right(1, 1);
      engine2 = down_right(2, 1);
   }
}

void sw(int steps) {
   for (int i = 0; i < steps; i++) {
      engine2 = down_right(2, 1);
      engine1 = up_left(1, 1);
   }
}
