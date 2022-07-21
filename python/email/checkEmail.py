#!/usr/bin/python3
import sys
import re
import DNS as dns
import smtplib
import ssl
import imaplib
import socket


def check_email_plain(targetaddress, target_domain_mx_port):

    targetaddress = str(targetaddress)

    # Syntax check
    syntax_valid_email = re.match('^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,10})$', targetaddress)

    if syntax_valid_email == None:
        print(f"Validation error: Syntax is bad {targetaddress}")
        return ValueError('Bad syntax')

    target_email_name   = targetaddress.split('@')[0]
    target_email_domain = targetaddress.split('@')[1]

    #dns records: Get MX server
    records = dns.mxlookup(target_email_domain);

    tmp, target_domain_mx = records[0]
    target_domain_mx = str(target_domain_mx)

    print(f"MX: {target_domain_mx} is mailserver for {target_email_domain}");

    # Verify from MX
    the_host = socket.gethostname()
    target_mx_server = smtplib.SMTP(target_domain_mx)
    print(f"MX: {target_domain_mx} connecting....");
    #target_mx_server.set_debuglevel(10)
    target_mx_server.set_debuglevel(0)
    target_mx_server.connect(target_domain_mx, target_domain_mx_port)
    print(f"MX: {target_domain_mx} connected....");

    if target_domain_mx_port == 587:
        ssl_ctx = ssl.SSLContext(protocol=ssl.PROTOCOL_SSLv23)
        print(f"MX: {target_domain_mx} starttls() ....")
        target_mx_server.starttls(context = ssl_ctx)
        
    print(f"MX: {target_domain_mx} ehlo ....");
    target_mx_server.ehlo(the_host)
    print(f"MX: {target_domain_mx} mail ....");
    target_mx_server.mail("nobody@lja.fi")
    print(f"MX: {target_domain_mx} rcpt to ....");
    returncode, message = target_mx_server.rcpt(targetaddress)
    print(f"MX: {target_domain_mx} rcpt valid: {message} ({returncode})....");
    target_mx_server.quit()

    # If ok -> return True
    if returncode == 250:
        return True;

    return False;

def check_email(targetaddress):
    rv = False
    
    for portti in [587, 25]:
        print(f"Checking for port {portti}")
        rv = check_email_plain(targetaddress, portti)
        if (rv == True):
            break;

    try:
        if rv == False:
            pass
            #rv = check_email_plain(targetaddress)
    except Exception as e:
        print(f"Plain failure: {e}");

    return rv;


#try:
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

#except Exception as e:
#	print(f"FAILURE: {e}");

