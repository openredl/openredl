from matplotlib.animation import FuncAnimation
from threading import Thread
import matplotlib.pyplot as plt
from matplotlib.widgets import TextBox
import pandas as pd
import serial
import serial.tools.list_ports
import time, os , datetime
import warnings
warnings.filterwarnings( "ignore", message = ".*This figure includes Axes that are not compatible with tight_layout.*" )
warnings.filterwarnings("ignore", message="No artists with labels found to put in legend.")
import time
import traceback

class SerialDL:
    """ Class that integrate the methods to read and plot in real time 
    DL measurements from the serial/USB port.
    """
    
    def __init__(self, port,
                 points=4*60,
                 cols_to_plot=[1,2,3,4],
                 select = "seconds"
                 ):
        """ Initialize variables"""
        self.port = port
        self.fig = None
        self.width = 0
        self.height = 0
        self.legend_adjusted = False
        self.select = select
        self.points = points
        self.cols_to_plot = cols_to_plot
        self.axes = []
        self.lines = []
        self.share_x = self.check_share_x()
        self.stop = False
        self.filename = 'data.csv'
        self.headers_filename = "headers.csv"
        self.ani = None
        self.last_label_on=True
        print(f'\n{"#"*65}\n{"#"*20} Datalogger Plotting Started! {"#"*20}\n{"#"*65}')

        ports = [p.device for p in list(serial.tools.list_ports.comports())]
        if port in ports:
            self.ser = serial.Serial(port, 115200, timeout=1)
            print("[INFO] Started serial port", port)
            time.sleep(1)
            self.ser.reset_input_buffer()
        else:
            print(f'[ERROR] Serial port {port} not found, try an existing one:')
            ports = serial.tools.list_ports.comports()
            for p in ports:
                print("port:", p.device, ', Manufacturer:', p.manufacturer)
            print(f'\n{"#"*65}\n{"#"*20} Data Logger Plotting Ended! {"#"*20}\n{"#"*65}')
            self.stop = True

    def start_read_serial(self):
        """Start to read data from the serial port."""
        
        try:
            while not self.stop:
                self.read_data()
        except Exception as e:
                self.stop = True # Stop the plot if there is exception
                print (f"[Exception] in start_read_serial: {e}")
                traceback.print_exc()
                print ("[INFO] To end program close plot window or press <Ctrl+c>")
                #raise Exception
        finally:
                self.end_read_serial()
            
            
    def end_read_serial(self):
        """End to read data from the DL serial port."""
        self.ser.close()
        print("[INFO] Ended serial port", self.port)
        path = os.path.dirname(os.path.realpath(__file__))
        file_path = os.path.join(path, self.filename)
        print(f"[INFO] The data is stored in the file:\n{file_path}")
        print(f'\n{"#"*65}\n{"#"*20} Data Logger Plotting Ended! {"#"*20}\n{"#"*65}')


    def check_share_x(self):
        """Check if all plots have default x axis."""
        for ax_num, cols in enumerate(self.cols_to_plot):
            if type(cols[0]) is dict:
                return False
        else:
            return True


    def read_data(self):
        '''Reads live data from the serial port and writes them into the data.csv file
        '''
        
        line = self.ser.readline()
        if line:
            #print('New line', line)
            try:
                string = line.decode("utf-8").replace('\r','')
                if string.startswith('M<') or string.startswith('M,'):
                    string = string.replace('M<', "M,")
                    string = string.replace('>', "")
                    #print('New data', string.replace('\n',''))
                    print(string.replace('M,', "").replace('\n','').replace(',','\t'))
                    with open(self.filename, "a") as f:
                        f.write(string)
                elif string.startswith('H<') or string.startswith('H,'):
                    string = string.replace('H<', "H,")
                    string = string.replace('>', "")
                    print(string.replace('H,', "").replace('\n','').replace(',',' '))
                    with open(self.filename, "a") as f:
                        f.write(string)
                    with open(self.headers_filename, "a") as f:
                        current_dt = datetime.datetime.now().strftime('%Y/%m/%d %H:%M:%S')
                        cols_list = string.replace("\n", "").split(",")
                        cols_list.pop(0) # dropping H column
                        f.write(f"{current_dt},{str(cols_list)}\n")    

                else: 
                     print(string.replace('\r','').replace('\n',''))
                     #with open(self.filename, "a") as f:
                     #   f.write(f"#{string}")

            except UnicodeDecodeError as e:
                print('[Warning]: Read incorrect data:', e)

            except Exception as e:
                print('error:', e)


    def submitnote(self, expression):
        """Submit note to data Logger via serial port interface"""
        if self.text_box.text != "" :
            print(f"Note sent: {expression}", flush = True)
            self.text_box.set_val("")
            note = "N:" + expression + "\n"
            self.ser.write(note.encode('utf-8'))


    def redistribute_plot(self, force=False):
        """Redistribute subblots and legend in window"""
        bbox = self.fig.get_window_extent().transformed(
                                            self.fig.dpi_scale_trans.inverted())
        width, height = bbox.width*self.fig.dpi, bbox.height*self.fig.dpi

        if abs(1 - self.width/width) > 0.1 or abs(1-self.height/height) > 0.1 or force:
            #print("resizing", flush = True)
            self.width = width
            self.height = height
            #sz = 60/height
            h2 = 20/height
            
            #self.fig.subplots_adjust(bottom=sz)

            pos1 = self.axbox.get_position() # get the original position 
            #print(pos1.x0, pos1.y0, pos1.width, pos1.height)
            pos2 = [pos1.x0, pos1.y0, pos1.width, h2] 
            self.axbox.set_position(pos2) # set a new position
            self.fig.tight_layout(pad = 0.0,
                                  rect = (0.02,pos1.y0 + h2*1.5, .99,.99),
                                  h_pad=0.2, w_pad=0.0
                                  )


    def start_animation(self):
        try:
            window_name = "OpenREDL - Real Time Plotter"
            self.fig, self.axes = plt.subplots(ncols=1,
                                               nrows=len(self.cols_to_plot),
                                               figsize=(12,6),
                                               sharex = self.share_x,
                                               num = window_name,
                                               #layout = "constrained"
                                               )
                                               

            #axbox = self.fig.add_axes([0.1, 0.05, 0.8, 0.075])
            self.axbox = self.fig.add_axes([0.15, 0.01, 0.7, 0.075])
            self.text_box = TextBox(self.axbox, "Add note", textalignment="left")
            self.text_box.on_submit(self.submitnote)
            #bbox = self.fig.get_window_extent().transformed(self.fig.dpi_scale_trans.inverted())
            #self.width, self.height = bbox.width*self.fig.dpi, bbox.height*self.fig.dpi
            #self.fig.tight_layout(rect = (0,pos1.y0 + h2 + 20, 1,1))
            #self.redistribute_plot()
            
            if len(self.cols_to_plot)>1: self.axes = self.axes.flatten()
            else: self.axes = [self.axes]
            #if not self.share_x:
                #pass
                #self.fig.tight_layout()
                #self.fig.subplots_adjust(bottom=0.15)
                

            self.ani = FuncAnimation(self.fig,
                                self.animate,
                                cache_frame_data=False,
                                interval=1000,
                                blit=False
                                )
            #plt.suptitle('Real Time Plotting of Data Logger Measurements.', x=0.1, ha="left")
            plt.show()
            
        except Exception:
            self.stop = True
            raise Exception


    def animate(self, i):
        '''
        Retrieves data from the data.csv file and creates a live plot.
        See Matplotlib Tutorial (Part 9): Plotting Live Data in Real-Time 
        https://www.youtube.com/watch?v=Ercd-Ip5PfQ
        '''

        if self.stop:
            # Stop animation and close plot(mainly during exceptions).
            self.ani.event_source.stop()
            #self.fig.canvas.draw_idle()
            #plt.close(self.fig)
        
        try:
            last_header_line = -1
            count_line = -1
            with open(self.filename, "r") as f:
                while new_line:=f.readline():
                    count_line += 1
                    if new_line.startswith("H,"):
                        last_header_line = count_line

            
            if last_header_line==0: #  NOTE: To take initial header. (Why?)
                headers = 0
            elif last_header_line>-1:
                headers = 1
            else:
                headers = None

            data = pd.read_csv(self.filename, header=headers,
                               skiprows=last_header_line-1,
                               index_col=1,
                               comment = "#"
                               )
            try:
                data.index = pd.to_datetime(data.index)
            except Exception as e:
                print(f"Exception while pd.to_datetime(data.index) : {e}")

            data = data.astype(float, errors = "ignore")
    
            # Select data to plot by seconds or points.
            if self.select == "points":
                data = data.tail(self.points)
            elif self.select == "seconds":
                time_delta = datetime.timedelta(seconds=self.points)
                current_time = datetime.datetime.now()
                time_filter = current_time - time_delta
                data = data[(data.index>time_filter)]
            else:
                data = data.tail(self.points)
            
            # Get column names
            if headers is not None:
                columns_names = data.columns.values
            else:
                columns_names = ["col" + str(i) for i in range(10)]
    
            for ax in self.axes:
                ax.cla()
                ax.grid(visible=True)

            ###########################################################            
            # Labeling x axes                
            ###########################################################
            if self.share_x:
                self.axes[-1].set_xlabel('Time', loc = "right", labelpad = 0)
            else:
                for ax_num, cols in enumerate(self.cols_to_plot):
                    #print("cols: ", cols, flush=True)
                    if type(cols[0]) is dict and "x" in cols[0].keys():
                        if isinstance(cols[0]['x'], int):
                            x_label = columns_names[cols[0]['x']]
                        else:
                            x_label = cols[0]['x']
                        self.axes[ax_num].set_xlabel(x_label,
                                                     loc="right",
                                                     labelpad=0
                                                     )
                        #print(f"x_label: {x_label}", flush=True)
                    else:
                        self.axes[ax_num].set_xlabel('Time',
                                                     loc="right",
                                                     labelpad=0
                                                     )
            ###########################################################
            # Default plot params
            kwds = {"lw": 0.75,
                    "marker": ".",
                    }

            ###########################################################
            # Selecting and ploting data
            ###########################################################
            if len(data):
                
                # Working each plot axis
                for ax_num, plot_axis_info in enumerate(self.cols_to_plot):
                    kwds_ax = kwds.copy()
                    
                    # if plotting axis info is given as dictionary
                    if type(plot_axis_info[0]) is dict:
                        plot_axis_info_tmp = plot_axis_info[0].copy()
                        y_cols = plot_axis_info_tmp.pop("y")
                                               
                        if "x" in plot_axis_info_tmp.keys():
                            if isinstance(plot_axis_info_tmp['x'], int):
                                x_label = columns_names[plot_axis_info_tmp.pop('x')]
                            else:
                                x_label = plot_axis_info_tmp.pop('x')
                            
                            x_plot = data[x_label]

                        # Loading plot specifications given for the axis
                        kwds_ax.update(plot_axis_info_tmp)                   
                                                         
                    # if plotting axis info is not given as dictionary    
                    else:
                        y_cols = plot_axis_info
                        x_plot = data.index # index is default x axis

                    # Plotting each y column
                    ###########################################################
                    for col in y_cols:
                        try:
                            # Get column name
                            if isinstance(col, str): # is the axis given by name?
                                y_name = col                               
                            else: # is the axis given by column number?
                                y_name = columns_names[col]
                                
                            y_plot = data[y_name]
                            kwds_ax["label"] = y_name
                            
                            self.axes[ax_num].plot(x_plot,
                                                   y_plot,
                                                   **kwds_ax
                                                   )
                            if self.last_label_on:
                                color_used = self.axes[ax_num].get_lines()[-1].get_color()
                                if isinstance(y_plot.iloc[-1], str):
                                    label_txt = y_plot.iloc[-1]
                                else:
                                    #label_txt = f"{y_plot.iloc[-1]:.2f}"
                                    label_txt = f"{y_plot.iloc[-1]}"
                                self.axes[ax_num].text(x_plot.tolist()[-1], y_plot.iloc[-1],
                                    " " + label_txt, color=color_used,  ha="left")

                        except IndexError:
                           print("[Warning] Column to plot not found:", col)
                        except KeyError:
                           print("[Warning] Key not found in data", col)
                    ###########################################################
                                
                for ax in self.axes:
                    ax.legend(loc='center left',
                              bbox_to_anchor=(1.01, 0.5),
                              ncols=1,
                              fontsize = "small"
                              )
                # Adjust plots once the legend is in the plot
                if not self.legend_adjusted:
                    self.redistribute_plot(force=True)
                    self.legend_adjusted = True
                
            else:
                print("[INFO] No data to plot after last headers.")
            ###########################################################
            
            
        except FileNotFoundError:
            print("[INFO] Data file to plot not found")
        except pd.errors.EmptyDataError:
            print("[INFO] No data found after last headers")
        except pd.errors.ParserError as e:
            print("Parse error:", e)
            
        self.redistribute_plot()
        #print("--- %s seconds ---" % (time.time() - start_time))


    def run(self):
        """Start reading serial data and plotting measurements"""
        
        try:
            if not self.stop:
                # Read serial port in a thread
                thread_read_serial = Thread(target=self.start_read_serial)
                thread_read_serial.start()
            
                # Run animation
                time.sleep(1)
                self.start_animation()
        except KeyboardInterrupt:
            pass

        # Stop reading serial port
        self.stop = True
        if 'thread_read_serial' in locals():            
            thread_read_serial.join()


def autoselect_port(terms=None, index=0):
    "Autoselect to serial port, searching datalogger with terms"
    ports = [(p.device, p.name, p.description, p.hwid, p.vid, p.pid, p.serial_number,
              p.location, p.manufacturer, p.product, p.interface
              ) for p in list(serial.tools.list_ports.comports())]

    selected_ports = []
    if terms is not None:
        for p in ports:
            p_string = str(p)
            if any(t.lower() in p_string.lower() for t in terms):
                selected_ports.append(p)
    else:
        selected_ports = ports

    if len(selected_ports) > 0:
        print(f'Found {len(selected_ports)} devices with matching terms:')
        print(*selected_ports, sep="\n")
        return selected_ports[index][0]
    else:
        #print(f'Port index no. {index} not found in {len(selected_ports)} ports selected.')
        print('Port terms not found in serial ports.')
        return ""


if __name__=="__main__":
    ##########################################################################
    # CONFIGURATION PARAMETERS
    ##########################################################################

    # Automatic port selection.
    terms = ['D307R956', 'arduino', "2341:0043"] #terms to search automatically for a port.
    port = autoselect_port(terms, 0)
    # Else, select manually the port of the Datalogger in your computers.
    # You can find the right port name in:
    # Arduino Ide > tools > port or in Windows: device manager.
    #port = '/dev/ttyACM0'
    #port = '/dev/rfcomm0'
    #port = 'COM3'
    
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
    # 
    columns_to_plot = [
       #[1,2,3,4], # example multiple y_axis with column0/datetime as x_axis
       #[1],  # example one y_axis with column0/datetime as x_axis
       #[2, "P1real[W]"], # example with y_axis as column number and as column_name
       #[{"x":2,"y":[2,"P1real[W]"]}], # example with other column as x _axis
       #[{"x":2,"y":[3], "linestyle":"none"}], # example with extra plot kwd,
       [1],
       [2],
       [3],
       [4],
       [5],
       [6],
                       ]
    ##########################################################################

    # Create object dl type SerialDL and run it!
    dl = SerialDL(port, amount, columns_to_plot, unit)
    dl.run()

