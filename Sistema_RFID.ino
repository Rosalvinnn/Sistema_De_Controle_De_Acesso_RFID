#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 7

MFRC522 rfid(SS_PIN, RST_PIN);

// =========================
// LEDs
// =========================
#define LED_VERDE 2
#define LED_VERMELHO 8

// =========================
// Ponte H L298N
// =========================
#define ENA 5
#define IN3 3
#define IN4 4

// =========================
// EEPROM
// =========================
const int MAX_CARTOES = 50;

const int ENDERECO_QTD = 0;
const int ENDERECO_UIDS = 1;

int quantidadeCartoes = 0;

// =========================
// MODOS
// =========================
bool modoCadastro = false;
bool modoRemocao = false;

// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);

  quantidadeCartoes = EEPROM.read(ENDERECO_QTD);

  if (quantidadeCartoes > MAX_CARTOES) {
    quantidadeCartoes = 0;
  }

  Serial.println("===== SISTEMA RFID =====");
  Serial.println("");
  Serial.println("COMANDOS:");
  Serial.println("CADASTRAR");
  Serial.println("REMOVER");
  Serial.println("LISTAR");
  Serial.println("LIMPAR");
  Serial.println("");
  Serial.println("Aproxime um cartao...");
}

// =========================
// LOOP
// =========================
void loop() {

  if (Serial.available()) {

    String comando = Serial.readStringUntil('\n');

    comando.trim();

    if (comando == "CADASTRAR") {

      modoCadastro = true;
      modoRemocao = false;

      Serial.println("=== MODO CADASTRO ===");
      Serial.println("Aproxime o cartao...");
    }

    else if (comando == "REMOVER") {

      modoRemocao = true;
      modoCadastro = false;

      Serial.println("=== MODO REMOCAO ===");
      Serial.println("Aproxime o cartao...");
    }

    else if (comando == "LISTAR") {

      listarCartoes();
    }

    else if (comando == "LIMPAR") {

      limparEEPROM();
    }
  }

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID: ");

  for (byte i = 0; i < 4; i++) {

    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();

  if (modoCadastro) {

    cadastrarCartao();

    modoCadastro = false;

    rfid.PICC_HaltA();

    return;
  }

  if (modoRemocao) {

    removerCartao();

    modoRemocao = false;

    rfid.PICC_HaltA();

    return;
  }

  bool acessoLiberado = verificarCartao();

  if (acessoLiberado) {

    Serial.println("Acesso liberado");

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

    abrirPortao();

  } else {

    Serial.println("Acesso negado");

    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }

  delay(2000);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  rfid.PICC_HaltA();
}

// =========================
// VERIFICAR CARTÃO
// =========================
bool verificarCartao() {

  for (int cartao = 0; cartao < quantidadeCartoes; cartao++) {

    int endereco = ENDERECO_UIDS + (cartao * 4);

    bool igual = true;

    for (byte i = 0; i < 4; i++) {

      if (rfid.uid.uidByte[i] != EEPROM.read(endereco + i)) {

        igual = false;
        break;
      }
    }

    if (igual) {
      return true;
    }
  }

  return false;
}

// =========================
// CADASTRAR
// =========================
void cadastrarCartao() {

  if (verificarCartao()) {

    Serial.println("Cartao ja cadastrado!");
    return;
  }

  if (quantidadeCartoes >= MAX_CARTOES) {

    Serial.println("Memoria cheia!");
    return;
  }

  int endereco = ENDERECO_UIDS + (quantidadeCartoes * 4);

  for (byte i = 0; i < 4; i++) {

    EEPROM.write(endereco + i, rfid.uid.uidByte[i]);
  }

  quantidadeCartoes++;

  EEPROM.write(ENDERECO_QTD, quantidadeCartoes);

  Serial.println("Cartao cadastrado!");

  digitalWrite(LED_VERDE, HIGH);

  delay(2000);

  digitalWrite(LED_VERDE, LOW);
}

// =========================
// REMOVER
// =========================
void removerCartao() {

  for (int cartao = 0; cartao < quantidadeCartoes; cartao++) {

    int endereco = ENDERECO_UIDS + (cartao * 4);

    bool igual = true;

    for (byte i = 0; i < 4; i++) {

      if (rfid.uid.uidByte[i] != EEPROM.read(endereco + i)) {

        igual = false;
        break;
      }
    }

    if (igual) {

      for (int j = cartao; j < quantidadeCartoes - 1; j++) {

        for (byte k = 0; k < 4; k++) {

          byte proximo =
            EEPROM.read(ENDERECO_UIDS + ((j + 1) * 4) + k);

          EEPROM.write(
            ENDERECO_UIDS + (j * 4) + k,
            proximo
          );
        }
      }

      quantidadeCartoes--;

      EEPROM.write(ENDERECO_QTD, quantidadeCartoes);

      Serial.println("Cartao removido!");

      digitalWrite(LED_VERMELHO, HIGH);

      delay(2000);

      digitalWrite(LED_VERMELHO, LOW);

      return;
    }
  }

  Serial.println("Cartao nao encontrado");
}

// =========================
// LISTAR
// =========================
void listarCartoes() {

  Serial.println("===== CARTOES =====");

  if (quantidadeCartoes == 0) {

    Serial.println("Nenhum cartao cadastrado");
    return;
  }

  for (int cartao = 0; cartao < quantidadeCartoes; cartao++) {

    int endereco = ENDERECO_UIDS + (cartao * 4);

    Serial.print("Cartao ");
    Serial.print(cartao + 1);
    Serial.print(": ");

    for (byte i = 0; i < 4; i++) {

      byte valor = EEPROM.read(endereco + i);

      Serial.print(valor, HEX);
      Serial.print(" ");
    }

    Serial.println();
  }
}

// =========================
// LIMPAR EEPROM
// =========================
void limparEEPROM() {

  for (int i = 0; i < 512; i++) {

    EEPROM.write(i, 0);
  }

  quantidadeCartoes = 0;

  EEPROM.write(ENDERECO_QTD, 0);

  Serial.println("Memoria apagada!");
}

// =========================
// ABRIR E FECHAR PORTÃO
// =========================
void abrirPortao() {

  Serial.println("Abrindo portao...");

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 255);

  delay(4000);

  analogWrite(ENA, 0);

  delay(15000);

  Serial.println("Fechando portao...");

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 255);

  delay(6500);

  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  Serial.println("Portao fechado");
}