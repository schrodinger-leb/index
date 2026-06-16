import asyncio
import socket
from bleak import BleakClient
 
BT05_ADDRESS   = "7C:66:9D:73:7D:A7"
UART_RX_CHAR   = "0000ffe1-0000-1000-8000-00805f9b34fb"
RSSI_THRESHOLD = -85  # adjust based on testing
 
async def main():
    print("Connecting to BT05...")
    async with BleakClient(BT05_ADDRESS) as client:
        print("BLE Connected!")
 
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind(('localhost', 5000))
        server.listen(1)
        server.setblocking(True)
        print("Waiting for Java connection on port 5000...")
 
        loop = asyncio.get_event_loop()
        conn, addr = await loop.run_in_executor(None, server.accept)
        print(f"Java connected from {addr}!")
        conn.setblocking(False)
 
        autonomous_mode = False
        last_cmd = None
        check_counter = 0
 
        while True:
            try:
                # Check RSSI every 50 loops
                check_counter += 1
                if check_counter >= 50:
                    check_counter = 0
                    try:
                        rssi = await client.get_rssi()
                        print(f"Signal strength: {rssi} dBm")
 
                        if rssi < RSSI_THRESHOLD and not autonomous_mode:
                            autonomous_mode = True
                            await client.write_gatt_char(UART_RX_CHAR, b'A', response=False)
                            print("OUT OF RANGE - Bot switching to autonomous!")
                            try:
                                conn.send(b'A')
                            except:
                                pass
 
                        elif rssi >= RSSI_THRESHOLD and autonomous_mode:
                            autonomous_mode = False
                            await client.write_gatt_char(UART_RX_CHAR, b'M', response=False)
                            print("IN RANGE - Bot switching to manual!")
                            try:
                                conn.send(b'M')
                            except:
                                pass
                    except Exception as e:
                        print(f"RSSI error: {e}")
 
                if not autonomous_mode:
                    try:
                        data = conn.recv(1)
                        if data and data != last_cmd:
                            last_cmd = data
                            await client.write_gatt_char(UART_RX_CHAR, data, response=False)
                            print(f"Sent: {data.decode()}")
                    except BlockingIOError:
                        pass
                    except Exception as e:
                        print(f"Recv error: {e}")
 
            except Exception as e:
                print(f"Error: {e}")
 
            await asyncio.sleep(0.01)
 
asyncio.run(main())