#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 30 10:53:35 2024

@author: aj
"""
from pyopenredl.logutils import read_data_to_df, t_start, t_end

if __name__ == "__main__":

    ##############################################################################
    # Configuration
    ##############################################################################
    ## Files to read
    dir_path = "example_sensors_sampling/ArduinoDL/"
    # offset to convert datetime from measurements
    time_correction_hours = 0
    time_correction_seconds = 0
    # Date filter (after correction) If not given, a default value is be used
    # format '%Y/%m/%d %H:%M:%S' e.g. "2024/09/26 12:50:00"
    t_start = "2024/7/26 12:30:00"
    t_end = "2024/7/30 10:00:00"

    # Colums to plot
    select_columns =[
                     "DIFF1_mean[mV]",
                     "DIFF1_min[mV]",
                     "DIFF1_max[mV]",
                     "V2rms[V]",
                     "P2real[W]",
                     "PF2[]",
                     "f2[Hz]"
                     ]
            
    ## Code
    ###########################################################################
    # Reading log files
    df = read_data_to_df( dir_path,
                          t_start,
                          t_end,
                          t_correction_h=time_correction_hours,
                          t_correction_s=time_correction_seconds,
                          select_columns=select_columns
                          )
    # Selecting dates
    # Resampling
    df_rs60s = df.resample('60s').mean()

    # Output
    # df: pandas dataframe with data read with the columns and datetime filter
    # df_rs60s: df resampled to 60 seconds
