#define MOTOR_DC_FRENTE 5
#define MOTOR_DC_TRAS 6
#define MOTOR_DC_ESQUERDA 10
#define MOTOR_DC_DIREITA 11

#define SENSOR_FRENTE 0
#define SENSOR_TRAS 2
#define SENSOR_ESQUERDA 1
#define SENSOR_DIREITA 3
#define SENSOR_SERVO_DIREITA 5
#define SENSOR_SERVO_ESQUERDA 4

#define LED_PISCA_DIREITA 12
#define LED_PISCA_ESQUERDA 13
#define LED_FREIO_ESQUERDA 3
#define LED_FREIO_DIREITA 2

int counterSensorPrint = 0;
int counterPisca = 0;
int alinharVolanteMeio = 1;
char volante = 'm';

void setup(){	
  	Serial.begin(9600);
	pinMode(LED_PISCA_DIREITA, OUTPUT);
	pinMode(LED_PISCA_ESQUERDA, OUTPUT);
    pinMode(LED_FREIO_DIREITA,OUTPUT);
    pinMode(LED_FREIO_ESQUERDA,OUTPUT);
}

void loop()
{
	if(alinharVolante == 1){
		alinharVolante();
		alinharVolanteMeio = 0;
	}

	alinharVolante();	
	impressaoValoresSensores();
	//buscarMenorLuminosidade();
	

	delay(10);
}

void alinharVolante() {
	int intensidadeServoEsquerda = analogRead(SENSOR_SERVO_ESQUERDA);
	int intensidadeServoDireita = analogRead(SENSOR_SERVO_DIREITA);
	if(intensidadeServoDireita > 300) {
		while(intensidadeServoDireita > 300) {
			analogWrite(MOTOR_DC_DIREITA, 200);
			analogWrite(MOTOR_DC_DIREITA, 0);
			intensidadeServoDireita = analogRead(SENSOR_SERVO_DIREITA);
		}
	}
	else if(intensidadeServoDireita < 295) {
		while(intensidadeServoDireita < 295) {
			analogWrite(MOTOR_DC_ESQUERDA, 200);
			analogWrite(MOTOR_DC_ESQUERDA, 0);
			intensidadeServoDireita = analogRead(SENSOR_SERVO_DIREITA);
		}
	}

}

void impressaoValoresSensores() {

	int intensidadeFrente = analogRead(SENSOR_FRENTE);
 	int intensidadeTRAS = analogRead(SENSOR_TRAS);
	int intensidadeEsquerda = analogRead(SENSOR_ESQUERDA);
	int intensidadeDireita = analogRead(SENSOR_DIREITA);
	int intensidadeServoEsquerda = analogRead(SENSOR_SERVO_ESQUERDA);
	int intensidadeServoDireita = analogRead(SENSOR_SERVO_DIREITA);

	if(counterSensorPrint++ % 100 == 0) {
		Serial.println("Intensidade do Sensor da Frente");
		Serial.println(intensidadeFrente);
		Serial.println("Intensidade do Sensor de Tras");
		Serial.println(intensidadeTRAS);
		Serial.println("Intensidade do Sensor da direita");
		Serial.println(intensidadeDireita);
		Serial.println("Intensidade do Sensor da esquerda"); 
		Serial.println(intensidadeEsquerda);
		Serial.println("Posicao do Volante"); 
		posicaoVolante(volante);
		Serial.println("Intensidade do Sensor Servo DIREITA");
		Serial.println(intensidadeServoDireita);
		Serial.println("Intensidade do Sensor Servo ESQUERDA");
		Serial.println(intensidadeServoEsquerda);
		Serial.println("-------------------");
	}
}

void buscarMenorLuminosidade() {

	int intensidadeFrente = analogRead(SENSOR_FRENTE) + 390;
 	int intensidadeTRAS = analogRead(SENSOR_TRAS) + 15;
	int intensidadeEsquerda = analogRead(SENSOR_ESQUERDA);
	int intensidadeDireita = analogRead(SENSOR_DIREITA) + 26;
	

	if(intensidadeFrente < intensidadeTRAS - 25){
		if(intensidadeDireita < intensidadeEsquerda - 15) {
			if(volante == 'e') 
				alinharVolanteEsquerda();
			if(volante == 'm')
				virarDireita();
			pisca('d');
		}
		else if(intensidadeEsquerda < intensidadeDireita - 15) {
			if(volante == 'd') 
				alinharVolanteDireita();
			if(volante == 'm')
				virarEsquerda();
			pisca('e');
		}
		else {
			if(volante == 'e')
				alinharVolanteEsquerda();
			if(volante == 'd')
				alinharVolanteDireita();	
			pararPiscas();		
		}
		andarFrente();
	}
	else if(intensidadeTRAS < intensidadeFrente - 5){
		if(intensidadeDireita < intensidadeEsquerda - 15) {
			if(volante == 'e') 
				alinharVolanteEsquerda();
			if(volante == 'm')
				virarDireita();
			pisca('d');
		}
		else if(intensidadeEsquerda < intensidadeDireita - 15) {
			if(volante == 'd') 
				alinharVolanteDireita();
			if(volante == 'm')
				virarEsquerda();
			pisca('e');
		}
		else {
			if(volante == 'e')
				alinharVolanteEsquerda();
			if(volante == 'd')
				alinharVolanteDireita();
			pararPiscas();
		}
	 	andarTRAS();
	}
	else {
		pararTodosMotores();
        pararPiscas();
      	if(volante == 'e')
			alinharVolanteEsquerda();
		if(volante == 'd')
			alinharVolanteDireita();

	}
	
	if(counterPisca == 100)
		counterPisca = 0;

	counterPisca++;
}

void posicaoVolante (char posicao) {
	switch (posicao) {
		case 'm':
			Serial.println("O volante esta no Meio");
			break;
		case 'd':
			Serial.println("O volante esta na Direita");
			break;
		case 'e':
			Serial.println("O volante esta na Esquerda");
			break;
	}
}

void virarEsquerda() {
  	virar(MOTOR_DC_ESQUERDA, 500);
	volante = 'e';
 	parar(MOTOR_DC_ESQUERDA);
}

void virarDireita() {
  	virar(MOTOR_DC_DIREITA, 500);  
	volante = 'd';
	parar(MOTOR_DC_DIREITA);
}

void pisca(char lado) {
	if(lado == 'e') {
		digitalWrite(LED_PISCA_DIREITA, LOW);	
		if(counterPisca >= 50) {			
			digitalWrite(LED_PISCA_ESQUERDA, HIGH);		
		}
		else {
			digitalWrite(LED_PISCA_ESQUERDA, LOW);
		 }
	}
	else {
		digitalWrite(LED_PISCA_ESQUERDA, LOW);
		if(counterPisca >= 50) {	
			digitalWrite(LED_PISCA_DIREITA, HIGH);			
		}	
		else {
			digitalWrite(LED_PISCA_DIREITA, LOW);	
		}
	}
}

void pararPiscas() {
	digitalWrite(LED_PISCA_ESQUERDA, LOW);			
	digitalWrite(LED_PISCA_DIREITA, LOW);	
}

void alinharVolanteEsquerda () {
	virar(MOTOR_DC_DIREITA, 500);
	volante = 'm';	
	parar(MOTOR_DC_DIREITA);
}

void alinharVolanteDireita () {
	virar(MOTOR_DC_ESQUERDA, 500);	
	volante = 'm';	
 	parar(MOTOR_DC_ESQUERDA);
}

void andarFrente() { 
	desligarLuzFreio();
	parar(MOTOR_DC_TRAS);
	analogWrite(MOTOR_DC_FRENTE,200);
}

void andarTRAS() { 
	desligarLuzFreio();
	parar(MOTOR_DC_FRENTE);
	analogWrite(MOTOR_DC_TRAS,200);
}

void virar(int motor, int tempo) {
  analogWrite(motor,200);
  delay(tempo);
  parar(motor);
}  

void ligarLuzFreio() {
	digitalWrite(LED_FREIO_DIREITA, HIGH);
	digitalWrite(LED_FREIO_ESQUERDA, HIGH);
}

void desligarLuzFreio() {
	digitalWrite(LED_FREIO_DIREITA, LOW);
	digitalWrite(LED_FREIO_ESQUERDA, LOW);
}

void pararTodosMotores() {
	parar(MOTOR_DC_FRENTE);
	parar(MOTOR_DC_TRAS);
	parar(MOTOR_DC_ESQUERDA);
	parar(MOTOR_DC_DIREITA);
	ligarLuzFreio();
}

void parar(int motor) {
  analogWrite(motor,0);
}  
