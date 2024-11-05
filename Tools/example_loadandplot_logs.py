#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 30 10:53:35 2024

@author: aj
"""
from pyopenredl.logutils import read_data_to_df, plot_data, check_share_x, \
kwds, t_start, t_end, cols_to_plot
import matplotlib.pyplot as plt

if __name__ == "__main__":

    ##############################################################################
    # Configuration
    ##############################################################################
    ## Files to read
    # (with the format [file_marker]YYMMDD[file_extension], e.g. M240726.TXT)
    dir_path = "example_sensors_sampling/ArduinoDL/"
    # offset to convert datetime from measurements
    time_correction_hours = 0
    time_correction_seconds = 0
    # Date filter (after correction)
    # format '%Y/%m/%d %H:%M:%S' e.g. "2024/09/26 12:50:00"
    t_start = "2024/7/27 00:00:00"
    t_end = "2024/7/30 00:00:00"

    # Colums to plot (If [] : empty array given, will plot all columns in data)
    cols_to_plot =[["DIFF1_mean[mV]", "DIFF1_min[mV]", "DIFF1_max[mV]"],
                  ["DIFF1_mean[mV]", "DIFF1_min[mV]", "DIFF1_max[mV]"],
                  "V2rms[V]",
                  "P2real[W]",
                  "PF2[]",
                  "f2[Hz]"
                  ]
    # Plot Configuration
    kwds = {
            #"linestyle": 'None',
            "marker": ".",
            "lw": 0.4,
            "marker": ".",
            "markersize":0.8
            }
            
    ## Code
    ###########################################################################
    # Reading log files
    df = read_data_to_df( dir_path,
                          t_start,
                          t_end,
                          t_correction_h=time_correction_hours,
                          t_correction_s=time_correction_seconds
                          )

    # Creating plot
    nrows = len(cols_to_plot) if cols_to_plot else len(list(df.columns.values))
    fig, axes = plt.subplots(ncols=1,
                             nrows=nrows,
                             figsize=(12,6),
                             sharex = check_share_x(cols_to_plot),
                             num = "Data Plot",
                             )

    # Plotting
    data = plot_data(df, fig, axes, cols_to_plot=cols_to_plot)
    # Plotting with resample
    #data_1min = plot_data(df, fig, axes, cols_to_plot=cols_toplot, resample_interval="1min")
    # example for resample
    #df_rs15min = df.resample('15min').mean()
   
    fig.tight_layout( )
    plt.show()
    
    ############################################################################
    # Output
    # data: pandas dataframe with data read with the columns and datetime filter
    # data_rs1min: df resampled to 1min
    # data_rs15min: df resampled to 15min