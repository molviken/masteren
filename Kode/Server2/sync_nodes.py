import openpyxl
import os
from base64 import b64encode
from ttn import HandlerClient
from datetime import datetime

access_key = "ttn-account-v2.2aTBPpoyM78nhSgOKTkNE1At8OzeSUPxMCKlCbRC4jo"
app_id = "lora-nodes"
lora_dev1 = "lora_node1"
lora_dev2 = "lora_node2"
lora_dev3 = "lora_node3"
lte_node1 = "lte-node1"

Start_date = datetime.now()
curr_sec = Start_date.time().strftime("%S")
current_directory = os.getcwd()

def sync_msg(node_name):
    base_cmd = "0a000000"
    final_directory = os.path.join(current_directory, (r''+'Logs'+'\\'+node_name))
    file_name = final_directory+'\\'+str(Start_date.date())+'.xlsx'
    wb  = openpyxl.load_workbook(file_name)
    sheet = wb.active
    last_row = sheet.max_row
    while sheet.cell(column=2, row=last_row).value is None and last_row > 0:
        last_row -= 1
    last_col_b_value = sheet.cell(column=2, row=last_row).value
    mins = int(last_col_b_value.strftime("%M"))
    secs = int(last_col_b_value.strftime("%S"))
    secs_hex = format(secs, '02x')
    cmd = base_cmd + secs_hex
    #print(cmd)
    cmd64 = b64encode(bytes(cmd, encoding="utf-8")).decode("utf-8")
    print(node_name+": "+str(secs))
    return cmd64, secs

if __name__ == "__main__":
    handler = HandlerClient(app_id, access_key,discovery_address="discovery.thethings.network:1900")
    mqtt_application = handler.application()
    mqtt_client = handler.data()
    mqtt_client.connect()
    msg1, secs1 = sync_msg("lora_node1")
    if (secs1 > 4):
        mqtt_client.send(dev_id="lora_node1", pay=msg1)

    msg2, secs2 = sync_msg("lora_node2")
    if (secs2 > 4):
        mqtt_client.send(dev_id="lora_node2", pay=msg2)

    msg3, secs3 = sync_msg("lora_node3")
    if (secs3 > 4):
        mqtt_client.send(dev_id="lora_node3", pay=msg3)




    mqtt_client.close()
    print("Ended")





