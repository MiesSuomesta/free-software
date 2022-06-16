<?php

namespace PHPMailer\PHPMailer;

require_once __DIR__ . '/PHPMailer/SMTP.php';

function lja_getip($target)
{
	return gethostbyname($target);
}

function lja_mailer_raw_check_email($targetaddress, $verbose = 0)
{

	$the_hostname = gethostname();
	$target_components = explode('@', $targetaddress, 2);
	$target_email_name   = $target_components[0];
	$target_email_domain = $target_components[1];

	if ($verbose > 0)
		error_log("target_email_name   found: $target_email_name\n");

	if ($verbose > 0)
		error_log("target_email_domain found: $target_email_domain\n");

	$target_dns_get_records = dns_get_record($target_email_domain, DNS_MX);
	$target_dns_get_record = false;

	if ( is_bool($target_dns_get_records) )
	{
		error_log("MX record not found: $target_dns_get_record\n");
		return false;
	} else {
		$target_dns_get_record = $target_dns_get_records[0];
	}

	$target_domain_mx = $target_dns_get_record['host'];
	if ($verbose > 0)
		error_log("MX record found: $target_domain_mx\n");
	//print_r($target_dns_get_record);

	$target_domain_mx_ip = lja_getip($target_domain_mx);
	$the_hostname_ip = lja_getip($the_hostname);

	$smtp = new SMTP();

	$rv = $smtp->connect($target_domain_mx_ip);
	//print_r($connect);
	if ( ! $rv ) 
	{
		error_log("MX connect respond error: $the_hostname( $the_hostname_ip ): $rv\n");
		return false;
	}
	if ($verbose > 0)
		error_log("MX $target_domain_mx ( $target_domain_mx_ip ) connected....\n");
	
	$rv = $smtp->hello($the_hostname_ip);
	//print_r($helo);
	if ( ! $rv ) 
	{
		error_log("MX hello respond error: $the_hostname( $the_hostname_ip ): $rv\n");
		return false;
	}
	if ($verbose > 0)
		error_log("MX HELO done to $target_domain_mx ( $target_domain_mx_ip )....\n");

	$rv = $smtp->mail("noname@foo.bar");
	//print_r($helo);
	if ( $rv != 1) 
	{
		error_log("MX mail respond error: $the_hostname( $the_hostname_ip )\n");
		error_log($rv);
		return false;
	}

	if ($verbose > 0)
		error_log("MX MailFrom done to $target_domain_mx ( $target_domain_mx_ip )....\n");
	
	$rcpt = $smtp->recipient($targetaddress);
	if ( $rcpt != 1 ) 
	{
		error_log("MX recipient respond error: $targetaddress: $rcpt\n");
		return false;
	}
	
	if ($verbose > 0)
		error_log("MX rcptTo done to $targetaddress....\n");

	$smtp->close();
	
	if ($verbose > 0)
		error_log("MX connection closed....\n");
	
	return true;
}

function lja_mailer_check_email($targetaddress, $ver)
{
	try {
		return lja_mailer_raw_check_email($targetaddress, $ver);
	} catch (Exception $e)
	{
		print($e);
		return false;
	}
}


function lja_is_email_valid($email)
{
        $rv = lja_mailer_check_email($email, true);
	error_log("LJA email check: " . $email .": $rv");
        return $rv;
}

?>
