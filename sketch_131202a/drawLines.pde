// This file creates all the graphing functions, lines, display numerical results

void drawLineInBoxWithCoordinatesAndData(int graphCoords[], int graphYValue, String SensorName){
  int x = graphCoords[0]; 
  int y = graphCoords[1];
  float w = graphCoords[2] * 1.00;
  int h = graphCoords[3];
  int barWidth = graphCoords[4];
  int minDataValue = graphCoords[6];
  int maxDataValue = graphCoords[7];
  
  float readXIncrement = graphCoords[5] * 0.1; // graphCoords[5]=1 pixel,by multiplying by 0.5 or similar we increment the graph plotting by 0.5 pixel
  
  float graphYValueMapped = map(graphYValue,minDataValue,maxDataValue,y,(y+h)) - y; //map values so it fits in the graph. Don't change sign values as it will screw thing up
  if(graphYValueMapped > h){ graphYValueMapped = h; } //making sure is doesn't go out of the graph (Y)
  if(graphYValueMapped < graphCoords[6]){ graphYValueMapped = 1; } //making sure is doesn't go out of the graph (Y) therefore  graphYValueMapped = 1
  if(readXIncrement >= w){ graphCoords[5] = 0; } //making sure is doesn't go out of the graph (X)
  
  noStroke(); //make sure there is no borders on plotted impulses
  
  fill(255,255,255,155);
  rect((readXIncrement+x),(y+h),barWidth,-h); // clear the previous background
  fill(252,90,11,255); // plot line colour
  rect((readXIncrement+x),(y+h),barWidth,-graphYValueMapped); //draw the line
  fill(0,0,0);
  if(counter % 400 == 0){  //change 100 to alter display time of drawGuides
    text((int)graphYValue,(readXIncrement+x-20),(y+h)-graphYValueMapped - 10);
  }  
  //draw Y value text
  fill(252,111,11,255);
  rect(x,y,150,30);
  fill(255,255,255);
  text(SensorName + (int)graphYValue,x+10,y+20);
  
  drawGuides(x,y,h,minDataValue,maxDataValue); //this creates constant value lines in grey
}                                             

//display numerical results along graph
void drawGuides(int x, int y, int h,int minVal,int maxVal){
  int range = maxVal - minVal; //=100
  range = range/5; //=20
  int unit = h/5;  //=40 
  fill(150);
  int scale = 5; // adjust scale of y-axis if needed
  for(int i=0;i<=5;i++){
    if(i>0 && i <5){
      text((range*scale),x,y+(unit*i));
    }
    rect(x,y+(unit*i),700,1);
    scale--;
  }
}

