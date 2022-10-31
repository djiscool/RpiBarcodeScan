
std::string getBarcode::getScan() {

    /*
    int fd, status;
    fd = open("/dev/input/by-id/usb-WIT_Electron_Company_WIT_122-UFS_V2.03-event-kbd", O_RDONLY);
    if(ioctl(fd, EVIOCGRAB, &status) == -1)
    	printf("EVIOCGRAB failed: %s\n", std::strerror(errno));
    */
    /*
    char * buf[13];
    read(fd, buf, 12);
    printf("%s\n");
    */

    std::cout << "Scan with scanner: ";
    std::string barcode;
    std::cin >> barcode;
    std::cout << "Scanned: " << barcode << std::endl;
    //std::cin >> barcode;

    return barcode;
}


void parseConfig(std::string filename, std::map<std::string,std::string> &varMap) {
