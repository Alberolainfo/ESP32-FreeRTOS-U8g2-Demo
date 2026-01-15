#include <Arduino.h>
#include <U8g2lib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define BOUTON 17 

// Configuration de l'écran OLED SSD1306 via I2C avec la bibliothèque U8g2
// OLED SSD1306 display configuration using the U8g2 library (I2C)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,     // rotation de l’écran (R0 = normal) / display rotation (R0 = normal)
  U8X8_PIN_NONE, // pas de pin reset séparé / no separate reset pin
  22,          // SCL = GPIO22
  23          // SDA = GPIO23
);

// Variables d'affichage, initialisées à 0 puis configurées dans le setup()
// Display variables, initialized to 0 and configured later in setup()
int largeurEcran =  0;
int hauteurEcran = 0;
int posTexteY = 0;

SemaphoreHandle_t mutex;// Mutex pour protéger les ressources partagées / Shared resources mutex

// Variables globales pour le chronomètre et le compteur
// Global variables for the timer and counter
unsigned long  previousMillis = 0;
int compteur = 0;

/**
 * @brief Met à jour l'écran OLED avec un message centré.
 * @brief Updates the OLED display with a centered message.
 *
 * Efface le buffer, affiche le texte via text_screen()
 * et envoie le contenu à l'écran.
 * Clears the buffer, draws the text using text_screen(),
 * and sends it to the display.
 *
 * @param msg Texte à afficher / Text to display
 */
void display_screen(const char *msg)
{ 
  u8g2.clearBuffer();
  text_screen(msg);
  u8g2.sendBuffer();
}

/**
 * @brief Affiche un texte centré sur l'écran OLED
 * @brief Displays centered text on the OLED display
 * 
 * Calcule la position horizontale pour centrer le texte
 * et l'affiche à la position verticale définie.
 * Calculates horizontal position to center the text
 * and draws it at the defined vertical position.
 * 
 * @param msg Texte à afficher / Text to display
 */
void text_screen(const char *msg)
{
  u8g2.setFont(u8g2_font_fub17_tf); 
  int positionMsg = (largeurEcran - u8g2.getStrWidth(msg)) / 2;
  u8g2.drawStr(positionMsg, posTexteY, msg);
}

/**
 * @brief Tâche FreeRTOS qui surveille l'état du bouton
 * @brief FreeRTOS task that monitors the button state
 * 
 * Si le bouton est pressé, la tâche prend le mutex,
 * affiche "OK" sur l'écran pendant 2 secondes,
 * puis libère le mutex pour permettre à d'autres tâches
 * d'accéder à l'écran.
 * 
 * If the button is pressed, the task takes the mutex,
 * displays "OK" on the OLED for 2 seconds,
 * then releases the mutex to allow other tasks
 * to access the display.
 *
 * @param params Paramètres de la tâche (non utilisés)
 * @param params Task parameters (not used)
 */
void monThread(void *params)
{
  while(true)
  {
    if(digitalRead(BOUTON) == LOW)
    {
      if (xSemaphoreTake(mutex, portMAX_DELAY))// Vérification si le mutex est libre / Check if mutex is free
      {
        display_screen("OK");
        vTaskDelay(pdMS_TO_TICKS(2000)); // Attend 2000 ms (2 secondes) sans bloquer le CPU / Waits 2000 ms (2 seconds) without blocking the CPU
        xSemaphoreGive(mutex);// Libère le mutex / Release mutex
      }
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  u8g2.begin(); 
  delay(100);

  // Récupération des dimensions de l'écran / Get the screen dimensions
  largeurEcran = u8g2.getDisplayWidth();
  hauteurEcran = u8g2.getDisplayHeight();
  posTexteY = (hauteurEcran / 2) + 8;// Position verticale du texte / Vertical text position

   // Display initial message "Thread" / Affichage d'un message initial "Thread"
  display_screen("Thread");
  delay(1500);

  // Configuration du bouton (résistance pull-up interne) / Configure the button (internal pull-up resistor)
  pinMode(BOUTON, INPUT_PULLUP);

  // Création du mutex / Create a mutex
  mutex = xSemaphoreCreateMutex();

  // Création de la tâche FreeRTOS / Create a FreeRTOS task
  xTaskCreatePinnedToCore(monThread, "monThread", 2048, NULL, 1, NULL, 1);

  // Initialisation du chrono / Initialize the timer
  previousMillis = millis();
}

void loop() 
{ 
  // Conversion du compteur en texte / Convert compteur to text
  char text[10];
  sprintf(text, "%d", compteur);

  // màj de l'écran si mutex libre / Update display if mutex is free
  if (xSemaphoreTake(mutex, 0) == pdTRUE)
  {
    display_screen(text);
    xSemaphoreGive(mutex);// Libère le mutex / Release mutex
  }

  // Gestion du chronomètre / Handle timer
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    compteur++;
  }

  vTaskDelay(pdMS_TO_TICKS(10));// Pause courte non bloquante / Short non-blocking delay
}