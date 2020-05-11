import os
from json import dumps
from datetime import datetime, timedelta
import csv
import dateutil.parser as dp
from math import pow
import datetime
from config import devices as dev
import pandas as pd
from openpyxl import load_workbook, Workbook
Datarates = ["SF12BW125", "SF11BW125", "SF10BW125","SF9BW125", "SF8BW125", "SF7BW125"]
current_directory = os.getcwd()

def payload_parser(payload):
    try:
        payload.decode("utf-8")
    except AttributeError:
        pass
    f_c = int(payload[0:2],16)
    dev_id = int(payload[2:4],16)
    ts = int(payload[4:12],16)
    bat = int(payload[12:14], 16)
    vbus = int(payload[14:18], 16)*0.04
    curr = int(payload[18:22], 16)*0.015
    return f_c, dev_id, ts, bat, vbus, curr

def timestamp_parser(ts):
    full_date_str = datetime.datetime.utcfromtimestamp(ts)
    date_str = full_date_str.strftime("%Y-%m-%d")
    clock_str = full_date_str.strftime("%H:%M")
    return date_str, clock_str

def WriteMetaToFile(payload, curr_date):
    frame_count, dev_id, ts, bat_lvl, vbus, curr = payload_parser(payload)
    date_str, clock_str = timestamp_parser(ts)
    #Create directory for each day
    try:
        dev_id_str = dev[dev_id]
    except KeyError:
        dev_id_str = 0
        print("Device not in list")
    directory = 'Logs'+'\\'
    final_directory = os.path.join(current_directory, (r''+directory+str(dev_id_str)))
    #print(final_directory)
    file_name = final_directory+"/"+str(curr_date.date())+".xlsx"
    if not os.path.exists(final_directory):
        os.makedirs(final_directory)
  
    try:
        wb = load_workbook(file_name)
        data_row = [frame_count, clock_str, bat_lvl, vbus, curr, vbus*curr]
        sheet = wb.active
        sheet.append(data_row)
        wb.save(file_name)
    
    except FileNotFoundError:
        header = ["ID", "Time", "Battery", "Voltage [V]", "Current [mA]", "Power"]
        data_row = [frame_count, clock_str, bat_lvl, vbus, curr, vbus*curr]
        wb = Workbook()
        sheet = wb.active
        sheet.append(header)
        sheet.append(data_row)
        wb.save(file_name)


        
#today = datetime.datetime.now()
#payload_ex = "040235b17f355403210876"
#WriteMetaToFile(payload_ex, datetime.datetime.now() + datetime.timedelta(days=2))