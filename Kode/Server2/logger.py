import os
from json import dumps
from datetime import datetime, timedelta
import csv
import dateutil.parser as dp
from math import pow
from datetime import datetime
from config import devices as dev

Datarates = ["SF12BW125", "SF11BW125", "SF10BW125","SF9BW125", "SF8BW125", "SF7BW125"]
current_directory = os.getcwd()

def payload_parser(payload):
    try:
        payload.decode("utf-8")
    except AttributeError:
        print("Payload not byte class")
    f_c = int(payload[0:2],16)
    dev_id = int(payload[2:4],16)
    bat = int(payload[12:14], 16)
    vbus = int(payload[14:18], 16)
    curr = int(payload[18:22], 16)
    return f_c, dev_id, bat, vbus, curr




def WriteMetaToFile(payload,curr_date, date_payload, clock_payload):
    frame_count, dev_id, bat_lvl, vbus, curr = payload_parser(payload)
    
    #Create directory for each day
    try:
        dev_id_str = dev[dev_id]
    except KeyError:
        dev_id_str = 0
        print("Device not in list")
    directory = 'Logs'+'\\'
    final_directory = os.path.join(current_directory, (r''+directory+str(dev_id_str)))
    #print(final_directory)
    if not os.path.exists(final_directory):
        os.makedirs(final_directory)
      
    try:
        Log = open(final_directory+"/"+str(date_only)+".csv",'x', newline='')
        wr = csv.writer(Log)
        temp1 = [[frame_count, clock_only, bat_lvl, vbus, curr] ]
        wr.writerows(temp1)
        Log.close()
    except FileExistsError:
        Log = open(final_directory+"/"+str(date_only)+".csv",'a', newline='')
        wr = csv.writer(Log)
        temp1 = [[frame_count, clock_only, bat_lvl, vbus, curr] ]
        wr.writerows(temp1)
        Log.close()
        

payload_ex = "660100000406549998FFFF"
#WriteMetaToFile(payload_ex)