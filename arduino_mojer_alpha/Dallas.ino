 /////////////////////////////////////////////////////////////программа   датчиков DALAS 
int DALASfunction(){
byte i;
byte present = 0;
byte data[12];
byte addr[8];
int Temp;
 int Var=0;
  char topic;
  
  if ( !ds.search(addr)) {
        //Serial.print("No more addresses.\n");
        ds.reset_search();
        return;
  }
  Serial.print("R==");  //R=28 Not sure what this is
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
    Var = Var+(addr[i]);
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n");
        return;
  }
  if ( addr[0] != 0x28) {
        Serial.print("Device is not a DS18S20 family device.\n");
        return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);        // start conversion, with parasite power on at the end
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);          // Read Scratchpad
  Serial.print("P===");
  Serial.print(present,HEX);
  Serial.print(" ");
  
  for ( i = 0; i < 9; i++) {         // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    
    Serial.print("  ");
   
  ///delay(200);//зависит от скорости передачи
  }
  Temp=(data[1]<<8)+data[0];//take the two bytes from the response relating to temperature
  Temp=Temp;//divide by 16 to get pure celcius readout
  Serial.print("T=");//output the temperature to serial port
  Serial.print(Temp/16);
  Serial.print(".");
  Serial.print(((Temp%16)*100)/16);
  Serial.print("  ");
  Serial.print(" CRC=");
  Serial.print( OneWire::crc8( data, 8), HEX);
  Serial.println();
  Serial.print(Var); Serial.print("___111111111111");
  
  if ( (Temp/16 > -20) and (Temp/16 > -20) ){ /////////////проверка корректности работы датчика
  dtostrf(Var, 3, 0, char_temp);// dtostrf(AnalogSensorValue[i], 3, 1, char_temp); 
  dtostrf((Temp/16), 3, 0, char_temp2);
  client.publish(char_temp, char_temp2);
  ////////char str =char_temp; client.publish(str, char_temp2); /////////попытка трансляции названия датчика 25/08/2020
  }
  Var=0;
}
