#!/usr/bin/python
import sys
import smtplib

emailcontent = 'fragsim232th_complete'

if len(sys.argv) > 1 :
	if len(sys.argv[1])!=0 :
		emailcontent = sys.argv[1]

# Import the email modules we'll need
from email.mime.text import MIMEText

# Open a plain text file for reading.  For this example, assume that
# the text file contains only ASCII characters.
fp = open(emailcontent,'rb')
# Create a text/plain message
msg = MIMEText(fp.read())
fp.close()

# me == the sender's email address
# you == the recipient's email address
msg['Subject'] = 'FragSim232Th Simulation Completed'
msg['From'] = 'jrtomps@physics.unc.edu'
msg['To'] = 'jrtomps@physics.unc.edu'

server = smtplib.SMTP('smtp.unc.edu',587) #port 465 or 587
server.ehlo()
server.starttls()
server.ehlo()
server.login('jrtomps','timuncpwd7!')
server.sendmail('jrtomps@unc.edu','jrtomps@physics.unc.edu',msg.as_string())
server.close()
