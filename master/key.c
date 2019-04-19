#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

#include "bot.h"

int key()
{
	Display*    dpy     = XOpenDisplay(0);
	Window      root    = DefaultRootWindow(dpy);
	XEvent      ev;

	unsigned int mods = 0;
	
	KeySym keys[6] = {XK_W, XK_S, XK_A, XK_D, XK_space, XK_Escape};

	for(int i=0; i < sizeof(keys)/sizeof(*keys) ; i++){
		int key = XKeysymToKeycode(dpy,keys[i]);
		XGrabKey(dpy, key, mods, root, 0, GrabModeAsync, GrabModeAsync);
	}

	XSelectInput(dpy, root, KeyPressMask);

	printf("waiting for keys\n");
	int exit = 0;
	do{
		XNextEvent(dpy, &ev);
		if(ev.type == KeyPress || ev.type == KeyRelease){
			KeySym K = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0);
			printf("keypress %lx\n", K);
			printf("%x\n",XK_W);
			switch(K){
			case XK_w:
				bots[0]->accelerate = ev.type == KeyPress ? 10 : 0;
				break;
			case XK_s:
				bots[0]->accelerate = ev.type == KeyPress ? -10 : 0;
				break;
			case XK_a:
				bots[0]->steer = ev.type == KeyPress ? 10 : 0;
				break;
			case XK_d:
				bots[0]->steer = ev.type == KeyPress ? -10 : 0;
				break;
			case XK_space:
				bots[0]->speed = 0;
				bots[0]->direction = 0;
				break;
			case XK_Escape:
				printf("escape\n");
				exit = 1;
				break;
			default: 
				printf("unmapped Key %lx\n", K);
				break;
			}
		}
	}while(!exit);

	for(int i=0; i < sizeof(keys)/sizeof(*keys) ; i++){
		int key = XKeysymToKeycode(dpy,keys[i]);
		XUngrabKey(dpy, key, mods, root);
	}
	XCloseDisplay(dpy);
	return 0;
}
