#include "FFISubgridModel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "H5Cpp.h"

using namespace H5;
using namespace std;

const H5std_string FILE_NAME("test.out.h5"); // input file
const int xgrid = 201;
const int ygrid = 201;
const int zgrid = 101;

const int ngridzones = 2;

const int NX = 3;
const int NY = 4;
const int NZ = 2;
//======//
// main //
//======//
int main(int argc, const char* argv[]){
  if (argc != 2) {
    cerr << "usage: example-app <path-to-exported-script-module>\n";
    return -1;
  }
  
  FFISubgridModel<3> model{string(argv[1])};
  
  /*
  input array initialization
  */
  int i, j, k, c;
  float data_out[2][NX][NY];
   
  for (j = 0; j < 2; j++)
  {
   for (i = 0; i < NX; i++)
   {
    for (k = 0; k < NY; k++)
     data_out[j][i][k] = 0.0;
    }
   }
   //------------------simulation data-------------------//
   H5File file (FILE_NAME, H5F_ACC_RDONLY); // open simulation file
   const char* datasets_fn[3] = {"/fn_e(1|ccm)", "/fn_a(1|ccm)", "/fn_x(1|ccm)"};
   const char* datasets_n[3] = {"/n_e(1|ccm)", "/n_a(1|ccm)", "/n_x(1|ccm)"};
  for (c = 0; c < 2; c++)
  {
   hsize_t offset[4]; //hyperslab offset in the file
   hsize_t count[4]; //size of the hyperslab in the file;
   offset[0] = 0;
   offset[1] = c;
   offset[2] = 0;
   offset[3] = 0;
   count[0] = 3;
   count[1] = 1;
   count[2] = 1;
   count[3] = 1;

   hsize_t dimsm[4]; //memory space dimensions
   dimsm[0] = NX;
   dimsm[1] = NY;
   dimsm[2] = 2;
   dimsm[3] = 1;


  for (i = 0; i < 3; i++)
  {
   cout << datasets_fn[i] << endl;
   DataSet d = file.openDataSet(datasets_fn[i]);
   DataSpace dataspace = d.getSpace();
   dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace(4, dimsm);
   /*
    * Define memory hyperslab
    */
   hsize_t offset_out[4]; // hyperslab offset in memory
   hsize_t count_out[4]; // size of the hyperslab in memory
   offset_out[0] =i; 
   offset_out[1] =0; 
   offset_out[2] =c; 
   offset_out[3] =0; 
   count_out[0] = 1;
   count_out[1] = 3;
   count_out[2] = 1;
   count_out[3] = 1;
   memspace.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   /*
    * Read data from hyperslab in the file into the hyperslab in
    * memory and display the data.
    */
   d.read(data_out, PredType::NATIVE_FLOAT, memspace, dataspace);
  }

  hsize_t offsetn[3]; // hyperslab offset in the file
  hsize_t countn[3];  // size of the hyperslab in the file
  offsetn[0] = 0; 
  offsetn[1] = 1; 
  offsetn[2] = c; 
  countn[0] = 1;
  countn[1] = 1;
  countn[2] = 1;

  hsize_t dimsmn[3]; // memory space dimensions
  dimsmn[0] = NX;
  dimsmn[1] = NY;
  dimsmn[2] = 2;

  for (j = 0; j < 3; j++)
  {
   cout << datasets_n[j] << endl;
   DataSet d = file.openDataSet(datasets_n[j]);
   DataSpace dataspace = d.getSpace();
   dataspace.selectHyperslab(H5S_SELECT_SET, countn, offsetn);
   DataSpace memspace(3, dimsmn);
   /*
    * Define memory hyperslab
    */
   hsize_t offset_out1[3]; // hyperslab offset in memory
   hsize_t count_out1[3];  // size of the hyperslab in memory
   offset_out1[0] = j; 
   offset_out1[1] = 3; 
   offset_out1[2] = c;
   count_out1[0] = 1;
   count_out1[1] = 1;
   count_out1[2] = 1;
   memspace.selectHyperslab(H5S_SELECT_SET, count_out1, offset_out1);
   /*
    * Read data from hyperslab in the file into the hyperslab in
    * memory and display the data.
    */
   d.read(data_out, PredType::NATIVE_FLOAT, memspace, dataspace);
  }
  }
   for (j = 0; j < 2; j++)
   { 
    for (i = 0; i < NX; i++)
    {
     for (k = 0; k < NY; k++)
      cout << data_out[j][i][k] << " ";
      cout << endl;
   }
   cout << endl;
   }
  cout << data_out[0][0][0] << endl; 
  cout << data_out[0][0][1] << endl; 
  cout << data_out[0][0][2] << endl; 
  cout << data_out[0][0][3] << endl; 
  cout << data_out[0][1][0] << endl; 
  cout << data_out[0][1][1] << endl; 
  cout << data_out[0][1][2] << endl; 
  cout << data_out[0][1][3] << endl; 
  cout << data_out[0][2][0] << endl; 
  cout << data_out[0][2][1] << endl; 
  cout << data_out[0][2][2] << endl; 
  cout << data_out[0][2][3] << endl; 
  cout << data_out[1][0][0] << endl; 
  cout << data_out[1][0][1] << endl; 
  cout << data_out[1][0][2] << endl; 
  cout << data_out[1][0][3] << endl; 
  cout << data_out[1][1][0] << endl; 
  cout << data_out[1][1][1] << endl; 
  cout << data_out[1][1][2] << endl; 
  cout << data_out[1][1][3] << endl; 
  cout << data_out[1][2][0] << endl; 
  cout << data_out[1][2][1] << endl; 
  cout << data_out[1][2][2] << endl; 
  cout << data_out[1][2][3] << endl; 
  
  //==================================================//
  // Create a sample tensor to pass through the model //
  //==================================================//
  // dimensions are [ngridzones, xyzt, nu/nubar, NF]
  torch::Tensor F4_in = torch::zeros({ngridzones,4,2,3});
  F4_in.index_put_({0, 0, 0, 0}, data_out[0][0][0]);
  F4_in.index_put_({0, 0, 1, 0}, data_out[0][2][0]);
  F4_in.index_put_({0, 1, 0, 0}, data_out[0][0][2]);
  F4_in.index_put_({0, 1, 1, 0}, data_out[0][2][2]);
  F4_in.index_put_({0, 2, 0, 0}, data_out[0][1][0]);
  F4_in.index_put_({0, 2, 1, 0}, data_out[1][0][0]);
  F4_in.index_put_({0, 3, 0, 0}, data_out[0][1][2]);
  F4_in.index_put_({0, 3, 1, 0}, data_out[1][0][2]);
  
  F4_in.index_put_({1, 0, 0, 0}, data_out[0][0][1]);
  F4_in.index_put_({1, 0, 1, 0}, data_out[0][2][1]);
  F4_in.index_put_({1, 1, 0, 0}, data_out[0][0][3]);
  F4_in.index_put_({1, 1, 1, 0}, data_out[0][2][3]);
  F4_in.index_put_({1, 2, 0, 0}, data_out[0][1][1]);
  F4_in.index_put_({1, 2, 1, 0}, data_out[1][0][1]);
  F4_in.index_put_({1, 3, 0, 0}, data_out[0][1][3]);
  F4_in.index_put_({1, 3, 1, 0}, data_out[1][0][3]);
  
  F4_in.index_put_({0, 0, 0, 1}, data_out[1][1][0]);
  F4_in.index_put_({0, 0, 1, 1}, data_out[1][1][0]);
  F4_in.index_put_({0, 0, 0, 2}, data_out[1][1][0]);
  F4_in.index_put_({0, 0, 1, 2}, data_out[1][1][0]);
  
  F4_in.index_put_({0, 1, 0, 1}, data_out[1][1][2]);
  F4_in.index_put_({0, 1, 1, 1}, data_out[1][1][2]);
  F4_in.index_put_({0, 1, 0, 2}, data_out[1][1][2]);
  F4_in.index_put_({0, 1, 1, 2}, data_out[1][1][2]);
  
  F4_in.index_put_({0, 2, 0, 1}, data_out[1][2][0]);
  F4_in.index_put_({0, 2, 1, 1}, data_out[1][2][0]);
  F4_in.index_put_({0, 2, 0, 2}, data_out[1][2][0]);
  F4_in.index_put_({0, 2, 1, 2}, data_out[1][2][0]);
  
  F4_in.index_put_({0, 3, 0, 1}, data_out[1][2][2]);
  F4_in.index_put_({0, 3, 1, 1}, data_out[1][2][2]);
  F4_in.index_put_({0, 3, 0, 2}, data_out[1][2][2]);
  F4_in.index_put_({0, 3, 1, 2}, data_out[1][2][2]);
  
  F4_in.index_put_({1, 0, 0, 1}, data_out[1][1][1]);
  F4_in.index_put_({1, 0, 1, 1}, data_out[1][1][1]);
  F4_in.index_put_({1, 0, 0, 2}, data_out[1][1][1]);
  F4_in.index_put_({1, 0, 1, 2}, data_out[1][1][1]);
  
  F4_in.index_put_({1, 1, 0, 1}, data_out[1][1][3]);
  F4_in.index_put_({1, 1, 1, 1}, data_out[1][1][3]);
  F4_in.index_put_({1, 1, 0, 2}, data_out[1][1][3]);
  F4_in.index_put_({1, 1, 1, 2}, data_out[1][1][3]);
  
  F4_in.index_put_({1, 2, 0, 1}, data_out[1][2][1]);
  F4_in.index_put_({1, 2, 1, 1}, data_out[1][2][1]);
  F4_in.index_put_({1, 2, 0, 2}, data_out[1][2][1]);
  F4_in.index_put_({1, 2, 1, 2}, data_out[1][2][1]);

  F4_in.index_put_({1, 3, 0, 1}, data_out[1][2][3]);
  F4_in.index_put_({1, 3, 1, 1}, data_out[1][2][3]);
  F4_in.index_put_({1, 3, 0, 2}, data_out[1][2][3]);
  F4_in.index_put_({1, 3, 1, 2}, data_out[1][2][3]);
  int a, b, l;
  a = 0; b = 0;
  cout << "i j k l a l b" << endl;
  for (i = 0; i < 2; i++)
  {
   for (j = 0; j < 4; j++)
   {
    for (l = 0; l < 3; l ++)
    {
     for (k = 0; k < 2; k++)
     {
      if (i == 1) {
      a = 1;
      } 
      cout << i << j << k << l << a << l << b << endl;
      //F4_in.index_put_({i, j, k, l}, data_out[a][l][b]);
      b = b+2;
      if (b > 3) {
       break;
      }
     }
    }
   }
  }
  
  cout << "input" <<F4_in << endl;
  cout << endl;
  
  torch::Tensor u = torch::zeros({ngridzones,4});
  u.index_put_({Slice(),3}, 1.0);
  
  // put the input through the model 10 times
  auto output = F4_in;
  for(int i=0; i<10; i++) output = model.predict_F4_Minkowski(output, u);
  
  // the expected result is an even mixture of all flavors
  torch::Tensor F4_expected = torch::zeros({ngridzones,4,2,3});
  F4_expected.index_put_({Slice(), 3, Slice(), Slice()}, 1.0);

  // check that the results are correct
  // by asserting that all elements are equal to 1 with an absolute and relative tolerance of 1e-2
  cout << "output" << output << endl;
  //assert(torch::allclose(output, F4_expected, 1e-2, 1e-2));
 
  return 0;
}
