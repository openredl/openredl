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





def get_files_list(dir_path = None,
                   file_extention = ".TXT",
                   file_marker = "M",
                   t_start = None,
                   t_end = None
                   ):
    """Filename format [file_marker, M]YYMMDD[file_extention, .TXT]"""
    
    if t_start is None: t_start = datetime.datetime(1900, 1, 1)
    if t_end is None:   t_end = datetime.datetime(2100, 1, 1)
    
    dt_start = datetime.datetime.strptime(t_start,
                                          '%Y/%m/%d %H:%M:%S')
    dt_end = datetime.datetime.strptime(t_end,
                                          '%Y/%m/%d %H:%M:%S')

    
    files = []
    if dir_path =="": dir_path = None
    for file in os.listdir(dir_path):
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




def load_files_in_df(files, path, t_start, t_end):
    
    df = pd.DataFrame()
    for file in files:
        filepath = os.path.join(path, file)
        t = ""
        with open(filepath) as f:
            print("file", filepath)
            for line in f:
                if line.startswith('H<'):
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
    
                if line.startswith('M<'): t += line[2:]
                
            df2 = pd.read_csv(StringIO(t),
                              index_col = 0,
                              parse_dates = True)
            df = pd.concat([df, df2], axis = 0)
    
    df.sort_index(inplace = True)
    tzoffset =  datetime.timedelta(hours=-2) # offset to convert datetime to UTC
    df['localdt'] = df.index + tzoffset
    df.set_index('localdt', inplace = True)
    df = df[(df.index> t_start) & (df.index< t_end)]
    #df.index = pd.to_datetime(df.index, unit="s")
    
    return df