#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 30 10:53:35 2024
@author: aj
"""
from pyopenredl.logutils import *
#!/usr/bin/env python3
# -*- coding: utf-8 -*-



if __name__ == "__main__":
    ##############################################################################
    # Configuration
    ##############################################################################
    ## Files to read
    # (with the format [file_marker]YYMMDD[file_extension], e.g. M240726.TXT)
    dir_path = "example_sensors_sampling/ArduinoDL/"
    file_extention = ".TXT"
    file_marker = "M"

    ## Date filter
    t_start = "2024/07/28 14:55:00"
    t_end = "2024/07/28 15:05:00"

    ## Columns filter
    columns = ["P2real[W]",
               'DIFF1_mean[mV]',
               ]
    ##############################################################################

    # Filter and resample data
    dt_start = datetime.datetime.strptime(t_start,
                                          '%Y/%m/%d %H:%M:%S')
    dt_end = datetime.datetime.strptime(t_end,
                                          '%Y/%m/%d %H:%M:%S')


    files = get_files_list(dir_path, file_extention, file_marker, t_start, t_end)
    df = load_files_in_df(files, dir_path, t_start, t_end)
    df = df.drop('notes>', axis=1)
    df = df[columns] # Selecting required columns
    df = df[(df.index> t_start) & (df.index< t_end)]
    df_rs60s = df.resample('60s').mean()
    df_rs60s = df[(df.index> t_start) & (df.index< t_end)]


    # Output
    # df: pandas dataframe with data read with the columns and datetime filter
    # df_rs60s: df resampled to 60 seconds
