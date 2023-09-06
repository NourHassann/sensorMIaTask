//define the encoder pins
#define Encoder_A 2
#define Encoder_B 3 

//define the counters variable 
long Previous_count = 0;
long counts = 0;
long Filtered_count = 0;
long Previous_filtered_count = 0;

void setup() {
  //the interrupt config
pinMode(Encoder_A, INPUT_PULLUP);
pinMode(Encoder_B, INPUT_PULLUP);
attachInterrupt(Encoder_A, ISR_ENCODER_FUN_1, change);
attachInterrupt(Encoder_B, ISR_ENCODER_FUN_2, change);
}

void loop() {
  //the transfer function which optained from the filter file 
 Filtered_count = -0.747 Previous_filtered_count + 0.874 count + 874 Previous_count;
 Serial.print("counts after filtered: ");
 Serial.println(Filtered_count);
 //assign new value to the previous count and the previous filter
 Previous_count = counts;
 Previous_filtered_count = Filtered_count;
}
//the function that will trigger when the encoder A change
void ISR_ENCODER_FUN_1(){
  //clockwise movement
  if(digital.Read(Encoder_A) != digital.Read(Encoder_B)){
    counts ++;
  }
  else   //counterclockwise movement 
    counts--;
}
//the function that will trigger when the encoder B change
void ISR_ENCODER_FUN_2(){
  //counterclockwise movement
  if(digital.Read(Encoder_A) == digital.Read(Encoder_B)){
    counts ++;
  }
  else  //clockwise movement
    counts--;
}
