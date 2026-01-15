<h1>ESP32 FreeRTOS U8g2 Demo</h1>

### Français
<h2>Description</h2>
<p>
  Ce projet est un exemple pédagogique pour apprendre à utiliser des threads (FreeRTOS) sur ESP32 avec l’environnement Arduino.<br>
  Il montre également comment gérer l’affichage sur un écran OLED SSD1306 avec la bibliothèque U8g2 et protéger l’accès à l’écran avec un mutex.
</p>

<p>
  <h3>Le projet inclut :</h3>
  <ul>
    <li>Une tâche FreeRTOS qui surveille un bouton et affiche un message “OK” sur l’écran.</li>
    <li>Une boucle principale (loop) qui affiche un compteur (chrono) incrémenté toutes les secondes.</li>
    <li>L’utilisation d’un mutex pour éviter les conflits d’accès à l’écran entre le thread et la boucle principale.</li>
  </ul>
  Ce code est destiné à apprendre la programmation multitâche sur ESP32, la gestion des ressources partagées, et la manipulation de l’OLED via I2C.
</p>

<p>
  <h3>Matériel requis</h3>
  <ul>
    <li>ESP32 (ou compatible).</li>
    <li>Écran OLED SSD1306 128x64 I2C</li>
    <li>Bouton poussoir (connecté à un GPIO)</li>
  </ul>
</p>
<hr style="border: 1px solid #333; width: 50%; margin: 20px 0;">

### English
<h2>Description</h2>
<p>
  This project is an educational example to learn how to use threads (FreeRTOS) on ESP32 with the Arduino environment.<br>
  It also shows how to handle display on an SSD1306 OLED using the U8g2 library and protect access to the display with a mutex.
</p>

<p>
  <h3>The project includes:</h3>
  <ul>
    <li>A FreeRTOS task that monitors a button and displays an “OK” message on the screen.</li>
    <li>A main loop (loop) that displays a counter incremented every second.</li>
    <li>Use of a mutex to prevent conflicts when accessing the display between the thread and the main loop.</li>
  </ul>
  This code is designed to learn multitasking on ESP32, management of shared resources, and handling an OLED via I2C.
</p>

<p>
  <h3>Required Hardware</h3>
  <ul>
    <li>ESP32 (or compatible)</li>
    <li>SSD1306 128x64 I2C OLED display</li>
    <li>Push button (connected to a GPIO)</li>
  </ul>
</p>

