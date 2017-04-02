# demoCore.py - Gather Serial Data and replace it in the HTML file.

import os
import serial
import cgi
import re
CURRENTDIR = os.path.dirname(__file__)
BASEDIR = os.path.dirname(CURRENTDIR)


last_received = ''

import datetime

class DemoCore():

    def index(self,request):
        ser = serial.Serial('/dev/ttyACM0', 9600) # This is the Serial Port we gather information from. You could replace this with the inbuild port if you wish
	data = ["","","","",""]

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
	buffer = ''
	while True:
		buffer = buffer + ser.read()
		if '\r\n' in buffer:
			# splitbuffer seems to store entire line in split notation
			splitbuffer = buffer.split('\r\n')

			# splitbuffer[1] stores nothing, carriage return?
			buffer = splitbuffer[1]
			while True:
				buffer = buffer + ser.read()
				if '\r\n' in buffer:
					# buffer in this scope stores the entire println string
					data[0] = buffer
					matchTemperature = re.match(r'(.*)Temperature: ([0-9]*)', buffer, re.M|re.I)
					if matchTemperature:
						data[1] = matchTemperature.group(2)
					matchPH = re.match(r'(.*)pH Levels: ([0-9]*)', buffer, re.M|re.I)
					if matchPH:
						data[2] = matchPH.group(2)
					matchOxygen = re.match(r'(.*)Dissolved Oxygen: ([0-9]*)', buffer, re.M|re.I)
					if matchOxygen:
						data[3] = matchOxygen.group(2)
					matchEConductivity = re.match(r'(.*)Electrical Conductivity: ([0-9]*)', buffer, re.M|re.I)
					if matchEConductivity:
						data[4] = matchEConductivity.group(2)
					return data
			

#		buffer = buffer + ser.read()
#		if '\r\n' in buffer:
#			tempbuffer = buffer.split('\r\n')
#			buffer = tempbuffer[1]
#			while True:
#				buffer = buffer + ser.read() 
#				if '\r\n' in buffer:
#					tempbuffer = buffer.split('\r\n')
#					buffer = tempbuffer[0]
#					return buffer

    def showDemoHTML(self,data):
        ## reads an html file and does things with it
        ## there are better ways, but they are more complicated

        today = datetime.datetime.today()
        aDate = datetime.datetime.strftime(today,"%Y-%m-%d %H:%M:%S")

        f = open(CURRENTDIR +"/demo.html")
        html = f.read()
        html = html.replace("%TodaysDate%",aDate) # If %TodaysDate% Is in the html file it will be replace by the current time
	html = html.replace("%Temperature%",str(data[1]))

	# used to test/debug data
	# html = html.replace("%tempData%",str(data[3]))

	html = html.replace("%pH%",str(data[2]))
	html = html.replace("%DissolvedOxygen%",str(data[3]))
	html = html.replace("%ElectricalConductivity%",str(data[4]))

        return html

if __name__ == "__main__":
    print "Please Run index.py instead";
