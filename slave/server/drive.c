
int drive(char mode){
	digitalWrite(pinLf,0);
	digitalWrite(pinRf,0);
	digitalWrite(pinLb,0);
	digitalWrite(pinRb,0);
	switch(mode){
		case 'L': 
			digitalWrite(pinRf,1);
			break; 
		case 'R': 
			digitalWrite(pinLf,1);
			break; 
		case 'F': 
			digitalWrite(pinRf,1);
			digitalWrite(pinLf,1);
			break; 
		case 'B': 
			digitalWrite(pinRb,1);
			digitalWrite(pinLb,1);
			break; 
	}                      
}                              
