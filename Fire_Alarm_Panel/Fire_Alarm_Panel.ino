// Hello there! It seems you have come to look at my code for my custom fire alarm control panel. 
// NOTE: I wrote this code in 2018, when I was just getting started with Arduino, so some of this code is bad, incomplete, or non-functional.

// Usage:
// I do not care how you use this, as long as you do not make money off of it.

// How to make this work.
// 1. You will need an Arduino Uno, along with a lot of breadboard wires, resistors, breadboards, and patience.
// 2. All inputs must be tied to ground through a 10K resistor, to avoid the Arduino doing random things.
// 3. Hook up everything according to this pinout:

// LCD: rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2
// A0: Silence button
// A1: Reset button
// A2: Your Zone 1 input. This only supports conventional pull stations.
// 8: Your Zone 2 input. This only supports conventional pull stations. This zone is also for supervisory only.
// 7: Zone Disable button. This is an input that will disable zones as you continue to press it, and then it will wrap around and everything will be active again.
// 6: Alarm indicator LED. Activates upon alarm condition.
// A4: Alarm Output. This can go into any 5 volt activated device that doesn't draw more power than Arduino can provide.
// A5: External annunciator input. I currrently do not remember the annunciator circuit, but whenever I do remember it, I will update this.
// 9 and 10: Zone power, 9 is for zone 1, 10 is for zone 2. These will provide 5 volts DC to a pull station if it is enabled. Without this, zone disabling would not work and the panel would get stuck in an activated loop.
// A3: Pre-Alarm output. If pre-alarm duration is greater than 0 milliseconds, this will provide output during that period.

// 4. Upload this sketch to the Arduino! Everything should work, but you may have to slightly modify my code to fit your needs.

// Thank you for checking out my fire alarm panel code, and happy tinkering! If there are any problems, please feel free to let me know.
// Also, I plan to add more comments to this in the future, but I just didn't feel like diving into this code to figure out what everything does again.


#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // These are the pinouts for the LCD screen. This is optional.
int preAlert = 0; //Time Before Alarm, in milliseconds.
int pointNum = 0;
String displayLine1 = "Andrew's Room";
String displayLine2 = "System Normal";
int oldval_Z1 = LOW;
int newval_Z1;
int oldval_Z2 = LOW;
int newval_Z2;
int oldval_Silence = LOW;
int newval_Silence;
int oldval_Reset = LOW;
int newval_Reset;
int zoneDisable_Counter = 3;
int oldval_zoneDisable = LOW;
int newval_zoneDisable;
 bool strobe = false;
String serialAnnunciatorText = "";
int Access = 1;
bool NAC1 = true;
bool NAC2 = true;
int Drill_minLevel = 3;
int zoneDisable_minLevel = 2;
int annunciatorIn_level = 0;
int LTchar;
int Alarm = 0;
byte LT[8] = { // This is a custom symbol for the LCD screen that is solid black. It is used to test that your LCD is functional.
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};


LiquidCrystal Display(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  
  Display.begin(16, 2);
  Serial.begin(9600);
  pinMode(A0, INPUT); //Silence Button
  pinMode(A1, INPUT); //Reset Button
  pinMode(A2, INPUT); //Zone 1
  pinMode(8, INPUT); //Zone 2
  pinMode(7, INPUT); //Zone Disable Button
  pinMode(6, OUTPUT); //LED Notifacation
  pinMode(A4, OUTPUT); //Horns
  pinMode(13, OUTPUT); // Built-in LED pin
  pinMode(A5, INPUT); //Annunciator In
  pinMode(9, OUTPUT); //Z1 Power
  pinMode(10, OUTPUT); //Z2 Power
  pinMode(A3, OUTPUT); //Prealarm
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  Display.createChar(1, LT);
  Display.clear();

  Serial.println("Normal"); 
}

void loop() {

  // put your main code here, to run repeatedly:
  delay(200);
  int dataAddress = analogRead(A2);
  int newval_Z1 = digitalRead(A2); //Check Z1
  int newval_Z2 = digitalRead(8); //Check Z2
  int newval_Silence = digitalRead(A0); //Check Silence
  int newval_Reset = digitalRead(7);
  annunciatorIn_level = analogRead(A5);
  if (Serial.available()) {
    delay(100);
    serialAnnunciatorText = Serial.readString();
  }
  if ((serialAnnunciatorText == "NAC1 Cut") and (Access > 2)) {
    NAC1 = false;
    Serial.println("NAC1 Disabled");
  }
  if ((serialAnnunciatorText == "NAC1 On") and (Access > 2)) {
    NAC1 = true;
    Serial.println("NAC1 Enabled");
  }
    if ((serialAnnunciatorText == "NAC2 Cut") and (Access > 2)) {
    NAC2 = false;
//    Serial.println("NAC2 Disabled");
  }
  if ((serialAnnunciatorText == "NAC2 On") and (Access > 2)) {
    NAC2 = true;
    Serial.println("NAC2 Enabled");
  }
  if (serialAnnunciatorText == "Unalarm") {
    Alarm = 0;
  }

  if ((newval_Z1 == HIGH) and (oldval_Z1 == LOW )){
    Alarm = 1;
    digitalWrite(13, HIGH); //Onboard Alert LED
    Display.clear();
    Display.write("Z1: PreAlm");
    Serial.println("Zone 2: R Heat");
    zoneDisable_Counter = 1;
    digitalWrite(A3, HIGH);
    delay(preAlert); // This can be set through the variable at the top of the code.
    Display.clear();
    Display.write("Z1: Pulls");
    Display.setCursor(0, 1);
    Display.write("Silence to quiet");
    digitalWrite(A3, LOW);
//        if (NAC1) {
          digitalWrite(A4, HIGH);
        //}
//        if (NAC2) {
          digitalWrite(6, HIGH);
        }  
  
  if (newval_Z2 == HIGH) {
    Display.clear();
    Display.write("Supervisory");
    digitalWrite(A4, HIGH);
    digitalWrite(6, HIGH);
  }
  if (newval_Z2 == LOW)  {
    if (Alarm == 0) {
      Display.clear();
      Display.write("Andrew's Room");
      Display.setCursor(0, 1);
      Display.write("System Normal");
      digitalWrite(A4, LOW);
      digitalWrite(6, LOW);
    }
    if (Alarm == 1) {
      Display.clear();
      Display.write("Z1: Pulls");
      Display.setCursor(0, 1);
      Display.write("Silence to quiet");
    }
  }

      
    
  
  
  if ((newval_Silence == HIGH) && (oldval_Silence == LOW) or (serialAnnunciatorText == "Silence") or ((annunciatorIn_level > 399) and (annunciatorIn_level < 1022))){

    //digitalWrite(A3, HIGH);
    digitalWrite(A4, LOW);
    bool strobe = true;
    digitalWrite(13, HIGH); //Onboard Alert LED
    Display.clear();
    Display.write("Silenced");
    Serial.println("Silenced");
    Display.setCursor(0, 1);
    Display.write("Reset to Exit.");
     
    }
  if (serialAnnunciatorText == "5922") {
    Access = 1;
    Serial.println("Level 1: Basic Access");
  }
  if (serialAnnunciatorText == "8613") {
    Access = 2;
    Serial.println("Level 2: Mini Manager");
  }
  if (serialAnnunciatorText == "9218") {
    Access = 3;
    Serial.println("Level 3: Manager");
  }
  if (serialAnnunciatorText == "SERVICE 8675") {
    Access = 4;
    Serial.println("Level 4: SERVICE");
    delay(1000);
    Serial.println("Trouble: SERVICE");
    digitalWrite(13, HIGH);
    digitalWrite(A4, HIGH);
    Display.clear();
    Display.write("Service Mode");
  }
  if (serialAnnunciatorText == "Display Level") {
    Serial.print("Access Level: ");
    Serial.println(Access);
  }
   if ((newval_Reset == HIGH) or (serialAnnunciatorText == "Reset") or (annunciatorIn_level == 1023)){
    
    Display.clear();
    Display.write("System Reset");
    Display.setCursor(0, 1);
    Display.write("Resetting Zones"); 
    Serial.println("Resetting Zones");
    digitalWrite(A3, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    delay(2500);
    digitalWrite(A4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    Alarm = 0;


    delay(1500);
    Serial.println("Final Checks...");
    Display.clear();
    Display.write("Initalizing...");
    Display.setCursor(0, 1);
    Display.write("Please Wait.");
    delay(500);
    Display.clear();
    for (LTchar=0;LTchar<16;LTchar++) {
      Display.write(byte(1));
    }
    Display.setCursor(0, 1);
    for (LTchar=0;LTchar<16;LTchar++) {
      Display.write(byte(1));
    }
    delay(2500);

    Display.clear();
    Display.write("Andrew's Room");
    Display.setCursor(0, 1);
    Display.write("System Normal");
    Serial.println("Normal");
    zoneDisable_Counter = 3;
    }
    
    

    if ((newval_zoneDisable == HIGH) && (oldval_zoneDisable == LOW) or ((serialAnnunciatorText == "Zone Disable") and Access >= 2)){
      ++zoneDisable_Counter;
      Serial.println("Zone may be disabled. See panel for details.");
    }
    if ((newval_zoneDisable == HIGH) && (oldval_zoneDisable == LOW) or (serialAnnunciatorText == "Zone Disable") and Access == 1){
      Serial.println("ERROR: No zone disable can be issued from Level 1.");
    }
    noInterrupts();
      if (zoneDisable_Counter == 1){
      digitalWrite(9,LOW);
      digitalWrite(10,HIGH);
                                         
      }if (zoneDisable_Counter == 2){
      digitalWrite(9,HIGH);
      digitalWrite(10,LOW);
                                         
      }if (zoneDisable_Counter == 3){
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
                                         
      }if (zoneDisable_Counter == 4){
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        }if (zoneDisable_Counter == 5){
          zoneDisable_Counter = 1;
        }
        interrupts();
      
        
        if ((newval_Silence == HIGH) && (oldval_Silence == LOW) && (newval_Reset == HIGH) && (oldval_Reset == LOW)) { //Drill System
          delay(200);
          digitalWrite(9, LOW);
          digitalWrite(10, LOW);
          digitalWrite(A3, HIGH);
          digitalWrite(6, HIGH);
          digitalWrite(A4, HIGH);
          Display.clear();
          Display.write("Fire Drill On ");
          Display.setCursor(0, 1);
          Display.write("Reset to Exit.");
          Serial.println("Drill");
        }
        if ((newval_Silence == HIGH) && (oldval_Silence == LOW) && (newval_zoneDisable == HIGH) && (oldval_zoneDisable == LOW)) { //Drill System
          delay(200);
          digitalWrite(A4, HIGH);
         Display.clear();
          Display.write("NAC1 manual.");
          Display.setCursor(0, 1);
          Display.write("Reset to exit.");
          Serial.println("NAC1 Manual. Reset to Exit.");
        }
      if ((Access >=Drill_minLevel) and (serialAnnunciatorText == "Drill")) {
        Serial.println("Drill in 3 secs");
        delay(3000);
        Serial.println("Fire Drill");
        if (NAC1) {
          digitalWrite(A4, HIGH);
        }
        if (NAC2) {
          digitalWrite(6, HIGH);
        }
      }
      if ((Access == 4) and (serialAnnunciatorText == "Drill: 1")) {
        Drill_minLevel = 1;
        
      }
      if ((Access == 4) and (serialAnnunciatorText == "Drill: 2")) {
        Drill_minLevel = 2;
        
      }
      if ((Access == 4) and (serialAnnunciatorText == "Drill: 3")) {
        Drill_minLevel = 3;
        
      }
      if ((Access == 4) and (serialAnnunciatorText == "Drill: 4")) {
        Drill_minLevel = 4;
        
      }
      if ((Access == 4) and (serialAnnunciatorText == "Zone Disable: 1")) {
        zoneDisable_minLevel = 1;
      }if ((Access == 4) and (serialAnnunciatorText == "Zone Disable: 2")) {
         zoneDisable_minLevel = 2;
      }
      if ((Access == 4) and (serialAnnunciatorText == "Zone Disable: 3")) {
        zoneDisable_minLevel = 3;
      }if ((Access == 4) and (serialAnnunciatorText == "Zone Disable: 4")) {
         zoneDisable_minLevel = 4;
      }
 digitalWrite(13, HIGH);
 delay(20);
 digitalWrite(13, LOW);
      
 int oldval_Z1 = newval_Z1;
 int oldval_Z2 = newval_Z2;
 int oldval_Silence = newval_Silence;
 int oldval_Reset = newval_Reset;
 int oldval_zoneDisable = newval_zoneDisable;
 serialAnnunciatorText = "";

}
