#include "esp_camera.h"
#include <WiFi.h>

#define CAMERA_MODEL_AI_THINKER
//------------------------------------------------
#define RELAY_PIN 15
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define I2C_SDA 13
#define I2C_SCL 14
LiquidCrystal_I2C lcd(0x27, 16, 2); 
//------------------------------------------------


#include "camera_pins.h"

const char* ssid = "Bank";
const char* password = "12345678";

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
//------------------------------------------------
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();          
  lcd.backlight();    
  lcd.clear();         
  
  
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  
  lcd.setCursor(0, 0); 
  lcd.print("Power ON"); 

//------------------------------------------------

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    //------------------------------------------------
  while(true){
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Error!");
  }
    //------------------------------------------------
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_CIF);



  WiFi.begin(ssid, password);
  //------------------------------------------------
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  int count = 0;
  //------------------------------------------------
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    //------------------------------------------------
    int dots = (count % 3) + 1; 
    
    lcd.setCursor(10, 0); 
    
    if (dots == 1) lcd.print(".  "); 
    else if (dots == 2) lcd.print(".. "); 
    else if (dots == 3) lcd.print("..."); 
    
    count++;
    //------------------------------------------------
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
  //------------------------------------------------
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);      
  lcd.print(WiFi.localIP());
  //------------------------------------------------
}

void loop() {
  //------------------------------------------------
  
  //------------------------------------------------
}

