"""Real Time Plot script addapted for battery storage experiment
   For execution, remove the extension .h, only necessary for autocopy.
"""
import pyopenredl.realtimeplotutils as rtp

##########################################################################
# CONFIGURATION PARAMETERS
##########################################################################

# Automatic port selection.
terms = ['D307R956', 'arduino'] #terms to search automatically for a port.
port = rtp.autoselect_port(terms, 0)
# Else, select manually the port of the Datalogger in your computers.
# You can find the right port name in:
# Arduino Ide > tools > port or in Windows: device manager.
#port = '/dev/ttyACM0'
#port = '/dev/rfcomm0'
#port = 'COM3'

# how many units of data should be plotted
unit = "seconds" # points/seconds
amount = 60*60 # how many units should be kept in the plot

# Which data columns should be plotted?
# Array of subarrays. Each subarray "[]" is a plotting axis.
# y_axis_n can be given by column_number or column_name
# column_number starts with column 0 (datetime)
# Possible subarrays format:
#       [y_axis1, y_axis2...]
#       [{"y":[y_axis1, y_axis2...]}]
#       [{"x":x_axis, "y":[y_axis1, y_axis2...], "<matplotlib_plot_kwd:value}]
columns_to_plot = [
   #[1,2,3,4], # example multiple y_axis with column0/datetime as x_axis
   #[1],  # example one y_axis with column0/datetime as x_axis
   #[2, "P1real[W]"], # example with y_axis as column number and as column_name
   #[{"x":2,"y":[2,"P1real[W]"]}], # example with other column as x _axis
   #[{"x":2,"y":[3], "linestyle":"none"}], # example with extra plot kwd,
    [1],
    [2],
    [3],
    #[4],
    #[5],
    #[6],
                   ]
##########################################################################

# Create object dl type arduino _serial and run it!
dl = rtp.SerialDL(port, amount, columns_to_plot, unit)
dl.run()

