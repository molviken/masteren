import pandas as pd
import matplotlib.pyplot as plt
import os
import datetime as dt
import openpyxl
v_col = "Voltage [V]"
c_col = "Current [mA]"
p_col = "Power [mW]"
bat = "Battery"
time = "Time"

current_dir = os.getcwd()
directory = current_dir+'\\'+'Logs'+'\\'
save_dir = "graphs"+'\\'
node1 = "lora_node1"+'\\'
node2 = "lora_node2"+'\\'
node3 = "lora_node3"+'\\'
lte_node = "LTE-node1"+'\\'
start_date = dt.datetime(2020, 5, 12)
def hit_rate(node, nr):
    frames_lost_temp = 0
    frames_lost = []
    total_frames = []
    percentage = []
    dates = []
    index = 0
    i = 0
    while( (start_date+dt.timedelta(days=i)).date() != dt.datetime.now().date() ):
        
        try:
            date = (start_date+dt.timedelta(days=i)).date().strftime("%Y-%m-%d")
            file_name = directory + node + date + ".xlsx"
            table = pd.read_excel(file_name, usecols="A", convert_float = True)
            dates.append(date)
            col_list = table['Frame'].tolist()
            total_frames.append(len(col_list))
            
            for j in range(len(col_list)-1):
                if ( (col_list[j] != col_list[j+1]-1) & (abs(col_list[j+1] - col_list[j]) < 80) ):
                # print(j)
                    frames_lost_temp = frames_lost_temp + (col_list[j+1] - col_list[j]-1)
                    #print(frames_lost)
                # print()  
            frames_lost.append(frames_lost_temp)
            percentage.append(round(100-(frames_lost_temp*100/total_frames[index]), 2))
            index = index +1    
          
        except FileNotFoundError:
            pass
        i = i+1
        
        


    df = pd.DataFrame(dates, columns=['date'])
    df['total_frames'] = total_frames
    df['lost'] = frames_lost
    df['hit'] = percentage
    print(df)
    #ax = plt.gca()
    ax = df.plot.bar(x='date', y='total_frames')

    #df.plot()
    #plt.locator_params(axis='x', nbins=7)
    plt.show()

    #plt.savefig(directory+node+nr+"_"+date+".png")
    


if __name__ == "__main__":
    hit_rate(node1, "1")
    #hit_rate(node2,"2")
    #hit_rate(node3, "3")
    #hit_rate(lte_node, "4")


    print("ended")
