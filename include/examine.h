#pragma once

typedef struct{
    size_t bytes_read;
    void* data;
}data_read;



int exemine(int argc,char** argv);
data_read* get_data_array(size_t count, size_t size,long adress);