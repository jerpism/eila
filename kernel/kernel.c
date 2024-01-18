
void main(){
    unsigned char *vga = (unsigned char*)0xb8000;

    /* Print out a XD at the top of the screen*/
    *vga = 'X';
    *(vga+1) = 0x0f;

    vga+=2;

    *vga = 'D';
    *(vga+1) = 0x0f;

    return;
}
