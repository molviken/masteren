from ttn import HandlerClient
import time
import os, sys
from datetime import timezone, datetime
from base64 import b64decode
from threading import Thread, Lock, Timer
from logger import WriteMetaToFile
import json
import paho.mqtt.client as mqtt
import config as cfg
import math
access_key = "ttn-account-v2.2aTBPpoyM78nhSgOKTkNE1At8OzeSUPxMCKlCbRC4jo"
app_id = "lora-nodes"
mqttc = mqtt.Client()
Start_date = datetime.now()
unixtime = Start_date.replace(tzinfo=timezone.utc).timestamp()
print(format((int)(unixtime), 'x'))
date = str(Start_date.day)+"." + str(Start_date.month)+"." + str(Start_date.year)
mtx = Lock()
killSelf2 = False
killSelf = False
inactive = False
print("PID: "+str(os.getpid()))

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



def uplink_callback(msg, client):
    global date, mtx, inactive
    print("Received uplink from ", msg.dev_id, datetime.now())
    
    mtx.acquire()
    inactive = False
    WriteMetaToFile(msg.payload_raw)
    mtx.release() 
def mqtt_1(name):
    print(name)
    handler = HandlerClient(app_id, access_key,discovery_address="discovery.thethings.network:1900")
    mqtt_application = handler.application()
    # using mqtt client
    mqtt_client = handler.data()
    mqtt_client.set_uplink_callback(uplink_callback)
    mqtt_client.connect()
    print("Serving ", app_id)
    my_app = mqtt_application.get()
    print(my_app)
    my_devices = mqtt_application.devices()
    print(my_devices)
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
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    mtx.acquire()
    date_str, clock_str = timestamp_parser(msg.payload)
    inactive = False
    WriteMetaToFile(msg.payload,Start_date, date_str, clock_str)
    mtx.release()
def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))
def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))
def on_log(mqttc, obj, level, string):
    print(string)
def mqtt_2():
    mqttc.on_message = on_message
    mqttc.on_connect = on_connect
    mqttc.on_publish = on_publish
    mqttc.on_subscribe = on_subscribe
    # Uncomment to enable debug messages
    # mqttc.on_log = on_log
    mqttc.connect(broker, 1883, 60)
    mqttc.subscribe(sub_topic, 0)
    mqttc.loop_forever()
    print("MQTT2 done")


def timestamp_compare(payload):
    diff = False
    ts = int(payload[4:12], 16)
    print(format((int)(ts), '08x'))
    if abs(ts-unixtime)>20:
        print("correct the time")
        diff = True

    full_date_str = datetime.utcfromtimestamp(ts)
    date_str = full_date_str.strftime("%Y-%m-%d")
    clock_str = full_date_str.strftime("%H:%M")
    return diff, date_str, clock_str

def payload_test(payload):
    global mqttc
    time_diff, date_str, clock_str = timestamp_compare(payload)
    if time_diff:
        print("Try pub")
        mqttc.publish("my/subscribe/topic", "hei")
        
    #inactive = False
    #WriteMetaToFile(msg.payload,Start_date, date_str, clock_str)
payload_ex = "06015eb17f35549998FFFF"
if __name__ == "__main__":
    #inactivity_check()
    #t1 = Thread(target=mqtt_1, args=('lora-node',))
    #t2 = Thread(target=mqtt_2)
    #t1.start()
    #t2.start()
    time.sleep(1)
    #payload_test(payload_ex)
    mqtt_2()
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
    
