<?php
	require_once "Net/SMTP.php";
	//require("SMTP");

function getip($target)
{
	return gethostbyname($target);
}

function raw_check_email($targetaddress, $verbose = 0)
{

	$the_hostname = gethostname();
	$target_components = explode('@', $targetaddress, 2);
 	$target_email_name   = $target_components[0];
 	$target_email_domain = $target_components[1];

	if ($verbose > 0)
		print("target_email_name   found: $target_email_name\n");

	if ($verbose > 0)
		print("target_email_domain found: $target_email_domain\n");

	$target_dns_get_record = dns_get_record($target_email_domain, DNS_MX)[0];

	if ( ! $target_dns_get_record )
	{
		print("MX record not found: $target_dns_get_record\n");
		return false;
	}

	$target_domain_mx = $target_dns_get_record['host'];
	if ($verbose > 0)
		print("MX record found: $target_domain_mx\n");
	//print_r($target_dns_get_record);

	$target_domain_mx_ip = getip($target_domain_mx);
	$the_hostname_ip = getip($the_hostname);

	$smtp = new Net_SMTP($target_domain_mx_ip, 25, $the_hostname_ip, false, 5000);

	$rv = $smtp->connect();
	//print_r($connect);
	if ( ! $rv ) 
	{
		print("MX connect respond error: $the_hostname( $the_hostname_ip ): $rv\n");
		return false;
	}
	if ($verbose > 0)
		print("MX $target_domain_mx ( $target_domain_mx_ip ) connected....\n");
	
	$rv = $smtp->helo($the_hostname_ip);
	//print_r($helo);
	if ( ! $rv ) 
	{
		print("MX helo respond error: $the_hostname( $the_hostname_ip ): $rv\n");
		return false;
	}
	if ($verbose > 0)
		print("MX HELO done to $target_domain_mx ( $target_domain_mx_ip )....\n");

	$rv = $smtp->mailFrom("noname@foo.bar");
	//print_r($helo);
	if ( $rv != 1) 
	{
		print("MX mail respond error: $the_hostname( $the_hostname_ip )\n");
		print_r($rv);
		return false;
	}

	if ($verbose > 0)
		print("MX MailFrom done to $target_domain_mx ( $target_domain_mx_ip )....\n");
	
	$rcpt = $smtp->rcptTo($targetaddress);
	if ( ! is_int($rcpt) ) 
	{
		print("MX rcpt respond error: $targetaddress: $rcpt\n");
		return false;
	}
	
	if ($verbose > 0)
		print("MX rcptTo done to $targetaddress....\n");

	$smtp->disconnect();
	return true;
}

function check_email($targetaddress, $ver)
{
	try {
		return raw_check_email($targetaddress, $ver);
	} catch (Exception $e)
	{
		print($e);
		return false;
	}
}


?>
