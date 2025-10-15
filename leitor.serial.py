#!/usr/bin/env python3
"""
Leitor Serial para Arduino PN532 NFC/RFID (I2C)
Exibe UID detectado no terminal e formata por bytes.
Uso: python leitor_serial_arduino.py
"""

import serial
import time
import re

# ========== CONFIGURAÇÃO ==========
PORTA = "/dev/cu.usbserial-110"  # Porta do Arduino
BAUDRATE = 115200                # Deve ser igual ao sketch Arduino
TIMEOUT = 1                      # Timeout da leitura serial
# ==================================

# Regex para detectar linha com UID
uid_pattern = re.compile(r"UID \(Identificador Unico\):\s*([0-9A-Fa-f]+)")

def main():
    try:
        ser = serial.Serial(PORTA, BAUDRATE, timeout=TIMEOUT)
        print(f"Conectado em {PORTA} @ {BAUDRATE}bps. Aguardando dados...\n")
        time.sleep(2)  # Dá tempo do Arduino reiniciar
    except Exception as e:
        print(f"Erro ao abrir porta {PORTA}: {e}")
        return

    try:
        while True:
            line = ser.readline().decode(errors='ignore').strip()
            if not line:
                continue

            # Mostra linha crua do Arduino
            print(line)

            # Detecta UID
            m = uid_pattern.search(line)
            if m:
                uid_hex = m.group(1).upper()
                # Formata por bytes xx:xx:xx
                bytes_list = [uid_hex[i:i+2] for i in range(0, len(uid_hex), 2)]
                print(f"-> UID detectado: {uid_hex}  (bytes: {' : '.join(bytes_list)})")
                print("-" * 40)

    except KeyboardInterrupt:
        print("\nEncerrando leitura serial...")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
