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
		print(f'|   Validation error: Syntax is bad (targetaddress): ')
		return ValueError('Bad syntax')

	target_email_name   = targetaddress.split('@')[0]
	target_email_domain = targetaddress.split('@')[1]

	#dns records: Get MX server
	print(f'|   Looking up MX for {target_email_domain}.....')
	try:
		records = dns.mxlookup(target_email_domain);

		tmp, target_domain_mx = records[0]
		target_domain_mx = str(target_domain_mx)
		print(f'|   ...MX for {target_email_domain} is {target_domain_mx}')

		# Verify from MX
		# Set socket timeout

		the_host = socket.gethostname()
		target_mx_server = smtplib.SMTP()
		socket.setdefaulttimeout(5)
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

	except Exception as e:
		print(f"|   ...DNS/MX FAILURE: {e}");
		pass

	return False;

def parse_cc_cvs(inputfile):
    titlet = {}
    org = {}
    lst = []
    with open(inputfile, "r") as f:
        rl = f.read()
        org = rl.split(",\n")

    first = True
    for ln in org:
        rivilista = ln.split(",")  
        if first:
            first = False

            t = 0;
            for ta in rivilista:
                titlet[ta] = t;
                titlet[t] = ta;
                t += 1
        else:
            ln.replace("\n","")
            rivilista = ln.split(",")
            
            putsattu = []
            
            for ln in rivilista:
                    p = ln.replace("\n","")
                    p = p.replace("\"","")
                    putsattu.append(p)
                    
            lst.append(putsattu)

    return titlet, lst
    

#CreatedOn,ModifiedOn,FirstName,LastName,Phone,CompanyName,JobTitle,EmailAddress,Source,OptIn,OptOut,OptInBy,OptOutBy

def append_to_valid(exporttifile, value_array):
    outputfile = exporttifile + ".valid.csv"
    dline = ",".join(value_array)
    with open(outputfile, "a+") as f:
        f.write(dline)
        f.write("\n")
    


def parse_cc_and_print(exporttifile):
    titlet, arvot = parse_cc_cvs(exporttifile)
    
    emailIdx        = titlet['EmailAddress'];
    FirstNameIdx    = titlet['FirstName'];
    LastNameIdx     = titlet['LastName'];
    rv = [];

    for value_array in arvot:
    
        if len(value_array) < 1:
            break;
    
        try:
           emaili = value_array[emailIdx]
           enimi  = value_array[FirstNameIdx]
           snimi  = value_array[LastNameIdx]
        except Exception as e:
           continue

        print(f',---- Email of {enimi} {snimi} <{emaili}> --------------------')

        email_ok    = check_email(emaili)

        ok = "NOK"
        if email_ok == True:
            append_to_valid(exporttifile, value_array)
            ok = "OK"
        print(f'`----> Status: {ok}')
        rv.append([emaili, ok])

    print("Summary:");
    for values in rv:
        ve = values[0];
        va = values[1];
        print(f"{ve}: {va}")
    

#try:

if len(sys.argv) > 2:
    for exporttifile in sys.argv[1:]:
        parse_cc_and_print(exporttifile)


else:
    parse_cc_and_print(sys.argv[1])
#try:
#except Exception as e:
#    print(f"FAILURE: {e}");

