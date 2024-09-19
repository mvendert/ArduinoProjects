/* CVO De Verdieping - Arduino Starters
 * De eerste sketch: Een led laten knipperen
 *  - De opmaak(uitlijning) ontbreekt.
 *  - ! Let op: Er zit een fout in de Sketch.
 * Davy Wolfs 
 * 2021
 */
 #define pinRood 13
 #define pinBlauw 12

 int teller = 0;
 int tellerRood = 0;
 int tellerBlauw = 0;
 
 const double HzRood =  15; //0.8;
 const double HzBlauw = 0.5; //2
 
 unsigned long msRood;        //ms dat een rode led uit en aan moet zijn
 unsigned long msBlauw;       //ms dat de blauwe led aan en uit moet zijn
 unsigned long msWait;        //berekende grootste gemene deler tussen deze twee
 unsigned int periodeRood;    //aantal keer mswait voor we rood van toestand moeten veranderen
 unsigned int periodeBlauw;   //aantal keer mswait voor we blauw van toestand mogen veranderen
 unsigned int tellerMax;      //het maximum van bovenstaande. Als we dit twee keer gedaan hebben mogen we de teller terug op 0 zetten en vooraan beginnen

 bool bRood;                  //de toestand van de rode led
 bool bBlauw;                 //de toestand van de blauwe led

// Snelle routine op arduino om de grootste gemene deler van 2 getallen te berekenen
// https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
 unsigned long gcd_ctz(unsigned long u, unsigned long v) {
  int shift;
  if (u == 0) return v;
  if (v == 0) return u;
  shift = __builtin_ctzl(u | v);
  u >>= __builtin_ctzl(u);
  do {
    v >>= __builtin_ctzl(v);
    if (u > v) {
      unsigned long t = v;
      v = u;
      u = t;
    }
    v = v - u;
  } while  (v != 0);
  return u << shift;
}

void setup() {
  pinMode(pinRood, OUTPUT);  
  pinMode(pinBlauw, OUTPUT);

  msRood = (unsigned long) (( 1000.0 / HzRood))/2;      //Hz naar milleseconden uit en aan
  msBlauw = (unsigned long) (( 1000.0 / HzBlauw)/2);  

  msWait = gcd_ctz (msRood, msBlauw);                   //de delay tijd is de grootst gemene deler van de twee
  periodeRood = (msRood / msWait);                      //het aantal teller ticks is dan het aantal ms / gemene deler
  periodeBlauw = (msBlauw / msWait); 
  tellerMax = max(periodeBlauw,periodeRood);            //En we weten niet welke van de twee de hoogste hz heeft ingegeven. Dus bepalen we zelf de max hier.
  bRood = false;                                        //De leds starten uit
  bBlauw = false;
}

void loop() {
  if (teller % periodeRood == 0) {                      // deelbaar door de rode periode in teller ticks... rood van status wijzigen
    bRood = !bRood;
    digitalWrite(pinRood, bRood);
  }
  if (teller % periodeBlauw == 0)                       // deelbaar door de rode periode in teller ticks... rood van status wijzigen
  {  
    bBlauw = !bBlauw;
    digitalWrite(pinBlauw,bBlauw);
  }
  teller = teller + 1;  
  if (teller == (tellerMax*2) ) {                       //na twee keer terug op 0 zetten
    teller = 0;
  }
  
  delay(msWait);                                        //en wachten zoals we berekend hebben
}