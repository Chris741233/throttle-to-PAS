# throttle-to-PAS
Arduino e-bike throttle PAS simulation and replication

Utilisation d'un throttle (gachette de pouce ou accélérateur) pour simuler un pédalage sur un e-bike
<br> Code pour Aduino Nano/Uno, etc ... (signal +5V)

---

Utilisation : Régler les constantes en début de code
<br> Adjust constants at the beginning of code

- Calibrer les  min et max throttle en décommentant la ligne "Serial" dans le loop (le recommenter après)
- Ajuster le marge de déclenchement "tr_marge"
- Choisir le nb. d'aimants "nb_poles" sur votre disque de PAS 
- Choisir les RPM min et et max de simulation de pédalage (valable seulement si le controleur accèpte un PAS proportionel)
- Adapter le duty-cycle high "duty_cycle" selon votre modèle de PAS (A ou B, voir plus bas)
- That's all ;)


<p align="left">
  <img src="./DOC/throttle-diagram.png" width="500" title="Arduino pinout">
</p>

<p align="left">
  <img src="./DOC/debug_val_throttle.jpg" width="500" title="Arduino pinout">
</p>

<p align="left">
  <img src="./DOC/oscillo_60rpm.jpg" width="700" title="Oscillo">
</p>


<p>
Doc on ebikes.ca : <br>
 <a href="https://ebikes.ca/learn/pedal-assist.html">Signal Types for Basic PAS Sensors</a> 
</p>

<p align="left">
  <img src="./DOC/PAS_Signal_Levels.jpg" width="700" title="Oscillo">
</p>


<p>
Discussion sur le forum Cyclurba : <br>
 <a href="https://cyclurba.fr/forum/forum_discussion.php?from=11&discussionID=31032&messageID=741856&rubriqueID=87">Cyclurba</a> 
</p>

<p></p>

<p><i>
Attention: La réglementation de certains pays n'autorise pas l'utilisation d'un accélérateur à plus de 6 km/h sans pédaler.<br>
L'utilisation de ce code et schéma sont sous votre propre responsabilité ! 
<br><br>
Warning: The regulations of some countries do not allow the use of an accelerator at more than 6 km/h without pedaling.<br>
The use of this code and diagram is under your own responsibility!
</i></p>
