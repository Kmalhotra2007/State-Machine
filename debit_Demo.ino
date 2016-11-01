/*
   FILE          : debit_Demo
   PROJECT       : PROG8125 - ASSIGNMENT #4
   PROGRAMMER    : Kunal Malhotra
   FIRST VERSION : 2016-07-13
   DESCRIPTION   : Demonstrates a debit machine banking transaction that implements a state machine. We take input from th serial ports and accordingly display the result .
                   We even ave a timer for case 2 which counts till 10 and by that time if no buttn is pressed, it changes the state to case 1.
                   Cancel state for each case has been impemented, for example if someone wants to cancel a state he can press the Cancel Button and go to stage 1.
                   The code accepts User inputs as the amount entered and swiches states depending on the input provided by the users .
                   The function DisplayWelcome() diplays welcome and asks what amount do you want to withdraw from the machine.
                   The function DisplayAmount(float amount) shows the amount you entered through Serial port on the serial port and LCD.
                   The function CheckIfAmountRecd() checks if any amount was entered otherwise returns a value.
                   The function CheckOkOrCancel() checks if Ok or Cancel button was pressed by the user on the bredboard
                   The function DisplayOkCancel() displays ok or cancel for one second on the LCD.
                   The function DisplayInputPassword() it asks the user the enter the password and displays it on the Serial Port.
                   The function DisplayBankAcceptance() it asks the Bank if it is accepting the transaction or not.
                   The function PasswordCheck() this checks if the password is entered, and if it is entered, is it correct or not.
                   The function CorrectPassword() It displays that the password is correct on the LCD
                   The function WrongPassword() It displays that the password entered is wrong and takes us the the first case of the Switch statement.
                   The function CForward() Displays that the user selected Chequing Account, displays chequing on the LCD and takes the Program to the next Transaction State.
                   The function SForwards() Displays that the user entered Savings Account,displays savings on the LCD and takes the Program to the next Transaction State.
                   The function MillisTimer() It counts if 10 secs have passed before the user presses Ok or cancel withing 10 secs or not, if he doesnt press the Button in 10 secs
                   the the state shifts to state 1.
                   The function voidsetup() consists of the initialisation of all the pins, LCD and serial port.
                   The function voidloop() consists of different switch statements and uses he above mentioned function to work the state machine properly.
                   The Pin Number connection from LCD to Teensy are as follows-
                    LCD RS pin to digital pin 12
                    LCD Enable pin to digital pin 11
                    LCD D4 pin to digital pin 5
                    LCD D5 pin to digital pin 4
                    LCD D6 pin to digital pin 3
                    LCD D7 pin to digital pin 2
                    LCD R/W pin to ground
                    LCD VSS pin to ground
                    LCD VCC pin to 5V
                    10K resistor:
                    ends to +5V and ground
                    wiper to LCD VO pin (pin 3)
*/

#include <LiquidCrystal.h>                                                                          //Header file including the library of LCD
#include <Bounce.h>                                                                                 //Header file including the Bounce Type Library
#define DISPLAYOKCANCEL 0                                                                           //
#define DISPLAYANDENTERAMOUNT 1                                                                     //
#define CHECKAMOUNTRECEIVE 2                                                                        //
#define OKCANCELTIMER 3                                                                             //Defining different cases used in the switch State
#define SELECTCHQORSAV 4                                                                            //
#define PASSWORDCHECK 5                                                                             //
#define BANKACCEPTCANCEL 6                                                                          //

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //create an lcd object                                       //Declaring pin numbers for LCD

Bounce ChequingPb(15, 10);                                                                          //
Bounce SavingsPb(14, 10);                                                                           //Defining the switch buttons as bounce time along with a stable time of 10 
Bounce CancelPb(16, 10);                                                                            //milliseconds at the end . this is done to avoid bouncing of the buttons
Bounce OkPb(17, 10);                                                                                //
int32_t StoredPassword = 1111;                                                                      // Correct password stored in this variable. ther passwords would be rejected
int16_t CorrectPasswordDelay = 500 ;                                                                //
int16_t WrongPasswordDelay = 500 ;                                                                  //Different Delay times uses all over the program, the variable name tell us
int16_t CForwardDelay = 1000;                                                                       //where the specific delays are used, for eg- SerialDeay is the time considered
int16_t SForwardDelay = 1000;                                                                       //to initialise the serial port,CorrectPassword & Wrong Password Delay are the 
int16_t BankNameDelay = 3000;                                                                       //time used to display correct and Wrong Password on the LCD for that particular time
int16_t SerialDelay = 2000;                                                                         //
int16_t CaseDelay = 1000;                                                                           //
int32_t AccountBlockDelay = 1500;                                                                   //                                                                        
int64_t SerialPortBaudRate=9600;                                                                    //Baud Rate for the Serial Port
int32_t TimerTime = 10000;                                                                          // Time limit before which the user has to press OK or Cancel Button
int8_t  InitialAmountValue = 0.0 ;                                                                  //The amount to be entered is initialised as 0
int8_t InitialTransactionState = 1 ;                                                                // The swtich statements should always start with transactionstate as 1
int8_t InitialPushButtonState = 0 ;                                                                 //Initial Push Button State is defined as 0
int8_t i = 0 ;                                                                                      //Initial State of flag set as 0
uint8_t NoAmountReceived = -1;                                                                      //
uint8_t CancelPressed = 3 ;                                                                         //
uint8_t OKPressed = 4;                                                                              //
uint8_t NoPasswordYet = 9;                                                                          //Different Return States for different Function throught 
uint8_t TimeOut = 1;                                                                                //the Program
uint8_t TimeLeft = 0;                                                                               //
uint8_t NothingPressed = 0 ;                                                                        //
uint8_t MaximumPinAttempts = 3;                                                                     //
uint8_t InitialPinFlagState = 0;                                                                    //
int16_t InitialPreviousMillisState = 0;                                                             //The initial Value of Previous Millis which is a static value
void DisplayWelcome();                                                                              //
void DisplayAmount(float amount);                                                                   //
float CheckIfAmountRecd();                                                                          //
int8_t CheckOkOrCancel();                                                                           //
void DisplayOkCancel();                                                                             //Declaring Function Prototypes , this way the compiler knows what to expect in the
void DisplayOkCancel();                                                                             //Program.
void DisplayCorS();                                                                                 //
void DisplayInputPassword();                                                                        //
void DisplayBankAcceptance();                                                                       //
int32_t PasswordCheck();                                                                            //
void CorrectPassword();                                                                             //
void WrongPassword();                                                                               //
void CForward();                                                                                    //
void SForward();                                                                                    //

// FUNCTION      : DisplayWelcome()
// DESCRIPTION   : Clears the LCD display and displays Welcome on line 1 of the display and along with that it also displays Amount Please ,which means how much amount do you want to
//                 enter in your in your Serial port, which is a indication for the user that he/she has to enter the amount throgh the serial port.
// PARAMETERS    : None
// RETURNS       : Nothing

void DisplayWelcome()
{
  lcd.clear();                                                                                     //Clears the LCD
  lcd.printf("Welcome!");                                                                          //Prints Welcome on the first line of LCD
  lcd.setCursor(0, 1);                                                                             //Sets the Cursor to second row and first collumn
  lcd.printf("Amount Please ?");                                                                   //Displays Amount Please? on the second Row
}

// FUNCTION      : DisplayAmount()
// DESCRIPTION   : Clears the LCD display and displays the $amount received on line 1 of the display                 
// PARAMETERS    : float amount - It is the amount that is displayed on the screen.
// RETURNS       : Nothing

void DisplayAmount(float amount)
{
  lcd.clear();                                                                                     //Clears the LCD
  lcd.printf("$%-9.2f", amount);                                                                   //Prints the amount that is entered by the user on the LCD
  Serial.printf("$%9.2f\n", amount);                                                               //Prints the amount entered by the user on Serial Port
}

// FUNCTION      : CheckIfAmountRecd()
// DESCRIPTION   : Checks the serial port to see if any chars are available. If so, reads the chars using the parseFloat method so that the $amount is read.                
// PARAMETERS    : none
// RETURNS       : float type- its the amount entered by the user, 1 - If no value is entered bu the user the cide returns 1.

float CheckIfAmountRecd()
{
  if (Serial.available())                                                                          //Checking if anythhing is available on the Serial Port
  {
    return (Serial.parseFloat());                                                                  //As soon as the value is entered, it returns a floating type Value
  }
  else
  {
    return NoAmountReceived;                                                                       //Returns -1 if no value is entered by the user
  }
}

// FUNCTION      : CheckOkOrCancel()
// DESCRIPTION   : Checks whether the OK or Cancel button has been pressed and returns the value on the basis of the type of button pressed                
// PARAMETERS    : None
// RETURNS       : 3- This value is returned if cancel pressed,
//                 4- This value is returned if ok is pressed  
//                 0-This value is returned when none of the buttons are pressed.

int8_t CheckOkOrCancel()
{
  if (CancelPb.fallingEdge())                                                                       //Checks, if the state of the Pushbutton went from High to Low
  {
      return CancelPressed;                                                                         //Returs 3 if the Cancel push button is pressed
  }
  else if (OkPb.fallingEdge())                                                                      //Checks, if the state of the Pushbutton went from High to Low
  {
    return OKPressed;                                                                               //Returs 4 if the OK push button is pressed
  }
  else
  {
    return NothingPressed;                                                                          //Returns 0 if nothing is pressed.
  }
}

// FUNCTION      : DisplayOkOrCancel()
// DESCRIPTION   : Displays "OK or Cancel?" on line 2 of LCD
// PARAMETERS    : None
// RETURNS       : Nothing.
void DisplayOkCancel()
{
  lcd.setCursor(0, 1);                                                                              //Move to second line first position
  lcd.printf("OK or Cancel?");                                                                      //Prints Ok or Cancel on the LCD
}

// FUNCTION      : DisplayCorS()
// DESCRIPTION   : Sets the Cursor to the Second and and diplays Chequing or Savings on the LCD.
// PARAMETERS    : None
// RETURNS       : Nothing

void DisplayCorS()
{
  lcd.setCursor(0, 1);                                                                             //Sets the Cursor to Second Row
  lcd.printf("Chq or Sav ?");                                                                      //Prints CHQ oR sav on the LCD
}

// FUNCTION      : DisplayInputPassword()
// DESCRIPTION   : Prints "Enter your Password" on the Serial Port & at the same time prints "Password ?" on the first row of LCD
// PARAMETERS    : None
// RETURNS       : Nothing
void DisplayInputPassword()
{
  Serial.printf(" Enter your 4 digit password \n ");                                                //Prints Enter you Password on the Serial Port
  lcd.setCursor(0, 0);                                                                              //Sets the cursor to first ro anf first collumn
  lcd.printf("Password ?");                                                                         //Prints "Password ?" on LCD
}
// FUNCTION      : DisplayBankAcceptance()
// DESCRIPTION   : Sets the Cursor to the first line and Prints "Bank Ok/Cancel?" , which mean its asking the bank should it process the transaction or not.
// PARAMETERS    : None
// RETURNS       : Nothing

void DisplayBankAcceptance()

{
  lcd.setCursor(0, 0);                                                                              //Sets the Cursor to first Row and First collumn
  lcd.printf("Bank ok/cancel?");                                                                    //Prints Bankok/Cancel ? on the LCD
}

// FUNCTION      : PasswordCheck()
// DESCRIPTION   : Sets the Cursor to the Second and and diplays Chequing or Savings on the LCD.
// PARAMETERS    : None
// RETURNS       : Serial.parseInt() - It consists of the password entered by the user
//                   9- the code returns 9 when nothing is entered by the user
                   
int32_t PasswordCheck()
{
  if (Serial.available())                                                                           // Checks if anything us avaiable on the Serial Port
  {
    return Serial.parseInt();                                                                       // Returns the value entered by the user
  }
  else
  {
    return NoPasswordYet;                                                                           // Returns 9 when nothing is entered by the user
  }
}

// FUNCTION      : CorrectPassword()
// DESCRIPTION   : This Function clears the LCD screen and prints correct password on the LCD as well as Serial port
// PARAMETERS    : None
// RETURNS       : Nothing

void CorrectPassword()
{
  lcd.clear();                                                                                      //Clears the LCD
  lcd.setCursor(0, 0);                                                                              //Sets the cursor to first row and first collumn
  lcd.printf("Password Correct");                                                                   //Prints correct Password on theLCD
  delay(CorrectPasswordDelay);                                                                      // A delay of 500 secs provided to keep "Password Correct" on the screen for 500 secs
  Serial.printf("Password Correct");                                                                // Prints Password Correct on the Serial Port
  lcd.clear();                                                                                      // Clears the LCD screen before it exits the Function
}

// FUNCTION      : WrongPassword()
// DESCRIPTION   : This Function clears the LCD screen and prints Wrong password on the LCD as well as Serial port
// PARAMETERS    : None
// RETURNS       : Nothing
void WrongPassword()
{
  lcd.clear();                                                                                       //Clears the LCD
  lcd.setCursor(0, 0);                                                                               //Sets the cursor to first row and first collumn
  lcd.printf("Sorry,Wrong");                                                                         //Prints Wrong Password on theLCD
  lcd.setCursor(0,1);                                                                                //Sets the cursor to second row
  lcd.printf("Pin");                                                                                 //Prints Wrong Pin on the second row                                                                   
  delay(WrongPasswordDelay);                                                                         // A delay of 500 secs provided to keep "Password Wrong" on the screen for 500 secs
  lcd.clear() ;                                                                                      //Clearing the screen after it is printed
  Serial.printf("Wrong Password");                                                                   // Prints Password Correct on the Serial Port
}

// FUNCTION      : Cforward()
// DESCRIPTION   : This Function clears the LCD screen and prints Chequing on the LCD and 
// PARAMETERS    : None
// RETURNS       : Nothing

void CForward()
{
  lcd.clear();                                                                                       //Clears the LCD
  lcd.setCursor(1, 1);                                                                               //Sets the Cursor to Second Row Second Collumn
  lcd.printf("Chequing");                                                                            //Prints Cheuquing on the LCD
  delay (CForwardDelay);                                                                             //Provides a little delay to keep "Chequing" on the screen before it moves to next case
  lcd.clear();                                                                                       //Clears the screen before it leaves the function
}

// FUNCTION      : Sforward()
// DESCRIPTION   : This Function clears the LCD screen and prints Savings on the LCD and 
// PARAMETERS    : None
// RETURNS       : Nothing

void SForward()
{
  lcd.clear();                                                                                       //Clears the LCD
  lcd.setCursor(1, 1);                                                                               //Sets the Cursor to Second Row Second Collumn  
  lcd.printf("Savings");                                                                             //Prints Savings on the LCD
  delay (SForwardDelay);                                                                             //Provides a little delay to keep "Savings" on the screen before it moves to next case
  lcd.clear();                                                                                       //Clears the screen before it leaves the function
}

// FUNCTION      : MillisTimer()
// DESCRIPTION   : This Function uses the millis() function to count time and display the current time on the serial Port.It checks if the difference between the current time and Precvios
//                 time is more than the specified time , then it returns "1" to the function, else it returns 0.
// PARAMETERS    : None
// RETURNS       : 1- when the difference in time is more than the valid time
//                 0- when the difference in time is less than the valid time. 

int8_t MillisTimer( void)
{
  uint64_t ValidTime = TimerTime;                                                                    // Assigns the the time required to a Variable called Valid Time
  static uint64_t PreviousMillis = InitialPreviousMillisState;                                       //Assigning PreviousMillis to 0 (as static) will be 0 only for the first time when the
                                                                                                     //function is called
  uint64_t CurrentMillis = millis();                                                                 //Assigning the millis() function to the variable valled Current Millis                                                               
  Serial.printf("%lu s\n", CurrentMillis);                                                           //Prints the Value of Current Millis on the Serial Port                                                        
  if (CurrentMillis - PreviousMillis >= ValidTime)                                                   //Checks if the difference between CurrentMillis nd Previous Millis is more than the
                                                                                                     //allowed valid time
  {
    PreviousMillis = CurrentMillis;                                                                  //If the condition is satisfied Previous Millis is equal to current Millis
    return TimeOut;                                                                                  //Returns 1 if the condition is satisfied
  }
  else
  { 
  return TimeLeft;                                                                                   //Returns 1 if the value is not more that the valid time
  }
}

void setup()
{
  pinMode(14, INPUT_PULLUP);                                                                         //
  pinMode(15, INPUT_PULLUP);                                                                         //Assigning the Switch buttons to input Pullup Pins
  pinMode(16, INPUT_PULLUP);                                                                         //
  pinMode(17, INPUT_PULLUP);                                                                         //

  Serial.begin(SerialPortBaudRate);                                                                  //Initialising the Serial Port at 9600 Baud Freequency
  delay(SerialDelay);                                                                                //A Deay of 2 seconds is used to allow the serial port to initialise
  Serial.printf("welcome");                                                                          //Prints Welcome on the Serial Port as soon as it is initialised

  lcd.begin (2, 16);                                                                                 //Initialising the LCD
  lcd.printf("KM bank");                                                                             //This appears only at the start for 3s when the Serial Port is being Initialised
  delay(BankNameDelay);                                                                              //A delay of certain time is provided to keep printing KM bank on the screen for a bit
  lcd.clear();                                                                                       //Clears the LCD which removes everything that was written on it
}

void loop()
{
  float Amount = InitialAmountValue ;                                                                //Used to hold the transaction amount,initially set tp 0.0
  static int8_t TransactionState = InitialTransactionState;                                          //Initially Setting the TransactionState to 1
  int8_t PbPressed = InitialPushButtonState;                                                         //Setting the Variable PbPressed as 1 intially
 
  /*states:   1   display Welcome Screen & Amount Please, wait for $ amount input from
                   Serial port
               2   @ amount Received, moves the fow to next case
               0   Displays Ok or cancel on the LCD and moves the flow to next case
               3   The millis TImer starts ad checks if ok or Cancel is pressed within the requried time Limits, if ok ois pressed-FLow moves to the next case,If cancel is pressed
                   Flow moves back to case 1 and if time is more that the valid time limit
               4   C or S received, waiting for PIN to be entered from  Serial Port          
               5   Pin Correct, send transaction data to bank. Flow moves to the next state where bank says ok or cancel to process the transaction
               6   If bank says OK  the machine says thaks and the case moves to case 1, if it says no- It moves to case 1 automatically
  */
                                                                                                      //
  ChequingPb.update();                                                                                //The Push Buttons are being updated here,this is done to check there current
  SavingsPb.update();                                                                                 //state after the fallingedge and rising edge functions are used
  CancelPb.update();                                                                                  //
  OkPb.update();                                                                                      //

  switch (TransactionState)                                                                           //The Switch Statement for transaction State is being Started
  {
    case DISPLAYANDENTERAMOUNT :                                                                      //Case 1
      DisplayWelcome();                                                                               //The function is called to display welcome and AMount received on the LCD
      TransactionState = CHECKAMOUNTRECEIVE  ;                                                        //The Flow if the Switch Statement moves to the Next State

    case CHECKAMOUNTRECEIVE :                                                                         //checking if an amount has been received (Case 2)
      Amount = CheckIfAmountRecd();
      if (Amount != NoAmountReceived )                                                                //Returns a -1 if an transaction amount has not been received on the serial port.
      { 
        DisplayAmount(Amount);                                                                        //Displays the Entered Amount
        TransactionState = DISPLAYOKCANCEL ;                                                          //The Flow if the Switch Statement moves to the Next State
      }
      break;

    case DISPLAYOKCANCEL :                                                                            //Case 0 
      DisplayOkCancel();                                                                              //This Function Displays OK or Cancel on the Screen
      TransactionState = OKCANCELTIMER ;                                                              //The Flow if the Switch Statement moves to the Next State

    case OKCANCELTIMER :                                                                              //Checks if OK or Cancel is pressed within the given time
      PbPressed = CheckOkOrCancel();

      if (PbPressed != NothingPressed)                                                                //The function returns 0 when nothing is pressed
      {
        if (PbPressed == CancelPressed)                                                               //3 is returned by the function when cancel is pressed
        {
          Serial.printf("Cancel Pressed \n");                                                         //Prints Cancel Pressed in the Serial Port.
          TransactionState = DISPLAYANDENTERAMOUNT ;                                                  //The Flow moves to the first case and prints Welcome on the LCD
        }
        else if (PbPressed == OKPressed)                                                              //4 is returned by the function when OK is pressed
        {    
          Serial.printf("OK Pressed \n");                                                             //Prints Ok Pressed in the Serial Port
          lcd.clear();                                                                                //Clears the LCD Screen
          TransactionState = SELECTCHQORSAV;                                                          //The Flow if the Switch Statement moves to the Next State
        }
      }

      else if ((PbPressed == NothingPressed) && (MillisTimer() == TimeOut))                           //If the pushbutton is not pressed and the time to press the button is over
      {
        Serial.printf("Timer out! Sorry!\n");                                                         //Prints Time out on the serial port
        TransactionState = DISPLAYANDENTERAMOUNT ;                                                    //The Flow moves to case 1 and everything starts from the start
      }
      break;

    case SELECTCHQORSAV :                                                                             //Case 4. Chequing or Saving is selected by the user
      DisplayCorS();                                                                                  //Displays Chequing or Saving on the LCD Screen
      PbPressed = CheckOkOrCancel();                                                                  //The value of PbPressed changes on the basis of the value returned by the function
      if (ChequingPb.fallingEdge())                                                                   //Checks if the state of the buton went from high to low
      {
        CForward();                                                                                   //Flashes Chquing
        TransactionState = PASSWORDCHECK ;                                                            //Moves the flow of the diagram to the next state
      }
      else if (SavingsPb.fallingEdge())                                                               //Checks if the state of the buton went from high to low
      {
        SForward();                                                                                   //Flashes Saving
        TransactionState = PASSWORDCHECK ;                                                            //Moves the flow of the diagram to the next state
      }
      else if (PbPressed == CancelPressed)                                                            //The functiion returns the Value 3 when cancel button is pressed
      {
        Serial.printf("Cancel Pressed \n");                                                           //Prints Cancel Pressed in the Serial Port.
        TransactionState = DISPLAYANDENTERAMOUNT ;                                                    //The Flow moves to transaction state one 
      }

      break;
    case PASSWORDCHECK :                                                                              //Case- 5, checks if the entered password is correct or not
      int32_t Password;
      DisplayInputPassword();                                                                         //Prints Enter your password on the Serial Port as well as the LCD
      Password = PasswordCheck();                                                                     //Checks if the entered password is equal to the correct password
      PbPressed = CheckOkOrCancel();                                                                  //Value of the variable updated on the basis of the return value from the function
      Serial.print(Password);                                                                         //Prints Password on the screen                                                                       
      if (Password != NoPasswordYet && (i < MaximumPinAttempts))                                      //if the value if Password is not 9(dependent on the return value from function)
                                                                                                      // and if the value of flag is less than 3           
      {
        if ( Password == StoredPassword)                                                              //Checks if the password is same as the stored password
        {
          CorrectPassword();                                                                          //Prints Correct Password on the screen
          TransactionState = BANKACCEPTCANCEL ;                                                       //The flow moves to the next state
        }
        else
        {
          WrongPassword();                                                                            //If the password did not match it prints wrong password
          TransactionState = PASSWORDCHECK ;                                                          //The Transaction State changes and asks the user to enter the password again
          i++;                                                                                        //Increment provided to the flas if the password is wrong,this is done because we
                                                                                                      //dont want to give user more than 3 attempts
        }
      }
      else if (PbPressed == CancelPressed)                                                            //Checks if cancel was pressed for this state
      {
        Serial.printf("Cancel Pressed");                                                              //Prints Cancel Pressed on the Serial Port
        TransactionState = DISPLAYANDENTERAMOUNT;                                                     //The Transaction State changes to state one and the program starts from the start
      }
      else if ( (i == MaximumPinAttempts))                                                            //This condition is executed if the user has entered 3 wrong Passwords
      {
        Serial.printf("Your Account might be Blocked \n");                                            //Prints account blocked on the serial port
        lcd.clear();                                                                                  //clears the LCD                                                          
        lcd.printf("Account Blocked");                                                                //Prints Accoun Blocked on the LCD
        delay(AccountBlockDelay);                                                                     //Delay of 1500us provided                                                                                
        TransactionState =  DISPLAYANDENTERAMOUNT ;                                                   //Takes the transaction state to case 1
        i=InitialPinFlagState;                                                                        //initialises i = 0 again for the next user to enter the value
      }

      break;
    case BANKACCEPTCANCEL :                                                                          //Case-6, checks if the bank presses OK or cancel for the transactopn
      DisplayBankAcceptance();                                                                       //Prints "BankOK/Cancel?" on the LCD Screen
      PbPressed = CheckOkOrCancel();
      if (PbPressed != NothingPressed)
      {
        if (PbPressed == CancelPressed )                                                             //Checks if the returned value from the function is equal to 3
        {
          Serial.printf("Cancel Pressed");                                                          //If the cancel Button is Pressed th State goes to State 1 and Cancel Pressed
          TransactionState = DISPLAYANDENTERAMOUNT;                                                 //is printed on the SerialPort
        }
        else if (PbPressed ==OKPressed )                                                            //Checks if the returned value from the function is equal to 3
        {
          Serial.printf("OK Pressed");                                                              //Prints OK pressed on the erial Port
          lcd.clear();                                                                              //Clears the LCD Screen
          lcd.printf("Thanks for");                                                                 //Prints "Thanks For" on the first row
          lcd.setCursor(0, 1);                                                                      //Cursor Brought to Second row,First Collumn
          lcd.printf("banking with us");                                                            //Prints"banking with us" on the second row
          delay(CaseDelay);                                                                         //A delay of 1000ms is given, that means it prints the above sentence for 1 sec on
                                                                                                    //the LCD screen
          TransactionState = DISPLAYANDENTERAMOUNT;                                                 //As there are no TransactionStates left, the flow moves to the forst state
        }
      }
      break;

    default:                                                                                        //If anything that is not expected is pressed or happens then the Default statement
                                                                                                    //comes to play
      TransactionState = DISPLAYANDENTERAMOUNT;                                                     // If default Statement is executed the flow moves to the first state
      break;
  }
}

