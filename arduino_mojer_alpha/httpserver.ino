void httpwebserwer(){
 
// listen for incoming clients
  EthernetClient clienthttp = serverhttp.available();
  if (clienthttp) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (clienthttp.connected()) {
      if (clienthttp.available()) {
        char c = clienthttp.read();////////////////////////обрати внимани только один символ те 20 это 2   
        Serial.write(c);/////////////////////////////////////
if (newInfo && c == ' '){ 
//если переменная новой информации = 1 и "с", в которой записан запрос, равен пустой строке 
newInfo = 0; //то обнуляем переменную поступления новой информации 
} 
if (c == '$'){ 
// если переменная "с", несущая отправленный нам запрос, содержит символ $ 
// "$" подразумевает разделение получаемой информации (символов) 
newInfo = 1; //то пришла новая информация, ставим метку новой информации в 1 
} 
/* ////////////приём данных с кнопок
if (newInfo == 1){ //если есть новая информация 
for (int i = 0; i < 10; i++) { 

if (c == (i)){ //и "с" содержит 1 
  clienthttp.println(i);
DigitalOutState[i]=1;}
}} */



//Проверяем содержание URL - присутствует $1 или $2 
if ((newInfo == 1)){////////////////////////////////////////////////////////////////////////////
  ////////////если есть новая информация  
Serial.println (c); int i =0;
if ((c == '0')&(newInfo_c != '0') ){ //и "с" содержит 1
Serial.println ("Включить");newInfo_c = '0';//////////////////////////////////////////////////////////////////////////////////////////////////';  
DigitalOutState[i]=1;} 
if ((c == '1')&(newInfo_c != '1') ){ //если "с" содержит 2 
DigitalOutState[i]=0;
newInfo_c = '1';//////////////////////////////////////////////////////////////////////////////////////////////////'; 
digitalWrite(DigitalOut[i], DigitalOutState[i]);
//////digitalWrite (8, LOW); //гасим светодиод 
} 
 //если есть новая информация 
if ((c == 'r')&(newInfo_c != 'r') ){ //и "с" содержит $reboot
Serial.println ("reboot on http"); 
newInfo_c = 'r'; //////////////////////////////////////////////////////////////////////////////////////////////////'; 
alarm_delay= (alarm_delay_reboot); soundalarm (); resetFunc();
}
  if ((c == 'a')&(newInfo_c != 'a') ){ //и "с" содержит $alarm
Serial.println ("$alarm on http"); 
newInfo_c = 'a'; //////////////////////////////////////////////////////////////////////////////////////////////////'; 
alarm_delay =(alarm_delay_alarm); soundalarm ();
}
}


//Проверяем содержание URL - присутствует $reboot
//////if (newInfo == 1){ //если есть новая информация 
/////Serial.println (c); int i =0;
////// }/////////////////////////////////////////////// $reboot











        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {


// Send a basic HTTP response header (the blank line at the end
// is required). Learn more about client.println()
clienthttp.println("HTTP/1.1 200 OK");
clienthttp.println("Content-Type: text/html");
clienthttp.println("Connection: close");  // the connection will be closed after completion of the response
          clienthttp.println("Refresh: 5");  // refresh the page automatically every 5 sec
          clienthttp.println();
          clienthttp.println("<!DOCTYPE HTML>");
          clienthttp.println("<html>");
          ////////clienthttp.println("<title> arduino </title>"); //название страницы В закладке/////////////////////
          clienthttp.println("<title>"); clienthttp.print(DevaceName); clienthttp.print("</title>"); 
          clienthttp.println();

// Now send whatever html you want to appear on the web page
clienthttp.println("<html><body marginwidth\"0\" marginheight=\"0\" topmargin=\"0\" leftmargin=\"0\" style=\"margin: 0; padding: 0;\">");
clienthttp.println("<table bgcolor=\"#999999\" border=\"0\" width=\"100%\" cellpadding=\"1\">");
clienthttp.println("<tr><td><font color=\"white\" size=\"2\" face=\"Verdana\">&nbsp Arduino Ethernet Shield setup page (SMS EDITION)</font></td></tr></table><br>");
clienthttp.println("<table border=\"0\" width=\"100%\"><tr><td width=110px>&nbsp</td><td width=200px><style>pre {font-size:8pt; letter-spacing:2px; line-height:8pt; font-weight:bold;}</style>");
clienthttp.println("<pre><font color=\"#00979d\">");
clienthttp.println("            #     TM");
clienthttp.println("           ### ");
clienthttp.println("    ##    ##### ");
clienthttp.println("   ####  ####### ");
clienthttp.println("  ############### ");
clienthttp.println(" #################  ");
clienthttp.println("###################  ");
clienthttp.println("                         since 1993  ");
clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");
clienthttp.println("<td width=500px><font color=\"#00979d\" size=\"6\" face=\"Verdana\"><strong> MEGA SYSTEMS 2021</strong></font></td><td>&nbsp</td></tr></table><br>");
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">Network info</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Network setup</font></td><td></td></tr></table></body></html>");

///break;

///clienthttp.println(" size=\"6\" face=\"Verdana\"> ");

          
          /* / send a standard http response header
          clienthttp.println("HTTP/1.1 200 OK");
          clienthttp.println("Content-Type: text/html");
          clienthttp.println("Connection: close");  // the connection will be closed after completion of the response
          clienthttp.println("Refresh: 5");  // refresh the page automatically every 5 sec
          clienthttp.println();
          clienthttp.println("<!DOCTYPE HTML>");
          clienthttp.println("<html>");*/

          
       ////////// strcpy(charAnalogINName[0],"KITCHEN_"); добавляет символ перед строкой

            ///clienthttp.println("------------------------------------------------------------------------------------------------------");
            ////clienthttp.println ("<hr/>"); //линия===================================== 
            ////clienthttp.println("<br />");
            clienthttp.print("Controller name : ");
            clienthttp.print(DevaceName);
            clienthttp.print(", MAC : ");
            for (byte i = 0; i < 6; i++) {Serial.print(mac[i], HEX);}////DEC 
            clienthttp.print(",   MQTT CONNECTION : ");////////////////ПОКАЗЫВАЕТ ЕСЛИ СОЕДИНЕНИЕ С СЕРВЕРОМ
            clienthttp.print(MQTT_connection); ////////////////ПОКАЗЫВАЕТ ЕСЛИ СОЕДИНЕНИЕ С СЕРВЕРОМ
            clienthttp.println("<br />");
            clienthttp.print("IP : ");
            clienthttp.print(Ethernet.localIP());
            clienthttp.print(",    MQTT IP : ");
            clienthttp.print(server);
           ///// clienthttp.println("<br />"); 
            clienthttp.println("         Free memory: ");
            clienthttp.println(freeRam()); clienthttp.println(" byte ");
           ////////////// clienthttp.println("<br />");

           clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");;
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">Analog inputs</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Detals</font></td><td></td></tr></table></body></html>");
       
          //  датчики аналоговые
          for (int i = 0; i < 8; i++) {               
            clienthttp.print("Analog in: ");
            clienthttp.print(charAnalogINName[i]);
            clienthttp.print(" = ");
            clienthttp.print(AnalogSensorValue[i]);
            clienthttp.print(" maximum value: ");
            clienthttp.print(charAnalogINMAXName[i]);
            clienthttp.print(" = ");
            clienthttp.print(AnalogINMAXVALUE[i]);
            clienthttp.println("<br />");
          }
                     clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");;
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">Digitals inputs</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Detals</font></td><td></td></tr></table></body></html>");
                    //  датчики дискретные
          for (int i = 0; i < 3; i++) {
            clienthttp.print("Digital in: ");
            clienthttp.print(i);
            clienthttp.print(" (Pin: ");
            clienthttp.print(DigitalIn[i]);
            clienthttp.print(") = ");
            clienthttp.print(DigitalInValue[i]);
            clienthttp.println("<br />");
          }
clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");;
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">DHT inputs</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Detals</font></td><td></td></tr></table></body></html>");
           //  датчики DHT  
            clienthttp.print("Temperature DHT 1 (Pin:3)= ");
            clienthttp.print(t);
           /////// clienthttp.print("<br />");
            clienthttp.print("         Humm DHT 1 (Pin:3)= ");
            clienthttp.print(h);
            clienthttp.println("<br />");
            clienthttp.print("Temperature DHT 2 (Pin:4)= ");
            clienthttp.print(t2);
            //////////clienthttp.print("<br />");
            clienthttp.print("         Humm DHT 2 (Pin:4)= ");
            clienthttp.print(h2 );
           clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");;
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">Digitals outputs</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Detals</font></td><td></td></tr></table></body></html>");//////////////////////// clienthttp.println("<br />");
            ////////////clienthttp.println("------------------------------------------------------------------------------------------------------");
            ////////////clienthttp.println("<br />"); /////////////////состояния реле-дискретных выходов
            for (int i = 0; i < 10; i++) {
            clienthttp.print("Outpin ");
            clienthttp.print(DigitalOutTopic[i])   ; 
            clienthttp.print(" (Pin: ");
            clienthttp.print(DigitalOut[i]);                       
            clienthttp.print(") System state: ");     
            clienthttp.print(DigitalOutState[i]); 
            clienthttp.print("     :");
            clienthttp. print ("<a href=\"/$"); clienthttp. print (i*2); clienthttp. print ( "\"><button>ON</button></a>"); //////кнопка включить
            clienthttp. print ("<a href=\"/$"); clienthttp. print ((i*2+1)); clienthttp. print ( "\"><button>OFF</button></a>"); //////кнопка включить 
            ///clienthttp.print ("<a href=\"/$"i+1"\"><button>OF</button></a>");//кнопка выключить 
            clienthttp.println("<br />"); }
            clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");;
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">Time periods</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Detals</font></td><td></td></tr></table></body></html>");  
            clienthttp.print("Period after disconnect to next try, s: "); 
            clienthttp.print(delay_connect_error/1000);
            clienthttp.println("<br />");
            clienthttp.print("Sound alarm period, s: "); 
            clienthttp.print(alarm_delay_alarm/1000);
            clienthttp.println("<br />");
            clienthttp.print("Sound sensor alarm period, s: "); 
            clienthttp.print(alarm_delay_sensor/1000);
            clienthttp.println("<br />");
            clienthttp.print("Sound alarm reboot period, s: "); 
            clienthttp.print(alarm_delay_reboot/1000); 
            clienthttp.println("<br />");           
            clienthttp.print("Analog sensor Thread period, s: "); 
            clienthttp.print(ThreadAnalogInterval/1000);
            clienthttp.println("<br />");
            clienthttp.print("DHT sensor Thread period, s: ");
            clienthttp.print(ThreadDHTInterval/1000);
            clienthttp.println("<br />");
            clienthttp.print("Dalas sensor Thread period, s: ");
            clienthttp.print(ThreadDALASInterval/1000); 
            clienthttp.println("<br />");
            clienthttp.print("MQTT RECONNECT Thread period, s: ");
            clienthttp.print(ThreadRECONNECTInterval/1000)   ;   
            clienthttp.println("</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>");;
clienthttp.println("<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>");
clienthttp.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\">IR sensor</font></td><td width=120px>");
clienthttp.println("<font color=\"white\" size=\"2\" face=\"Verdana\">Detals</font></td><td></td></tr></table></body></html>");///////////////IR sensor
            clienthttp.println ("<hr/>"); //линия===================================== 
            clienthttp.print("IR sensor (Pin: 6) = ");
            clienthttp.print(results.value); 
         
   clienthttp.println ("<hr/>"); //линия===================================== 
 

clienthttp.print("Controller name : ");
clienthttp.println("<input type=text name='foo' size=20>");
clienthttp.print ("<a href=\"/$s\"><button>Save</button></a>"); 
clienthttp.println ("<hr/>"); //линия===================================== 
clienthttp.println ("<a href=\"/$r\"><button>Reboot controller</button></a>"); ///////////кнопка перезагрузки контроллера
clienthttp.println ("<a href=\"/$a\"><button>Alarm</button></a>"); ///////////кнопка перезагрузки контроллера
clienthttp.println ("<a href=\"/$f\"><button>Refresh comand</button></a>"); ///////////кнопка перезагрузки контроллера
clienthttp.println("<br />"); 



          
            
            clienthttp.println("<br />");
          
          clienthttp.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      } 
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    clienthttp.stop();
    Serial.println("client disconnected");



    
  }
}

int freeRam () {
 extern int __heap_start, *__brkval;  int v;  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); }

 
