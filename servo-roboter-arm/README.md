# Servo Roboter-Arm

![Roboter-Arm](img/jht-roboter-arm-2.jpg)

Der Roboter-Arm basiert auf [diesem Projekt](https://www.printables.com/model/414563-9g-servo-arm).
Unter [Bauteile](Bauteile) findest du die Teile, um dir so einen Arm selber zu bauen.

Wir verwenden hier noch einen [Arduino Uno](https://store.arduino.cc/collections/uno/products/arduino-uno-rev3) und ein [PCA9685 Servotreiber-Modul von AZ-Delivery](https://www.az-delivery.de/products/pca9685-servotreiber).

Es sind noch vier Potentiometer verbaut, die im Programm z.B. verwendet werden können, um jeden Motor auf eine bestimmte Position zu setzen.

Damit die Servos zuverlässig funktionieren, sollte man das Servotreiber-Modul direkt von einem 5V-Netzteil betreiben, das mindestens 2 A Strom liefern kann.

Die Servos für den Greifer und das obere Gelenk sind billige Standard-9g-Servos aus der Riege Aliexpress (Suchbegriffe können sein `Servo`, `9g`, `SG90`).

Für die beiden unteren Gelenke waren diese Servos nicht stark genug.
Also haben wir dafür Servos vom Typ `Corona DS-939MG` verwendet, die zum Beispiel [hier](https://vgr-rc.de/p/corona-servo-ds939mg-ii-digital-metall-heli-t-rex-450-0-14sec-4-1kg-15-1g-450-ds-939mg) erhältlich sind.

Weil die etwas andere Maße haben als die Standard-Billoservos, haben wir die entsprechenden 3D-Druckteile ([DS-939MG-socket.stl](Bauteile/DS-939MG-socket.stl) und [DS-939MG-9gServoCageFor.stl](Bauteile/DS-939MG-9gServoCageFor.stl)) noch etwas angepasst.

[servoArmPfad.ino](servoArmPfad/servoArmPfad.ino) ist ein Beispielprogramm für den Arduino, mit dem der Arm einen vorgegebenen Pfad abfährt:

![Roboterarm arbeitet Pfad ab](img/jht-roboter-arm-arbeitet.gif)
