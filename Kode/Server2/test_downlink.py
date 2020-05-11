



import time
from ttn import HandlerClient

dev_eui1 = "0004A30B00EB9F66"
app_eui = "70B3D57ED002E533"
access_key = "ttn-account-v2.2aTBPpoyM78nhSgOKTkNE1At8OzeSUPxMCKlCbRC4jo"
app_id = "lora-nodes"

handler = HandlerClient(app_id, access_key, discovery_address="discovery.thethings.network:1900")
mqtt_application = handler.application()
mqtt_client = handler.data()
mqtt_client.connect()
my_devices = mqtt_application.devices()
print(my_devices)
mqtt_client.send(dev_id="sensor2",  pay= "deadbeef", port=1, conf=False, sched="replace")
time.sleep(60)
mqtt_client.close()