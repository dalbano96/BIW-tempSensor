# demoCore.py - Gather Serial Data and replace it in the HTML file.

import os
import serial
import cgi
CURRENTDIR = os.path.dirname(__file__)
BASEDIR = os.path.dirname(CURRENTDIR)


last_received = ''

import datetime

class DemoCore():

    def index(self,request):
        ser = serial.Serial('/dev/ttyACM0', 9600) # This is the Serial Port we gather information from. You could replace this with the inbuild port if you wish

#	form = cgi.FieldStorage() # Prep demoCore to receive input data
#        toggle = form.getvalue("toggle") # Retrieve user toggle, important for toggling device
#	toggle = "yes" 

#	if toggle == "yes":
#		ser.write("y")
#	else:
#		ser.write("n")

        data = self.receiving(ser)             
        
        html = self.showDemoHTML(data)

        return html

    def receiving(self, ser):
        global last_received
	data = ["","","","",""]
        # global ser

        buffer = ''
        while True:
            buffer = buffer + ser.read()
            if '\r\n' in buffer:
                tempbuffer = buffer.split('\r\n')
                buffer = tempbuffer[1]
                while True:
                    buffer = buffer + ser.read()
			if '\r\n' in buffer:
                        	data[0] = buffer 
		return data

    def showDemoHTML(self,data):
        ## reads an html file and does things with it
        ## there are better ways, but they are more complicated

        today = datetime.datetime.today()
        aDate = datetime.datetime.strftime(today,"%Y-%m-%d %H:%M:%S")

        f = open(CURRENTDIR +"/demo.html")
        html = f.read()
        html = html.replace("%TodaysDate%",aDate) # If %TodaysDate% Is in the html file it will be replace by the current time
	html = html.replace("%Temperature%",str(data[0]))
	html = html.replace("%pH Level%",str(data[1]))
	html = html.replace("%Dissolved Oxygen%",str(data[2]))
	html = html.replace("%Electrical Conductivity%",str(data[3]))

        return html

if __name__ == "__main__":
    print "Please Run index.py instead";
