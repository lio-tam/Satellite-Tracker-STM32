import requests
import serial

# Replace with your actual serial port and baud rate
SERIAL_PORT = "/dev/tty.usbmodem113103"  # Update with your port
BAUD_RATE = 9600

def fetch_location():
    response = requests.get("https://ipinfo.io/loc")
    if response.status_code == 200:
        lat, lon = response.text.strip().split(",")
        return float(lat), float(lon)
    else:
        raise Exception("Unable to fetch location")

def send_to_stm32(latitude, longitude):
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
        nmea_sentence = f"$GPGLL,{latitude},N,{longitude},W,225444,A\n"
        ser.write(nmea_sentence.encode())
        print(f"Sent to STM32: {nmea_sentence}")

if __name__ == "__main__":
    lat, lon = fetch_location()
    print(f"Latitude: {lat}, Longitude: {lon}")
    send_to_stm32(lat, lon)

