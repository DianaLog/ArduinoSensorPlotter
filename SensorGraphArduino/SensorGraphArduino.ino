//This is the main file that communicates with Arduino microcontroller.
// Connected to Arduino are two sensors: Galvanic Skin Response (pin A1) and Pulse sensor (oin A2). More sensors can be added.

// The signal from GSR is ready to send to PROCESSING in its raw form, however signal from the Pulse sensor needs to go via additional detection algorithm to pass only the signal when
// a beat is detected. The detection is tailored to suit Pulse sensor like this one https://pulsesensor.com, results were validated against another more reliable reference heart rate monitoring instrument.

//Diana Kardys

const int skinSensor = A5; // Galvanic Skin Response (GSR) sensor  connected to analog pin A5
const int pulsePin = A1;  // Pulse Sensor purple wire connected to analog pin A2

int counter = 0;
 
//______________________________Initialise values needed for processing pulse sensor
 const int arrayLenght = 40; // create empty array of 40 values in order to store incoming 50 samples, this is an 
                           //estimated length. The purpose is to create an array of 40 cells which will be continuously 
                           //updated once the array gets full, a new set of values will replace the old ones. 
 int BPMArray[6] = {60,60,60,60,60,60};  //BPMArray[6] is an array with 6 values
 int valuesArray[arrayLenght]; 
 //______________________________
 
 // Pulse sensor
 // Method for storing previous and current beat, 'millis' is Arduino library to create a timer
 unsigned long LastTime = millis(); // LastTime = last milisecond since the program started. Once LastTime detects a signal, LastTime will be stored in millis
                                   // The first time a beat is detected it is stored as LastTime
 unsigned long CurrentTime = millis(); //THe following beat is stored as CurrentTime whilst the previous beat movees to be stored as LastTime, and it goes on in a loop
 int BPM = 0; //initialise, by =0 we assume there are no beats detected beforehand (no difference between LastTime and BPM)
 int beatOn = 0; //variable for storing the status of detected peak level, at any time it has value of 0 or 1, where 1=peak detected, 0=no peak
 
 String beat = ""; //the string value will send the data within " " and send it forward to Processing (i.e the serial data connection)

//Arduino library 
void setup(){ //Java 
  pinMode(skinSensor,INPUT); //Arduino method: assign the Arduino pin to the GSR sensor so the correct data is stored
  pinMode(pulsePin,INPUT); // Arduino method: assign the Arduino pin to the Pulse sensor so the correct data is stored
  Serial.begin(9600); //Arduino method: the speed at which the serial port will communicate with Arduino
}

//____________________________________READ VALUES FROM PULSE SENSOR
void loop(){ //Java method overwritten by Arduino to loop
  int skinSensorValue = analogRead(skinSensor); // analogRead= Arduino method which reads the input
  int currentValue = analogRead(pulsePin); // analogRead= Arduino method which reads the input, i.e. get the value from the sensor
  
   //___________________________________PROCESSING READ PULSE SENSOR VALUES INTO ARRAY OF 40 CELLS
  if(sizeof(valuesArray) < arrayLenght){    //sizeof will return number of items in an array (equivalent to Matlab 'numel'). IF THERE ARE LESS THAN 40 VALUES...    
    valuesArray[(sizeof(valuesArray) - 1)] = currentValue; //save val to array   ...ADD ONE VALUE
  } else {
    for(int i=0; i < arrayLenght-1; i++){ //shift array values, use the array of 50 cells to fill with data ...OR SHIFT ARRAY OF CURRENT VALUES ONE TO THE LEFT...
      valuesArray[i] = valuesArray[i+1];  // shift the values so that incoming samples can be added to the array  ...PERFORM THE SHIFTING SO WE CAN ADD ONE MORE SAMPLE WAITING
    //Serial.println(valuesArray[i]);
    }
    valuesArray[arrayLenght - 1] = currentValue; // ..AND FINALLY PUT THE WAITING SAMPLE IN THE TRAIN (i.e. append at the end of 40 cell array, 
                                                //arrayLenght starts at 0 therefore we have to say 'arrayLenght - 1')
  }                //array processing loop ends

//Pulse sensor processing
//Code to retrieve peaks from continuous stream of data
  int lowest = 0; //these two variables will keep track of the lowest, highest of the 50 sample arrays
  int highest = 0;
  int mid = 0; //this variable will store the average value of the 40 sample arrays (sum of 40 array values/number of values)
 
  for(int i=0; i < arrayLenght; i++){  //loop through array of 40 samples to find...
    if(lowest == 0){ lowest = valuesArray[i]; } //...find lowest value: 'if(lowest == 0)' then set it to the corresponding value of  'valuesArray[i]'
    if(highest == 0){ highest = valuesArray[i]; } //...find highest value: 'if(highest == 0)' then set it to the corresponding value of  'valuesArray[i]'
                                                //lowest and highest are only for the first sample which is=0, when the program is started, afterwards 
                                                //these two commands are not executed although the loop continues to run and execute the rest of the code below
    if(valuesArray[i] < lowest){ lowest = valuesArray[i]; } //...if we found even lower value than the previous 'lowest', then replace it, otherwise do nothing
    if(valuesArray[i] > highest){ highest = valuesArray[i]; }//...if we found even higher value than the previous 'highest', then replace it, otherwise do nothing
    mid = (lowest + highest) / 2; //calculate mid-point of the current array of 40 samples
  } //loop for finding highest and lowest ends
  
 
  //______________________________DETECT HEART BEAT
  CurrentTime = millis(); //millis = tells me the current time stored from the beginning of the program (defined earlier). The empty brackets'()'store whatever time it is in miliseconds
  if((LastTime + 300) < CurrentTime){
    if(currentValue > mid){ //set the 'mid' as threshold for detecting beats, if signal stored in currentValue is above it then count is as a beat
      if(beatOn == 0){ //keep track of the state of the beat: is it on(=1) or off(=0)
        int x = 60/(((float)CurrentTime - (float)LastTime)/1000); // x=BPM, CurrentTime-LastTime is the time difference between peaks of two consecutive beats
                                                                  //needed to specify floats, otherwise would have errors
        //_________PERFORM ERROR HANDLING - CHECK VALIDITY OF DETECTED BEATS
        if(x > 40 && x < 200){ //if the heart heat is within the possible range I set i.e. 40-200 BPM then it is a valid beat
          BPMArray[0] = BPMArray[1]; //shift again to make space for the incoming x
          BPMArray[1] = BPMArray[2]; //shift again to make space for the incoming x
          BPMArray[2] = BPMArray[3]; //shift again to make space for the incoming x
          BPMArray[3] = BPMArray[4]; //shift again to make space for the incoming x
          BPMArray[4] = BPMArray[5]; //shift again to make space for the incoming x
          BPMArray[5] = x; //place x
          BPM = (BPMArray[0] + BPMArray[1] + BPMArray[2] + BPMArray[3] + BPMArray[4] + BPMArray[5]) / 6;
        } 
        beatOn = 1;
        LastTime = CurrentTime - 0;   //This time latency has been set to 0 - this was the problem with the code before. Push the value of current time into the LastTime so we cann execute the loop agaiin and compute the time differences 
                                     //between LastTime - CurrentTime; 
      }
    } 
  } else {
    beatOn = 0;
    beat = " no beat ";
  }
  delay(10); //slow down data transmission to avoid crushing the program
  Serial.println((String)skinSensorValue + "," + (String)BPM +",10"); //the actual printing command (ARDUINO method),
                                                                     //this will pass on values from pin A1 in its raw form and pin A2 after it has gone through beat detection algorithm
                                                                     //those signals will then be picked up by PROCESSING for live display 
  delay(10);// delay again to smooth out the program flow between the printed lines
}

