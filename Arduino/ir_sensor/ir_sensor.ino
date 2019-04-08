 int LED = 13; // Use the onboard Uno LED
int isObstaclePin = A0; // This is our input pin
int isObstacle = HIGH; // HIGH MEANS NO OBSTACLE

int isObstaclePin2 = A1; // This is our input pin
int isObstacle2 = HIGH; // HIGH MEANS NO OBSTACLE

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(isObstaclePin, INPUT);
    Serial.begin(9600);
}

void loop() {
    isObstacle = digitalRead(isObstaclePin);
    isObstacle2 = digitalRead(isObstaclePin2);    
    if (isObstacle == LOW) {
        Serial.println("1: OBSTACLE!!");
        digitalWrite(LED, HIGH);
    } 
    if (isObstacle == HIGH) {
        Serial.println("clear1");
        digitalWrite(LED, LOW);
    }
        if (isObstacle2 == LOW) {
        Serial.println("2: OBSTACLE!!");
        digitalWrite(LED, HIGH);
    } 
    if (isObstacle2 == HIGH) {
        Serial.println("clear2");
        digitalWrite(LED, LOW);
    }
    delay(100);
}
