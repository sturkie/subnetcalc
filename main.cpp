#include <iostream>
#include <vector>
#include <string>
#include "input_check.h"

using namespace std;

//Outputs a vector with each element in its own line
void output_vector(vector<int> v){
    
    for(int i = 0; i < v.size(); i++){
        cout << v.at(i) << endl;
    }
    return;
}

//Outputs a vector in dot decimal form
void output_ip(vector<int> v){
    for(int i = 0; i < v.size()-1; i++){
        cout << v.at(i) << ".";
    }
    cout << v.back();
    return;
}

//Add 1 to address
void add_addr(vector<int>& addr, int index){
    if(addr.at(index) == 255){
        addr.at(index-1) += 1;
        addr.at(index) = 0;
    }
    return;
}

//Convert CIDR notation to Dot Decimal format
vector<int> cidr_to_dd(int cidr){
    vector<int> mask;
    int counter = 0;
    int interesting_octet = 0x00;
    
    for(int i = 0; i < cidr; i++){
        counter++;
        if(counter==8){
            mask.push_back(255);
            counter=counter-8;
        }
    }
    
    //Calculate the subnet mask for the interesting octet
    while(counter > 0){
        unsigned char bin = 0x80; //1000 0000
        
        //Move counter - 1 times
        bin = bin >> (counter-1);
        
        //OR with interesting octet
        interesting_octet = interesting_octet | bin;
        counter--;
    }
    //Add the interesting octet value to the mask
    if(mask.size() < 4){
        mask.push_back(interesting_octet);
    }
    
    //Any remaining octets fill with 0
    while(mask.size() < 4){
        mask.push_back(0);
    }
    
    cout << "Mask: ";
    output_ip(mask);
    cout << endl;
    
    return mask;
}



//Find the subnet of the given IP address
vector<int> find_network(vector<int> ip, vector<int> mask){
    vector<int> network;
    
    //AND the network with the subnet mask
    for(int i = 0; i < ip.size(); i++){
        network.push_back(ip.at(i) & mask.at(i));
    }
    
    return network;
}

//Find the broadcast address by turning all host bits to 1
vector<int> find_broadcast(vector<int> network, vector<int> mask){
    vector<int> broadcast;
    vector<unsigned int> inv_mask;
    
    //Turn all host bits to 1 (invert the mask)
    for(int i = 0; i < mask.size(); i++){
        inv_mask.push_back(0xff & ~mask.at(i));
    }
    
    //OR network with inv_mask to get broadcast address
    for(int i = 0; i < network.size(); i++){
        broadcast.push_back(network.at(i) | inv_mask.at(i));
    }
    
    return broadcast;
}

//Find the first host of the given subnet
//Add 1 to network address
vector<int> find_first(vector<int> network){
    vector<int> first;
    for(int i = 0; i < network.size(); i++){
        first.push_back(network.at(i));
    }
    first.back() = first.back()+1;
    
    return first;
}

//Find the last host of a given subnet
//Subtract 1 from broadcast address
vector<int> find_last(vector<int> broadcast){
    vector<int> last;
    for(int i = 0; i < broadcast.size(); i++){
        last.push_back(broadcast.at(i));
    }
    last.back() = last.back() - 1;
    return last;
}


// This function will combine all of the different calculations for a total subnet including: network, broadcast, first host, last host, and next subnet
void calculate_subnet(int cidr, vector<int> ip, vector<int> mask){
    
    vector<int> network;
    vector<int> broadcast;
    vector<int> first;
    vector<int> last;
    vector<int> next;
    vector<string> subnet;
    
    //Base cases (e.g. /31, /32)
    if(mask.back() == 254){ //31
        //Network
        network = find_network(ip, mask);
        cout << endl;
        cout << "Network: ";
        output_ip(network);
        cout << endl;
        
        //Broadcast
        broadcast = find_broadcast(network, mask);
        cout << "Broadcast: ";
        output_ip(broadcast);
        cout << endl;
        
        cout << "No usable host IPs\n";
        return;
    }
    else if(mask.back() == 255){ //32
        //Network
        network = find_network(ip, mask);
        cout << endl;
        cout << "Network: ";
        output_ip(network);
        cout << endl;
        
        //Broadcast
        broadcast = network;
        cout << "Broadcast: ";
        output_ip(broadcast);
        cout << "\nNo usable host IPs\n";
        return;
    }
    
    //Network
    //First, find the network this IP address exists on based on the IP address and subnet mask
    network = find_network(ip, mask);
    cout << endl;
    cout << "Network: ";
    output_ip(network);
    cout << endl;
    
    //Broadcast
    broadcast = find_broadcast(network, mask);
    cout << "Broadcast: ";
    output_ip(broadcast);
    cout << endl;
    
    //First host
    first = find_first(network);
    cout << "First host: ";
    output_ip(first);
    cout << endl;
    
    //Last host
    last = find_last(broadcast);
    cout << "Last host: ";
    output_ip(last);
    cout << endl;
    
    //Next subnet
    /*
     TODO: Next subnet calculation
     */
    
    
    return;
}

//Will parse through the user input string and break each octet down into its own index in a vector.
vector<int> parse(string input_ip){
    vector<int> ip;
    int prev_pos = 0;
    
    //base cases
    //only 1 number
    if(input_ip.find('.') == string::npos){
        ip.push_back(-1);
        return ip;
    }
    //Ends in '.'
    if(input_ip.back()=='.'){
        ip.push_back(-1);
        return ip;
    }
    
    for(int i=0; i<input_ip.length(); i++){
        if(input_ip.at(i) == '.'){
            if(prev_pos==0){ //first octet
                ip.push_back(stoi(input_ip.substr(prev_pos,i)));
            }
            else{ //2nd and 3rd octet
                ip.push_back(stoi(input_ip.substr(prev_pos+1, i-prev_pos+1)));
            }
            prev_pos = i;
        }
    }
    //last octet
    ip.push_back(stoi(input_ip.substr(prev_pos+1)));
    
    return ip;
}


int main(){
    
    string input_ip;
    int format = 0;
    vector<int> ip;
    int cidr = -1;
    string subnet_mask;
    vector<int> mask;
    
    cout << "=================================\n\tSubnet Calculator\n=================================\n";
    
    do{
        cout << "Enter IPv4 Address (A.B.C.D): ";
        cin >> input_ip;
    }
    while(check_ip(parse(input_ip)));
    /*
     TODO: Implement a more strict IP address error check (prevent seg faults and out of range errors)
     */
    
    ip = parse(input_ip);
    while(format > 2 || format <= 0){
        cout << "Enter [1] for CIDR or [2] for dot decimal format: ";
        cin >> format;
    }
    if(format == 1){ //CIDR
        cout << "Enter a CIDR value between 9-32\n";
        do{
            cout << "/";
            cin >> cidr;
        } while( cidr > 32 || cidr < 9);
        //Converts CIDR to dot decimal format
        mask = cidr_to_dd(cidr);
    }
    else if(format == 2){ //Dot decimal format
        do{
            cout << "Enter subnet mask (A.B.C.D): ";
            cin >> subnet_mask;
        }
        while(check_mask(parse(subnet_mask)));
        /*
         TODO: Implement a more strict subnet mask error check
         */
        
        mask = parse(subnet_mask);
    }
    
    calculate_subnet(cidr, ip, mask);
    
    return 0;
}
