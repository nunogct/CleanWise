#include "CWeightScale.h"

CWeightScale::CWeightScale(){
    total_weight = 0;
    new_weight = 0;
    status = 0;

    hx = new HX711; 

    hx->chip = gpiod_chip_open_by_name(chipname);
    hx->pd_sck = gpiod_chip_get_line(hx->chip, LOADCELL_SCK_PIN);
    hx->dout = gpiod_chip_get_line(hx->chip, LOADCELL_DOUT_PIN);
    hx->GAIN = 2.25;
    hx->OFFSET = 244000;
    hx->SCALE = 1000;
    // Set direction to output for pd_sck
    gpiod_line_request_output(hx->pd_sck, "pd_sck", 0);

    // Set direction to input for dout
        struct gpiod_line_request_config config = {
            .consumer = "dout",
            .request_type = GPIOD_LINE_REQUEST_DIRECTION_INPUT,
        };

    gpiod_line_request(hx->dout, &config, 0);
    gpiod_line_set_value(hx->pd_sck, 0);
    
    std::cout << "CWeightScale Object created with success!" << std::endl;
}

CWeightScale::~CWeightScale(){
    delete hx; 
    std::cout << "CWeightScale Object destructed with success!" << std::endl;
}

bool CWeightScale::HX711_is_ready() {
    int value = gpiod_line_get_value(hx->dout);
    return value==0;
}

int CWeightScale::convertValue(){
    unsigned long raw_value = 0;
    int data[3] = {0};
    int converted_value = 0;

    while (!HX711_is_ready()) {}

    for(int i = 2; i >= 0; i--){
        for(int j = 7; j >= 0; j--){
            gpiod_line_set_value(hx->pd_sck, 1);
            gpiod_line_set_value(hx->pd_sck, 0);
            raw_value = gpiod_line_get_value(hx->dout);
            data[i] <<= 1;
            data[i] |= raw_value;
        }
    }
    
    raw_value = ((unsigned long)data[2] << 16) | ((unsigned long)data[1] << 8) | ((unsigned long)data[0]);
    converted_value = hx->GAIN *  static_cast<int>(raw_value - hx->OFFSET) / hx->SCALE;
    return (converted_value <= 0) ? 0 : converted_value;

}
int CWeightScale::getNewWeight(){
    return new_weight;
}

void CWeightScale::run(){
    int reading_val = 0;

        if(HX711_is_ready()) {
            reading_val = convertValue();
            std::cout << "HX711 reading: " << reading_val << "g" << std::endl;
    }else{
        std::cout << "HX711 not ready!" << std::endl;
    }
    new_weight = reading_val - total_weight;
    //total_weight = reading_val;
}

void CWeightScale::saveTotalWeight(){
    total_weight = reading_value;
}

int CWeightScale::getTotalWeight(){
    return total_weight;
}

void CWeightScale::setStatus(bool status){
    this->status = status;
}

bool CWeightScale::getStatus(){
    return status;
}

/*int main(){
    CWeightScale ws;
    sleep(1);
    ws.run();
    std::cout << "Novo peso: " << ws.getNewWeight() << " gramas" << std::endl;
    sleep(1);
    ws.run();
    std::cout << "Novo peso: " << ws.getNewWeight() << " gramas" << std::endl;
    sleep(1);
    ws.run();
    std::cout << "Novo peso: " << ws.getNewWeight() << " gramas" << std::endl;
}*/   