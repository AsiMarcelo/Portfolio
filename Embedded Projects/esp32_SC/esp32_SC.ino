//DOIT ESP32 DEVKIT V1
// Sketch for Soil Conductance
//Okayna: LCD, Buzzer, Sensor Calib ñ 


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// defining constant values such as the max voltage output of the YL69 sensor, buzzer pin, fixed resistor and sensor input pin
#define sens_pin 27
#define R_FIXED 10000.0  //Resistorin the YL38 module
#define V_REF 3.3 // 3.3 because it is powered by the esp32
#define buzz 2

LiquidCrystal_I2C lcd(0x27, 20, 4); //establishing dimensions of the LCD and address so the ESP32 can see it

float volt; //voltage across the yl69
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Wire.begin(21, 22); 
  Serial.begin(9600);

  pinMode(sens_pin, INPUT); //pinMode set to input mode
  pinMode(buzz, OUTPUT); //set to output 

}

void loop() {
  lcd.setCursor(0,0);
  lcd.clear();// clearing lcd to ensure no abnormal characters or text overlay one another
  int raw_adc = analogRead(sens_pin); // raw values of the sensor
  //Serial.println(raw_adc);
  volt = raw_adc * (V_REF / 4095.0); //the voltage output of the probe, the higher it is means the electricity go through the probes <nextLine>
  //and not through the soil

//0.8-2.5 optimal range according to the table
  
  lcd.print("Voltage: "); lcd.print(volt); lcd.print("V | "); //showing the voltage
  delay(3500);
  //the following if-else statements are based on the values of dS/m  
  if (volt <= 0.01) {
  lcd.clear();
  lcd.print("Soil is very wet");
  tone(buzz, 10, 300);
  delay(4000);
  
  } 
  else if (volt >= 3.29){
    lcd.clear();
    lcd.print("Conductivity is less than 1.5 dS/m");
    tone(buzz, 900, 3000);
    delay(4000);
  }
  else{
  lcd.clear();
  // the formula uses the 10k resistor fixed on the YL38 board (marked with 103) to serve as the voltage divider </nextLine>
  // making it so that the esp32 can read the raw values from the sensor 
  // it uses ohm's law to calculate the resistance of the soil 
  float conductance = (1000.0 / R_FIXED) * ((3.3 - volt) / volt) * 10;
  //calculating the conductance from the output voltage
  //1000 / R_FIXED scaling factor to normalize the resistor value and prevent ridiculously high and low values
  //*10 is the convertion factor from Siemens to turn it back to decisiemens
  // (3.3-volt) /volt relationship between the soil's resistance and 10k resistor
  lcd.setCursor(0, 1);
  lcd.print("Conductance:");
  lcd.setCursor(0, 2);
  lcd.print(conductance, 4);
  lcd.print(" dS/m");
  //Below are varying buzzer frequencies and duration to showcase urgency and state of the soil 
  if (conductance < 1.5){
    for(int r =0; r < 10; r++){
      tone(buzz, 20, 100);
      delay(500);
      tone(buzz, 20, 100);     
      delay(500);
    }
  }
  else if( conductance < 3.0 && conductance > 1.5){
    for(int r =0; r < 10; r++){
      tone(buzz, 80, 100);
      delay(500);
      tone(buzz, 140, 100);
      delay(500);
    }
  }
    else if( conductance < 4.0 && conductance >= 3.0){
    for(int r =0; r < 10; r++){
      tone(buzz, 100, 50);
      delay(500);
      tone(buzz, 200, 50);
      delay(500);
    }
  }
    else if( conductance < 8.0 && conductance >= 4.0){
    for(int r =0; r < 10; r++){
      tone(buzz, 220, 1000);
      delay(1000);
      tone(buzz, 300, 1000);
      delay(1000);
    }
  }
  else if( conductance > 8.0){
    for(int r =0; r < 10; r++){
      tone(buzz, 400, 2000);
      delay(3000);
      tone(buzz, 600, 2000);
      delay(3000);
    }
  }
  else{
    digitalWrite(buzz, HIGH);
    delay(5000);
    digitalWrite(buzz,LOW);
  }
  delay(4000);
  }

  delay(50);


}
