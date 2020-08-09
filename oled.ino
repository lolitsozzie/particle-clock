// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_SSD1306.h>

#include <string.h>

#define OLED_RESET D4
#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
Adafruit_SSD1306 oled(OLED_RESET);

String strData;
String sEconds;
String mInutes;
String message;
String amPM;
String scroll = "";
int red_light_pin= 4;
int green_light_pin = 5;
int blue_light_pin = 6;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int customColor = 0;
int counter = 0;
int seconds;
int minutes;
bool run;
bool tick;
bool colorChange;

void setup() {
    Particle.function("timerDisplay", displayMessage);
    Particle.function("setRedValue", setRedValue);
    Particle.function("setGreenValue", setGreenValue);
    Particle.function("setBlueValue", setBlueValue);
    Particle.function("setCustomColor", setCustomColor);
    Particle.variable("redValue", &redValue, INT);
    Particle.variable("greenValue", &greenValue, INT);
    Particle.variable("blueValue", &blueValue, INT);
    Particle.variable("customColor", &customColor, INT);
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    oled.clearDisplay();
    oled.display();
    dataDisplay("");
    pinMode(red_light_pin, OUTPUT);
    pinMode(green_light_pin, OUTPUT);
    pinMode(blue_light_pin, OUTPUT);
    message = "ozzie=cool";
    Time.zone(-5);
    Serial.begin(9600);
    minutes = 0;
    seconds = 0;
    run = true;
}

void loop() {
  WiFiSignal sig = WiFi.RSSI();
  if(run){
      delay(200);
      if(colorChange){
          redValue += random(0, 35);
          greenValue += random(0, 35);
          blueValue += random(0, 35);
      }
      if(redValue > 255){
          redValue = 0;
      }
      if(greenValue > 255){
          greenValue = 0;
      }
      if(blueValue > 255){
          blueValue = 0;
      }
      
      

      Particle.syncTime();
      if(customColor == 0){
          if(amPM == "am"){
          RGB_color(255, 255, 51);
          }else{
          RGB_color(255, 0, 255);
          }
      }else{
          RGB_color(redValue, greenValue, blueValue);
      }
      if(minutes != Time.minute()){
          minutes = Time.minute();
      }
      if(seconds != Time.second()){
          seconds = Time.second();
      }
      
      if(seconds < 10){
          sEconds = "0" + String(seconds);
      }else{
          sEconds = String(seconds);
      }
      if(minutes < 10){
          mInutes = "0" + String(minutes);
      }else{
          mInutes = String(minutes);
      }
      if(Time.hour() > 12){
          amPM = "pm";
      }else{
          amPM = "am";
      }
      
      message = ("WiFi: " + String(int(sig.getQuality())) + "%");
      dataDisplay(String((Time.hour() + 1) % 12) + ":" + mInutes + ":" + sEconds + amPM);
    }if(!run){
        oled.clearDisplay();
        oled.display();
    }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
    
int setRedValue(String command){
    redValue = command.toInt();
    return redValue;
}

int setGreenValue(String command){
    greenValue = command.toInt();
    return greenValue;
}

int setBlueValue(String command){
    blueValue = command.toInt();
    return blueValue;
}


int dataDisplay(String command) {
    if(run){
        strData = String(command);
        oled.clearDisplay();
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(0,0);   
        oled.print(String(Time.month()) + "/" + String(Time.day()) + "/" + String(Time.year()).substring(2));
        oled.setCursor(0,25);
        oled.print(strData);
        oled.setTextSize(2);
        oled.setCursor(0,46);
        if(tick){
            oled.print(scroll);
        }else{
            oled.print(message);
        }
        oled.print(scroll);
        oled.setTextColor(BLACK, WHITE);
        oled.display();
    }
}

int displayMessage(String command){
    if(command == "tick"){
        if(tick){
            tick = false;
            return 0;
        }else{
            tick = true;
            return 2;
        }
    }if(command == "***"){
        run = !run;
    }
    message = command;
    return 1;
}

int setCustomColor(String command){
    customColor = command.toInt();
    return customColor;
}

