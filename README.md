# 自走車專案

本專案結合紅外線循跡與超音波避障，讓 Arduino 控制的自走車能在黑線路徑上行駛並自動避開前方障礙物。
專案目標為綜合應用紅外線循跡模組與超音波測距模組，設計並實現一台具備循跡與簡易避障功能的自走車，能在指定賽道上自動前進、辨識障礙物並進行動作調整。
---

## 一、專案特色

- **循跡 (Line Following)**：雙紅外線感測器偵測黑線，依偏差自動前進、左轉、右轉。
- **避障 (Obstacle Avoidance)**：超音波模組測量前方距離，低於設定閾值時自動後退並轉向。
- **後退 (Reversing)**：遇障退避後，再進行方向修正後返回循跡。
- **LED 狀態指示**：前進、轉彎與停止時的指示燈提示。
- **甩尾**：偵測到前方有障礙物時，後退且甩尾。

---

## 二、硬體需求

- Arduino Uno (或相容開發板)
- L298N 雙 H 橋馬達驅動板
- 2x 紅外線循跡感測器 (IR modules)
- 1x HC-SR04 超音波感測器
- 2x 直流馬達 + 車輪套件
- 2x LED 指示燈
- 連接線、電源模組
- 

### 腳位對應

| 功能             | 腳位         |
| ---------------- | ------------ |
| ENA (左馬達 PWM) | D3           |
| IN1, IN2 (左輪)  | D2, D4       |
| ENB (右馬達 PWM) | D5           |
| IN3, IN4 (右輪)  | D6, D7       |
| IR_left, IR_right| D9, D8       |
| TRIG, ECHO       | D10, D11     |
| LED_LEFT, LED_RIGHT | D12, D13 |

---

## 三、軟體結構

```
sketch_apr8a/
├── 000.ino          // 主程式：初始化、主 Loop
├── planning.md      // 邏輯規劃文件
└── README.md        // 使用說明
```

### 核心函式

- `void moveForward(int speedL,int speedR) {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, speedL);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, speedR);
Serial.println("動作:前進");
}`：前進並亮雙燈
- `void turnLeft(int speedL,int speedR) {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, speedL);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, speedR);
Serial.println("動作:左轉修正");
}`、`//右轉
void turnRight(int speedL,int speedR) {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, speedL);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, speedR);
Serial.println("動作:右轉修正");
}`：原地轉彎並亮對應 LED
- `void moveBackward(int speedL, int speedR) {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, speedL);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, speedR);
Serial.println("動作:後退");
}
void setup() {
)`：後退並熄滅 LED
- `void stopMotors() {
analogWrite(ENA, 0);
analogWrite(ENB, 0);
Serial.println("動作:停止");
}`：停止四輪並熄滅 LED
- `float getDistance() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
long duration = pulseIn(echoPin, HIGH);
float distance = duration * 0.034 / 2.0; // cm
return distance;
}`：讀取超音波距離值

---

## 四、安裝與上傳

1. 下載或複製本專案至 Arduino IDE。
2. 選擇對應開發板 (Arduino Uno) 與序列埠。
3. 點擊「上傳」編譯並燒錄至主控板。

---

## 五、參數調整

- **MOTOR_SPEED**：前進速度 (0~200)
- **TURN_SPEED**：左右轉彎`delay(90)`旋轉`delay(40)`與後退速度`delay(200)`
- **距離閾值**：`if (distance < 15)` 中的 20 cm
- **延遲時間**：避障後退`delay(200)`與轉彎的 `delay(500)` 長度

---

## 六、運作邏輯與流程說明
1.避障邏輯
- 停止車輛，LED 閃爍三次警示。
- 車輛左轉閃避障礙物（左燈亮）。
- 轉彎時對應方向燈亮。
- 車輛後退（LED 閃爍 0.5 秒）。
- 執行完畢後回到主循環

2.轉彎邏輯（Turning）依據紅外線感測器判斷：
- 左 LOW、右 HIGH：進入左轉模式，持續左轉直到兩感測器狀態一致。
- 左 HIGH、右 LOW：進入右轉模式，持續右轉直到兩感測器狀態一致。
- 轉彎時對應方向燈亮。
  
3. 執行邏輯（Line Following)
- 兩紅外線感測器皆為 LOW（偵測到黑線）時，車輛直行。
- 直行時兩顆 LED 皆亮。
  
4.燈號邏輯（LED Indicators）
- 前進：兩燈亮。
- 左轉：左燈亮。
- 右轉：右燈亮。
- 停止：兩燈全熄。
- 遇障：兩燈閃爍三次。
- 後退：兩燈閃爍 0.5 秒。
  
5.後退邏輯（Backing Up）
- 僅於避障時觸發。
- 車輛後退一小段，LED 閃爍 0.5 秒。

## 七、遇到的問題
1.困難一
車子不會沿著地上的黑線跑1.
解決:
- 確認程式是否錯誤
- 確認巡跡模組是否正常運作
- 調整巡機模組靈敏度以及感測器的角度

2.困難二
左右輪不會同時轉動、轉動不順
解決:
- 因帶回家時下雨，懷疑是因為淋雨導致晶圓件損壞，因此換了新的 arduino 和
L298N，以及確認線路沒有因為移動而脫落
- 發現電池漏液，更換新的電池後明顯恢復正常

3.困難三
走起來很沒力
解決:
-檢查 PWM 設為適當數值(通常為 60、70 以上)
-使用三用電表測量電池電量是否充足

3.困難四
車子太快，導致沒偵測到黑線
解決:
-調低 leftSpeed, rightSpeed
-延長 delay()（但可能導致控制反而不精確）
-改寫 loop() 裡的邏輯：移除多餘的 delay()，用「即時偵測 + 控制」的方式
，控制邏輯變成流暢連續，不再暴衝
