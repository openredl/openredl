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

    # Date filter
    t_start = "2024/07/25 15:00:00"
    t_end = "2024/07/30 8:00:00"

    # Colums to plot
    cols_toplot =[ "DIFF1_mean[mV]",
                  "DIFF3_mean[mV]",
                  "V2rms[V]",
                  ]


    ###########################################################################

    dt_start = datetime.datetime.strptime(t_start,
                                          '%Y/%m/%d %H:%M:%S')
    dt_end = datetime.datetime.strptime(t_end,
                                          '%Y/%m/%d %H:%M:%S')

    ###########################################################################    
    # Reading log files
    files = get_files_list(dir_path, file_extention, file_marker, t_start, t_end)
    df = load_files_in_df(files, dir_path, t_start, t_end)
    df = df.drop('notes>', axis=1)
    # Time correction
    df.index = df.index + pd.Timedelta(10, unit='s')
    # Filtering time
    df = df[(df.index> t_start) & (df.index< t_end)]
    # Resampling
    df_rs15min = df.resample('15min').mean()


    ###########################################################################    
    # Plotting
    ###########################################################################    
    fig, axes = plt.subplots(ncols=1, nrows=len(cols_toplot),
                             figsize=(9,9),
                             sharex = True)
    fig.tight_layout(rect=[0.02, 0.02, .98, 0.95])
    axes.flatten()

    lw =0.75
    mk = ","
    mk_sz = 2
    
    for i in range(len(cols_toplot)):
        axnum = i

        x = df.index
        y = df[cols_toplot[i]]
        axes[axnum].step(x, y, marker = mk, markersize = mk_sz, 
                         where = 'post',
                         linewidth = lw,
                         label = cols_toplot[i],
                         )
        x = df_rs15min.index
        y = df_rs15min[cols_toplot[i]]
        axes[axnum].step(x, y, marker = mk, markersize = mk_sz, 
                         where = 'post',
                         linewidth = lw,
                         label = cols_toplot[i] + "_15min",
                         )
        
        axes[axnum].legend(loc = 'upper left')
        axes[axnum].grid()

    plt.show()
    
    ############################################################################
