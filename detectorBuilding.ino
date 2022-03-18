#define aRefVolt 5.0
// pins
#define RED 7
#define GREEN 6
#define BLUE 5
#define PROBE 12
#define analog 0

#define resKnown 560.0	// Resistance of the known resistace in the voltage divider

#define refresh 1000 // Delay in ms per loop
#define salConst 7500000.0

int raw = 0,			// For raw value reading of analog pin
	salinity = 0,		// Salinity in parts per million
	resUnknown = 0.0	// Resistance of the unknown resistor (calculated using voltDrop)
;
float voltDrop = 0.0;	// Voltage drop across unknown resistor (voltage we are trying to measure)

// Bounds
#define redLen 2
#define greenLen 1
#define blueLen 1

int redBounds[redLen][2] = {
	{0, 2000},
	{5000, 6000}
};
int greenBounds[greenLen][2] = {
	{1000, 4000}
};
int blueBounds[blueLen][2] = {
	{3000, 6000}
};


void setup(){
	Serial.begin(9600);

	analogReference(EXTERNAL); 	// Use lower reference voltage
	
	// Configure LED pins
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
	pinMode(PROBE, OUTPUT);
	
	measure();
	delay(refresh);
	
	int sum[3] = {0, 0, 0};
	float voltSum = 0;
	for (int i = 0; i < 5; i++) {
		measure();
		stats();
		sum[0] += raw;
		voltSum += voltDrop;
		sum[2] += resUnknown;
		sum[3] += salinity;
	delay(refresh); }
	
	raw = sum[0] / 5;
	voltDrop = voltSum / 5;
	resUnknown = sum[2] / 5;
	salinity = sum[3] / 5;
	
	readout();
	setLEDs();
}

void loop(void) {}

void measure() {
	digitalWrite(PROBE, HIGH);
	
	raw = analogRead(analog);
	voltDrop = (raw * aRefVolt) / 1024.0;
	resUnknown = resKnown * (aRefVolt / voltDrop - 1);
	salinity = 3469.29135 * pow(1/(5 - voltDrop), 2.0785831);
	
	digitalWrite(PROBE, LOW);
}

void stats() {
	Serial.print(raw); Serial.print(",");
	Serial.print(voltDrop); Serial.print(",");
	Serial.print(resUnknown); Serial.print(",");
	Serial.println(salinity);
}

void readout() {
	// Print out stats
	Serial.println();
	Serial.print("Raw Reading:  "); Serial.println(raw);
	Serial.print("Volt Reading: "); Serial.println(voltDrop);
	Serial.print("Resistance:   "); Serial.println(resUnknown);
	Serial.print("Salinity:     "); Serial.println(salinity);
}

void setLEDs() {
	boolean red = false,
		green = false,
		blue = false
	;
	
	for (int i = 0; i < redLen;   i++) { if (redBounds[i][0]   <= salinity && salinity <= redBounds[i][1]  ) red   = true; }
	for (int i = 0; i < greenLen; i++) { if (greenBounds[i][0] <= salinity && salinity <= greenBounds[i][1]) green = true; }
	for (int i = 0; i < blueLen;  i++) { if (blueBounds[i][0]  <= salinity && salinity <= blueBounds[i][1] ) blue  = true; }
	
	digitalWrite(RED,   red   ? HIGH : LOW);
	digitalWrite(GREEN, green ? HIGH : LOW);
	digitalWrite(BLUE,  blue  ? HIGH : LOW);
}
