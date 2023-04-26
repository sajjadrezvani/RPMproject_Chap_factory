// by SRK

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

const char* ssid     = "Honor 10 Lite";
const char* password = "sajjad2000";

const char* resource = "/trigger/send/with/key/pHCHbxwgVjDaNC6wE6sQC2WsCf9MVuFMieLP2fCrcPC";
const char* server = "maker.ifttt.com";

const char* resource2 = "/trigger/sms/with/key/pHCHbxwgVjDaNC6wE6sQC2WsCf9MVuFMieLP2fCrcPC";


int   rpm=0 ,last_rpm=0 , digit=0 , d1=0 , d2=0 , d3=0 , num = 0, d1B=0, d2B=0 , d3B=0 , wif=-1; 
float last_vol =0 ,myTime=-1 ;
double input=20 ,vol = 0 , spi[10]={0};
unsigned long int t1=0 ,t2=0 ;


// Time to sleep
uint64_t TIME_TO_SLEEP = 30;

void setup() //////////////////////////////////////////////////
{
  Serial.begin(115200); 
  delay(2000);

    pinMode(5,OUTPUT);
    pinMode(21,OUTPUT);
    pinMode(19,OUTPUT);
    pinMode(18,OUTPUT);
    
    pinMode(15,OUTPUT);
    pinMode(16,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(2,OUTPUT);

    pinMode(14,OUTPUT);
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    pinMode(27,OUTPUT);

  initWifi();  //
    
//  #ifdef ESP32
//    // enable timer deep sleep
//    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);    
//    Serial.println("Going to sleep now");
//    esp_deep_sleep_start();
//  #else
//    Serial.println("Going to sleep now");
//    ESP.deepSleep(TIME_TO_SLEEP * 1000000); 
//  #endif
}

void loop()    ////////////////////////////////////////////////////////////////////
{
  
    last_vol = vol;
    input =analogRead(33);
    vol = input* 3.3 / 4095.0;
    Serial.print("vol:");
    Serial.println(vol);


    if(millis()- t1 >5000)
      {
        rpm=0;   ////////////////////////////////////////////////////////////////////
        digit=rpm;
      d1= digit%10;
      digit/=10;
      d2= digit%10;
      digit/=10;
      d3= digit%10;

      d1B=dec2by(d1);
      d2B=dec2by(d2);
      d3B=dec2by(d3);

      digitalWrite(5,d1B%10);   /// D1
      d1B/=10;
      digitalWrite(21,d1B%10);
      d1B/=10;
      digitalWrite(19,d1B%10);
      d1B/=10;
      digitalWrite(18,d1B%10);



      digitalWrite(15,d2B%10);      /// D2
      d2B/=10;
      digitalWrite(16,d2B%10); 
      d2B/=10;
      digitalWrite(4,d2B%10); 
      d2B/=10;
      digitalWrite(2,d2B%10);

  
      digitalWrite(14,d3B%10);    ///D3
      d3B/=10;
      digitalWrite(25,d3B%10);
      d3B/=10;
      digitalWrite(26,d3B%10);
      d3B/=10;
      digitalWrite(27,d3B%10);

      }
    
    if(vol<1.3 && last_vol>2)
   {
      wif++;
    
      t2=millis();
      myTime=t2-t1;
      last_rpm = rpm;
      rpm= round(60.0/(myTime/1000));
      t1=t2;



       if(60 > last_rpm-rpm && abs(rpm-last_rpm)>4)
      {

       Serial.print("$$$$$$$$$$$$$$ rpm:");
       Serial.println(rpm);



       
      digit=rpm;
      d1= digit%10;
      digit/=10;
      d2= digit%10;
      digit/=10;
      d3= digit%10;

      d1B=dec2by(d1);
      d2B=dec2by(d2);
      d3B=dec2by(d3);

      digitalWrite(5,d1B%10);   /// D1
      d1B/=10;
      digitalWrite(21,d1B%10);
      d1B/=10;
      digitalWrite(19,d1B%10);
      d1B/=10;
      digitalWrite(18,d1B%10);



      digitalWrite(15,d2B%10);   /// D2
      d2B/=10;
      digitalWrite(16,d2B%10); 
      d2B/=10;
      digitalWrite(4,d2B%10); 
      d2B/=10;
      digitalWrite(2,d2B%10);

  
      digitalWrite(14,d3B%10);    ///D3
      d3B/=10;
      digitalWrite(25,d3B%10);
      d3B/=10;
      digitalWrite(26,d3B%10);
      d3B/=10;
      digitalWrite(27,d3B%10);

      }
      
   }
    
   delay(10);   

      if(wif%60 ==0)
      {
              makeIFTTTRequest();//

              if(rpm<130)
              {
              makeIFTTTRequest_sms();//
              }
              wif++;
      }
      

}



void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service



void makeIFTTTRequest() //////////////////////////////////////////////////////
{
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + rpm + "\",\"value2\":\"" + 999  
                      + "\",\"value3\":\"" + 500 + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

   
 
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  Serial.println("\nclosing connection");
  client.stop(); 
}


void makeIFTTTRequest_sms()   ////////////////////////////////////////////////
{
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + rpm + "\",\"value2\":\"" + 999  
                      + "\",\"value3\":\"" + 500 + "\"}";

  client.println(String("POST ") + resource2 + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

   
 
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  Serial.println("\nclosing connection");
  client.stop(); 
}

int dec2by(int num)
{
    int toBit=0 ,i=0 , iter=0;
    while(num>0)
    {
        iter= num%2;
        toBit = toBit+iter*(pow(10,i));
        num/=2;
        i++;
    }
    return toBit;
}
