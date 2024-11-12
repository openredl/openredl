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
    dir_path = "data/example_sensors_sampling/ArduinoDL/"
    # offset to convert datetime from measurements
    time_correction_hours = 0
    time_correction_seconds = 0
    # Date filter (after correction) If not given, a default value is be used
    # format '%Y/%m/%d %H:%M:%S' e.g. "2024/09/26 12:50:00"
    t_start = "2024/7/27 00:00:00"
    t_end = "2024/7/28 00:00:00"

    # Columns to select. Empty array will select all columns in data
    select_columns =[
                     #"DIFF1_mean[mV]",
                     #"DIFF1_min[mV]",
                     #"DIFF1_max[mV]",
                     #"V2rms[V]",
                     #"P2real[W]",
                     #"PF2[]",
                     #"f2[Hz]",
                     1,
                     2,
                     ]

         
    #######################################################################           
    ## Example load and select data from logs
    #######################################################################
    df = read_data_to_df( dir_path,
                          t_start,
                          t_end,
                          t_correction_h=time_correction_hours,
                          t_correction_s=time_correction_seconds,
                          select_columns=select_columns
                          )
    #######################################################################
    # Example Resampling
    #######################################################################
    # df_rs60s = df.resample('60s').mean()
    #######################################################################
    # Example Plotting
    #######################################################################
    plot_df=True
    if plot_df:
        # Colums to plot (If [] : empty array given, will plot all columns in data)
        #cols_to_plot =[
        #              ["DIFF1_mean[mV]", "DIFF1_min[mV]", "DIFF1_max[mV]"],
        #              "V2rms[V]",
        #              "P2real[W]",
        #              "PF2[]",
        #              "f2[Hz]"
        #              ]
        # Plot Configuration
        kwds = {
                #"linestyle": 'None',
                "marker": ".",
                "lw": 0.6,
                "marker": ".",
                "markersize":0.8
                }
        # Creating plot
        nrows = len(cols_to_plot) if cols_to_plot else len(list(df.columns.values))
        fig, axes = plt.subplots(ncols=1,
                                 nrows=nrows,
                                 figsize=(12,6),
                                 sharex = check_share_x(cols_to_plot),
                                 num = "Data Plot",
                                 )

        plot_data(df, fig, axes, cols_to_plot=cols_to_plot)
    # Plotting resampled data
    plot_resample=True
    if plot_resample:
        # Plot with given plot function. For more detail better use matplotlib
        df_rs5min = plot_data(df, fig, axes, cols_to_plot=cols_to_plot,
                            resample_interval="5min")
    ###########################################################################
    # Final Plot Format
    fig.tight_layout( )
    plt.show()
    
    ############################################################################
    # Output
    # data: pandas dataframe with data read with the columns and datetime filter
    # data_rs60s: df resampled to 60s
    # data_rs5min: df resampled to 5min

