#include <Arduino.h>
#include <U8g2lib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define BOUTON 17 

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,     // rotation de l’écran (R0 = normal)
  U8X8_PIN_NONE, // pas de pin reset séparé
  22,          // SCL = GPIO22
  23          // SDA = GPIO23
);

//variable affichage et disposition ecran
int largeurEcran =  0;
int hauteurEcran = 0;
int posTexteY = 0;

//variable securité du thread 
SemaphoreHandle_t mutex;

//variable globale 
unsigned long  previousMillis = 0;
int compteur = 0;

/**
 * @brief Gère l'affichage complet de l'écran.
 * 
 * Efface le contenu actuel, puis affiche successivement :
 * - le message texte centré via `text_screen()`
 * 
 * Enfin, envoie le tout à l'écran avec `sendBuffer()`.
 * 
 * @param msg Texte à afficher.
 */
void display_screen(const char *msg)
{ 
  u8g2.clearBuffer();
  text_screen(msg);
  u8g2.sendBuffer();
}

/**
*@brief Affiche le texte envoyer en parametre à l'écran
*
*@param msg texte à afficher.
*/
void text_screen(const char *msg)
{
  u8g2.setFont(u8g2_font_fub17_tf); 
  int positionMsg = (largeurEcran - u8g2.getStrWidth(msg)) / 2;
  u8g2.drawStr(positionMsg, posTexteY, msg);
}

/**
* @brief thread de test
*
* @param
*/
void monThread(void *params)
{
  while(true)
  {
    if(digitalRead(BOUTON) == LOW)
    {
      if (xSemaphoreTake(mutex, portMAX_DELAY))
      {
        display_screen("OK");
        vTaskDelay(pdMS_TO_TICKS(2000)); 
        xSemaphoreGive(mutex);
      }
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  u8g2.begin(); 
  delay(100);
  largeurEcran = u8g2.getDisplayWidth();
  hauteurEcran = u8g2.getDisplayHeight();
  posTexteY = (hauteurEcran / 2) + 8;

  display_screen("Thread");
  delay(1500);

  pinMode(BOUTON, INPUT_PULLUP);

  mutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(monThread, "monThread", 2048, NULL, 1, NULL, 1);
  previousMillis = millis();
}

void loop() 
{ 
  char text[10];
  sprintf(text, "%d", compteur);
  if (xSemaphoreTake(mutex, 0) == pdTRUE)
  {
    display_screen(text);
    xSemaphoreGive(mutex);
  }

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    compteur++;
  }
  vTaskDelay(pdMS_TO_TICKS(10));
}