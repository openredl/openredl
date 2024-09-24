## 4. Real Time Plotting

The library includes a Python Script (real_time_plot.py) to plot the datalogger measurements in real time. The script reads the measurements from the serial port, store them in a file called **data.csv** and plot them.
The datalogger sends a headers line when it starts running, and only the data after the last headers is plotted.
In order to plot, the data should be in a TIMESERIES format.

### 4.1 How to use it?

#### Installin Python
To use the real time plotting script, you need to be able to run python.
If you do not have it, you can do it, for example, installing miniforge:

https://conda-forge.org/miniforge/

Download and execute the file for your operating system.
In windows, just run the exe file.
In linux or mac, open a Terminal and run:
    ```console<br/>
    $ chmod +x ~Downloads/[TheDownloadedMiniforgefile].sh<br/>
    $ sh ~Downloads/[TheDownloadedMiniforgefile].sh<br/>
    $ source ~/miniforge/bin/activate<br/>
    ```


#### 4.2 Create a Python environment
You can do it with any prefered method. The steps using Anaconda / Miniconda Terminal are:
1. Open a command line environment in which Conda instructions run e.g. Open Anaconda Powershell
2. Create a new Python environment with the required libraries.
    Format: [conda create -n] [environment_name] [library1] [library2] …
    ```console
    $ conda create -n openredl-env matplotlib pandas pyserial
    ```
3. Activate the environment
    ```console
    $ conda activate openredl-env
    ```
5. Install the tools library pyopenredl
    Go to the Tools folder:
   ```console
    $ cd .\openredl\Tools\
    $ python -m pip install -e ./pyopenredl/
    

4. Open the script file **real_time_plot.py** and configure it as required. After modifying it, **save the file**!
    
As seen in the script:

```python
    ##########################################################################
    # CONFIGURATION PARAMETERS
    ##########################################################################<br/>
        # Automatic port selection.
    terms = ['D307R956', 'arduino'] #terms to search automatically for a port.
    port = autoselect_port(terms, 0)
    # Else, select manually the port of the Datalogger in your computers
    # port = '/dev/ttyACM0'
    # how many units of data should be plotted
    
    unit = "points" # points/seconds
    amount = 10*60 # how many units should be kept in the plot
    
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
```

    
6. Execute the script in Terminal or e.g. Anaconda Powershell.
    Go to the folder where the plot script is stored:
    ```console
    $ cd .\openredl\Tools\
    $ python real_time_plot.py
    ```
A file called _data.csv_ will be stored with the data read from the DL. You should not open it while running the script, as it is continuously being updated.
The data received is printed in the terminal where the script is executed, and can be read directly.

(Optional Interaction)
If you want to:<br/>
O7. **add a note to the log file** just write it in the textbox and type <enter>.<br/>
O8. **restart** the plot as a new one, press the datalogger restart button.


#### 4.3 Other Useful Instructions
To remove an Anaconda environment run:<br/>
    ```console
    $ conda env remove -n [env-name]
    ```



