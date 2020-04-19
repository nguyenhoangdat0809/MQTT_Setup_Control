#include <iNut.h>
iNut sensor;
static bool blink_flag = false;

#define LED 5 //chân 13 là chân LED 
void setup() {
  Serial.begin(9600); //bật baudrate ở mức 9600

  Serial.println("San sang nhan lenh");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); //Bật đèn LED sáng

  //Khai báo số lượng luồn cảm biến
  sensor.setup(8); //Sẽ có 08 luồn cảm biến
  sensor.addCommand("LED", ledFunction);
  sensor.setDefaultHandler(defaultHandler);
}

void loop() {
#define Blink_INIT 0
#define Blink_ON 1
#define Blink_Wating 2
#define Blink_OFF 3
  static int Blink_LED = Blink_INIT;
  static unsigned long Blink_timer = 0;
  sensor.setValue(0, digitalRead(LED)); //đọc giá trị của đèn LED
  //giá trị cho các luồn từ 1-7 là giá trị random
  ///Tại sao lại sử dụng random? Vì đây là thử nghiệm, bạn có thể thay thế giá trị random bằng một giá trị cảm biến hoặc trị số mà bạn mong muốn đồng bộ lên Internet!
  for (byte i = 1; i <= 7; i++) {
    unsigned long value = random() % 10000; //trả về giá trị random - "hên xui" - tự do
    sensor.setValue(i, value); //gán giá trị trên vào luồn
  }

  if (blink_flag) {
    static unsigned long timer = 0;
    if (millis() - timer > 100UL) {
      timer = millis();
      digitalWrite(LED, !digitalRead(LED));
    }
  }
  sensor.loop();
}

//thực hiện khi nhận lệnh LED
//gửi từ node RED xuống chú ý "LED", ví dụ muốn đèn sáng phải gửi lệnh là LED ON không phải mỗi ON không, chú ý viết hoa viết thường.
void ledFunction() {
  char *arg = sensor.next();
  Serial.print("trạng thái arg: ");
  Serial.println(arg);
  if (*arg == NULL)
    return;
  if (strcmp(arg, "ON") == 0) { //nếu tham số 1 là ON. LED ON. Thì đèn sáng
    digitalWrite(LED, HIGH);
    Serial.println("Bat den");
  } 
  else if (strcmp(arg, "OFF") == 0) { // LED OFF. Đèn tắt
    digitalWrite(LED, LOW);
    Serial.println("Tat den");   
  } 
  else if (strcmp(arg, "TOGGLE") == 0) { //Đảo trạng thái của LED
    digitalWrite(LED, !digitalRead(LED));
    Serial.println("Doi trang thai den");
  }
  else if (strcmp (arg, "BLINK") == 0) {
    blink_flag = true;
  } 
  else if (strcmp(arg, "BLINK_OFF") == 0)  {
    blink_flag = false;
  }
  else {
    Serial.println("Khong nam trong tap hop lenh");
  }
}


//Nếu nhận được lệnh không phải từ những lệnh được đăng ký trước thì sẽ ghi ra ở đây
void defaultHandler(const char *command) {
  Serial.print(F("---Not found handler: "));
  Serial.println(command);
  //Đoạn chương trình dưới chỉ dùng để debug, để các bạn biết chương trình của mình ở lỗi ở đâu để khắc phục, cũng không cần hiểu nha.
  int idx = 0;
  char *arg;
  while ((arg = sensor.next()) != NULL) { //còn tham số nào thì in ra
    Serial.print("-----> Gia tri thu #");
    Serial.print(idx);
    Serial.print(": ");
    Serial.println(arg);
    idx++;
  }
}
