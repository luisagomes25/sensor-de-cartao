// Programa: Teste módulo PN532 NFC RFID - I2C (Arduino Uno)
// Autor: Adaptado por ChatGPT (Base: Exemplo Adafruit PN532)

#include <Wire.h> 
#include <Adafruit_PN532.h>

#define PN532_IRQ   -1 
#define PN532_RESET -1  

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial); 

  Serial.println("*** Teste Módulo PN532 NFC RFID (I2C) ***");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) 
  {
    Serial.println("ERRO: Placa PN53x não encontrada. Verifique conexões e alimentacao.");
    while (1); 
  }

  Serial.print("Chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware: ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.SAMConfig();

  Serial.println("------------------------------------------");
  Serial.println("Aguardando aproximacao de um cartao ou tag...");
  Serial.println("------------------------------------------");
  Serial.println("");
}

void loop(void) 
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; 
  uint8_t uidLength;                       

  // Espera detecção de um cartão Mifare/Ultralight
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) 
  {
    Serial.println(">>> TAG DETECTADA COM SUCESSO! <<<");
    
    // 1. Informação Amigável
    Serial.print("Tipo de Cartao: ");
    if (uidLength == 4) {
      Serial.println("MIFARE Classic (4-byte UID)");
    } else if (uidLength == 7) {
      Serial.println("MIFARE DESFire ou Ultralight (7-byte UID)");
    } else {
      Serial.print(uidLength, DEC);
      Serial.println("-byte UID");
    }

    // 2. Formatação do UID para fácil cópia
    Serial.print("UID (Identificador Unico): ");
    
    // Imprime o UID como um número hexadecimal contínuo
    for (uint8_t i = 0; i < uidLength; i++) 
    {
      if (uid[i] < 0x10) Serial.print("0"); // Adiciona um zero inicial se for menor que 16
      Serial.print(uid[i], HEX);
    }
    
    Serial.println("");
    
    // 3. Mensagem de Ação para o Usuário
    Serial.println("------------------------------------------");
    Serial.println("Remova o cartao/tag para ler o proximo...");
    Serial.println("------------------------------------------");
    Serial.println("");

    // Pausa para evitar múltiplas leituras rápidas, já embutida no seu código original
    delay(1500);
  }
}


