// fill() function args: fill(v1, v2, v3, opacity)
// rect() function args: rect(x, y, width, height, radii for all corners)
void drawTimers(){
  
  // display elapsed seconds
  fill(88,88,88,255); 
  rect(900,10,90,30); 
  fill(255,255,255);
  text(counter + " Secs",910,30);
  
  // display local system time
  fill(88,88,88,255);
  rect(720,10,150,30); 
  fill(255,255,255); 
  text("Local time: " + hour() + ":" + minute() + ":" + second(),730,30);
}

void drawBoxWithCoordinates(int x,int y,int w,int h){ 
  fill(255,255,255);
  noStroke();
  rect(x,y,w,h); 
}
