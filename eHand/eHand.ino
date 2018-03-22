const bool DEBUG = false;
//debug NOT enabled by default, change to a 1 to enable

const int BAUDRATE = 115200;
//usb speed

int streamsize = 0;
byte checksum = 0;
byte bytestream[64];
//for checking data integrity and holding the incomming data

byte poor = 0;
byte attention = 0;
byte meditation = 0;
//eeg outputs





void setup() {
  Serial.begin(BAUDRATE);
}





byte ByteReader() {
//reads one byte, 170 is the sync byte, size must be less than 170
  
  byte readbyte;
  while(Serial.available() <= 0);
  readbyte = Serial.read();
  //waits until there is at least one byte and reads it

  if(DEBUG) {
    Serial.print("DEBUG, byte read: ");
    Serial.println((char)readbyte);
  }
  //prints the byte if in debug mode

  return readbyte;
}





void StreamReader() {
  if(ByteReader() == 170 and ByteReader() == 170) {
  //needs two sync bytes to continue
  
    streamsize = ByteReader();
    if(streamsize <= 169) {
    //then the next byte needs to be the byte stream size that must be 169 or less
    
      checksum = 0;
      //for verifying data integrity
        
      for(int i = 0; i < streamsize; i++) {
        bytestream[i] = ByteReader();
        checksum += bytestream[i];
      }
      //sequentially reads incomming data
      
      if(ByteReader() == 255 - checksum) {
        return bytestream;
      }
      //checks for data integrity
    }
  }
}





void loop() {  

  StreamReader();
  for(int i = 0; i < streamsize; i++) {
    switch (bytestream[i]) {
      case 2:
        i++;            
        poor = bytestream[i];
        break;
      case 4:
        i++;
        attention = bytestream[i];                        
        break;
      case 5:
        i++;
        meditation = bytestream[i];
        break;
      case 0x80:
        i = i + 3;
        break;
      case 0x83:
        i = i + 25;      
        break;
      default:
        break;
    }
  }
  
  if(DEBUG) {
    Serial.print("DEBUG, poor: ");
    Serial.println((char)poor);
    Serial.print("DEBUG, attention: ");
    Serial.println((char)attention);
    Serial.print("DEBUG, meditation: ");
    Serial.println((char)meditation);
  }
}
    


