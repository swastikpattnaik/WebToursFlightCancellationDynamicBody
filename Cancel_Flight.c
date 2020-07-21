Cancel_Flight()
{
		
//	<input type="hidden" name="flightID" value="123311596-87802-SP"  />

	web_reg_save_param_ex(
		"ParamName=cFlightID",
		"LB=<input type=\"hidden\" name=\"flightID\" value=\"",
		"RB=\"",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="hidden" name=".cgifields" value="1"  />
	
	web_reg_save_param_ex(
		"ParamName=cCGIFields",
		"LB=<input type=\"hidden\" name=\".cgifields\" value=\"",
		"RB=\"",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);
	
	lr_start_transaction("SC03_CancelFlight_T03_Click_Itinerary");

	web_reg_find("Text=Flight Transaction Summary",
	             "SaveCount=TxnSummary_Count",
				 LAST);
	
	web_url("Itinerary Button", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/welcome.pl?page=itinerary", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/nav.pl?page=menu&in=home", 
		"Snapshot=t3.inf", 
		"Mode=HTML", 
		LAST);

	if(atoi(lr_eval_string("{TxnSummary_Count}")) > 0)
	{
		lr_end_transaction("SC03_CancelFlight_T03_Click_Itinerary", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC03_CancelFlight_T03_Click_Itinerary", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	lr_think_time(10);
		
	paramArray_count = atoi(lr_eval_string("{cFlightID_count}"));

	//For picking up a random booking to cancel
	sprintf(buffRow, "%d", rand() % paramArray_count + 1);

//	lr_output_message("%s", buffRow);
	
	lr_param_sprintf("pReqBody", "%s=on", buffRow);		

	for(i=1; i<=paramArray_count; i++)
	{		
		sprintf(pFlightID, "{cFlightID_%d}", i);
		
		lr_param_sprintf("flightIDval", "%s", lr_eval_string(pFlightID));
			
		lr_param_sprintf("pReqBody", "%s&flightID=%s", lr_eval_string("{pReqBody}"), lr_eval_string("{flightIDval}"));
		
		sprintf(pCGIField, "{cCGIFields_%d}", i);
		
		lr_param_sprintf("cgiFieldval", "%s", lr_eval_string(pCGIField));
			
		lr_param_sprintf("pReqBody", "%s&.cgifields=%s", lr_eval_string("{pReqBody}"), lr_eval_string("{cgiFieldval}"));
			
	}
	
	lr_param_sprintf("pReqBody", "%s&removeFlights.x=37&removeFlights.y=7", lr_eval_string("{pReqBody}"));

//	lr_output_message(lr_eval_string("{pReqBody}"));
	
	lr_start_transaction("SC03_CancelFlight_T04_Click_CancelChecked");

	web_reg_find("Text=Flights List", 
	             "SaveCount=FlightsList_Count",
				 LAST);

/*
	web_submit_data("itinerary.pl", 
		"Action=http://{pServerName}:{pPortNo}/cgi-bin/itinerary.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/itinerary.pl", 
		"Snapshot=t4.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=1", "Value={c1}", ENDITEM, 
		"Name=flightID", "Value={cFlightID}", ENDITEM, 
		"Name=removeFlights.x", "Value=48", ENDITEM, 
		"Name=removeFlights.y", "Value=7", ENDITEM, 
		"Name=.cgifields", "Value={cCGIFields}", ENDITEM, 
		LAST);
*/

	web_custom_request("itinerary.pl", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/itinerary.pl", 
		"Method=POST", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/itinerary.pl", 
		"Snapshot=t4.inf", 
		"Mode=HTTP", 
		"Body={pReqBody}", 
		LAST);
		
	if(atoi(lr_eval_string("{FlightsList_Count}")) > 0)
	{
		lr_end_transaction("SC03_CancelFlight_T04_Click_CancelChecked", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC03_CancelFlight_T04_Click_CancelChecked", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	lr_think_time(10);

	i = 1;
	
	lr_param_sprintf("pReqBody", "");		
		
	return 0;
}