from ttn import HandlerClient
import time
import os, sys
from datetime import timezone, datetime
from base64 import b64decode, b64encode
from threading import Thread, Lock, Timer
from logger import WriteMetaToFile
import json
import paho.mqtt.client as mqtt
import config as cfg
import math
dev_eui1 = "0004A30B00EB9F66"
app_eui = "70B3D57ED002E533"
access_key = "ttn-account-v2.2aTBPpoyM78nhSgOKTkNE1At8OzeSUPxMCKlCbRC4jo"
app_id = "lora-nodes"
mqttc = mqtt.Client()
Start_date = datetime.now()
unixtime = Start_date.replace(tzinfo=timezone.utc).timestamp()
#unixtime_int = (int)(unixtime)
#unixtime_str = str(unixtime_int)
#unixtime_hex = format(unixtime_int, 'x')
#print(unixtime_hex+"0a")
#unixtime_b64 = b64encode(bytes(str(unixtime_hex), encoding="utf-8")).decode("utf-8")
#print(unixtime_hex)
#print(unixtime_b64)
#print(format((int)(unixtime), 'd'))
date = str(Start_date.day)+"." + str(Start_date.month)+"." + str(Start_date.year)
mtx = Lock()
killSelf2 = False
killSelf = False
inactive = False
print("PID: "+str(os.getpid()))
time_sync_cmd = "0a"
def inactivity_check():
    global mqttc
    global inactive, killSelf, killSelf2
    if inactive:
        killSelf = True
        mqttc.disconnect()
        return

    mtx.acquire()
    inactive = True
    mtx.release()
    Timer(600, inactivity_check).start()

def timestamp_compare(payload):
    print("isndie compare")
    date_now = datetime.now()
    unixtime = date_now.replace(tzinfo=timezone.utc).timestamp()
    print((int)(unixtime))
    date = str(date_now.day)+"." + str(date_now.month)+"." + str(date_now.year)
    diff = False
    ts = int(payload[4:12], 16)
    print(ts)
    if abs(ts-unixtime)>60:
        print("correct the time")
        diff = True
    return diff

def print_hello():
    print("Hello")

def uplink_callback(msg, client):
    global date, mtx, inactive
    print("Received uplink from ", msg.dev_id, datetime.now())
    payload = b64decode(msg.payload_raw).hex()
    mtx.acquire()
    inactive = False
    WriteMetaToFile(payload, datetime.now(), msg.dev_id)
    print("Written to file")
    mtx.release() 
def mqtt_1():
    handler = HandlerClient(app_id, access_key,discovery_address="discovery.thethings.network:1900")
    mqtt_application = handler.application()
    mqtt_client = handler.data()
    mqtt_client.set_uplink_callback(uplink_callback)
    mqtt_client.connect()
    while True:
        if killSelf:
            break
        pass
    mqtt_client.close()
    print("MQTT1 done")

broker = "mqtt.eclipse.org"
pub_topic = "my/subscribe/topic"
sub_topic = "my/publish/topic"
def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))
def on_message(mqttc, obj, msg):
    global inactive
    payload = msg.payload.decode()
    Start_date = datetime.now().time().strftime("%H:%M:%S")
    print(Start_date+ " " + str(payload))
    if msg.payload.decode() == "started":
        print("started")
    else:
        mtx.acquire()
        inactive = False
        WriteMetaToFile(payload, datetime.now(), "lte-node1")
        print("Written to file for LTE-node1")
        mtx.release()
def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))
def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))
def on_log(mqttc, obj, level, string):
    print(string)
def mqtt_2():
    mqttc = mqtt.Client()
    mqttc.on_message = on_message
    mqttc.on_connect = on_connect
    mqttc.on_publish = on_publish
    mqttc.on_subscribe = on_subscribe
    # Uncomment to enable debug messages
    # mqttc.on_log = on_log
    mqttc.connect("mqtt.eclipse.org", 1883, 60)
    mqttc.subscribe("my/publish/topic", 0)
    mqttc.loop_forever()
    print("MQTT2 done")


def payload_test(payload):
    global mqttc
    time_diff, date_str, clock_str = timestamp_compare(payload)
    if time_diff:
        print("Try pub")
        mqttc.publish("my/subscribe/topic", "hei")
        
    #inactive = False
    #WriteMetaToFile(msg.payload,Start_date, date_str, clock_str)

if __name__ == "__main__":
    inactivity_check()
    t1 = Thread(target=mqtt_1)
    t2 = Thread(target=mqtt_2)
    t1.start()
    t2.start()
    print("Main thread")
    while True:
        if(killSelf):
            print("hmm")
            break
        if(Start_date.day != datetime.now().day):
            Start_date = datetime.now()
            mtx.acquire()
            date = str(Start_date.day)+"." + str(Start_date.month)+"." + str(Start_date.year)
            mtx.release()
            print("New Day!")
        pass
    t2.join()
    #t1.join()
    
    print("Main dead")
    os.execv(sys.executable, ['python'] + sys.argv)
    
