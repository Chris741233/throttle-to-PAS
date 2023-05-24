/******************************************************************
Created with PROGRAMINO IDE for Arduino - 17.05.2023 | 10:21:23
Project     :
Libraries   :
Author      :  Chris74

Description :  e-bike throttle to PAS - Arduino Uno/Nano
- Simulation pedalage et envoi au controleur

Code and info on Github : https://github.com/Chris741233/throttle-to-PAS


Doc PAS ebikes.ca (Signal Types for Basic PAS Sensors) : 
- https://ebikes.ca/learn/pedal-assist.html
Doc PAS on pedelecforum.de
- https://www.pedelecforum.de/forum/index.php?threads/funktionsweise-digitaler-pas-sensoren-details-fuer-interessierte.9539/


Discussion Cyclurba (Soupaloignon) : 
- https://cyclurba.fr/forum/741850/arduino-l-assistance-d-un-vae.html?from=11&discussionID=31032&messageID=741850&rubriqueID=87
- https://cyclurba.fr/forum/738995/fonctionnent-capteurs-pn-dalage.html?discussionID=5512

Other source : Pedelecs forum 
- https://www.pedelecs.co.uk/forum/threads/how-to-add-a-throttle-to-a-carrera-vengeance-e-spec.31483/


******************************************************************/



// -------- GPIO Arduino Uno/Nano --------------

const int THR_PIN = 0;   // -- THROTTLE A0

const int PAS_PIN = 2;   // -- PAS Hall D2, sans resistance (cf INPUT_PULLUP) + Interrupt

const int LED_PIN = 13;  // -- LED et CONTROLER in (D13 = LED_BUILTIN)


// -------- SETTING CONSTANTS ------------------------


// -- throttle setting (cf debug Serial in loop) 
const int   tr_min    = 220;    // throttle min - marge ajoutee dans map() 
const int   tr_max    = 856;    // throttle max - marge deduite dans map()
const int tr_margin   = 15;     // marge (margin) throttle avant envois signal PWM  et en deduction de tr_max  

// -- reglages nb. d'aimants et simulation RPM pedalage
const int   nb_magnets  = 6;      // nb. d'aimants (nb. of magnets) sur le disque du PAS (default 6p)
const int   slow_rpm  = 20;     // simulation RPM pedalier min (default 20 rpm)
const int   fast_rpm  = 60;     // simulation RPM pedalier max (default 60 rpm)

// -- setting duty-cycle high signal, cf doc ebikes.ca : https://ebikes.ca/learn/pedal-assist.html 
// -- attention var float : si nombre entier ajouter au moins une decimal .0 Par exemple 60% indiquer 60.0% !!
const float duty_cycle = 56.70;  // % duty high (56.70% on my PAS, type varied width, but 57.0% (or 60.00% ?) must be Ok




// ******** Calcul constantes : ne pas modifier a partir d'ici ! ***********
// **************************************************************

// ne pas enlever les decimales aux chiffres entiers !
const int slow_pas = (1000.0 / (slow_rpm / 60.0 * nb_magnets)) * (duty_cycle/100);   // Signal PAS high lent en ms, % duty_cycle
const int fast_pas = (1000.0 / (fast_rpm / 60.0 * nb_magnets)) * (duty_cycle/100);   // Signal PAS high rapide en ms, % duty_cycle

// calc ratio low (high/low), round 2 decimale
const float t1 = duty_cycle / (100 - duty_cycle);
const float ratio_low = floor(100*t1+0.5)/100; // round decimal to 2 digit (10 for 1 digit)

// -- expl. periode PWM ; 60Rpm / 60sec = 1Rps. 1Rps / 6poles =  1sec / 6 = 0.166 ou 166ms
// -- duty cycle 56.7% de 166ms = 94ms high
// -- ratio_low = percent / (100 - percent); // en duty high 56.75%  = ~1.30


// -------- GLOBAL VARIABLES ----------------------

unsigned long start = 0;           // timer

volatile bool throt_on = false;    // throttle in use = false au boot 
volatile bool pas_on   = false;    // test interrupt PAS 


// -------- MAIN PROG ----------------------

void setup() {
    Serial.begin(115200);
    
    pinMode(LED_PIN, OUTPUT);
    pinMode(THR_PIN, INPUT);
    
    pinMode(PAS_PIN, INPUT_PULLUP);  // PAS Hall, sans ajout de résistance, signal à 1 devant aimant
    
    digitalWrite(LED_PIN, LOW);      // signal led/controleur sur Low au boot
    
    // -- Interrupt pedaling : appel "isr_pas" sur signal CHANGE  
    attachInterrupt(digitalPinToInterrupt(PAS_PIN), isr_pas, CHANGE); 
    
    
    // attention, ne pas mettre d'instruction "delay" ici (et dans le loop egalement) !
    // no delay() here !
    
}


void loop()
{
    int val = analogRead(THR_PIN); // lit la valeur analogique du throttle 
    //Serial.println(val);         // debug val throttle : env. 210 au min et 860 au max sur mon montage
    
    
    // map val : Plus on appuie sur le throttle plus le signal PAS high doit etre court !
    int duty = map(val, tr_min+tr_margin, tr_max-tr_margin, slow_pas, fast_pas);
    
    //Serial.println(duty);             // debug duty
    //Serial.println(t1, 5);            // debug ratio
    //Serial.println(ratio_low, 5);     // debug ratio
    
    // juste pour tester l'interrupt ...
    if (pas_on) {
        //Serial.println(" signal changed");
        pas_on = false;
    }
    
    // si throttle en fonction on simule un signal PAS, fréquence et duty selon throttle
    if (val > tr_min + tr_margin) {
        
        throt_on = true; 
        
        // Signal high
        start = millis();
        while (millis() - start <= duty) {
            if (val > tr_min + tr_margin) digitalWrite(LED_PIN, HIGH);
        }
        // Signal low ( ratio calcule dans constantes)
        start = millis();
        while (millis() - start <= duty / ratio_low) {
            if (val > tr_min + tr_margin) digitalWrite(LED_PIN, LOW);
        }
    }
    else throt_on = false;
    
    
    // No delay() here !
    
} // end loop


// -- ISR - appel interrupt 
// - simple replication des signaux du PAS vers le controleur lors du mode pedalage sans appuyer sur le throttle
void isr_pas() {
    
    pas_on = true;    
    
    // replication du pedalge seulement si le throttle n'est pas en fonction
    if (throt_on == false) { 
        
        if (digitalRead(PAS_PIN) == HIGH) digitalWrite(LED_PIN, HIGH);
        else  digitalWrite(LED_PIN, LOW);
        //Serial.println("pedalage..."); // pour debug, eviter si possible un serial dans une interruption
    } 
    
} // endfunc




