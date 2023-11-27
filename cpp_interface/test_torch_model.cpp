#include "FFISubgridModel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "H5Cpp.h"

using namespace H5;
using namespace std;

const H5std_string FILE_NAME("model_rl0_orthonormal.h5"); // input file

const int ngridzones = 20;
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
  
  int i, j, k, l;
  
  float fne_out[3][ngridzones][1][1];
  float fna_out[3][ngridzones][1][1];
  float fnx_out[3][ngridzones][1][1];
  float ne_out[1][ngridzones][1];
  float na_out[1][ngridzones][1];
  float nx_out[1][ngridzones][1];
  


  for (i = 0; i < ngridzones; i++)
  { 
   ne_out[0][i][0] = 0.0;
   na_out[0][i][0] = 0.0;
   nx_out[0][i][0] = 0.0;
   for (j = 0; j < 3; j++)
   {
    fne_out[j][i][0][0] = 0.0;
    fna_out[j][i][0][0] = 0.0;
    fnx_out[j][i][0][0] = 0.0;
   }
  }
  //------------------simulation data-------------------//
   
  H5File file (FILE_NAME, H5F_ACC_RDONLY); 
  const char* datasets_fn[3] = {"/fn_e(1|ccm)", "/fn_a(1|ccm)", "/fn_x(1|ccm)"};
  const char* datasets_n[3] = {"/n_e(1|ccm)", "/n_a(1|ccm)", "/n_x(1|ccm)"};
   
  hsize_t dimsm[4]; //memory space dimensions
  dimsm[0] = 3;
  dimsm[1] = ngridzones;
  dimsm[2] = 1;
  dimsm[3] = 1;
   
  hsize_t dimsmt[3]; // memory space dimensions
  dimsmt[0] = 1;
  dimsmt[1] = ngridzones;
  dimsmt[2] = 1;

  for (k = 0; k < ngridzones; k++)
  { 
   hsize_t offset[4]; //hyperslab offset in the file
   hsize_t count[4]; //size of the hyperslab in the file;
   offset[0] = 0;
   offset[1] = k;
   offset[2] = 0;
   offset[3] = 0;
   count[0] = 3; //3 --> x,y,z given by the first index of the dataset
   count[1] = 1; 
   count[2] = 1;
   count[3] = 1;

   hsize_t offset_out[4]; // hyperslab offset in memory
   hsize_t count_out[4]; // size of the hyperslab in memory
   offset_out[0] =0; 
   offset_out[1] =k; 
   offset_out[2] =0; 
   offset_out[3] =0; 
   count_out[0] = 3;
   count_out[1] = 1;
   count_out[2] = 1;
   count_out[3] = 1;

   DataSet d1 = file.openDataSet(datasets_fn[0]);
   DataSpace dataspace1 = d1.getSpace();
   dataspace1.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace1(4, dimsm);
   memspace1.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   d1.read(fne_out, PredType::NATIVE_FLOAT, memspace1, dataspace1);

   DataSet d2 = file.openDataSet(datasets_fn[1]);
   DataSpace dataspace2 = d2.getSpace();
   dataspace2.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace2(4, dimsm);
   memspace2.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   d2.read(fna_out, PredType::NATIVE_FLOAT, memspace2, dataspace2);
   
   DataSet d3 = file.openDataSet(datasets_fn[2]);
   DataSpace dataspace3 = d3.getSpace();
   dataspace3.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace3(4, dimsm);
   memspace3.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   d3.read(fnx_out, PredType::NATIVE_FLOAT, memspace3, dataspace3);

   hsize_t offsett[3]; // hyperslab offset in the file
   hsize_t countt[3];  // size of the hyperslab in the file
   offsett[0] = 0; 
   offsett[1] = k; 
   offsett[2] = 0; 
   countt[0] = 1;
   countt[1] = 1;
   countt[2] = 1;

   hsize_t offset_outt[3]; // hyperslab offset in memory
   hsize_t count_outt[3];  // size of the hyperslab in memory
   offset_outt[0] = 0;  
   offset_outt[1] = k; 
   offset_outt[2] = 0; 
   count_outt[0] = 1;
   count_outt[1] = 1;
   count_outt[2] = 1;

   DataSet d4 = file.openDataSet(datasets_n[0]);
   DataSpace dataspace4 = d4.getSpace();
   dataspace4.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace4(3, dimsmt);
   memspace4.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d4.read(ne_out, PredType::NATIVE_FLOAT, memspace4, dataspace4);
   
   DataSet d5 = file.openDataSet(datasets_n[1]);
   DataSpace dataspace5 = d5.getSpace();
   dataspace5.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace5(3, dimsmt);
   memspace5.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d5.read(na_out, PredType::NATIVE_FLOAT, memspace5, dataspace5);

   DataSet d6 = file.openDataSet(datasets_n[2]);
   DataSpace dataspace6 = d6.getSpace();
   dataspace6.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace6(3, dimsmt);
   memspace6.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d6.read(nx_out, PredType::NATIVE_FLOAT, memspace6, dataspace6);
   
  // for (l = 0; l < 3; l++)
   //{
    //cout << fne_out[l][k][0][0] << endl; 
    //cout << fna_out[l][k][0][0] << endl; 
    //cout << fnx_out[l][k][0][0] << endl; 
   //}
   //cout << ne_out[0][k][0] << endl; 
   //cout << na_out[0][k][0] << endl; 
   //cout << nx_out[0][k][0] << endl; 
  }
  //==================================================//
  // Create a sample tensor to pass through the model //
  //==================================================//
  // dimensions are [ngridzones, xyzt, nu/nubar, NF]
  torch::Tensor F4_in = torch::zeros({ngridzones,4,2,3});

  for (k = 0; k < ngridzones; k++)
  {
   for (l = 0; l < 3; l++)
   {
    F4_in.index_put_({k, l, 0, 0}, fne_out[l][k][0][0]);
    F4_in.index_put_({k, l, 1, 0}, fna_out[l][k][0][0]);
   }
   F4_in.index_put_({k, 3, 0, 0}, ne_out[0][k][0]);
   F4_in.index_put_({k, 3, 1, 0}, na_out[0][k][0]);
  }
  
  for (k = 0; k < ngridzones; k++)
  {
   for (j = 1; j < 3; j++)
   {
    for (i = 0; i < 2; i++)
    {
     for (l = 0; l < 3; l++)
     {
      F4_in.index_put_({k, l, i, j}, fnx_out[l][k][0][0]);
     }
     F4_in.index_put_({k, 3, i, j}, nx_out[0][k][0]);
    }
   }
  }
  cout << endl;
  cout << "input" << F4_in << endl;
 
  torch::Tensor u = torch::zeros({ngridzones,4});
  u.index_put_({Slice(),3}, 1.0);
  
  // put the input through the model 10 times
  auto output = F4_in;
  for(i=0; i<10; i++) output = model.predict_F4_Minkowski(output, u);
  
  // the expected result is an even mixture of all flavors
  torch::Tensor F4_expected = torch::zeros({ngridzones,4,2,3});
  F4_expected.index_put_({Slice(), 3, Slice(), Slice()}, 1.0);

  // check that the results are correct
  // by asserting that all elements are equal to 1 with an absolute and relative tolerance of 1e-2
  cout << "output" << output << endl;
 // assert(torch::allclose(output, F4_expected, 1e-2, 1e-2));
 
  return 0;
}
