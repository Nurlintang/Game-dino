// C++ code
//

#include<LiquidCrystal.h>

//definisikan pin untuk LCD
LiquidCrystal lcd(2, 3, 5, 6, 7, 8);

//membuat karakter T-Rex
byte trex[8]
{ 
  B00000,
  B00111,
  B10101,
  B10111,
  B11100,
  B11111,
  B01101,
  B01100,
};

//membuat karakter pohon
byte pohon[8]
{ 
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B01110,
};

//inisialisasi variabel
const int Button_Enter = 11;
const int Button_Select = 12;
const int Menu_Size = 2;
const int LCD_Column = 16;
const int Pohon_Char = 6;
const int Trex_Char = 7;

boolean isPlaying = false;
boolean isShowScore = false;
boolean isTrexOnGround = true;

int currentIndexMenu = 0;
int score = 0;
int scoreListSize = 0;
String scoreList[20];

void setup() {
  lcd.begin(16,2);
  lcd.createChar(Trex_Char, trex);
  lcd.createChar(Pohon_Char, pohon);
  Serial.begin(9600);
  pinMode(Button_Enter, INPUT_PULLUP);
  pinMode(Button_Select, INPUT_PULLUP);
}

void loop() {
  lcd.clear();
  handleMenu();
  delay(300);
}

void handleMenu() {
  String menu[Menu_Size] = { "START", "SCORE" };
  for (int i=0; i < Menu_Size; i++) {
    if (i == currentIndexMenu) {
      lcd.setCursor(0, i);
      lcd.print("-> ");
    }
    lcd.setCursor(3, i);
    lcd.print(menu[i]);
  }
  
  if (digitalRead(Button_Select) == LOW) {
    currentIndexMenu = currentIndexMenu == 0 ? : 0;
  }
  
  if (digitalRead(Button_Enter) == LOW) {
    currentIndexMenu == 0 ? startGame() : showScore();
  }
}

void showScore() {
  isShowScore = true;
  delay(200);
  
  int currentIndex = 0;
  const int lastIndex = scoreListSize - 1;
  
  printScore(currentIndex, lastIndex);
  
  while (isShowScore) {
    if (digitalRead(Button_Select) == LOW) {
      currentIndex = currentIndex < lastIndex ? currentIndex + 1 : 0;
      printScore(currentIndex, lastIndex);
    }
    
    if (digitalRead(Button_Enter) == LOW) {
      isShowScore = false;
    }
    delay(200);
  }
}

void printScore(int index, int lastIndex) {
  lcd.clear();
  if (lastIndex == -1) {
    lcd.print("NO SCORE");
  }
  else {
    lcd.print(scoreList[index]);
    if (index < lastIndex) {
      lcd.setCursor(0,1);
      lcd.print(scoreList[index + 1]);
    }
  }
}

void startGame() {
  isPlaying = true;
  
  while (isPlaying) {
    handleGame();
  }
}

void handleGame() {
  lcd.clear();
  
  int buttonPressedTimes = 0;
  
  //inisialisasi jarak random antara satu pohon dengan pohon lainnya
  int secondPosition = random (4,9);
  int thirdPosition = random (4,9);
  int firstPohonPosition = LCD_Column;
  
  const int columnValueToStopMovePohon = -(secondPosition + thirdPosition);
  
  //loop untuk membuat pohon bergerak
  //loop akan berjalan sampai semua pohon bergerak
  for (; firstPohonPosition >= columnValueToStopMovePohon; firstPohonPosition--) {
    
    lcd.setCursor(13, 0);
    lcd.print(score);
    
    defineTrexPosition();
    
    int secondPohonPosition = firstPohonPosition + secondPosition;
    int thirdPohonPosition = secondPohonPosition + thirdPosition;
    
    showPohon(firstPohonPosition);
    showPohon(secondPohonPosition);
    showPohon(thirdPohonPosition);
    
    if (isTrexOnGround) {
      if (firstPohonPosition == 1 || secondPohonPosition == 1 || thirdPohonPosition == 1) {
        handleGameOver();
        delay(500);
        break;
      }
      buttonPressedTimes = 0;
      
    } else {
      if (buttonPressedTimes > 3) {
        score-=3;
      }
      buttonPressedTimes++;
    }
    
    score++;
    delay(500);
  }
}

void handleGameOver() {
  lcd.clear();
  lcd.print("GAME OVER");
  
  lcd.setCursor(0,1);
  lcd.print("SCORE: ");
  lcd.print(score);
  
  delay(2000);
  saveScore();
}

void saveScore() {
  lcd.clear();
  
  scoreList[scoreListSize] = score;
  scoreListSize++;
  
  isPlaying = false;
  score = 0;
}

void showPohon (int position) {
  lcd.setCursor(position, 1);
  lcd.write(Pohon_Char);
  
  //membersihkan posisi sebelumnya
  lcd.setCursor(position + 1, 1);
  lcd.print(" ");
}

void defineTrexPosition() {
  int buttonState = digitalRead(Button_Enter);
  buttonState == HIGH ? putTrexOnGround() : putTrexOnAir();
}

void putTrexOnGround() {
  lcd.setCursor(1,1);
  lcd.write(Trex_Char);
  lcd.setCursor(1,0);
  lcd.print(" ");
  
  isTrexOnGround = true;
}

void putTrexOnAir() {
  lcd.setCursor(1,0);
  lcd.write(Trex_Char);
  lcd.setCursor(1,1);
  lcd.print(" ");
  
  isTrexOnGround = false;
}