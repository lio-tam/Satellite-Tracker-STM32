import requests
import serial
import time

#api und stm32 konfigurieren
API_URL="https://api.wheretheiss.at/v1/satellites/25544"
STM32_PORT="COM1"
BAUD_RATE = 9600
nummer = 1


# Verbindung zum STM32 herstellen
try:
    stm32 = serial.Serial('/dev/tty.usbmodem1453203', 9600, bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=1)
    print(f"Verbindung zu {STM32_PORT} hergestellt.")
except Exception as e:
    print(f"Fehler beim Verbinden mit {STM32_PORT}: {e}")
    exit()

def get_iss_coordinates():
    try:
        response = requests.get(API_URL)
        response.raise_for_status()
        data = response.json()
        latitude = data['latitude']
        longitude = data['longitude']
        altitude = data['altitude']
        return latitude, longitude, altitude
    except Exception as e:
        print(f"Fehler beim Abrufen des ISS-Daten: {e}")
        return None, None, None
try:
    while True:
        lat, lon, alt = get_iss_coordinates()
        if lat is not None and lon is not None and alt is not None:
            # Senden der Koordinaten an das STM32-Board
            message = f'{lat:.6f},{lon:.6f},{alt:.6f}\r\n'  # Format: "Breite,Länge,Höhe\n"
            #message = f'{lat}\r\n'
            stm32.write(message.encode('utf-8'))
            print(f'#{nummer} Gesendet: {message.strip()}')
            nummer+=1
        else:
            print("Keine gültigen Koordinaten empfangen.")
        time.sleep(1)
except KeyboardInterrupt:
    print("Beendet.")
finally:
    stm32.close()
