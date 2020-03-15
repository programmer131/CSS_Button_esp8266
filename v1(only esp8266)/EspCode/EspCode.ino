#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "data.h"
#include <WiFiManager.h>


ESP8266WebServer server(80);

/*********HTML Contents************/
const char get_root[]="/";
const char get_index[]="/index.html";

/********* CSS ************/
const char get_style[]="/css/style.css";

/**********JS ***********/
const char get_jquery_js[]= "/js/jquery.min.js";
const char get_raphael_js[]= "/js/raphael-2.1.4.min.js";
const char get_justGage_js[]= "/js/justgage.js";

/**********dynamic contents ***********/
const char get_io[]={"/io"};

/****images**/
const char get_favi[]="/favicon.png";
const char get_pattern[]="/css/pattern.jpg";

/*********audio**********/
const char get_click_sound[]="/sound1.mp3";
char  jsonResp[200];         // a string to hold incoming data

int status = WL_IDLE_STATUS;
void handleJustGageJs()
{ 
    server.send_P ( 200, "application/js", justGage_js);
}
void handleClickSOundMp3()
{ 
    server.send_P ( 200, "audio/mpeg", sound_mp3,sizeof(sound_mp3));
}
void handlePatternJpg()
{ 
    server.send_P ( 200, "image/jpg", pattern_jpg,sizeof(pattern_jpg));
}
void handleFaviconJpg()
{ 
    server.send_P ( 200, "image/png", favicon_png,sizeof(favicon_png));
}
void handlejQuery(){
    server.send_P ( 200, "application/js", jquery_js);
}

void handleRaphael(){
   server.send_P ( 200, "application/js", raphael_js); 
}
void handleToggle(){
  if(server.hasArg("action"))
  {
    if(server.arg("action").equals("toggle"))
    {
    digitalWrite(D1,!digitalRead(D1));
    }  
  }
  if(digitalRead(D1)==1)
    server.send ( 200, "text/html", "1"); 
  else
    server.send ( 200, "text/html", "0");  	
}

void handleRoot() {    
    server.send_P ( 200, "text/html", index_html);   
}
void handleCss() {
      server.send_P ( 200, "text/css", style_css);
}



void handleNotFound(){
    String message = "File Not Found\n\n";
    server.send ( 404, "text/plain", message );    
}



void setup ( void ) {    
	  pinMode(D1, OUTPUT);
    digitalWrite(D1, LOW);
    Serial.begin ( 115200 );       
    WiFiManager wifiManager;
    wifiManager.autoConnect("mcu");
    Serial.println("");	
    Serial.print("IP address:");  
    Serial.println(WiFi.localIP());
    server.on (get_index, handleRoot );
	server.on (get_favi, handleFaviconJpg );
	server.on (get_pattern, handlePatternJpg );
    server.on (get_root, handleRoot );
    server.on (get_style, handleCss);   
	//server.on (get_status, handleStatus);   
		server.on (get_io, handleToggle);   
    server.on (get_jquery_js,handlejQuery);      
    server.on (get_justGage_js,handleJustGageJs);  
    server.on (get_raphael_js,handleRaphael);  
	server.on (get_click_sound,handleClickSOundMp3);
    //server.on(get_status,handleStatus);  
    server.onNotFound ( handleNotFound );
    server.begin();
}

void loop ( void ) {
    server.handleClient();
}
