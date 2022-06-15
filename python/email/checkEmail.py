#!/usr/bin/python3
import sys
import re
import DNS as dns
import smtplib
import socket


def check_email(targetaddress):
	
	targetaddress = str(targetaddress)

	# Syntax check
	syntax_valid_email = re.match('^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$', targetaddress)

	if syntax_valid_email == None:
		print(f'Validation error: Syntax is bad (targetaddress)')
		return ValueError('Bad syntax')

	target_email_name   = targetaddress.split('@')[0]
	target_email_domain = targetaddress.split('@')[1]

	#dns records: Get MX server
	records = dns.mxlookup(target_email_domain);

	tmp, target_domain_mx = records[0]
	target_domain_mx = str(target_domain_mx)

	# Verify from MX
	the_host = socket.gethostname()
	target_mx_server = smtplib.SMTP()
	#target_mx_server.set_debuglevel(10)
	target_mx_server.set_debuglevel(0)
	target_mx_server.connect(target_domain_mx)
	target_mx_server.helo(the_host)
	target_mx_server.mail("nobody@lja.fi")
	returncode, message = target_mx_server.rcpt(targetaddress)
	target_mx_server.quit()

	# If ok -> return True
	if returncode == 250:
		return True;

	return False;

try:

	if len(sys.argv) > 2:
		for email in sys.argv[1:]:
			ok = check_email(email)
			if ok == True:
				print(f'Email {email}:  OK')
			else:
				print(f'Email {email}: NOK')
	else:
		ok = check_email(sys.argv[1])

		if ok == True:
			exit(0)

		exit(1)

except Exception as e:
	print(f"FAILURE: {e}");

