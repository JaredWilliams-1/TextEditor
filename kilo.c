/*** includes ***/
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)

/*** data ***/
struct termios orig_termios;

void die(const char *s){
    perror(s);
    exit(1);
}

/*** terminal ***/

void disableRawMode(){
    // saves back to origin settings
    if (tcsetattr(STDERR_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode(){
    // This function gets the terminal attributes, puts them in a var
    // makes a copy, modifies the copy 
    // then saves it basically

    if (tcgetattr(STDERR_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // input flag
    raw.c_iflag &= ~(OPOST); // output
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); //local flags 
    raw.c_cc[VMIN] = 0; // cc = control characters
    raw.c_cc[VTIME] = 1;
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

// char editorReadKey

/*** init ***/

int main(){
    
    enableRawMode();

    while (1){
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");

        if (iscntrl(c)){ // tests if it is a control char (ASCII 0-31)
            printf("%\r\n", c);
        } else {
            printf("%d ('%c)rn\n", c, c); //prints the ascii value and the char itself
        }
        if (c == CTRL_KEY('q'))
            break;
    }

    return 0;
}
