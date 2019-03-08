


pid_t streamer(in_addr host , uint port){
	pid_t child = fork();
	if(child == 0){
		char cmd[256];
		sprintf(cmd ,"gst-launch-1.0 rpicamsrc bitrate=1000000 brightness=60 ! 'video/x-h264,width=640,height=480' ! h264parse ! queue ! rtph264pay config-interval=1 pt=96 ! udpsink host=%s port=%u", inet_ntoa(host), port);
		//wordexp_t arg;
		//if(wordexp(cmd,&arg,0)) perror("word");
		//char** w = arg.we_wordv;
		//execvp("gst-launch-1.0" ,w);
		//execl("/bin/sh", "sh", "-c", cmd, (char *) 0);
		system(cmd);
		//gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp" ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false
	}
	printf("child: %i\n", child);
	return child;

}
