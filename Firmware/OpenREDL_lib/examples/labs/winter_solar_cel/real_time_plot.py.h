"""Real Time Plot script template
   ** THIS IS A PYTHON SCRIPT, not a .h header ** 
   To execute it:
   - remove the file name extension .h (only necessary for autocopy)
   - activate the right environment: conda activate openredl-env 
   - execute the script: python real_time_plot.py
"""

import pyopenredl.realtimeplotutils as rtp

##########################################################################
# CONFIGURATION PARAMETERS
##########################################################################

#####################################################################
########## Port Selection ###########################################
#####################################################################
# Default is autoselect. Else, select manually the port of the Datalogger in your computers.
# You can find the right port name in:
# Arduino Ide > tools > port or in Windows: device manager.
#port = '/dev/ttyACM0'
#port = '/dev/rfcomm0'
#port = 'COM3'
#terms to search automatically for a port.
terms = ['D307R956', 'arduino', "2341:0043"]
# Automatic port selection.
port = rtp.autoselect_port(terms, 0)

#####################################################################
########## Plot Length ##############################################
#####################################################################
# how many units of data should be plotted
unit = "seconds" # points/seconds
amount = 10*60 # how many units should be kept in the plot

#####################################################################
########## Data to Plot #############################################
#####################################################################
# Which data columns should be plotted?
# Array of subarrays. Each subarray "[]" is a plotting axis.
# y_axis_n can be given by column_number or column_name
# column_number starts with column 0 (datetime)
# Possible subarrays format:
#       [y_axis1, y_axis2...]
#       [{"y":[y_axis1, y_axis2...]}]
#       [{"x":x_axis, "y":[y_axis1, y_axis2...], "<matplotlib_plot_kwd:value}]
#
#       [1,2,3,4], # example multiple y_axis with column0/datetime as x_axis
#       [1],  # example one y_axis with column0/datetime as x_axis
#       [2, "P1real[W]"], # example with y_axis as column number and as column_name
#       [{"x":2,"y":[2,"P1real[W]"]}], # example with other column as x _axis
#       [{"x":2,"y":[3], "linestyle":"none"}], # example with extra plot kwd,
#       [{"x":2,"y":[3,4], "linestyle":"none"}], # example with

# Headers example: ['datetime', 'DIFF1_mean[mV]', 'DIFF1_max[mV]', 'DIFF2_mean[mV]', 'DIFF2_max[mV]', 'notes']

columns_to_plot = [
                    [1], # V
                    [2], # I
                    [3], # G?
                    [{"x":1,"y":[2], "linestyle":"none"}],
                   ]
                   
#####################################################################
########## Run the code #############################################
#####################################################################
# Create data logger object and run it!
dl = rtp.SerialDL(port, amount, columns_to_plot, unit)
dl.run()


