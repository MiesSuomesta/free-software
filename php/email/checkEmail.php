<?php

require_once "Net/SMTP.php";

function lja_email_checker_getip($target)
{
	return gethostbyname($target);
}

function lja_log_print($msgs)
{
	$parr = $msgs;
	if (! is_array($msgs) )
		$parr = explode("\n", $msgs);
	
	foreach ($parr as $pstr)
	{
		$pstr = trim($pstr);
		$pstr = rtrim($pstr);
		//print($pstr);
		error_log($pstr, 0);
	}
	
}

function lja_email_checker_handle_return_value_messaging($rv, $msgError , $msgOk, $msgVerbose, $print_error, $given_rv)
{
	if ($rv) {
		if ($msgVerbose) {
			lja_log_print($msgOk);
			if ($print_error) {
				lja_log_print($given_rv);
			}
		}
	} else {
		lja_log_print($msgError);
		if ($print_error) {
			lja_log_print($given_rv);
		}
	}
}

function lja_email_checker_handle_return_value_ok($given_rv, $expected , $msgError , $msgOk = False, $msgVerbose = False, $print_error = False)
{
	$rv = False;
	if (        is_bool($given_rv) ) {
		$rv = ($given_rv == $expected);
	} else if ( is_int($given_rv) ) {
		$rv = ($given_rv == $expected);
	} else {
		if ( is_object($rv) )
		{
			$rc = $rv->getCode();
			$rv =($rc == $expected);
		}
	}
	
	lja_email_checker_handle_return_value_messaging($rv, $msgError , $msgOk, $msgVerbose, $print_error, $given_rv);
	
	return $rv;
}

function lja_email_checker_raw_check_email($targetaddress, $verbose = False, $print_error = False)
{

	$the_hostname = gethostname();
	$target_components = explode('@', $targetaddress, 2);
 	$target_email_name   = $target_components[0];
 	$target_email_domain = $target_components[1];

	/* email lenght check */
	$msgName     = "Email user  : $target_email_name";
	$msgDomain   = "Email domain: $target_email_domain";
	
	$uOK = strlen($target_email_name) > 0;
	$dOK = strlen($target_email_domain) > 1;
	
	$rv = $uOK && $dOK;
	
	if ($uOK)
		$uOK = "OK";
	else
		$uOK = "NOK";
	
	if ($dOK)
		$dOK = "OK";
	else
		$dOK = "NOK";
	
	$msgOK     = "Email to properly check:\n$msgName\n$msgDomain";
	$msgError  = "Email not ok:\n$msgName ($uOK)\n$msgDomain ($uOK)";
	$the_rv = lja_email_checker_handle_return_value_ok(	$rv, 
								True, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == False )	return false;

	$target_dns_get_record = dns_get_record($target_email_domain, DNS_MX);

	if ( is_bool($target_dns_get_record) )
	{
		lja_log_print("MX record not found (Check 1)\n");
		return false;
	}
	
	

	if (! array_key_exists(0, $target_dns_get_record) )
	{
		lja_log_print("MX record not found (Check 2)\n");
		return false;
	}

	$target_domain_rec = $target_dns_get_record[0];
	
	if (! array_key_exists('host', $target_domain_rec) )
	{
		lja_log_print("MX record not found (Check 3)\n");
		return false;
	}
	
	//print_r($target_dns_get_record);
	
	$target_domain_mx = $target_domain_rec['host'];
	$target_domain_mx_ip = lja_email_checker_getip($target_domain_mx);
	//print_r($target_domain_mx_ip);

	/* MX Record check */
	$msgOK    = "MX record has no IP: $target_domain_mx";
	$msgError = "MX record found: $target_domain_mx ($target_domain_mx_ip)";
	$the_rv = lja_email_checker_handle_return_value_ok(	$target_domain_mx_ip,
								False, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == True )	return false;

	
	$the_hostname_ip = lja_email_checker_getip($the_hostname);

	/* Hostname IP check */
	$msgOK     = "Hostname IP check FAIL: $the_hostname";
	$msgError  = "Hostname IP check OK: $the_hostname ($the_hostname_ip)";
	$the_rv = lja_email_checker_handle_return_value_ok(	$the_hostname_ip, 
								False, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == True )	return false;

	$smtp = new Net_SMTP($target_domain_mx_ip, 25, $the_hostname_ip, false, 1);


	/* Connection check */
	$rv = $smtp->connect();
	$conn_from = "Connecting from : $the_hostname ($the_hostname_ip)";
	$conn_to   = "Connecting to   : $target_domain_mx ($target_domain_mx_ip)";
	$conn_mx   = "MX $target_domain_mx ( $target_domain_mx_ip ) connected....";
	$msgOK     = "$conn_from\n$conn_to\n$conn_mx";
	$msgError  = "Connect failed to: $target_domain_mx ($target_domain_mx_ip)";
	$the_rv = lja_email_checker_handle_return_value_ok(	$rv, 
								1, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == False )	return false;


	$rv = $smtp->helo($the_hostname_ip);

	/* HELO check */
	$msgOK     = "MX HELO done to $target_domain_mx ( $target_domain_mx_ip ) as $the_hostname( $the_hostname_ip )....";
	$msgError  = "MX HELO respond error: $the_hostname( $the_hostname_ip )";
	$the_rv = lja_email_checker_handle_return_value_ok(	$rv, 
								1, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == False )	return false;


	$rv = $smtp->mailFrom("noname@foo.bar");

	/* mailFrom check */
	$msgOK     = "MX mailFrom done to $target_domain_mx ( $target_domain_mx_ip ) as $the_hostname( $the_hostname_ip )....";
	$msgError  = "MX mailFrom respond error: $the_hostname( $the_hostname_ip )";
	$the_rv = lja_email_checker_handle_return_value_ok(	$rv, 
								1, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == False )	return false;

	
	$rv = $smtp->rcptTo($targetaddress);

	/* mailFrom check */
	$msgOK     = "MX rcpt respond done to $target_domain_mx ( $target_domain_mx_ip ), email $targetaddress is valid ....";
	$msgError  = "MX rcpt respond error: $targetaddress";
	$the_rv = lja_email_checker_handle_return_value_ok(	$rv, 
								1, 
								$msgError,
								$msgOK,
								$verbose,
								$print_error);
	if ( $the_rv == False )	return false;

	$smtp->disconnect();
	return true;
}

function lja_email_checker_check_email($targetaddress, $ver)
{
	try {
		return lja_email_checker_raw_check_email($targetaddress, $ver);
	} catch (Exception $e)
	{
		lja_log_print($e);
		return false;
	}
}

// Should fail
//lja_email_checker_check_email("lja@lja.fi", True);
//lja_email_checker_check_email("uhxndfrgrpt@m142.store", True);

?>
