*各個腳位設定*
// 紅外線感測器腳位
const int IR_L = 8;
const int IR_R = 9;
// 馬達控制腳位
const int ENA = 3;
const int IN1 = 2;
const int IN2 = 4;
const int ENB = 5;
const int IN3 = 6;
const int IN4 = 7;
//LED 燈腳位
const int LED_L = 12;
const int LED_R = 13;
// HC-SR04 超音波感測器腳位
const int trigPin = 10;
const int echoPin = 11;
*速度設定*
int leftSpeed = 90;
int rightSpeed = 90;
int rotateSpeedBuff = 40;
int RrightSpeed;
int RleftSpeed;
*設定*
void setup() {
// 紅外線感測器設定
pinMode(IR_L, INPUT);
pinMode(IR_R, INPUT);
RrightSpeed = rightSpeed + rotateSpeedBuff;
RleftSpeed = leftSpeed + rotateSpeedBuff;
// 馬達控制腳位
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
//燈控制腳位
pinMode(LED_L, OUTPUT);
pinMode(LED_R, OUTPUT);
// 超音波腳位
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
Serial.println("循機車啟動");
}
*馬達函式*
//前進
void moveForward(int speedL,int speedR) {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, speedL);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, speedR);
Serial.println("動作:前進");
}
//左轉
void turnLeft(int speedL,int speedR) {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, speedL);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, speedR);
Serial.println("動作:左轉修正");
}
//右轉
void turnRight(int speedL,int speedR) {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, speedL);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, speedR);
Serial.println("動作:右轉修正");
}
//停止
void stopMotors() {
analogWrite(ENA, 0);
analogWrite(ENB, 0);
Serial.println("動作:停止");
}
//後退
void moveBackward(int speedL, int speedR) {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, speedL);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, speedR);
Serial.println("動作:後退");
}
void setup() {
// 紅外線感測器設定
pinMode(IR_L, INPUT);
pinMode(IR_R, INPUT);
RrightSpeed = rightSpeed + rotateSpeedBuff;
RleftSpeed = leftSpeed + rotateSpeedBuff;
// 馬達控制腳位
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
//燈控制腳位
pinMode(LED_L, OUTPUT);
pinMode(LED_R, OUTPUT);
// 超音波腳位
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
Serial.println("循機車啟動");
}
// 馬達函式
//前進
void moveForward(int speedL,int speedR) {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, speedL);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, speedR);
Serial.println("動作:前進");
}
//左轉
void turnLeft(int speedL,int speedR) {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, speedL);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, speedR);
Serial.println("動作:左轉修正");
}
//右轉
void turnRight(int speedL,int speedR) {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, speedL);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, speedR);
Serial.println("動作:右轉修正");
}
//停止
void stopMotors() {
analogWrite(ENA, 0);
analogWrite(ENB, 0);
Serial.println("動作:停止");
}
//後退
void moveBackward(int speedL, int speedR) {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, speedL);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, speedR);
Serial.println("動作:後退");
}
*迴圈*
void loop() {
int leftIR = digitalRead(IR_L);
int rightIR = digitalRead(IR_R);
float distance = getDistance();
//距離<15 led 燈閃爍三次
if (distance < 15) {
stopMotors();
setLED("obstacle");
Serial.println("前方障礙物，停止");
delay(500);
// 後退
moveBackward(leftSpeed, rightSpeed);
setLED("backward"); // 後退時閃爍 0.5 秒
stopMotors();
delay(200);
// 左轉閃避（固定短暫左轉）
turnLeft(RleftSpeed, RrightSpeed);
setLED("left");
delay(500);
stopMotors();
delay(200);
return; // 執行完閃避就回到 loop 開頭
}
Serial.print("距離: ");
Serial.print(distance);
Serial.print(" 紅外線 | 左 ");
Serial.print(leftIR == LOW ? "黑" : "白");
Serial.print(" /右 ");
Serial.println(rightIR == LOW ? "黑" : "白");
Serial.print("|");
if (leftIR == LOW && rightIR == LOW) {
moveForward(leftSpeed, rightSpeed);
setLED("forward"); // 兩燈亮
delay(40); // 更靈敏
stopMotors();
delay(10);
}
else if (leftIR == LOW && rightIR == HIGH) {
Serial.println("狀態:進入左轉模式");
while (true) {
turnLeft(RleftSpeed, RrightSpeed);
setLED("left"); // 左燈亮
delay(40);
stopMotors();
delay(20);
int l = digitalRead(IR_L);
int r = digitalRead(IR_R);
if (l == LOW && r == LOW || l == HIGH && r == HIGH) {
break;
}
}
}
else if (leftIR == HIGH && rightIR == LOW) {
Serial.println("狀態:進入右轉模式");
while (true) {
turnRight(RleftSpeed, RrightSpeed);
setLED("right"); // 右燈亮
delay(40);
stopMotors();
delay(20);
int l = digitalRead(IR_L);
int r = digitalRead(IR_R);
if (l == LOW && r == LOW || l == HIGH && r == HIGH) {
break;
}
}
}
else {
stopMotors();
setLED("stop"); // 全熄
}
delay(100);
}
*HC-SR04 距離計算*
float getDistance() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
long duration = pulseIn(echoPin, HIGH);
float distance = duration * 0.034 / 2.0; // cm
return distance;
}
//燈函式
void setLED(String status) {
if (status == "forward") {
digitalWrite(LED_L, HIGH);
digitalWrite(LED_R, HIGH);
} else if (status == "left") {
digitalWrite(LED_L, HIGH);
digitalWrite(LED_R, LOW);
} else if (status == "right") {
digitalWrite(LED_L, LOW);
digitalWrite(LED_R, HIGH);
} else if (status == "stop") {
digitalWrite(LED_L, LOW);
digitalWrite(LED_R, LOW);
} else if (status == "obstacle") {
for (int i = 0; i < 3; i++) {
digitalWrite(LED_L, HIGH);
digitalWrite(LED_R, HIGH);
delay(200);
digitalWrite(LED_L, LOW);
digitalWrite(LED_R, LOW);
delay(200);
}
}else if (status == "backward") {
unsigned long startTime = millis();
while (millis() - startTime < 500) { // 倒車時閃爍 0.5 秒
digitalWrite(LED_L, HIGH);
digitalWrite(LED_R, HIGH);
delay(150);
digitalWrite(LED_L, LOW);
digitalWrite(LED_R, LOW);
delay(150);
}
}
}