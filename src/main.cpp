#include <Arduino.h>
#include <PS4Controller.h>
#define MOTOR_1_A 2
#define MOTOR_1_B 15
#define MOTOR_2_A 13
#define MOTOR_2_B 12
#define FORWARD_CHANNEL 1
#define BACKWARD_CHANNEL 2
#define MOTOR_FREQUENCY 5000
#define MOTOR_RESOLUTION 8

#define FRONT_LIGHT_1 21
#define FRONT_LIGHT_2 22
#define FRONT_LIGHT_CHANNEL 3

uint8_t front_light_power = 0;

void setup()
{
  Serial.begin(115200);
  Serial.printf("Hello world\n");

  PS4.begin((char *)"b0:be:83:7f:20:2d");
  pinMode(MOTOR_1_A, OUTPUT);
  pinMode(MOTOR_1_B, OUTPUT);

  ledcSetup(FORWARD_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
  ledcSetup(BACKWARD_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
  ledcSetup(FRONT_LIGHT_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
  ledcAttachPin(MOTOR_2_B, FORWARD_CHANNEL);
  ledcAttachPin(MOTOR_2_A, BACKWARD_CHANNEL);
  ledcAttachPin(FRONT_LIGHT_1, FRONT_LIGHT_CHANNEL);
  ledcAttachPin(FRONT_LIGHT_2, FRONT_LIGHT_CHANNEL);
}

void test_loop()
{
  digitalWrite(MOTOR_1_A, HIGH);
  digitalWrite(MOTOR_1_B, LOW);
  delay(500);
  digitalWrite(MOTOR_1_A, LOW);
  digitalWrite(MOTOR_1_B, HIGH);
  delay(500);
  digitalWrite(MOTOR_1_A, LOW);
  digitalWrite(MOTOR_1_B, LOW);
  delay(500);

  for (int dutyCycle = 0; dutyCycle <= 30; dutyCycle++)
  {
    ledcWrite(FORWARD_CHANNEL, dutyCycle);
    delay(65);
  }
  ledcWrite(FORWARD_CHANNEL, 0);

  for (int dutyCycle = 0; dutyCycle <= 30; dutyCycle++)
  {
    // changing the LED brightness with PWM
    ledcWrite(BACKWARD_CHANNEL, dutyCycle);
    delay(65);
  }
  ledcWrite(BACKWARD_CHANNEL, 0);

  delay(500);
}

int r = 255;
int g = 0;
int b = 0;
// Calculates the next value in a rainbow sequence
void nextRainbowColor()
{
  if (r > 0 && b == 0)
  {
    r--;
    g++;
  }
  if (g > 0 && r == 0)
  {
    g--;
    b++;
  }
  if (b > 0 && g == 0)
  {
    r++;
    b--;
  }
}

void loop()
{

  if (PS4.isConnected())
  {

    ledcWrite(FORWARD_CHANNEL, PS4.R2Value());
    ledcWrite(BACKWARD_CHANNEL, PS4.L2Value());

    digitalWrite(MOTOR_1_A, PS4.Left() ? HIGH : LOW);
    digitalWrite(MOTOR_1_B, PS4.Right() ? HIGH : LOW);
    ledcWrite(FRONT_LIGHT_CHANNEL, front_light_power);

    if (PS4.Circle() && front_light_power < 254)
    {
      front_light_power++;
    }
    if (PS4.Cross() && front_light_power > 0)
    {
      front_light_power--;
    }

    Serial.printf("INPUTS : TRIG %d BATT %d\n", PS4.R2Value(), PS4.Battery());
    // nextRainbowColor();
    //     PS4.setLed(r, g, b);
    //     // PS4.setRumble(PS4.RStickY(), PS4.RStickX());
    //     PS4.sendToController();
    //     // PS4.setLed(r, g, b);
    delay(10);
  }
}