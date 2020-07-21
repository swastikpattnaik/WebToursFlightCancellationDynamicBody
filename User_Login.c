User_Login()
{
	web_set_sockets_option("SSL_VERSION", "2&3");

	srand(time(NULL));
		
	web_reg_find("Text=Welcome to the Web Tours site.",
				 "SaveCount=Welcome_Count",	             
				 LAST);

//	<input type="hidden" name="userSession" value="128842.050222762zzcHDVcpQfiDDDDDDQHzDpiHHHf"/>

	web_reg_save_param_ex(
		"ParamName=cUserSession",
		"LB=<input type=\"hidden\" name=\"userSession\" value=\"",
		"RB=\"/>",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="hidden" name="JSFormSubmit" value="off"/>
	
	web_reg_save_param_ex(
		"ParamName=cJSFormSubmit",
		"LB=<input type=\"hidden\" name=\"JSFormSubmit\" value=\"",
		"RB=\"/>",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
	lr_start_transaction("SC03_CancelFlight_T01_LaunchPage");

	web_url("WebTours", 
		"URL=http://{pServerName}:{pPortNo}/WebTours/", 
		"TargetFrame=", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t1.inf", 
		"Mode=HTML", 
		LAST);

	if(atoi(lr_eval_string("{Welcome_Count}")) > 0)
	{
		lr_end_transaction("SC03_CancelFlight_T01_LaunchPage", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC03_CancelFlight_T01_LaunchPage", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	lr_think_time(10);
	
	web_reg_find("Text=Welcome, <b>{pUsername}</b>, to the Web Tours reservation pages.",
				 "SaveCount=WelcomeHome_Count",	             
				 LAST);
	
	lr_start_transaction("SC03_CancelFlight_T02_Login");

	web_submit_data("login.pl", 
		"Action=http://{pServerName}:{pPortNo}/cgi-bin/login.pl", 
		"Method=POST", 
		"TargetFrame=body", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/nav.pl?in=home", 
		"Snapshot=t2.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=userSession", "Value={cUserSession}", ENDITEM, 
		"Name=username", "Value={pUsername}", ENDITEM, 
		"Name=password", "Value={pPassword}", ENDITEM, 
		"Name=login.x", "Value=71", ENDITEM, 
		"Name=login.y", "Value=10", ENDITEM, 
		"Name=JSFormSubmit", "Value={cJSFormSubmit}", ENDITEM, 
		LAST);

	if(atoi(lr_eval_string("{WelcomeHome_Count}")) > 0)
	{
		lr_end_transaction("SC03_CancelFlight_T02_Login", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC03_CancelFlight_T02_Login", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	lr_think_time(10);
	
	return 0;
}
