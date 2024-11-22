#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "edge";
const char* password = "12345678900";
const char* API_KEY = "AIzaSyADHyNXAVa5a_Q-CYHYLp7HjxoYK5UvV9E";


void fetchTrafficInfo(float originLat, float originLng, float destLat, float destLng) {
  // Establish WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // API URL
  String url = "https://maps.googleapis.com/maps/api/distancematrix/json?";
  url += "origins=" + String(originLat, 6) + "," + String(originLng, 6);
  url += "&destinations=" + String(destLat, 6) + "," + String(destLng, 6);
  url += "&departure_time=now";
  url += "&key=" + String(API_KEY);

  // Send HTTP GET request
  HTTPClient http;
  http.begin(url);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println(payload); // Print response from Google Maps API
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  WiFi.disconnect(true);
}
// fetchTrafficInfo(40.7128, -74.0060, 34.0522, -118.2437);
void callFetchTrafficInfo() {
  // Example origin and destination pairs
  float origins[4][2] = {
    {40.7128, -74.0060}, // New York, NY
    {34.0522, -118.2437}, // Los Angeles, CA
    {51.5074, -0.1278}, // London, UK
    {48.8566, 2.3522} // Paris, France
  };
  float destinations[4][2] = {
    {34.0522, -118.2437}, // Los Angeles, CA
    {40.7128, -74.0060}, // New York, NY
    {48.8566, 2.3522}, // Paris, France
    {51.5074, -0.1278} // London, UK
  };

  // Call fetchTrafficInfo with each pair
  for (int i = 0; i < 4; i++) {
    Serial.print("Fetching traffic info for pair ");
    Serial.println(i + 1);
    fetchTrafficInfo(origins[i][0], origins[i][1], destinations[i][0], destinations[i][1]);
    delay(5000); // Delay between each API call
  }
}



LiquidCrystal_I2C lcd(0x27, 16, 4);

int irsensor = 3;
int motor = 4;
int maxtime = 60;
long int trafficIntensity[4];
int maxIndex=-1;
int cnt=0;
int p=0;
long int trafficval[3][4] = { 
  {30,20,25,25},
  {32,18,23,28},
  {27,23,24,26},
  {18,32,34,16},
  {20,30,36,24},
  {19,33,22,26},
  {36,26,22,28},
  {35,25,25,25},
  {23,27,22,28}
};


int redled[4] = {32, 33, 25, 26};
int greenled[4] = {27, 14, 12, 13};
long int totaltime[4];


void setup() {
  Serial.begin(9600);
  pinMode(irsensor, INPUT);
  pinMode(motor, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(redled[i], OUTPUT);
    pinMode(greenled[i], OUTPUT);
  }

  lcd.init();
  lcd.clear();
  lcd.backlight();

  byte customChar0[8] = {0b00010, 0b00100, 0b01000, 0b11111, 0b01000, 0b00100, 0b00010, 0b00001};
  byte customChar1[8] = {0b01000, 0b00100, 0b00010, 0b11111, 0b00010, 0b00100, 0b01000, 0b10000};
  byte customChar2[8] = {0b00100, 0b01110, 0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100};
  byte customChar3[8] = {0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b11111, 0b01110, 0b00100};

  lcd.createChar(0, customChar0);
  lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);
  lcd.createChar(3, customChar3);

  lcd.setCursor(0, 0);
  lcd.print("Adaptive traffic");
  lcd.setCursor(0, 1);
  lcd.print("light management");
  delay(5000);
  lcd.clear();

  for (int i = 0; i < 4; i++) {
    trafficIntensity[i] = random(100);
    Serial.print("Random number for Route ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(trafficIntensity[i]);
  }

  findMaxTrafficRoute();
}
void gettraffic() {
  lcd.clear();

  lcd.setCursor(0, 0);
  // print message
  lcd.print("Route 1 ");
  // string s=to_string(trafficIntensity[0])+" "+to_string(totaltime[0]);
  lcd.print(trafficIntensity[0]);
  lcd.print("  ");
  lcd.print(totaltime[0]);
  // clears the display to print new message

  // set cursor to first column, second row
  lcd.setCursor(0, 1);
  lcd.print("Route 2 ");
  // s=to_string(trafficIntensity[1])+" "+to_string(totaltime[1]);
  lcd.print(trafficIntensity[1]);
  lcd.print("  ");
  lcd.print(totaltime[1]);

  lcd.setCursor(-4, 2);
  lcd.print("Route 3 ");
  // s=to_string(trafficIntensity[2])+" "+to_string(totaltime[2]);
  lcd.print(trafficIntensity[2]);
  lcd.print("  ");
  lcd.print(totaltime[2]);

  lcd.setCursor(-4, 3);
  lcd.print("Route 4 ");
  // s=to_string(trafficIntensity[3])+" "+to_string(totaltime[3]);
  lcd.print(trafficIntensity[3]);
  lcd.print("  ");
  lcd.print(totaltime[3]);
  delay(5000);
}
void timeCalculate(){
  // long int total=trafficIntensity[0]+trafficIntensity[1]+trafficIntensity[2]+trafficIntensity[3];
  for(int i=0;i<4;i++){
    totaltime[i]=trafficIntensity[i]*(240)/100;
  }
}

void datafetch(){
  lcd.setCursor(0, 1);
  lcd.print("Get the traffic ");

  lcd.setCursor(-4, 2);
  lcd.print("data from the ");

  lcd.setCursor(-4, 3);
  lcd.print("google api");
  delay(3000);


}

void loop() {
if(cnt%4==0){
    for(int j=0;j<4;j++){
        trafficIntensity[j]=trafficval[p][j];
        timeCalculate();

    }
    // trafficIntensity=trafficval[p];

    p+=1;
    p=p%3;
    datafetch();
    gettraffic();
}
cnt+=1;

  updateTrafficLights();
  findMaxTrafficRoute() ;
//   delay(10000);
}

void findMaxTrafficRoute() {
//   long int maxValue = trafficIntensity[0];
//   for (int i = 1; i < 4; i++) {
//     if (trafficIntensity[i] > maxValue) {
//       maxValue = trafficIntensity[i];
//       maxIndex = i;
//     }
//   }

    maxIndex+=1;
    maxIndex=maxIndex%4;
    
  Serial.print("Maximum traffic route: ");
  Serial.println(maxIndex + 1);
}
void displayTimer(){
    int k=0;
    int xx=totaltime[maxIndex];
    while (k < xx) {
      lcd.setCursor(6, 3);
      //lcd.print("Route 1  ");
      // up();  // print the custom char at the desired position
      lcd.print(xx - k);
      Serial.println("Route 1 is selected");
      k = k + 1;
      delay(1000);
    }
}


void updateTrafficLights() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    if (i == maxIndex) {
      digitalWrite(greenled[i], HIGH);
      digitalWrite(redled[i], LOW);
    } else {
      digitalWrite(greenled[i], LOW);
      digitalWrite(redled[i], HIGH);
    }
  }
  displayTimer();


//   lcd.setCursor(0, 0);
//   lcd.print("Route 1: ");
//   lcd.print(trafficIntensity[0]);

//   lcd.setCursor(0, 1);
//   lcd.print("Route 2: ");
//   lcd.print(trafficIntensity[1]);

//   lcd.setCursor(0, 2);
//   lcd.print("Route 3: ");
//   lcd.print(trafficIntensity[2]);

//   lcd.setCursor(0, 3);
//   lcd.print("Route 4: ");
//   lcd.print(trafficIntensity[3]);
}