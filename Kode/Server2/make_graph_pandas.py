import pandas as pd
import matplotlib.pyplot as plt
import os
import datetime as dt
import openpyxl
v_col = "Voltage [V]"
c_col = "Current [mA]"
p_col = "Power"
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

time_list = []

graph_start = dt.time(12,30,0)
graph_end = dt.time(14,0,0)
print(graph_start)

def slice_table(df):
    tmp_list = []
    col_list = df['Time'].tolist()
    for j in range(len(col_list)):
        if ( (col_list[j] > graph_start) & (col_list[j] < graph_end) ):
            #print(col_list[j])
            tmp_list.append(j)
    print(min(tmp_list), max(tmp_list))
    sliced = df.loc[min(tmp_list):max(tmp_list)]
    return sliced


def make_graph(node, nr):
    i = 0
    #while( (start_date+dt.timedelta(days=1)).date() != dt.datetime.now().date() ):
        
    try:
        
        date = (start_date+dt.timedelta(days=12)).date().strftime("%Y-%m-%d")
        file_name = directory + node + date + ".xlsx"
        
        table = pd.read_excel(file_name, usecols="B:F", convert_float = True)
        #table.rename(columns={'Power':'Power [mW]'})
    
        sliced = slice_table(table)
        ax = plt.gca()

        #plt.cla()
        sliced.plot(kind='line',x=time, y=p_col, ax=ax)
        
        
        
        # plt.cla()
        # table.plot(kind='line', x=time, y=v_col, ax=ax)
        # plt.locator_params(axis='x', nbins=7)
        # plt.savefig(directory+node+save_dir+"voltage"+'\\'+nr+"_test"+date+".png")
        
        # plt.cla()
        # table.plot(kind='line', x=time, y=c_col, ax=ax)
        # plt.locator_params(axis='x', nbins=7)
        # plt.savefig(directory+node+save_dir+"current"+'\\'+nr+"_"+date+".png")

        # plt.cla()
        # table.plot(kind='line', x=time, y=p_col, ax=ax)
        # plt.locator_params(axis='x', nbins=7)
        # plt.savefig(directory+node+save_dir+"power"+'\\'+nr+"_"+date+".png")

        # plt.cla()
        # table.plot(kind='line', x=c_col, y=v_col, ax=ax)
        # plt.locator_params(axis='x', nbins=7)
        # plt.savefig(directory+node+save_dir+nr+"_"+date+".png")

        # plt.cla()
        # table.plot(kind='line', x=time, y=bat, ax=ax)
        # plt.locator_params(axis='x', nbins=7)
        # plt.savefig(directory+node+save_dir+"battery"+'\\'+nr+"_"+date+".png")

        #plt.show()
    except FileNotFoundError:
        pass
        i = i+1


if __name__ == "__main__":
    make_graph(node1, "1")
    make_graph(node2,"2")
    #make_graph(node3, "3")
    #make_graph(lte_node)
    plt.show()


    print("ended")
