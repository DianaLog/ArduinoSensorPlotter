import processing.serial.*;
Serial myPort;  // Create object from Serial class

int counter = 0;

//input sensor 1
String g1_name = "Galvanic sensor ";
int g1 = 10; //initial value for graph  y axis
int graph1Coords[] = { 10, 10, 700, 320, 1, 0, 0, 1024}; //x,y,w,h,barWidth, graphOneDataXValue, minDataValue, maxDataValue

//input sensor 2
String g2_name = "Heart rate ";
int g2 = 0;
int graph2Coords[] = { 10, 340, 700, 250, 1, 0, 0, 120}; //x,y,w,h,barWidth, graphOneDataXValue, minDataValue, maxDataValue

//input sensor 3
String g3_name = "Some sensor ";
int g3 = 0;
int graph3Coords[] = { 720, 50, 270, 280, 2, 0, 0, 350}; //x,y,w,h,barWidth, graphOneDataXValue, minDataValue, maxDataValue

//input sensor 4
String g4_name = "Some sensor ";
int g4 = 0;
int graph4Coords[] = { 720, 340, 270, 250, 1, 0, 0, 850}; //x,y,w,h,barWidth, graphOneDataXValue, minDataValue, maxDataValue

void setup(){
 size(1000,600, "processing.core.PGraphicsRetina2D"); 
 background(230,230,230);
 //println(//Serial.list()); //print the list of serial devices to find out on which port number is arduino 
 String portName = Serial.list()[2]; //change number according to the above
 myPort = new Serial(this, portName, 9600);
 //myPort.clear();            // flush buffer
 //myPort.bufferUntil('\n');  // set buffer full flag on receipt of carriage return
 
 //Draw the box for graph 1
 drawBoxWithCoordinates(graph1Coords[0],graph1Coords[1],graph1Coords[2],graph1Coords[3]);
 
 //Draw the box for graph 2
 drawBoxWithCoordinates(graph2Coords[0],graph2Coords[1],graph2Coords[2],graph2Coords[3]);
 
 //Draw the box for graph 3
 drawBoxWithCoordinates(graph3Coords[0],graph3Coords[1],graph3Coords[2],graph3Coords[3]);
 
 //Draw the box for graph 4
 drawBoxWithCoordinates(graph4Coords[0],graph4Coords[1],graph4Coords[2],graph4Coords[3]);
 
}

void draw(){ 
  if ( myPort.available() > 0) { //if reading value from arduino as positive value=do stuff
    String read = myPort.readStringUntil('\n');  //take each value and divide it into separate line
    if(read != null){ //make sure that there is a value
      String[] dataArray = split(read, ','); //turn arduino data into an array of values, each item = sensor. Break apart values from two sensors.
      
      if(dataArray.length > 2){ // make sure we have at least 2 values coming in from  Arduino//this  is equivalent to error handling to avoid displaying invalid data 
        
        String graph1Data = dataArray[0]; 
        graph1Data = trim(graph1Data); //trim=remove blank spaces at the beginning or end of value(presented as string)
        if(graph1Data.length() > 0){ //if there is at least one character=continue, if not then ignore
          g1 = Integer.parseInt(graph1Data); //g1 was a string, now we converted(parsed) it into integer. g1 represents y axis
          graph1Coords[5]+=graph1Coords[4]; //the x value should continue to increase and will be reset by the drawer method when it's over the width
          drawLineInBoxWithCoordinatesAndData(graph1Coords,g1,g1_name); //refer to  function crated in 'drawLines'
        }
        
        String graph2Data = dataArray[1];
        graph2Data = trim(graph2Data);
        if(graph2Data.length() > 0){
          g2 = Integer.parseInt(graph2Data); //just simulating data (y)
          graph2Coords[5]+=graph2Coords[4]; //the x value should continue to increase and will be reset by the drawer method when it's over the width
          drawLineInBoxWithCoordinatesAndData(graph2Coords,g2,g2_name);
        }
        
        // repeat above if more sensors exist
      }      
      counter++; //increment the counter just so we know where we are
    }
  }
  drawTimers(); //display and update the timers
}


