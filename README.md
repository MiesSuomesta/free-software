#
# Copyleft (2022) Lauri Jakku <lja@lja.fi>
# Lisence: GPL-3.0
#
# If you use this, please give me credit by putting my name and email address somewhere visible.
#
# Thank You! 
#
# -- lja

C++: 
	template_helpers.h:

		Template classes: LessThan, MoreThan & WithinRange

			USAGE: LessThan<double> less_than_50 {50}; MoreThan<double> more_than_50 {50}; 
				WithinRange<double> is_valid_percent {-100 , 100};

				bool pa = is_valid_percent(50); bool pb = is_valid_percent(0.35); bool pc 
				= is_valid_percent(101); bool la = less_than_50(50); bool lb = 
				less_than_50(0.35); bool lc = less_than_50(101); bool ma = 
				more_than_50(50); bool mb = more_than_50(0.35); bool mc = 
				more_than_50(101);


Python3:

	email/
		checkEmail.py

			USAGE: python3 checkEmail.py <email> [<email2> <email3> ..... <emailN>]

				Output:

					One argument:

						exit value 0 if email address is ok.

						exit value 1 if email address is not ok.

					two or more arguments:

						list of OK or NOK


PHP:

	wordpress/
		wordpress_email_checker.php
		wordpress_email_checker.patch

			USAGE: 

				cd <WP_ROOT>/wp-include
				patch -p0 < wordpress_email_checker.patch

				cp wordpress_email_checker.php <WP_ROOT>/wp-include


Happy coding....
