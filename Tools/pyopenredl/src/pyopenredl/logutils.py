"""
Created on Tue Jul 30 10:53:35 2024

@author: aj
"""
import os, datetime
import pandas as pd
from io import StringIO
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
from numpy import nan
from pathlib import Path
import warnings
warnings.filterwarnings("ignore", message="No artists with labels found to put in legend.")

file_extention = ".TXT"
file_marker = "M"
t_start = "2000/01/01 00:00:00"
t_end = "2100/01/01 00:00:00"
cols_to_plot = []



def get_files_list(dir_path = None,
                   t_start = None,
                   t_end = None,
                   file_extention = file_extention,
                   file_marker = file_marker,
                   ):
    """Filename format [file_marker, M]YYMMDD[file_extention, .TXT]"""
    
    if t_start is None: t_start = datetime.datetime(1900, 1, 1)
    if t_end is None:   t_end = datetime.datetime(2100, 1, 1)
    
    dt_start = datetime.datetime.strptime(t_start,
                                          '%Y/%m/%d %H:%M:%S')
    dt_end = datetime.datetime.strptime(t_end,
                                          '%Y/%m/%d %H:%M:%S')

    
    files = []
    if dir_path =="":
        dir_path_formated = None
    else:
        dir_path_formated = Path(dir_path)
    for file in os.listdir(dir_path_formated):
        if file.endswith(file_extention) and file.startswith(file_marker):
            #print(file)
            year = int(file[1:3])+2000
            month = int(file[3:5])
            day = int(file[5:7])
            #print(year, month, day)
            dt_file = datetime.datetime(year, month, day)
            if (dt_file >= dt_start.replace(minute = 0, hour = 0, second = 0)
                and dt_file <= dt_end.replace(minute = 0, hour = 0, second = 0)):
                files.append(file)
    #print(files)
    return files




def load_files_in_df(files, path, t_start, t_end, t_correction_h=0, t_correction_s=0 ):
    
    df = pd.DataFrame()
    for file in files:
        filepath = os.path.join(path, file)
        t = ""
        with open(filepath) as f:
            print("file", filepath)
            for line in f:
                if line.startswith('H<') or line.startswith('H,'):
                    #print(line)
                    if t != "":
                        #print("set in file", file)
                        df2 = pd.read_csv(StringIO(t),
                                          index_col = 0,
                                          parse_dates = True)
                        #print(df2)
                        if not df2.empty:
                            df = pd.concat([df, df2], axis = 0)

                    t = line[2:]
    
                if line.startswith('M<') or line.startswith('M,'):
                    t += line[2:]
                
            df2 = pd.read_csv(StringIO(t),
                              index_col = 0,
                              parse_dates = True)
            df = pd.concat([df, df2], axis = 0)
    
    if len(df):
        df.sort_index(inplace = True)
        #tzoffset =  datetime.timedelta(hours=-2) # offset to convert datetime to UTC
        #df['localdt'] = df.index + tzoffset
        #df.set_index('localdt', inplace = True)
        df.index = (df.index +
                pd.Timedelta(t_correction_s, unit='s') +
                pd.Timedelta(t_correction_h, unit='h')
               )


        df = df[(df.index> t_start) & (df.index< t_end)]
        #df.index = pd.to_datetime(df.index, unit="s")
        # Filtering time
        df = df[(df.index> t_start) & (df.index< t_end)]
        if "notes>" in df.columns:
             df = df.drop('notes>', axis=1)
    
    return df
    
def read_data_to_df(dir_path,
                   t_start=None,
                   t_end=None,
                   t_correction_h=0,
                   t_correction_s=0,
                   select_columns=None
                   ):
                   
    files = get_files_list(dir_path = dir_path,
                     t_start = t_start,
                     t_end = t_end,
                     )
                     
    df = load_files_in_df(files,
                     dir_path,
                     t_start,
                     t_end,
                     t_correction_h,
                     t_correction_s
                     )
                     
    if not len(df):
        print("No data found for the given time interval.\n")
    else:
        print(f"{len(df)} rows found for the given time interval.\n")
        df = df.dropna(axis=1, how='all')
        print(f"Columns with data in the given time: {list(df.columns)}")

    if isinstance(select_columns, list) and len(select_columns)>0:

        columns = list(df.columns.values)
               
        cols_selected=[]
        for col in select_columns:
            if isinstance(col, str):
                col_tag = col
            elif isinstance(col, int):
                try:
                    col_tag = columns[col]
                except IndexError:
                    print(f"Column {col} discarded, not in index")                    
            else:
                print(f"Column {col} discarded, must be int or str")
                continue
            
            if col_tag in columns:
                cols_selected.append(col_tag)
            else:
                print(f"Column {col} discarded, not in df columns")

        # Remove duplicted items
        cols_selected = list(set(cols_selected))

        if len(cols_selected)>0:
            print(f"Columns selected: {cols_selected}")    # Selecting columns
            df = df[cols_selected]

    df = df.dropna(axis=1, how='all')
    return df



def check_share_x(cols_to_plot):
    """Check if all plots have default x axis."""
    if len(cols_to_plot)==0:
        return True
    for ax_num, cols in enumerate(cols_to_plot):
        if type(cols[0]) is dict:
            return False
    else:
        return True

kwds = {#"linestyle": 'None',
        #"marker": ".",
        "lw": 0.4,
        "marker": ".",
        "markersize":0.8
        }


def plot_data(df, fig, axes, cols_to_plot=None, resample_interval = None, ):

    columns = df.columns.values
   
    if len(cols_to_plot) == 1:
        axes = [axes]
    
    if resample_interval is None:
        data = df
        resample_label=""
    else:
        data = df.resample(resample_interval).mean()
        resample_label= "_" + resample_interval

    if cols_to_plot == None or len(cols_to_plot)==0:
        cols_to_plot = [[c] for c in data.columns]
    elif len(cols_to_plot) == 0:
        print("Colums to plot is empty")
     
    for ax_num, col_info in enumerate(cols_to_plot):
        kwds_ax = kwds.copy()
        x_plot = data.index # index is default x axis
        
        
        if type(col_info[0]) is dict:
            col_info_tmp = col_info[0].copy()
            cols = col_info_tmp.pop("y")
            
            if "x" in col_info_tmp.keys():
                x_plot = data[columns[col_info_tmp.pop('x')]]

            kwds_ax.update(col_info_tmp)
            
        elif type(col_info) is str:
            cols = [col_info]
            
        elif type(col_info) is int:
        
            cols = [col_info]
        
        else:
            cols = col_info

        for col in cols:
            try:
                # is the axis given by name?
                if isinstance(col, str):
                    y_plot = data[col]
                    kwds_ax["label"] = col + resample_label

                    
                # is the axis given by column number?
                else:
                    y_plot = data[columns[col]]
                    kwds_ax["label"] = columns[col] + resample_label
                
                axes[ax_num].plot(x_plot,
                                  y_plot,
                                   **kwds_ax
                                       )
            except IndexError:
               print("[Warning] Column to plot not found:", col)
            except KeyError:
               print("[Warning] Key not found in data", col)
               
        for ax in axes:
            ax.legend(loc='center left',
                      bbox_to_anchor=(1.01, 0.5),
                      ncols=1,
                      fontsize = "small"
                      )
            ax.grid(visible=True)
    #fig.tight_layout( )
    #plt.show()

               
               
    return data
                           
