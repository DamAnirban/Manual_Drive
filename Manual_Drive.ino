#include <Cytron_PS2Shield.h>

Cytron_PS2Shield ps2(2, 3);

#define DEBUG 0

//*********************************INITIALIZE_VALUES*************************************
#define nema_stepPin        5
#define nema_dirPin         4


#define trigPin_front_right A5
#define echoPin_front_right A4
#define trigPin_front_left  A2
#define echoPin_front_left  A3
#define trigPin_right_right   6
#define echoPin_right_right   7

#define reset_pin A1

float distance_front_right;
float distance_front_left;
float distance_right_right;

#define motor_pwm1 9
#define motor_pwm2 11
#define motor_dir1 8
#define motor_dir2 13
#define motor_dir3 12
#define motor_dir4 10
// --------- Motors ---------
//    2   1
//    3   4
//#define callibrationLed1 13
#define pwm_value 130
#define maxpwm_value 130
#define minpwm_value 30
#define calibpwm_value 40

#define minpwm_value_right 40
#define calibpwm_value_right 50

int js_triangle = 1,
    js_cross = 1,
    js_up = 1,
    js_dn = 1,
    js_left = 1,
    js_right = 1,
    js_circle = 1,
    js_square = 1,
    js_r1 = 1,
    js_r2 = 1,
    js_l1 = 1,
    js_start = 1,
    flag1 = 0,
    flag2 = 0,
    flag3 = 0;
//*********************************SETUP_PINS_SERIAL******************************************
void setup()
{
  digitalWrite(reset_pin, HIGH);
  pinMode(nema_stepPin, OUTPUT);
  pinMode(nema_dirPin, OUTPUT);
  pinMode(motor_pwm1, OUTPUT);
  pinMode(motor_pwm2, OUTPUT);
  pinMode(motor_dir1, OUTPUT);
  pinMode(motor_dir2, OUTPUT);
  pinMode(motor_dir3, OUTPUT);
  pinMode(motor_dir4, OUTPUT);
  pinMode(reset_pin, OUTPUT);

  pinMode(trigPin_front_right, OUTPUT);
  pinMode(echoPin_front_right, INPUT);
  pinMode(trigPin_front_left, OUTPUT);
  pinMode(echoPin_front_left, INPUT);

  ps2.begin(9600);
#if DEBUG
  Serial.begin(9600);
  Serial.println("Init");
#endif
  motorcontrol(5);
}

//**********************************PS2_DATA_READ*************************************
void readPS2()
{
#if DEBUG
  Serial.println(" :: readPS2 : PS2_reading");
#endif
  js_triangle = ps2.readButton(PS2_TRIANGLE);
  js_cross = ps2.readButton(PS2_CROSS);
  js_up = ps2.readButton(PS2_UP);
  js_dn = ps2.readButton(PS2_DOWN);
  js_right = ps2.readButton(PS2_RIGHT);
  js_left = ps2.readButton(PS2_LEFT);
  js_circle = ps2.readButton(PS2_CIRCLE);
  js_square = ps2.readButton(PS2_SQUARE);
  js_r1 = ps2.readButton(PS2_RIGHT_1);
  js_r2 = ps2.readButton(PS2_RIGHT_2);
  js_l1 = ps2.readButton(PS2_LEFT_1);
  js_start = ps2.readButton(PS2_START);
}

//**********************************NEMA_ROTATION****************************************
void nema()
{
#if DEBUG
  Serial.print(" :: nema : ");
#endif
  if (js_triangle == 0)
  {
#if DEBUG
    Serial.println("stepper clockwise");
#endif
    digitalWrite(nema_dirPin, HIGH);
    for (int x = 0; x < 100; x++)
    {
      digitalWrite(nema_stepPin, HIGH);
      delayMicroseconds(50);
      digitalWrite(nema_stepPin, LOW);
      delayMicroseconds(50);
      if ((js_triangle = ps2.readButton(PS2_TRIANGLE)) == 1)
        break;
    }
  }
  else if (js_cross == 0)
  {
#if DEBUG
    Serial.println("stepper anticlockwise");
#endif
    digitalWrite(nema_dirPin, LOW);
    for (int x = 0; x < 100; x++)
    {
      digitalWrite(nema_stepPin, HIGH);
      delayMicroseconds(50);
      digitalWrite(nema_stepPin, LOW);
      delayMicroseconds(50);
      if ((js_cross = ps2.readButton(PS2_CROSS)) == 1)
        break;
    }
  }
}

//**********************************MOTOR_CONTROL****************************************
#define pickupmillis 1000
unsigned long startmillis = 0;
float del = 0;
byte curpwm_value = 30, lastdir = 5;
void motorcontrol(int dir)
{
  if ((! (dir / 10) || ! (lastdir / 10) || dir / 10 == lastdir / 10) && dir != lastdir)
  {
    startmillis = millis();
  }


  del = float(millis() - startmillis) / pickupmillis;
#if DEBUG
  Serial.print(" :: motor_control : ");
#endif
  switch (dir)
  {
    case 8:
#if DEBUG
      Serial.println("Move Forward");
#endif
      curpwm_value = (del <= 1) ? (del * (maxpwm_value - minpwm_value) + minpwm_value) : (maxpwm_value);
      Serial.println(curpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, HIGH);
      digitalWrite(motor_dir2, HIGH);
      digitalWrite(motor_dir3, HIGH);
      digitalWrite(motor_dir4, HIGH);
      break;
    case 2:
#if DEBUG
      Serial.println("Move Backward");
#endif
      curpwm_value = (del <= 1) ? (del * (maxpwm_value - minpwm_value) + minpwm_value) : (maxpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, LOW);
      digitalWrite(motor_dir2, LOW);
      digitalWrite(motor_dir3, LOW);
      digitalWrite(motor_dir4, LOW);
      break;
    case 56:
#if DEBUG
      Serial.println("CW rotation");
#endif
      curpwm_value = (del <= 1) ? (del * (maxpwm_value - minpwm_value) + minpwm_value) : (maxpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, HIGH);
      digitalWrite(motor_dir2, LOW);
      digitalWrite(motor_dir3, LOW);
      digitalWrite(motor_dir4, HIGH);

      break;
    case 54:
#if DEBUG
      Serial.println("CCW rotation");
#endif
      curpwm_value = (del <= 1) ? (del * (maxpwm_value - minpwm_value) + minpwm_value) : (maxpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, LOW);
      digitalWrite(motor_dir2, HIGH);
      digitalWrite(motor_dir3, HIGH);
      digitalWrite(motor_dir4, LOW);

      break;
    case 6:
#if DEBUG
      Serial.println("move right");
#endif
      curpwm_value = (del <= 1) ? (del * (maxpwm_value - minpwm_value) + minpwm_value) : (maxpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, LOW);
      digitalWrite(motor_dir2, HIGH);
      digitalWrite(motor_dir3, LOW);
      digitalWrite(motor_dir4, HIGH);

      break;
    case 4:
#if DEBUG
      Serial.println("move left");
#endif
      curpwm_value = (del <= 1) ? (del * (maxpwm_value - minpwm_value) + minpwm_value) : (maxpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, HIGH);
      digitalWrite(motor_dir2, LOW);
      digitalWrite(motor_dir3, HIGH);
      digitalWrite(motor_dir4, LOW);
      break;
    case 5:
#if DEBUG
      Serial.println("STOP");
#endif
      analogWrite(motor_pwm1, 0);
      analogWrite(motor_pwm2, 0);
      digitalWrite(motor_dir1, LOW);
      digitalWrite(motor_dir2, LOW);
      digitalWrite(motor_dir3, LOW);
      digitalWrite(motor_dir4, LOW);
      break;
    case 62:
#if DEBUG
      Serial.println("move right backward");
#endif
      curpwm_value = (del <= 1) ? (del * (calibpwm_value - minpwm_value) + minpwm_value) : (calibpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir2, LOW);
      digitalWrite(motor_dir3, LOW);
      break;
    case 68:
#if DEBUG
      Serial.println("move right forward");
#endif
      curpwm_value = (del <= 1) ? (del * (calibpwm_value - minpwm_value) + minpwm_value) : (calibpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir2, HIGH);
      digitalWrite(motor_dir3, HIGH);
      break;
    case 42:
#if DEBUG
      Serial.println("move left backward");
#endif
      curpwm_value = (del <= 1) ? (del * (calibpwm_value - minpwm_value) + minpwm_value) : (calibpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      digitalWrite(motor_dir1, LOW);
      digitalWrite(motor_dir4, LOW);
      break;
    case 48:
#if DEBUG
      Serial.println("move left forward");
#endif
      curpwm_value = (del <= 1) ? (del * (calibpwm_value - minpwm_value) + minpwm_value) : (calibpwm_value);
      analogWrite(motor_pwm1, curpwm_value);
      digitalWrite(motor_dir1, HIGH);
      digitalWrite(motor_dir4, HIGH);
      break;
    //////////////////////////////////////////////////
    case 66:
#if DEBUG
      Serial.println("move right right");
#endif
      curpwm_value = (del <= 1) ? (del * (calibpwm_value_right - minpwm_value_right) + minpwm_value_right) : (calibpwm_value_right);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, LOW);
      digitalWrite(motor_dir2, HIGH);
      digitalWrite(motor_dir3, LOW);
      digitalWrite(motor_dir4, HIGH);
      break;
    case 44:
#if DEBUG
      Serial.println("move left left");
#endif
      curpwm_value
      = (del <= 1) ? (del * (calibpwm_value_right - minpwm_value_right) + minpwm_value_right) : (calibpwm_value_right);
      analogWrite(motor_pwm1, curpwm_value);
      analogWrite(motor_pwm2, curpwm_value);
      digitalWrite(motor_dir1, HIGH);
      digitalWrite(motor_dir2, LOW);
      digitalWrite(motor_dir3, HIGH);
      digitalWrite(motor_dir4, LOW);
      break;
    //////////////////////////////////////////////
    case 45:
#if DEBUG
      Serial.println("stop left");
#endif
      analogWrite(motor_pwm2, 0);
      break;
    case 65:
#if DEBUG
      Serial.println("stop right");
#endif
      analogWrite(motor_pwm1, 0);
      break;
  }
  lastdir = dir;
}

//*****************************DISTANCE_CALCULATE*************************************
void ultra_distance(int z)
{
  int c = z;
  if (c == 1) // front right
  {
    digitalWrite(trigPin_front_right, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin_front_right, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_front_right, LOW);

    distance_front_right = pulseIn(echoPin_front_right, HIGH) * 0.017;
#if DEBUG
    Serial.print(" :: ultra_distance : Distance_front_right: ");
    Serial.println(distance_front_right);
#endif
  }
  if (c == 2) // front left
  {
    digitalWrite(trigPin_front_left, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin_front_left, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_front_left, LOW);

    distance_front_left = pulseIn(echoPin_front_left, HIGH) * 0.017;
#if DEBUG
    Serial.print(" :: ultra_distance : Distance_front_left: ");
    Serial.println(distance_front_left);
#endif
  }
  if (c == 3) // left
  {
    digitalWrite(trigPin_right_right, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin_right_right, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_right_right, LOW);

    distance_right_right = pulseIn(echoPin_right_right, HIGH) * 0.017;
#if DEBUG
    Serial.print(" :: ultra_distance : distance_right_right: ");
    Serial.println(distance_right_right);
#endif
  }
}

//*****************************MANUAL_CALLIBRATE**************************************
bool done_front = 0;
bool done_right = 0;
bool done_front1 = 0;
void manual_callibrate_normal_first()
{
#if DEBUG
  Serial.println("manual callibration front");
#endif
  //digitalWrite(callibrationLed1, HIGH);
  ultra_distance(1);
  ultra_distance(2);
  ultra_distance(3);
  
  ///////////////////////////////////////////////////////////////////first front///////////////////////////////////////////////////////////////////
  while( !done_front && !done_right &&!done_front1)
  {
     ultra_distance(1);
     ultra_distance(2);
    if (distance_front_left >= 66 && distance_front_left <= 200)
      motorcontrol(48);
    else if (distance_front_left <= 64)
      motorcontrol(42);
    else
    {
      motorcontrol(45);
    }
    if (distance_front_right >= 66 && distance_front_right <= 200)
      motorcontrol(68);
    else if (distance_front_right <= 64)
      motorcontrol(62);
    else
    {
      motorcontrol(65);
    }

    ultra_distance(1);
    ultra_distance(2);
    if (distance_front_left >= 64 && distance_front_left <= 66 && distance_front_right >= 64 && distance_front_right <= 66)
      done_front = 1;
    else
      done_front = 0;

    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************

  }

///////////////////////////////////////////////////right/////////////////////////////////////////////////////    

 while(!done_right &&!done_front1 && done_front)
 {  
    ultra_distance(3);
    if (distance_right_right >= 33 && distance_right_right <= 100)
      motorcontrol(66);
    else if (distance_right_right <= 31)
      motorcontrol(44);
    else
    {
      motorcontrol(5);
    }
    ultra_distance(3);
    //delay(100);
    if (distance_right_right >= 31 && distance_right_right <= 33)
      done_right = 1;
    else
      done_right = 0;
    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************

  }

  //////////////////////////////////////////////////second front////////////////////////////////////////////////////////////////////////
  while(!done_front1 && done_front && done_right)
  { 
    ultra_distance(1);
    ultra_distance(2);
    if (distance_front_left >= 64 && distance_front_left <= 200)
      motorcontrol(48);
    else if (distance_front_left <= 62)
      motorcontrol(42);
    else
    {
      motorcontrol(45);
    }

    if (distance_front_right >= 64 && distance_front_right <= 200)
      motorcontrol(68);
    else if (distance_front_right <= 62)
      motorcontrol(62);
    else
    {
      motorcontrol(65);
    }
    
    ultra_distance(1);
    ultra_distance(2);
    //delay(100);
    if (distance_front_left >= 62 && distance_front_left <= 64 && distance_front_right >= 62 && distance_front_right <= 64)
      done_front1 = 1;
    else
      done_front1 = 0;
    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (distance_front_left >= 62 && distance_front_left <= 64 && distance_front_right >= 62 && distance_front_right <= 64 && distance_right_right >= 31 && distance_right_right <= 33)
  {
    done_front = 0;
    done_right = 0;
    done_front1 = 0;
  }
}
void manual_callibrate_normal_second()
{
#if DEBUG
  Serial.println("manual callibration front");
#endif
  //digitalWrite(callibrationLed1, HIGH);
  ultra_distance(1);
  ultra_distance(2);
  ultra_distance(3);
  ///////////////////////////////////////////////////////////////////first front///////////////////////////////////////////////////////////////////
  while ( !done_front && !done_right)
  {
    if (distance_front_left >= 36 && distance_front_left <= 200)
      motorcontrol(48);
    else if (distance_front_left <= 34)
      motorcontrol(42);
    else
    {
      motorcontrol(45);
    }
    if (distance_front_right >= 36 && distance_front_right <= 200)
      motorcontrol(68);
    else if (distance_front_right <= 34)
      motorcontrol(62);
    else
    {
      motorcontrol(65);
    }

    ultra_distance(1);
    ultra_distance(2);
    if (distance_front_left >= 34 && distance_front_left <= 36 && distance_front_right >= 34 && distance_front_right <= 36)
      done_front = 1;
    else
      done_front = 0;

    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************

  }


  ///////////////////////////////////////////////////right///////////////////////////////////////////////////////////////////////
  while (!done_right && done_front)
  {
    if (distance_right_right >= 28 && distance_right_right <= 50)
      motorcontrol(44);
    else if (distance_right_right <= 26)
      motorcontrol(66);
    else
    {
      motorcontrol(5);
    }
    ultra_distance(3);
    if (distance_right_right >= 26 && distance_right_right <= 28)
      done_right = 1;
    else
      done_right = 0;
    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************

  }

  //////////////////////////////////////////////////second front////////////////////////////////////////////////////////////////////////
  while (!done_front1 && done_front && done_right)
  {
    if (distance_front_left >= 27 && distance_front_left <= 200)
      motorcontrol(48);
    else if (distance_front_left <= 25)
      motorcontrol(42);
    else
    {
      motorcontrol(45);
    }

    if (distance_front_right >= 27 && distance_front_right <= 200)
      motorcontrol(68);
    else if (distance_front_right <= 25)
      motorcontrol(62);
    else
    {
      motorcontrol(65);
    }
    ultra_distance(1);
    ultra_distance(2);
    if (distance_front_left >= 25 && distance_front_left <= 27 && distance_front_right >= 25 && distance_front_right <= 27)
      done_front1 = 1;
    else
      done_front1 = 0;
    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (distance_front_left >= 25 && distance_front_left <= 27 && distance_front_right >= 25 && distance_front_right <= 27 && distance_right_right >= 26 && distance_right_right <= 28)
  {
    done_front = 0;
    done_right = 0;
    done_front1 = 0;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void manual_callibrate_gold()
{
#if DEBUG
  Serial.println("manual callibration front");
#endif
  //digitalWrite(callibrationLed1, HIGH);
  ultra_distance(1);
  ultra_distance(2);
  ultra_distance(3);
  ///////////////////////////////////////////////////////////////////first front///////////////////////////////////////////////////////////////////
  while ( !done_front && !done_right)
  {
    if (distance_front_left >= 56 && distance_front_left <= 200)
      motorcontrol(48);
    else if (distance_front_left <= 54)
      motorcontrol(42);
    else
    {
      motorcontrol(45);
    }
    if (distance_front_right >= 56 && distance_front_right <= 200)
      motorcontrol(68);
    else if (distance_front_right <= 54)
      motorcontrol(62);
    else
    {
      motorcontrol(65);
    }

    ultra_distance(1);
    ultra_distance(2);
    if (distance_front_left >= 54 && distance_front_left <= 56 && distance_front_right >= 54 && distance_front_right <= 56)
      done_front = 1;
    else
      done_front = 0;

    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************

  }


  ///////////////////////////////////////////////////right///////////////////////////////////////////////////////////////////////
  while (!done_right && done_front)
  {
    if (distance_right_right >= 58 && distance_right_right <= 100)
      motorcontrol(44);
    else if (distance_right_right <= 56)
      motorcontrol(66);
    else
    {
      motorcontrol(5);
    }
    ultra_distance(3);
    if (distance_right_right >= 56 && distance_right_right <= 58)
      done_right = 1;
    else
      done_right = 0;
    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************

  }

  //////////////////////////////////////////////////second front////////////////////////////////////////////////////////////////////////
  while (!done_front1 && done_front && done_right)
  {
    if (distance_front_left >= 47 && distance_front_left <= 200)
      motorcontrol(48);
    else if (distance_front_left <= 45)
      motorcontrol(42);
    else
    {
      motorcontrol(45);
    }

    if (distance_front_right >= 47 && distance_front_right <= 200)
      motorcontrol(68);
    else if (distance_front_right <= 45)
      motorcontrol(62);
    else
    {
      motorcontrol(65);
    }
    ultra_distance(1);
    ultra_distance(2);
    if (distance_front_left >= 45 && distance_front_left <= 47 && distance_front_right >= 45 && distance_front_right <= 47)
      done_front1 = 1;
    else
      done_front1 = 0;
    //***********************MANUAL RESET***********************
    js_start = ps2.readButton(PS2_START);
    if (js_start == 0)
    {
      digitalWrite(reset_pin, LOW);
    }
    //**********************************************************
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (distance_front_left >= 45 && distance_front_left <= 47 && distance_front_right >= 45 && distance_front_right <= 47 && distance_right_right >= 56 && distance_right_right <= 58)
  {
    done_front = 0;
    done_right = 0;
    done_front1 = 0;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//*********************************MAIN_LOOP*******************************************
void loop()
{
  readPS2(); //reads the buttons of PS2 controller
  nema();
  if (js_up == 0)
  {
    motorcontrol(8); //forward
  }
  else if (js_dn == 0)
  {
    motorcontrol(2); //backward
  }
  else if (js_right == 0)
  {
    motorcontrol(56); //right rotation
  }
  else if (js_left == 0)
  {
    motorcontrol(54); //left rotation
  }
  else if (js_circle == 0)
  {
    motorcontrol(6); //right movement
  }
  else if (js_square == 0)
  {
    motorcontrol(4); //left movement
  }
  else if (js_r1 == 0)
  {
    manual_callibrate_normal_first();
  }
  else if (js_r2 == 0)
  {
    manual_callibrate_normal_second();
  }
  else if (js_l1 == 0)
  {
    manual_callibrate_gold();
  }
  else if (js_start == 0)
  {
#if DEBUG
    Serial.println("*******************Reset******************");
#endif
    digitalWrite(reset_pin, LOW);
  }
  else
  {
    motorcontrol(5);
  }
#if DEBUG
  Serial.println(" : ------------------------- LOOP ENDED -------");
#endif
}
