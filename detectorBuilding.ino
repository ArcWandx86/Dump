#define aRefVolt 3.3	// Reduces noise for more accurate sensor reading
// pins
#define RED 7
#define GREEN 6
#define BLUE 5
#define analog 0

#define resKnown 10000.0	// Resistance of the known resistace in the voltage divider

int raw = 0,			// For raw value reading of analog pin
	salinity = 0,		// Salinity in parts per million
	resUnknown = 0.0,	// Resistance of the unknown resistor (calculated using voltDrop)
	
	// Ranges for LEDs
	redLower = 0,
	redUpper = 1700,
	greenLower = 1700,
	greenUpper = 3600,
	blueLower = 3600,
	blueUpper = 5000,

	refresh = 1000,	// Delay in ms per loop
;
float voltDrop = 0.0,	// Voltage drop across unknown resistor (voltage we are trying to measure)
;

void setup(){
	Serial.begin(9600);

	analogReference(EXTERNAL); 	// Use lower reference voltage
	
	// Configure LED pins
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
}

void loop(void) {
	// Calculate salinity
	raw = analogRead(analog);
	voltDrop = (raw * aRefVolt) / 1024.0;
	resUnknown = resKnown * (aRefVolt / voltDrop - 1);
	salinity = (9000000 / resUnknown);
	
	// Print out stats
	Serial.print("Raw Reading:  "); Serial.println(raw);
	Serial.print("Volt Reading: "); Serial.println(voltDrop);
	Serial.print("Resistance:   "); Serial.println(resUnknown);
	Serial.print("Salinity:     "); Serial.println(salinity);
	Serial.println();

	// Set the LEDs appropriately
	digitalWrite(RED,LOW);
	digitalWrite(GREEN,LOW);
	digitalWrite(BLUE,LOW);
	
	if ((salinity >= redLower) && (salinity <= redUpper))
	{ // Red LED control
		digitalWrite(RED, HIGH);
	}
 	if ((salinity >= greenLower) && (salinity <= greenUpper))
	{ // Green LED control
		digitalWrite(GREEN, HIGH);
	}
	if ((salinity >= blueLower) && (salinity <= blueUpper))
	{ // Blue LED control
		digitalWrite(BLUE, HIGH);
	}

	delay(refresh);
}
