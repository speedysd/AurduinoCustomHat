int const MICROPHONE_PIN = A1;
int const VOLUME_SUPRESS_RIGHT_PIN = 9;
int const VOLUME_SUPRESS_LEFT_PIN = 10;
int const VOLUME_SUPRESS_INDICATOR_PIN = 11;
int const VOLUME_SUPRESS_THRESHOLD_INDICATOR_PIN = 4;
int const VOLUME_SUPRESS_THRESHOLD_PIN = A5;
int const INERTIA_PIN = A4;
int inertiaValue = 0;
float inertia = 0;
int volumeSupressSupressThreshold = 75;
int soundLaudness = 0;
float averageSoundLaudness = 0;
float averageVolumeSupress = 0;

void setup() {
  Serial.begin(230400);
  pinMode(VOLUME_SUPRESS_THRESHOLD_INDICATOR_PIN, OUTPUT);
  pinMode(VOLUME_SUPRESS_RIGHT_PIN, OUTPUT);
  pinMode(VOLUME_SUPRESS_LEFT_PIN, OUTPUT);
  pinMode(VOLUME_SUPRESS_INDICATOR_PIN, OUTPUT);
  TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz pins: 9, 10
  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz pins: 3, 11
}

void loop() {
  int maxSoundValue = 0;
  int minSoundValue = 1023;
  for (int i = 0; i < 10; i++) {
    int soundValue = analogRead(MICROPHONE_PIN);
    maxSoundValue = max(maxSoundValue, soundValue);
    minSoundValue = min(minSoundValue, soundValue);
    printSoundValue(soundValue);
  }
  
  soundLaudness = maxSoundValue - minSoundValue;
  averageSoundLaudness = 
    averageSoundLaudness * inertia + soundLaudness * (1.0 - inertia);

  int volumeSupress = 0;
  inertiaValue = analogRead(INERTIA_PIN);
  inertia = (512 + 256 + 128 + 64 + 32 + inertiaValue/32) / 1024.0;
  volumeSupressSupressThreshold = analogRead(VOLUME_SUPRESS_THRESHOLD_PIN);
  if (averageSoundLaudness > volumeSupressSupressThreshold) {
    digitalWrite(VOLUME_SUPRESS_THRESHOLD_INDICATOR_PIN, HIGH);
    volumeSupress = min(255, (averageSoundLaudness - volumeSupressSupressThreshold));
  } else {
    digitalWrite(VOLUME_SUPRESS_THRESHOLD_INDICATOR_PIN, LOW);
  }
  averageVolumeSupress = 
    averageVolumeSupress * inertia + volumeSupress * (1.0 - inertia);
  analogWrite(VOLUME_SUPRESS_RIGHT_PIN, averageVolumeSupress);
  analogWrite(VOLUME_SUPRESS_LEFT_PIN, averageVolumeSupress);
  analogWrite(VOLUME_SUPRESS_INDICATOR_PIN, averageVolumeSupress);
}

void printSoundValue(int soundValue) {
  Serial.print(0);
  Serial.print(" ");
  Serial.print(soundValue);
  Serial.print(" ");
  Serial.print(soundLaudness);
  Serial.print(" ");
  Serial.print(averageSoundLaudness);
  Serial.print(" ");
  Serial.print(volumeSupressSupressThreshold);
  Serial.print(" ");
  Serial.print(averageVolumeSupress);
  Serial.print(" ");
  Serial.print(inertiaValue);
  Serial.print(" ");
  Serial.print(1023);
  Serial.println();
}
