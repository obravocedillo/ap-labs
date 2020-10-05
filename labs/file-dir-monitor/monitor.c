#include<stdio.h>
#include<sys/inotify.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h> 
#include<ftw.h>
#include<string.h>
#include"logger.h" 

#define MAX_EVENTS 2048
#define NAME_MAX 1024 
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int inotifyFd,wd;
char buf[BUF_LEN] __attribute__ ((aligned(8)));
ssize_t numRead;
char *p;
struct inotify_event *event;
int newDirectory = 0;

static void displayInotifyEvent(struct inotify_event *i){
    char oldName[NAME_MAX];
    if (i->mask & IN_CREATE){
        if(event->mask & IN_ISDIR){
            infof("- [Directory - Create] - %s\n",i->name);
            newDirectory = 1;
        }else{
            infof("- [File - Create] - %s\n",i->name);
        }
        
    } 
    if (i->mask & IN_DELETE){
        if(event->mask & IN_ISDIR){
            infof("- [Directory - Removal] - %s\n",i->name);
        }else{
            infof("- [File - Removal] - %s\n",i->name);
        }
        
    }
    if (i->mask & IN_MODIFY){
        if(event->mask & IN_ISDIR){
            infof("- [Directory - Modify] - %s\n",i->name);
        }else{
            infof("- [File - Modify] - %s\n",i->name);
        }
        
    } 

    if (i->mask & IN_MOVED_FROM){
        strcpy(oldName,i->name);
    } 

    if (i->mask & IN_MOVED_TO){
        if(i->cookie>0){
            if(event->mask & IN_ISDIR){
                infof("- [Directory- Rename] - %s -> %s\n",oldName,i->name);
            }else{
                infof("- [File - Rename] - %s -> %s\n",oldName,i->name);
            }
            
        }
        memset(oldName,0,strlen(oldName));  
    } 
  

}

 
int watchDocuments(const char *pathname, const struct stat *sb, int tflag){
    wd = inotify_add_watch(inotifyFd,pathname,IN_ALL_EVENTS);  
    if(wd == -1){
        return -1;
    }  
    if(newDirectory == 0){
        infof("Watching : %s\n",pathname);
    }
    return 0;
}

int main(int argc, char *argv[]){
    if(argc == 2){
        inotifyFd = inotify_init();
        if(inotifyFd == -1){
            errorf("Error initializing inotify");
            exit(0);
        }
        ftw(argv[1],watchDocuments,2048);
        
        for(;;){
      
            numRead = read(inotifyFd, buf, BUF_LEN);
            if (numRead == 0){
                infof("read() from inotify fd returned 0!");
            }
            for (p = buf; p < buf + numRead; ) {
                event = (struct inotify_event *) p;
                displayInotifyEvent(event);
                p += sizeof(struct inotify_event) + event->len;
            }

            if(newDirectory == 1){
                ftw(argv[1],watchDocuments,2048);
                newDirectory = 0;
            }
        }
        inotify_rm_watch( inotifyFd, wd );
        close( inotifyFd );
        return 0;
    }else{
        errorf("Error you are missing or using more arguments than needed");
        return -1;
    }
}