#include <Adafruit_Sensor.h> // Required for temperature/humidity sensor
#include <DHT.h> // Temperature/humidity sensor library
#include <DHT_U.h> // Temperature/humidity sensor library
#include <Wire.h> // Necessary library for I2C communication
#include "MutichannelGasSensor.h" // The library for the Grove Multichannel Gas Sensor
#include <Adafruit_GFX.h> // The library to render images for the OLED
#include <Adafruit_SSD1306.h> // The library to actually communicate with the OLED

#define SCREEN_WIDTH 128 // Define the OLED's pixel dimensions
#define SCREEN_HEIGHT 64
#define DHTPIN 2         // Define the pin that the DHT11 is connected to
#define DHTTYPE DHT11    // Specifies the model of the DHT sensor
#define OLED_RESET     4 // Reset pin number
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Init the display

DHT_Unified dht(DHTPIN, DHTTYPE); // Init the DHT sensor
uint32_t delayMS;   // Declare a variable for the delay time

void setup()
{
    Serial.begin(115200);  // Start serial for output
    dht.begin(); 
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Begin communicating with the OLED at address 0x3C
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
    gas.begin(0x04); // Init the gas sesnro as the default I2C address of the slave
    gas.powerOn();
    delayMS = sensor.min_delay / 1000;
 
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    display.clearDisplay();
    display.display();
}

void loop()
{
    float measure1, measure2, measure3, air_quality_measure;
    display.setTextSize(0.5); 
    display.setCursor(0, 0); 
    float c; // Declare variable to be used to send the data over serial
    sensors_event_t event;
/*
    The reading and sending of gas data over serial will be structured as follows:
    c = gas.measure_<gas>(); 
    Serial.print("<identifier>: ")
    if(c >= 0) {
        Serial.println(c);
        measure<air_quality_measure_gas> = <equation>
    }
    else Serial.println("-1") // Signifies an error
*/

/*
    The reading and sending of temperature data over serial will be structured as follows:
    dht.<temp/humidity>().getEvent(&event);
    Serial.print("<identifier>: ");
    if(event.<temp/humidity>) {
        Serial.println(event.<temp/humidity>);
        display.print("<identifier: ");
        display.println(event.<temp/humidity>);
    }
    else {
        Serial.println("-1");
    }
*/
    c = gas.measure_NH3(); // Measures NH3
    Serial.print("A: ");
    if(c >= 0) {
        Serial.println(c);
      	measure1 = 0.246*c-7.222;
    }
    else Serial.println("-1");
    c = gas.measure_CO();
    Serial.print("B: ");
    if(c >= 0){
      	Serial.println(c);
      	measure2 = (-1*(pow(c,2))+23.824*c+14.482);
    }
    else Serial.println("-1");
    c = gas.measure_NO2();
    Serial.print("C: ");
    if(c >= 0){
      	Serial.println(c);
      	measure3 = 1.01 * c + 12.439;
    }
    else Serial.println("-1");
    c = gas.measure_C3H8();
    Serial.print("D: ");
    if(c >= 0){
      	Serial.println(c);
    }
    else Serial.println("-1");
    c = gas.measure_C4H10();
    Serial.print("E: ");
    if(c >= 0){
      	Serial.println(c);
    }
    else Serial.println("-1");
    c = gas.measure_CH4();
    Serial.print("F: ");    
    if(c >= 0){
	Serial.println(c);
    }
    else Serial.println("-1");
    c = gas.measure_H2();
    Serial.print("G: ");
    if(c >= 0){  
    	Serial.println(c);
    }
    else Serial.println("-1");
    c = gas.measure_C2H5OH();
    Serial.print("H: ");
    if(c >= 0){
      	Serial.println(c);
    }
    else Serial.println("-1");
    dht.temperature().getEvent(&event);
    Serial.print("T: ");
    if(event.temperature) {
        Serial.println(event.temperature);
        display.print("Temp: ");
        display.println(event.temperature);
    }
    else {
        Serial.println("-1");
    }
    
    dht.humidity().getEvent(&event);
    Serial.print("I: ");
    if(event.relative_humidity){
        Serial.println(event.relative_humidity);
        display.print("Humidity: ");
        display.println(event.relative_humidity);
    }
    else {
      Serial.println("-1");
    }
    air_quality_measure = max(measure1, measure2);
    air_quality_measure = max(air_quality_measure, measure3);
    if(air_quality_measure) {
        Serial.print("J: ");
        Serial.println(air_quality_measure);
        display.print("Air Quality: ");
        display.println(air_quality_measure,2);
    }
    display.display(); // Display on the OLED
    delay(3000);
    display.clearDisplay(); // Clear the display for the next cycle
}
