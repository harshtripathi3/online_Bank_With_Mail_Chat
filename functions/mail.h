
int mail(char *to,char *bd);
int mail(char *to,char *bd) {

        char cmd[1024];  // to hold the command.
        //char to[] = ""; // email id of the recepient.
        char body[] = "Congratulations...\t:)\nCongratulations...Account created successfully ... Your account number is ";    // email body.
        strcat(body, bd);
        char tempFile[100];     // name of tempfile.
		
        strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

        FILE *fp = fopen(tempFile,"w"); // open it for writing.
        fprintf(fp,"%s\n",body);        // write body to it.
        fclose(fp);             // close it.

        sprintf(cmd,"sendmail %s < %s",to,tempFile); // prepare command.
        system(cmd);     // execute it.

        return 0;
}
