#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 30 10:53:35 2024

@author: aj
"""
from pyopenredl.logutils import *


if __name__ == "__main__":
    ##############################################################################
    # Configuration
    ##############################################################################
    ## Files to read
    # (with the format [file_marker]YYMMDD[file_extension], e.g. M240726.TXT)
    dir_path = "example_sensors_sampling/ArduinoDL/"
    file_extention = ".TXT"
    file_marker = "M"

    # offset to convert datetime from CET to UTC
    tzoffset =  datetime.timedelta(hours=-2)

    ## Date filter
    t_start = "2024/07/25 15:00:00"
    t_end = "2024/07/30 8:00:00"


    ###########################################################################

    dt_start = datetime.datetime.strptime(t_start,
                                          '%Y/%m/%d %H:%M:%S')
    dt_end = datetime.datetime.strptime(t_end,
                                          '%Y/%m/%d %H:%M:%S')

    ###########################################################################    
    # Reading log files from OpenREdl
    files = get_files_list(dir_path, file_extention, file_marker, t_start, t_end)
    df = load_files_in_df(files, dir_path, t_start, t_end)
    df = df.drop('notes>', axis=1)
    df.index = df.index + pd.Timedelta(10, unit='s')
    #df = df[(df.index> t_start) & (df.index< t_end)]
    #df_selected = df[columns]
    df = df[(df.index> t_start) & (df.index< t_end)]
    df_rs = df.resample('10s').mean()
    df_rs = df_rs[(df_rs.index> t_start) & (df_rs.index< t_end)]
    df_rs60s = df.resample('60s').mean()
    
    # Labels correcponding to the data of each channel
    df_labels = {"pv": "DIFF1",
                 "pyran": "DIFF3",
                 "Vac": "DIFF5"
                 }
    
    ###########################################################################    
    # Reading log files from Campbell1000
    file_path = "example_sensors_sampling/Campbell1000/CR1000_2_Table_10s.dat"
    df2 = pd.read_csv(file_path,
                  index_col = 0,
                  parse_dates = True,
                  skiprows=[0,2,3]
                  )
    

    df2['localdt'] = df2.index + tzoffset
    df2.set_index('localdt', inplace = True)    
    df2 = df2.drop('RECORD', axis=1)
    df2 = df2[(df2.index> t_start) & (df2.index< t_end)]
    df2.replace('NAN', nan, inplace = True) 
    df2_rs = df2[["PV_Voc_Avg", "Voltage_Avg", "Pyr_Avg"]].resample('10s').mean()
    df2_rs60s = df2[["PV_Voc_Avg", "Voltage_Avg", "Pyr_Avg"]].resample('60s').mean()
    df2_rs = df2_rs[(df2_rs.index> t_start) & (df2_rs.index< t_end)]
    #smallest_df_len = min(len(df2), len(df))
    #df = df.head(smallest_df_len)
    #df2 = df2.head(smallest_df_len)
    #df_rs = df_rs.head(smallest_df_len)
    #df2_rs = df2_rs.head(smallest_df_len)

    ###########################################################################    

    # Plotting

    fig, axes = plt.subplots(ncols=1, nrows=6, figsize=(9,9),
                              sharex = True)
    fig.tight_layout(rect=[0.02, 0.02, .98, 0.95])
    axes.flatten()

    lw =0.75
    mk = ","
    mk_sz = 2
    ###########################################################################    
    axnum = 0

    x = df.index
    y = df[df_labels["pyran"]+"_mean[mV]"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                      linewidth = lw,
                        #color = 'red', 
                        label ="OpenDL",
                        )
       
    x = df2.index
    y = df2["Pyr_Avg"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="Campbell1000",
                        )

    axes[axnum].legend(loc = 'upper left')
    axes[axnum].grid()
    axes[axnum].set_ylabel("mV")
    axes[axnum].set_title('Pyranometer')    

    ###########################################################################    
    axnum = 1
    ax_r = axes[axnum].twinx()
    
    y = 1 - df_rs60s[df_labels["pyran"]+"_mean[mV]"] / df2_rs60s["Pyr_Avg"]
    x = y.index
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="1 - OpenDL/Campbell1000",
                        )
    
    y = df_rs[df_labels["pyran"]+"_mean[mV]"] - df2_rs["Pyr_Avg"]
    x = y.index
    ax_r.step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        color = 'red', 
                    label ="OpenDL - Campbell1000",
                        )
    
    axes[axnum].yaxis.set_major_formatter(mtick.PercentFormatter(1.0,2))
    axes[axnum].legend(loc = 'upper left')
    ax_r.legend(loc = 'upper right')
    axes[axnum].grid()
    axes[axnum].set_ylabel("mV/mV")
    ax_r.set_ylabel('mV')    
    axes[axnum].set_title('Pyranometer difference')    

    ###########################################################################    
    axnum = 2

    x = df.index
    y = df["V2rms[V]"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="OpenDL",
                        )
    
    x = df2.index
    y = df2["Voltage_Avg"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="Campbell1000",
                        )
    
    axes[axnum].legend(loc = 'upper left')
    axes[axnum].grid()
    axes[axnum].set_ylabel("V")
    axes[axnum].set_title('Vac')    

    ###########################################################################    
    axnum = 3
    ax_r = axes[axnum].twinx()


    x = df_rs.index
    y = 1- df_rs["V2rms[V]"] / df2_rs["Voltage_Avg"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                     #color = 'red', 
                    label ="1 - OpenDL/Campbell1000",
                    )
    
    y = df_rs["V2rms[V]"] - df2_rs["Voltage_Avg"]
    x = y.index
    ax_r.step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        color = 'red', 
                    label ="OpenDL - Campbell1000",
                        )

    axes[axnum].yaxis.set_major_formatter(mtick.PercentFormatter(1.0,2))
    axes[axnum].legend(loc = 'upper left')
    ax_r.legend(loc = 'upper right')
    axes[axnum].grid()
    axes[axnum].set_ylabel("V/V")
    ax_r.set_ylabel('V')
    axes[axnum].set_title('Vac difference')    

############################################################################    
    axnum = 4

    x = df.index
    y = df[df_labels["pv"]+"_mean[mV]"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="OpenDL",
                        )
    
    x = df2.index
    y = df2["PV_Voc_Avg"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="Campbell1000",
                        )
    
    axes[axnum].legend(loc = 'upper left')
    axes[axnum].grid()
    axes[axnum].set_ylabel("mV")
    axes[axnum].set_title('PV Voc')    

############################################################################    

    axnum = 5
    ax_r = axes[axnum].twinx()


    x = df_rs.index
    y = 1 - df_rs[df_labels["pv"]+"_mean[mV]"] / df2_rs["PV_Voc_Avg"]
    axes[axnum].step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                        #color = 'red', 
                        label ="1 - OpenDL/Campbell1000",
                        )

    y = df_rs[df_labels["pv"]+"_mean[mV]"] - df2_rs["PV_Voc_Avg"]
    x = y.index
    ax_r.step(x, y, marker = mk, markersize = mk_sz, where = 'post',
                    color = 'red', 
                    label ="OpenDL - Campbell1000",
                    )
    
    axes[axnum].yaxis.set_major_formatter(mtick.PercentFormatter(1.0,2))
    axes[axnum].legend(loc = 'upper left')
    ax_r.legend(loc = 'upper right')
    axes[axnum].grid()
    axes[axnum].set_ylabel("mV/mV")
    ax_r.set_ylabel('mV')    
    axes[axnum].set_title('PV Voc difference')
    
############################################################################
    plt.show()
