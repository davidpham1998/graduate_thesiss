#include <Arduino_FreeRTOS.h>
 
// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );
 
// the setup function runs once when you press reset or power the board
void setup() {

// initialize serial communication at 9600 bits per second
Serial.begin(9600)

while (!Serial) {
; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards
}
 
// Now set up two tasks to run independently
xTaskCreate
TaskBlin
, (const portCHAR *)"Blink" // A name just for human
, 128 // This stack size can be checked & adjusted by reading the Stack Highwate
, NUL
, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest
, NULL )
 
xTaskCreate
TaskAnalogRea
, (const portCHAR *) "AnalogRead
, 128 // Stack siz
, NUL
, 1 // Priorit
, NULL )
 
// Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started
}
vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one secon

}
 
void TaskAnalogRead(void *pvParameters) // This is a task.
{
(void) pvParameters

/*
AnalogReadSeria
Reads an analog input on pin 0, prints the result to the serial monitor
Graphical representation is available using serial plotter (Tools > Serial Plotter menu
Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground
 
This example code is in the public domain
*/
 
for (;;)
{
// read the input on analog pin 0
int sensorValue = analogRead(A0);
// print out the value you read
Serial.println(sensorValue);
vTaskDelay(1); // one tick delay (15ms) in between reads for stabilit
  }
}
