/**************************************************************
  Name      LCD_LM35_Demo                                
  Author    Bob Powell 
            texanfromiowa@gmail.com
            Copyright (C) 2013, Parallelus Automation, Inc.
          
  Date      June 22, 2013   
  Modified  June 24, 2013                                
  Version   1.0.0      
  Arduino   1.0.5
  
  Notes     This is a demo of a LM35 temp sensor, with output on an LCD screen.
  
            Datasheet for the sensor used: http://www.mouser.com/ds/2/282/snis159b-186967.pdf
            
            The sensor is connected to the 5V supply and ground from the Arduino, and analog
            input pin A1.  
            
            The temperature formula used came from here:  http://playground.arduino.cc/Main/LM35HigherResolution
                     
            As the note in this article reminds us, the range in this configuration is
            from 0 to 110 degrees.
            
                        
            The LCD used: http://www.sainsmart.com/sainsmart-iic-i2c-twi-serial-2004-20x4-lcd-module-shield-for-arduino-uno-mega-r3.html
                     
            Default I2C setup used:  LCD pins  - GRD = ground
                            - VCC = 5V from Arduino
                            - SDA = Arduino A4    - This is the default pin
                            - SCL = Arduino A5    - This is the default pin
                            
                            Warning: Be sure to have an additional power supply
                                     connected to your Arduino.  The LCD requires
                                     enough power that it can overload the USB
                                     hub on your computer.
            
            
            
Legal Stuff:
============

            This program is free software: you can redistribute it and/or modify
            it under the terms of the GNU General Public License as published by
            the Free Software Foundation, either version 3 of the License, or
            at your option, any later version.
	 
            This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
            GNU General Public License for more details.
	 
            You should have received a copy of the GNU General Public License
            along with this program.  If not, see <http://www.gnu.org/licenses/>.
          
   
   
     
Personal note:
==============

            If you do something interesting with this code, expand it, or just
            have a question, please email me at the address above.  

            I hope you find this example helpful.  Enjoy.

            Bob

****************************************************************/
/***************************************************************
   The LiquidCrystal_I2C library has been modified.  The library
   used was first modified by Mike Myers on the show Lets Make It,
   (www.letsmakeit.tv).  One additional, convenience function was
   added from Mike's modifications.  The .h and .cpp files are 
   included with this file on GitHub.
****************************************************************/
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 


// Set default number of columns and rows
int COLUMNS = 20;
int ROWS = 4;
// set the LCD address to 0x27 for a 20 chars and 4 line display
// With no changes from the standard I2C setup, no extra pin 
// definitions are required.
LiquidCrystal_I2C lcd(0x3F, COLUMNS, ROWS);  

// Set the input pin for the LM35
int tempPin = A1;


/**************************************************************
Function: setup
Purpose:  set up Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino
***************************************************************/
void setup(){
  // Initialize the LCD
  lcd.init();                      // initialize the lcd 
  // Reset the LCD
  lcd.reset();                    
  // turn on the backlight   lcd.noBacklight();  will turn it off
  lcd.backlight();                
  
  // Initial screen for this demo
  initialDemoScreen();
  lcd.clear();
  analogReference(INTERNAL);
  // Used for debugging
  //Serial.begin(9600);
  //Serial.println("Started");
  
}

/**************************************************************
Function: loop
Purpose:  loop funtion for Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino, and the 
          Arduino will loop through this function indefinately.
***************************************************************/
void loop()
{
  
  // Create variables
  int loopDelay = 1000;
  int reading;
  // For a more precise display, use floats for the temperature variables,
  // or use the int for a display with whole numbers.
  //int tempinC;
  //int tempinF;
  float tempinC;
  float tempinF;
    
  // Read the input pin and calculate temps
  reading = analogRead(tempPin);
  // The LM35 is calibrated for C, so we calculate temp in C
  // Note: Since this demo using the basic setup of the sensor,
  //       0 to 110 degrees, this is the calculation needed.
  //       For setups with greater range, different calculations
  //       are needed.
  tempinC = reading / 9.31;
  // Convert C to F
  tempinF = ((tempinC*9)/5) + 32 ;
  
  // Used for debugging
  //Serial.print("reading: ");
  //Serial.println(reading);
  //Serial.print("tempinC: ");
  //Serial.println(tempinC);
  //Serial.print("tempinF: ");
  //Serial.println(tempinF);
  
  //  Verify we have a valid reading
  if( (reading <= 0) || (reading >= 307) ) {
      // Flash screen to get someones attention
      lcd.noBacklight();
      delay(100);
      lcd.backlight();
      printCentered(0, "Invalid Sensor Data");           
      printCentered(1, "Check your");           
      printCentered(2, "connection"); 
      clearRow(3);
    // Wait and start loop again
    delay(loopDelay);
    lcd.clear();
  } 
  // if the data is good, show the temp
  else {

    lcd.setCursor(0, 0);
    lcd.print("Current Temp is:   ");
    lcd.setCursor(1,1);
    lcd.print(tempinC);
    lcd.print((char)223);
    lcd.print("C or ");
    lcd.print(tempinF);
    lcd.print((char)223);
    lcd.print("F");
 
    // Check if its hot or cold, and display a message if needed
    if( tempinF >= 82){
      printCentered(2, "WOW, its HOT");       
      printCentered(3, "Turn on the A/C!");           
    }
    else if( tempinF <= 68){
      printCentered(2, "Brrrrr, its COLD");       
      printCentered(3, "Turn on the Heat");           
    }
    else{
      clearRow(2);
      clearRow(3);
    }
  
    // Wait and start loop again
    delay(loopDelay);

  }


} // end of loop


/**************************************************************
Function: initialDemoScreen
Purpose:  Initial message for this demo
Args:     none
Returns:  nothing
Notes:    
**************************************************************/
void initialDemoScreen(){
  
  lcd.clear();
  printCentered(0, "LM35 Sensor Demo");  
  printCentered(2, "As seen on:");        
  printCentered(3, "www.letsmakeit.tv");         
  delay(5000);
  
  lcd.clear();
  printLeft(0,"Code is at:");  
  printCentered(1, "www.github.com/");        
  printCentered(2, "texanfromiowa");        
  printRight(3,"or the show notes.");  
  delay(5000);

}



/**************************************************************
Function: printCentered
Purpose:  Centers text on a given row
Args:     int row - which row to display text
          String string - text to display
Returns:  nothing
Notes:    
**************************************************************/
void printCentered(int row, String string){
  //int columns = 20;
  int strLen = string.length();
  int position = (int)((COLUMNS - strLen)/2);
  clearRow(row);
  lcd.setCursor(position, row);
  lcd.print(string); 
}
 
/**************************************************************
Function: printRight
Purpose:  Right justifies text
Args:     int row - which row to display text
          String string - text to display
Returns:  nothing
Notes:    
**************************************************************/
void printRight(int row, String string){
  int strLen = string.length();
  int position = COLUMNS - strLen;
  clearRow(row);
  lcd.setCursor(position, row);
  lcd.print(string); 
}
 
/**************************************************************
Function: printLeft
Purpose:  Left justifies text
Args:     int row - which row to display text
          String string - text to display
Returns:  nothing
Notes:    
**************************************************************/
void printLeft(int row, String string){
  clearRow(row);
  lcd.setCursor(0, row);
  lcd.print(string); 
}
  
/**************************************************************
Function: clearRow
Purpose:  clears a row
Args:     int row - which row to clear
Returns:  nothing
Notes:    
**************************************************************/
void clearRow(int row){
  lcd.setCursor(0, row);
  for(int i = 0;i < COLUMNS;i++){
   lcd.print(" "); 
  }
}



