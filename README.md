## PoC-Projet-Musee-dort-Sec


Afin de reproduire notre projet, vous allez avoir besoin de:
    
    * Carte électronique ESP32 DEVKIT V1 avec un microprocesseur ESP32-S2
    ![image](https://user-images.githubusercontent.com/63933712/135761636-f02e6487-7b20-459d-aa06-b5a37c8b5ef2.png)



 
    * Capteur de température et d’humidité BME680 
    ![image](https://user-images.githubusercontent.com/63933712/135761645-c6300b4a-6afd-4705-810e-140aafaced94.png)

    
    *  Capteur de luminosité LDR (Photorésistance) 
    ![image](https://user-images.githubusercontent.com/63933712/135761701-79914c7f-5aca-4a9a-b619-56854079a67e.png)




    * Une BreadBoard pour câbler les composants
    ![image](https://user-images.githubusercontent.com/63933712/135761651-e3fca98a-a89f-4101-a7fc-14e14660d2c4.png)




Voici le montage que vous devrez réaliser :

![image](https://user-images.githubusercontent.com/63933712/135760741-20b1009e-1b71-4324-859b-743405da8bcf.png)

Puis dans le code fournis, vous allez devoir changer 2 paramètres, il s'agit du nom de votre point d'accès Wi-Fi et du mot de passe associé.

![image](https://user-images.githubusercontent.com/63933712/135761128-c98f5d52-db42-470c-b28d-c1ee874503c6.png)

Une fois la modification effectué, branchez la carte ESP32, puis lancez le programme. Ensuite vous pouvez vous rendre sur le site suivant : 
https://io.adafruit.com/PelouseCitron/feeds 

Vous devriez voir des données apparaitre toutes les 30 min.





