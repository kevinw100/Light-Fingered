 #include <Servo.h> 
   
  Servo myservo;  // create servo object to control a servo 
                  // a maximum of eight servo objects can be create
  int pos = 0;    // variable to store the servo position 
  int internalPos = 0; // variable to store the absolute level of the servo
  int level = 0;
  int maxLevel = 7;
  int minLevel = 0;
  int successLED = 3;
  int failureLED = 4;
  int levUpLED = 2;
  //photoResistor = A0 (must connect photoresistor to A0)
  float thresholdVoltage =600;
  int numSuccess = 0;
  int numFailure = 0;
  float volt;
  void setup() 
  {
    Serial.begin(9600);
    Serial.println("In Setup");
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
    //sets up pins
    pinMode(successLED, OUTPUT);
    pinMode(failureLED, OUTPUT);
    pinMode(levUpLED, OUTPUT);
    digitalWrite(successLED,LOW);
    digitalWrite(failureLED,LOW);
    digitalWrite(levUpLED,LOW);
    //photoResistor must be put in pin A0.
   //finding baseline voltage to set against success and failure
    float sum = 0;
    int numRead = 0;
    //DO NOT TOUCH THE SETUP WHILE RUNNING THIS BASELINE!!!
    while(millis() < 500){
      sum += analogRead(A0);
      Serial.println(sum);
      numRead++;
    }
    Serial.println(numRead);
    //set the threshold to be 10% of the average reading
    int average = sum/numRead;
    Serial.print("Average: ");
    Serial.println(average);
    //thresholdVoltage = sum / numRead * .1;

    
  } 
  //Lights up a light when resistance drops down to a certain point 
  void success(){
    volt = analogRead(A0);
    Serial.println("SUCCESSS!!!!");
    Serial.print("Voltage in success: ");
    Serial.println(volt);
    
    //checks if the level increased
    if(didLevel() && volt <= thresholdVoltage)
    {
      Serial.println("LEVEL UP!!!");
      //turns on LED's, increases level, resets success/failure counts, then turns off LED
      digitalWrite(levUpLED, HIGH);
      digitalWrite(successLED, HIGH);
      numSuccess = 0;
      numFailure = 0;
      level++;
      delay(5000);
      digitalWrite(levUpLED,LOW);    
      digitalWrite(successLED,LOW);    
    }
    else if(volt <= thresholdVoltage)
    {
      Serial.println("SUCCESS!!!");
      digitalWrite(successLED, HIGH);
      delay(1000);
      digitalWrite(successLED,LOW);
      numSuccess++;
    }
  }
   
  //Lights up the failureLED when the resistor isn't grabbed within 5 seconds
  void failure(){
    if(levelDown())
    {
      level--;
      numFailure = 0;
      numSuccess = 0;
      digitalWrite(failureLED, HIGH);
      digitalWrite(levUpLED, HIGH);
      delay(1000);
      digitalWrite(levUpLED,LOW);
      digitalWrite(failureLED,LOW);
    }
    else{
      numSuccess = 0;
      numFailure++;
      digitalWrite(failureLED, HIGH);
      delay(1000);
      digitalWrite(failureLED, LOW);
    }
  }
  //If a certain number of successes is reached, you level up, unless if you're at the maximum level
  boolean didLevel(){
    return (numSuccess >= level && level < maxLevel);
  }
  boolean levelDown(){
    return (numFailure >= maxLevel + 1 - level && level > minLevel);
  }  
/*  void levelUp()
  {
    Serial.println("LEVEL UP!!!!");
    digitalWrite(levUpLED, HIGH);
    numSuccess = 0;
    level++;
    delay(1000);
    digitalWrite(levUpLED,LOW);    
  }*/
  void loop() 
  { 
    Serial.println("In Loop");
    Serial.println(thresholdVoltage);
    volt = analogRead(A0);
    Serial.println(volt);
    int randNum = random(180);
    Serial.print("level: ");
    Serial.println(level);
    Serial.print("Number of Successes ");
    Serial.println(numSuccess);
    if (pos < randNum){
      for(pos; pos < randNum; pos += level+1)  // goes from 0 degrees to 180 degrees 
      { 
        //checks if a success occurred 
        //*** this may throw some errors***
        if(volt <= thresholdVoltage)
        {
          Serial.print("Voltage: ");
          Serial.println(volt);
       //   delay(1000);
          success();
          
        }        
        myservo.write(pos); // tell servo to go to position in variable 'pos' 
        delay(15);         // waits 15ms for the servo to reach the position 
      } 
    }
    else if(pos > randNum)
    {
      while(pos > randNum)     // goes from 180 degrees to 0 degrees 
      {                                   
        //checks if a success occurred 
        //*** this may throw some errors***
        if(volt <= thresholdVoltage)
        {
          success(); 
      //    delay(1000);
        } 
        myservo.write(pos); // tell servo to go to position in variable 'pos'        
        pos -= level + 1; 
        delay(15); // waits 15ms for the servo to reach the position 
       }
    } 
  }

  
