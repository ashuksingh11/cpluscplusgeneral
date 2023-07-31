// we are given mac address of format "aa:bb:cc:dd:11:22" we need to find whether
// it is a valid mac address or nto

#include<iostream>
#include <string>
const int validMacLen = 17;

bool IsValidHex(char ch) {
    // return true if between a-f or 0-9
    if( (std::toupper(ch) >= 'A' && std::toupper(ch) <= 'F') || 
        std::isdigit(ch) ) {
            return true;
    }
    else {
        return false;
    }
}

bool IsValidMac(std::string _mac) {
    // len should be 17 

    if(_mac.length() != validMacLen ) {
        return false;
    }

    for(int i=0; i<_mac.size(); i++) {
        char ch = _mac[i];
        //every 3rd char should be : or - (in some cases)
        if(i%3 == 2) {
            if (ch != ':' && ch != '-') {
                return false;
            }
        }
        else {
            
            // every other char than 3rd should be valid hex
            if(!IsValidHex(ch)) {
                return false;
            }
        }
    }

    return true;
}

int main() {
    std::string mac1 = "aa:bb:cc:dd:11:22";
    std::string mac2 = "aa:xx:cc:dd:11:22";

    std::cout << IsValidMac(mac2);

    return 0;
}