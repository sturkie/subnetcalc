#include "input_check.h"

const int max_octet = 255;
const int min_octet = 0;

vector< vector<int> > range
{
    {0,0,0,0},
    {255,255,255,255}
};

vector<int> subnet_values {0,128,192,224,240,248,252,254,255};

//Checks to see if the values within the mask vector are valid mask values
bool check_value(vector<int> mask){
    
    unsigned char matches = 0;
    
    for(int i = 0; i < mask.size(); i++){
        for(int j = 0; j < subnet_values.size(); j++){
            if(mask.at(i) == subnet_values.at(j)){
                matches++;
            }
        }
    }
    
    if(matches == 4){
        return false;
    }

    return true;
    
}

bool check_ip(vector<int> ip){
    
    //Things to check for:
    // [x] If octets are in range 0 to 255
    // [x] If there are an appropriate amount of octets
    if(ip.front() == -1){
        cout << "Error found in parse. Try again.\n";
        return true;
    }
    
    if(ip.size() != 4){
        cout << "Error in IP size. Try again.\n";
        return true;
    }
    
    for(int i = 0; i < ip.size(); i++){
        //Check for value
        if(ip.at(i) > max_octet || ip.at(i) < min_octet){
            cout << "Error in octet size, must be 0-255. Try again.\n";
            return true;
        }
    }
    
    return false;
}

bool check_mask(vector<int> mask){
    
    //Things to check for
    // [x] If there are an appropriate amount of octets
    // [x] If the subnet is in correct format
    //      [x] If its using the valid values (255, 254, 252, 240, ...)
    //      [x] If its in the correct order (255.255.240.0 not 240.255.0.255)
    
    bool match = false;
    
    if(mask.front() == -1){
        cout << "Error found in parse. Try again.\n";
        return true;
    }
    
    if(mask.size() > 4){
        cout << "Error in mask size. Try again\n";
        return true;
    }
    
    //First element must be 255
    if(mask.at(0) != 255){
        cout << "Error in mask input (1). Try again.\n";
        return true;
    }
    
    if(check_value(mask)){
        cout << "Invalid mask value. Try again.\n";
        return true;
    }
    
    for(int i = 0; i < mask.size(); i++){
        //Check min/max
        if(mask.at(i) > max_octet || mask.at(i) < min_octet){
            cout << "Error in mask input (2). Try again.\n";
            return true;
        }
        
        //If mask is in descending order
        if(i+1 != mask.size()){
            if(mask.at(i) < mask.at(i+1)){
                cout << "Error in mask input (3). Try again.\n";
                return true;
            }
        }

    }

    return false;
}
