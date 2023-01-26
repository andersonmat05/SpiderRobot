// Spider Robot - Author: Umut Göler
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Type used to store starting time of animations
typedef unsigned long Timer;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"

// Enable or disable serial port output
#define DEBUG false
#define THRESHOLD (int)400
int TargetDirection = 0;

#pragma region Timers
Timer WalkTimer = 0;
Timer StartupTimer = 0;
Timer StandTimer = 0;
#pragma endregion

#pragma region Pins
#define BUZZER 11

#define F_MIC (int)A4
#define LB_MIC (int)A7
#define RB_MIC (int)A1

#define PIR (int)5

#pragma endregion

#pragma region Animation Data
// AUTO GENERATED - DO NOT EDIT WITHOUT PYTHON SCRIPT
const uint_least8_t anim_pose[] PROGMEM {
        1,
        0, 172,
        0, 97,
        0, 97,
        0, 82,
        0, 157,
        0, 157,
        0, 82,
        0, 157,
        0, 157,
        0, 172,
        0, 97,
        0, 97
};
const uint_least8_t anim_startup[] PROGMEM {
        30,
        0, 128, 8, 144, 10, 177, 13, 175, 16, 174, 19, 172, 29, 172,
        0, 21, 7, 19, 8, 0, 16, 1, 19, 43, 29, 97,
        0, 35, 7, 60, 8, 55, 10, 87, 13, 68, 16, 59, 19, 68, 29, 97,
        0, 126, 7, 89, 8, 77, 13, 79, 16, 80, 19, 82, 29, 82,
        0, 233, 4, 235, 7, 255, 16, 246, 19, 211, 29, 157,
        0, 219, 4, 194, 7, 184, 8, 167, 10, 179, 13, 191, 16, 194, 19, 187, 29, 157,
        0, 126, 13, 77, 16, 79, 19, 82, 29, 82,
        0, 233, 8, 239, 10, 235, 13, 255, 19, 211, 29, 157,
        0, 219, 8, 213, 10, 194, 13, 167, 16, 192, 19, 187, 29, 157,
        0, 128, 16, 177, 19, 172, 29, 172,
        0, 21, 13, 19, 16, 0, 19, 43, 29, 97,
        0, 35, 13, 60, 16, 87, 19, 68, 29, 97
};
const uint_least8_t anim_walk_forward[] PROGMEM {
        25,
        0, 168, 1, 165, 3, 160, 5, 154, 7, 147, 9, 140, 11, 133, 13, 124, 15, 117, 17, 110, 19, 103, 21, 140, 23, 171,
        24, 168,
        0, 99, 5, 100, 19, 100, 21, 0, 23, 99,
        0, 97, 1, 96, 3, 93, 5, 90, 7, 89, 11, 87, 17, 89, 19, 90, 21, 42, 23, 99, 24, 97,
        0, 86, 1, 89, 3, 94, 5, 100, 7, 107, 9, 77, 11, 58, 13, 60, 15, 65, 17, 69, 19, 73, 21, 77, 23, 83, 24, 86,
        0, 155, 5, 154, 9, 253, 11, 150, 13, 153, 17, 154, 19, 155,
        0, 157, 1, 158, 3, 161, 5, 164, 7, 165, 9, 191, 11, 126, 13, 133, 15, 138, 17, 143, 19, 148, 21, 153, 23, 155,
        24, 157,
        0, 86, 1, 83, 3, 77, 5, 73, 7, 69, 9, 65, 11, 60, 13, 58, 15, 77, 17, 107, 19, 100, 21, 94, 23, 89, 24, 86,
        0, 155, 7, 154, 9, 153, 13, 150, 15, 253, 17, 154, 21, 155,
        0, 157, 1, 155, 3, 153, 5, 148, 7, 143, 9, 138, 11, 133, 13, 126, 15, 191, 17, 165, 19, 164, 21, 161, 23, 158,
        24, 157,
        0, 168, 1, 171, 3, 140, 5, 103, 7, 110, 9, 117, 11, 124, 13, 133, 15, 140, 17, 147, 19, 154, 21, 160, 23, 165,
        24, 168,
        0, 99, 3, 0, 5, 100, 21, 99,
        0, 97, 1, 99, 3, 42, 5, 90, 7, 89, 9, 87, 15, 89, 19, 90, 21, 93, 23, 96, 24, 97
};
const uint_least8_t anim_turn_right[] PROGMEM {
        25,
        0, 172, 1, 175, 3, 181, 5, 187, 6, 189, 8, 195, 10, 202, 12, 208, 14, 213, 16, 219, 18, 225, 20, 196, 22, 167, 24, 172,
        0, 97, 20, 0, 22, 97, 24, 97,
        0, 97, 5, 96, 10, 97, 16, 99, 20, 49, 22, 97, 24, 97,
        0, 82, 1, 85, 3, 90, 5, 96, 6, 99, 8, 70, 10, 41, 12, 46, 14, 52, 16, 59, 18, 65, 20, 70, 22, 76, 24, 82,
        0, 157, 8, 255, 10, 157, 24, 157,
        0, 157, 3, 155, 6, 154, 8, 205, 10, 157, 16, 158, 20, 157, 24, 157,
        0, 82, 1, 85, 3, 55, 5, 26, 6, 29, 8, 35, 10, 41, 12, 46, 14, 52, 16, 59, 18, 65, 20, 70, 22, 76, 24, 82,
        0, 157, 3, 255, 5, 157, 24, 157,
        0, 157, 3, 205, 5, 154, 6, 155, 10, 157, 16, 158, 20, 157, 24, 157,
        0, 172, 1, 175, 3, 181, 5, 187, 6, 189, 8, 195, 10, 202, 12, 208, 14, 178, 16, 150, 18, 155, 20, 161, 22, 167, 24, 172,
        0, 97, 14, 0, 16, 97, 24, 97,
        0, 97, 5, 96, 10, 97, 14, 51, 16, 100, 20, 99, 22, 97, 24, 97
};
const uint_least8_t anim_turn_left[] PROGMEM {
        25,
        0, 172, 1, 170, 3, 164, 5, 158, 6, 155, 8, 150, 10, 144, 12, 140, 14, 134, 16, 128, 19, 121, 21, 147, 23, 175, 24, 172,
        0, 97, 12, 99, 21, 0, 23, 97, 24, 97,
        0, 97, 3, 99, 6, 100, 10, 102, 12, 103, 14, 104, 16, 106, 19, 109, 21, 56, 23, 97, 24, 97,
        0, 82, 1, 79, 3, 73, 5, 68, 6, 65, 8, 59, 10, 52, 12, 46, 14, 76, 16, 104, 19, 96, 21, 90, 23, 85, 24, 82,
        0, 157, 14, 255, 16, 157, 24, 157,
        0, 157, 5, 158, 10, 157, 14, 204, 16, 154, 19, 155, 23, 157, 24, 157,
        0, 82, 1, 79, 3, 73, 5, 68, 6, 65, 8, 94, 10, 120, 12, 114, 14, 110, 16, 104, 19, 96, 21, 90, 23, 85, 24, 82,
        0, 157, 8, 255, 10, 155, 14, 157, 24, 157,
        0, 157, 5, 158, 8, 201, 10, 150, 12, 151, 14, 153, 16, 154, 19, 155, 23, 157, 24, 157,
        0, 172, 1, 170, 3, 199, 5, 228, 6, 225, 8, 219, 10, 213, 12, 208, 14, 202, 16, 195, 19, 187, 21, 181, 23, 175, 24, 172,
        0, 97, 3, 0, 5, 97, 24, 97,
        0, 97, 3, 49, 5, 100, 6, 99, 10, 97, 16, 96, 21, 97, 24, 97
};
const uint_least8_t anim_stand_up[] PROGMEM {
        10,
        0, 172, 9, 172,
        0, 97, 9, 178,
        0, 97, 9, 185,
        0, 82, 9, 82,
        0, 157, 9, 76,
        0, 157, 9, 69,
        0, 82, 9, 82,
        0, 157, 9, 76,
        0, 157, 9, 69,
        0, 172, 9, 172,
        0, 97, 9, 178,
        0, 97, 9, 185
};
const uint_least8_t anim_stand_down[] PROGMEM {
        10,
        0, 172, 9, 172,
        0, 178, 9, 97,
        0, 185, 9, 97,
        0, 82, 9, 82,
        0, 76, 9, 157,
        0, 69, 9, 157,
        0, 82, 9, 82,
        0, 76, 9, 157,
        0, 69, 9, 157,
        0, 172, 9, 172,
        0, 178, 9, 97,
        0, 185, 9, 97
};
#pragma endregion

#pragma region Motors
// Servo settings
#define SERVO_MIN  100
#define SERVO_MAX  500
#define SERVO_FREQ 50
// Driver object
Adafruit_PWMServoDriver PWMDriver = Adafruit_PWMServoDriver();
// Angles added to fix mechanical issues
const int_least8_t MotorFixDelta[12] = // stored in ram for fast access
        {-4, 3, 15,
         -8, 0, 0,
         -9, -5, -17,
         -14, 3, 5};
// Array to blend motor rotations
unsigned int MotorRotation[12]; // not using uint8 since all layers add together in here
#pragma endregion

// Timer settings
unsigned long PreviousMillis = 0;
#define TickInterval (unsigned long )40 // 25 frames per second

// Returns the pin equivalent of the motor index
int GetServoPin(int Index) {
    return Index + (Index / 3);
}

// Clamp the input between min and max
long Clamp(long const Value, long const Min, long const Max) {
    return ((Value < Min) ? Min : ((Value > Max) ? Max : Value));
}

// Rotate a motor connected to the PWMDriver
// 0: 0, 127: 90, 255: 180
void RotateMotor(int MotorIndex, uint8_t ByteAngle) {
    PWMDriver.setPWM(GetServoPin(MotorIndex), 0, map(
            Clamp(map(ByteAngle, 0, 256, 0, 181) + MotorFixDelta[MotorIndex], 0, 180),
            0, 180, SERVO_MIN, SERVO_MAX));
}

// Apply the rotations in the array
void RotateMotors(float Weights) {
    // use the rotations from array
    // divide motor rotations by total of weights than rotate each motor
    for (int i = 0; i < 12; ++i) {
        RotateMotor(i, (uint8_t) (MotorRotation[i] / Weights));
    }
    // clear out the list
    for (auto &a: MotorRotation) a = 0;
}

void BuzzerSOS() {
    // . . .
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

    delay(800);

    // _ _ _
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);

    delay(800);

    // . . .
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

    delay(1500);
}

// Calculates rotations and blends according to weight
void AnimTick(unsigned char const *Animation, int ArrayLength, unsigned long Time, float Weight = 1.f) {
    // do not edit the array if there is no weight
    if (Weight == 0.f) {
        return;
    }

    // calculate the frame from time
    uint8_t Frame = Time / TickInterval;
    uint8_t MotorKeys[12]; // array to keep starting indexes of each motor
    MotorKeys[0] = 2; // first motor always starts with 2

    // determine the starting index of key data for each motor and assign to the array
    uint8_t MotorIndex = 1;
    for (int i = 2; i <= ArrayLength; i += 2) {
        if (pgm_read_byte_near(&Animation[i - 1]) == 0) {
            if (i > 2) {
                MotorKeys[MotorIndex] = i;
                ++MotorIndex;
            }
        }
    }

    for (int Motor = 0; Motor < 12; ++Motor) {
        bool Exist = false;
        // storing as indexes result in less accesses to the array
        unsigned int PrevIndex = 127; // storing as array indexes
        unsigned int NextIndex = 127; // should work without init

        // loop through the animation for each motor (animlen + animlen + 1 accesses) (for 20 frames 123 cpu clocks)
        // accesses the whole array, can't carry to ram
        for (int i = MotorKeys[Motor]; i < pgm_read_byte_near(&Animation[0]) + MotorKeys[Motor]; i += 2) {
            // check if the frame data exists
            if (pgm_read_byte_near(&Animation[i - 1]) == Frame) {
                // we found data for current frame
                PrevIndex = i;
                Exist = true;
                // there is no need to keep searching
                break;
            } else if (i != ArrayLength) { // check if we are still in the array
                if (pgm_read_byte_near(&Animation[i + 1]) == 0) {
                    // if one frame data for motor, assume it exists
                    Exist = i - MotorKeys[Motor] == 0; // assigning a bool condition works (checked from docs)
                    if (Exist) {
                        PrevIndex = i;
                    }
                    // stop if we reach out to the end of animation data for this motor
                    break;
                }
            } else {
                // reached out of animation data, stop before reading unassociated data
                Exist = false;
                break;
            }
        }
        int Rotation;
        if (Exist) {
            // if frame data exists, rotate motors (prev frame used)
            Rotation = pgm_read_byte_near(&Animation[PrevIndex]);
        } else {
            // if frame data is not existent, search for previous and next frame data
            // no need to do a condition for first frame nor last frame
            // data should exist for first frame and loops handle the last frames
            for (int i = MotorKeys[Motor]; i < pgm_read_byte_near(&Animation[0]) + MotorKeys[Motor]; i += 2) {
                // start to end loop
                if (pgm_read_byte_near(&Animation[i - 1]) > Frame) {
                    NextIndex = i;
                    break;
                } else if (i != ArrayLength) { // check if we are still in the array
                    if (pgm_read_byte_near(&Animation[i + 1]) == 0) break;
                } else break;
            }
            for (int i = MotorKeys[Motor]; i < pgm_read_byte_near(&Animation[0]) + MotorKeys[Motor]; i += 2) {
                // start to end loop
                if (pgm_read_byte_near(&Animation[i - 1]) < Frame) {
                    PrevIndex = i;
                } else if (i != ArrayLength) { // check if we are still in the array
                    if (pgm_read_byte_near(&Animation[i + 1]) == 0) break;
                } else break;
            }
            // Liniear interpolation
            Rotation = (int) map((long) Time,
                                 pgm_read_byte_near(&Animation[PrevIndex - 1]) * TickInterval,
                                 pgm_read_byte_near(&Animation[NextIndex - 1]) * TickInterval,
                                 pgm_read_byte_near(&Animation[PrevIndex]), pgm_read_byte_near(&Animation[NextIndex]));
        }
        MotorRotation[Motor] += (unsigned int) (Rotation * Weight);
        // done for this motor
    }
    // done for all motors
}

void EssentialSensors() {
    if(digitalRead(PIR) == HIGH) {
        AnimTick(anim_pose, sizeof(anim_pose) / sizeof(anim_pose[0]), 0, 1.f);
        RotateMotors(1.f);
        while(true) {
            // endless loop
            BuzzerSOS();
        }
    }
}

// Ticks animation with time calculation (can use this as a base for locomotion loop)
void PlayMontage(unsigned char const *Animation, int ArrayLength, unsigned long &Timer) {
    Timer = millis();
    while (millis() - Timer < pgm_read_byte_near(&Animation[0]) * TickInterval) {
        EssentialSensors();
        float Layer1 = 1.f;
        AnimTick(Animation, ArrayLength, millis() - Timer, Layer1);
        RotateMotors(Layer1);
        delay(TickInterval);
    }
}

// Locate the source of the sound from 3 sensor values
int SoundLocate(int LB, int F, int RB) {
    // 0 front, 1 right, 2 back, 3 left

    if (LB - F > THRESHOLD || F - LB > THRESHOLD ||
        F - RB > THRESHOLD || RB - F > THRESHOLD ||
        RB - LB > THRESHOLD || LB - RB > THRESHOLD) {

        // Right sensor detected
        if (RB > (LB + F)/2) {
            return 1;
        }
        // Left sensor detected
        if (LB > (RB + F)/2) {
            return 3;
        }

        // Front sensor detected
        if(abs(LB - RB) < THRESHOLD) {
            if(F > (LB + RB)/2) {
                return 0;
            }
            if(F < (LB + RB)/2) {
                return 2;
            }
        }
    } else {
        return 0;
    }
    return 0;
}

// stand up and read sensors
void Listen() {
    delay(200);
    StandTimer = millis();
    while (millis() - StandTimer < pgm_read_byte_near(&anim_stand_up[0]) * TickInterval) {
        float Layer1 = 1.f;
        AnimTick(anim_stand_up, sizeof(anim_stand_up) / sizeof(anim_stand_up[0]), millis() - StandTimer, Layer1);
        RotateMotors(Layer1);
        delay(TickInterval);
    }
    for (int i = 0; i < 2; ++i) {
        digitalWrite(BUZZER, HIGH);
        delay(40);
        digitalWrite(BUZZER, LOW);
        delay(25);
    }
    delay(100);

    int Mic_LB = 0;
    int Mic_F = 0;
    int Mic_RB = 0;

    for (int i = 0; i < 50; ++i) {
        Mic_LB += analogRead(LB_MIC);
        Mic_F += analogRead(F_MIC);
        Mic_RB += analogRead(RB_MIC);
        delay(5);
    }

    TargetDirection = SoundLocate((int)Mic_LB / 50, (int)Mic_F / 50, (int)Mic_RB / 50);
    delay(100);
    for (int i = 0; i < 5; ++i) {
        digitalWrite(BUZZER, HIGH);
        delay(40);
        digitalWrite(BUZZER, LOW);
        delay(25);
    }

    delay(200);

    StandTimer = millis();
    while (millis() - StandTimer < pgm_read_byte_near(&anim_stand_down[0]) * TickInterval) {
        float Layer1 = 1.f;
        AnimTick(anim_stand_down, sizeof(anim_stand_down) / sizeof(anim_stand_down[0]), millis() - StandTimer, Layer1);
        RotateMotors(Layer1);
        delay(TickInterval);
    }
    delay(200);
}

#pragma region Framework

// Essential startup functions
void Init() {
    PWMDriver.begin();
    PWMDriver.setOscillatorFrequency(27000000);
    PWMDriver.setPWMFreq(SERVO_FREQ);
    pinMode(11, OUTPUT);
    pinMode(PIR, INPUT);
    // power on beep
    for (int i = 0; i < 9; ++i) {
        digitalWrite(BUZZER, HIGH);
        delay(40);
        digitalWrite(BUZZER, LOW);
        delay(25);
    }

    yield();
    digitalWrite(BUZZER, LOW);;
#if DEBUG
    Serial.begin(9600);
    Serial.println(F("Initialization complete"));
#endif
}

// Called every frame
void Tick() {
 // not used
}

void setup() {
    Init();
    delay(500);
    // init done beep

    AnimTick(anim_startup, sizeof(anim_startup) / sizeof(anim_startup[0]), 0, 1.f);
    RotateMotors(1.f);

    // Wait for PIR to heat up
    for (int i = 0; i < 3; ++i) {
        delay(2000);
        digitalWrite(BUZZER, HIGH);
        delay(60);
        digitalWrite(BUZZER, LOW);
    }

    for (int i = 0; i < 2; ++i) {
        digitalWrite(BUZZER, HIGH);
        delay(200);
        digitalWrite(BUZZER, LOW);
        delay(100);
    }
    StartupTimer = millis();
    while (millis() - StartupTimer < pgm_read_byte_near(&anim_startup[0]) * TickInterval) {
        float Layer1 = 1.f;
        AnimTick(anim_startup, sizeof(anim_startup) / sizeof(anim_startup[0]), millis() - StartupTimer, Layer1);
        RotateMotors(Layer1);
        delay(TickInterval);
    }

    AnimTick(anim_pose, sizeof(anim_pose) / sizeof(anim_pose[0]), 0, 1.f);
    RotateMotors(1.f);
    delay(1200);
}

void loop() {

    Listen();
    switch (TargetDirection) {
        // 0 front, 1 right, 2, back, 3 left
        case 0:
            for (int i = 0; i < 8; ++i) {

                PlayMontage(anim_walk_forward, sizeof(anim_walk_forward) / sizeof(anim_walk_forward[0]), WalkTimer);
            }
            break;
        case 1:
            for (int i = 0; i < 7; ++i) {
                PlayMontage(anim_turn_right, sizeof(anim_turn_right) / sizeof(anim_turn_right[0]), WalkTimer);
            }
            break;
        case 2:
            for (int i = 0; i < 11; ++i) {
                PlayMontage(anim_turn_right, sizeof(anim_turn_right) / sizeof(anim_turn_right[0]), WalkTimer);
            }
            break;
        case 3:
            for (int i = 0; i < 7; ++i) {
                PlayMontage(anim_turn_left, sizeof(anim_turn_left) / sizeof(anim_turn_left[0]), WalkTimer);
            }
            break;
        default:
            for (int i = 0; i < 8; ++i) {
                PlayMontage(anim_walk_forward, sizeof(anim_walk_forward) / sizeof(anim_walk_forward[0]), WalkTimer);
            }
            break;
    }

    /*
    const unsigned long CurrentMillis = millis();
    if ((unsigned long) (CurrentMillis - PreviousMillis) >= TickInterval) {
        Tick();
        PreviousMillis = CurrentMillis;
    }
     */
}

#pragma endregion
#pragma clang diagnostic pop
