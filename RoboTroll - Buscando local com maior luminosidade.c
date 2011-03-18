//Portas Digitais
#define MOTOR_DC_FRENTE 5
#define MOTOR_DC_TRAS 6
#define MOTOR_DC_ESQUERDA 10
#define MOTOR_DC_DIREITA 11

#define LED_PISCA_DIREITA 12
#define LED_PISCA_ESQUERDA 13
#define LED_FREIO_ESQUERDA 3
#define LED_FREIO_DIREITA 2

//Portas Analogicas
#define SENSOR_FRENTE 0
#define SENSOR_TRAS 2
#define SENSOR_ESQUERDA 1
#define SENSOR_DIREITA 3
#define SENSOR_SERVO 4

//Constantes
#define SERVO_MEIO 365
#define SERVO_DIREITA 330
#define SERVO_ESQUERDA 400

//Variaveis
int counterSensorPrint = 0;
int counterPisca = 0;
char volante = 'm';
bool alinhar = true;

void setup(){	
  	Serial.begin(9600);
	pinMode(LED_PISCA_DIREITA, OUTPUT);
	pinMode(LED_PISCA_ESQUERDA, OUTPUT);
    pinMode(LED_FREIO_DIREITA,OUTPUT);
    pinMode(LED_FREIO_ESQUERDA,OUTPUT);
}

void loop() {
	//alinhar o volante no meio toda vez que é iniciado o arduino
	if(alinhar){
		alinharVolante();	
		alinhar = false;		
	}
	//leitura dos sensores
	int intensidadeFrente = analogRead(SENSOR_FRENTE) - 110;
	intensidadeFrente = intensidadeFrente - (intensidadeFrente*0.57);
 	int intensidadeTRAS = analogRead(SENSOR_TRAS)- 640;
	int intensidadeEsquerda = analogRead(SENSOR_ESQUERDA) - 280;
	int intensidadeDireita = analogRead(SENSOR_DIREITA) - 280;
	int intensidadeServo = analogRead(SENSOR_SERVO);

	//chamada dos metodos
	impressaoValoresSensores(intensidadeFrente, intensidadeTRAS, intensidadeEsquerda, intensidadeDireita, intensidadeServo);
	buscarMenorLuminosidade(intensidadeFrente, intensidadeTRAS, intensidadeEsquerda, intensidadeDireita);
	delay(10);
}

void alinharVolante() {
	int intensidadeServo = analogRead(SENSOR_SERVO);
	if(intensidadeServo > SERVO_MEIO + 2) {
		while(intensidadeServo > SERVO_MEIO + 2) {
			analogWrite(MOTOR_DC_DIREITA, 255);
			delay(1);
			analogWrite(MOTOR_DC_DIREITA, 0);
			intensidadeServo = analogRead(SENSOR_SERVO);			
		}
	}
	else if(intensidadeServo < SERVO_MEIO - 2) {
		while(intensidadeServo < SERVO_MEIO - 2) {
			analogWrite(MOTOR_DC_ESQUERDA, 255);
			delay(1);
			analogWrite(MOTOR_DC_ESQUERDA, 0);
			intensidadeServo = analogRead(SENSOR_SERVO);			
		}
	}
	
	volante = 'm';
}

void impressaoValoresSensores(int intensidadeFrente, int intensidadeTRAS, int intensidadeEsquerda, int intensidadeDireita, int intensidadeServo) {

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
		Serial.println("Intensidade do Sensor Servo");
		Serial.println(intensidadeServo);
		Serial.println("---------------------------------");
	}
}

void buscarMenorLuminosidade(int intensidadeFrente, int intensidadeTRAS, int intensidadeEsquerda, int intensidadeDireita) {

	if(intensidadeFrente < intensidadeTRAS - 25){
		if(intensidadeDireita < intensidadeEsquerda - 15) {
			if(volante == 'e') 
				alinharVolante();
			if(volante == 'm')
				virarDireita();
			pisca('d');
		}
		else if(intensidadeEsquerda < intensidadeDireita - 15) {
			if(volante == 'd') 
				alinharVolante();
			if(volante == 'm')
				virarEsquerda();
			pisca('e');
		}
		else {
			alinharVolante();
			pararPiscas();		
		}
		andarFrente();
	}
	else if(intensidadeTRAS < intensidadeFrente - 25){
		if(intensidadeDireita < intensidadeEsquerda - 15) {
			if(volante == 'e') 
				alinharVolante();
			if(volante == 'm')
				virarDireita();
			pisca('d');
		}
		else if(intensidadeEsquerda < intensidadeDireita - 15) {
			if(volante == 'd') 
				alinharVolante();
			if(volante == 'm')
				virarEsquerda();
			pisca('e');
		}
		else {
			alinharVolante();
			pararPiscas();
		}
	 	andarTRAS();
	}
	else {
		pararTodosMotores();
        pararPiscas();
		alinharVolante();

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
	int intensidadeServo = analogRead(SENSOR_SERVO);
	while(intensidadeServo < SERVO_ESQUERDA) {
		analogWrite(MOTOR_DC_ESQUERDA, 255);
		delay(1);
		analogWrite(MOTOR_DC_ESQUERDA, 0);
		intensidadeServo = analogRead(SENSOR_SERVO);		
	}
	volante = 'e';
}

void virarDireita() {
  	int intensidadeServo = analogRead(SENSOR_SERVO);
	while(intensidadeServo > SERVO_DIREITA) {
		analogWrite(MOTOR_DC_DIREITA, 255);
		delay(1);
		analogWrite(MOTOR_DC_DIREITA, 0);
		intensidadeServo = analogRead(SENSOR_SERVO);
	}
	volante = 'd';
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
