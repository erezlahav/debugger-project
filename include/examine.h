#pragma once

typedef struct{
    size_t bytes_read;
    void* data;
}data_read;



int exemine(int argc,char** argv);
data_read* get_data_array(int count, int size,long adress);