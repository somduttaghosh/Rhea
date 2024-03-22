#include "FFISubgridModel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include <torch/torch.h>

using namespace H5;
using namespace std;
using namespace std::chrono;
const H5std_string INFILE_NAME("model_rl2_orthonormal.h5"); // input file

const int xgrid = 256;
const int ygrid = 256;
const int zgrid = 1;

const int ngridzones = xgrid*ygrid*zgrid;
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

  double fne_in[3][xgrid][ygrid][zgrid];
  double fna_in[3][xgrid][ygrid][zgrid];
  double fnx_in[3][xgrid][ygrid][zgrid];
  double ne_in[xgrid][ygrid][zgrid];
  double na_in[xgrid][ygrid][zgrid];
  double nx_in[xgrid][ygrid][zgrid];
  double delta[xgrid][ygrid][zgrid];
  
  for (i = 0; i < 3; i++)
  { 
   for (j = 0; j < xgrid; j++)
   {
   for (k = 0; k < ygrid; k++)
   {
   ne_in[i][j][k] = 0.0;
   na_in[i][j][k] = 0.0;
   nx_in[i][j][k] = 0.0;
   delta[i][j][k] = 0.0;
   for (l = 0; l < zgrid; l++)
   {
    fne_in[i][j][k][l] = 0.0;
    fna_in[i][j][k][l] = 0.0;
    fnx_in[i][j][k][l] = 0.0;
   }
   }
   }
  }
/*  for (i = 0; i < xgrid; i++)
  {
  for (j = 0; j < ygrid; j++)
  {
  for (k = 0; k < zgrid; k++)
  {
   ne_in[i][j][k] = 0.0;
   na_in[i][j][k] = 0.0;
   nx_in[i][j][k] = 0.0;
  }
  }
  }*/
  //------------------simulation data-------------------//
   
  H5File file (INFILE_NAME, H5F_ACC_RDONLY); 
  const char* datasets_fn[3] = {"/fn_e(1|ccm)", "/fn_a(1|ccm)", "/fn_x(1|ccm)"};
  const char* datasets_n[3] = {"/n_e(1|ccm)", "/n_a(1|ccm)", "/n_x(1|ccm)"};
   
  hsize_t dimsm[4]; //memory space dimensions
  dimsm[0] = 3;
  dimsm[1] = xgrid;
  dimsm[2] = ygrid;
  dimsm[3] = zgrid;
   
  hsize_t dimsmt[3]; // memory space dimensions
  dimsmt[0] = xgrid;
  dimsmt[1] = ygrid;
  dimsmt[2] = zgrid;

   hsize_t offset[4]; //hyperslab offset in the file
   hsize_t count[4]; //size of the hyperslab in the file;
   offset[0] = 0;
   offset[1] = 0;
   offset[2] = 0;
   offset[3] = 129;
   count[0] = 3; //3 --> x,y,z given by the first index of the dataset
   count[1] = xgrid; 
   count[2] = ygrid;
   count[3] = zgrid;
  
 // for (k = 0; k < ngridzones; k++)
  //{ 
   hsize_t offset_out[4]; // hyperslab offset in memory
   hsize_t count_out[4]; // size of the hyperslab in memory
   offset_out[0] =0; 
   offset_out[1] =0; 
   offset_out[2] =0; 
   offset_out[3] =0; 
   count_out[0] = 3;
   count_out[1] = xgrid;
   count_out[2] = ygrid;
   count_out[3] = zgrid;

   DataSet d1 = file.openDataSet(datasets_fn[0]);
   DataSpace dataspace1 = d1.getSpace();
   dataspace1.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace1(4, dimsm);
   memspace1.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   d1.read(fne_in, PredType::NATIVE_DOUBLE, memspace1, dataspace1);

   DataSet d2 = file.openDataSet(datasets_fn[1]);
   DataSpace dataspace2 = d2.getSpace();
   dataspace2.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace2(4, dimsm);
   memspace2.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   d2.read(fna_in, PredType::NATIVE_DOUBLE, memspace2, dataspace2);
   
   DataSet d3 = file.openDataSet(datasets_fn[2]);
   DataSpace dataspace3 = d3.getSpace();
   dataspace3.selectHyperslab(H5S_SELECT_SET, count, offset);
   DataSpace memspace3(4, dimsm);
   memspace3.selectHyperslab(H5S_SELECT_SET, count_out, offset_out);
   d3.read(fnx_in, PredType::NATIVE_DOUBLE, memspace3, dataspace3);

   hsize_t offsett[3]; // hyperslab offset in the file
   hsize_t countt[3];  // size of the hyperslab in the file
   offsett[0] = 0; 
   offsett[1] = 0; 
   offsett[2] = 129; 
   countt[0] = xgrid;
   countt[1] = ygrid;
   countt[2] = zgrid;

   hsize_t offset_outt[3]; // hyperslab offset in memory
   hsize_t count_outt[3];  // size of the hyperslab in memory
   offset_outt[0] = 0;  
   offset_outt[1] = 0; 
   offset_outt[2] = 0; 
   count_outt[0] = xgrid;
   count_outt[1] = ygrid;
   count_outt[2] = zgrid;

   DataSet d4 = file.openDataSet(datasets_n[0]);
   DataSpace dataspace4 = d4.getSpace();
   dataspace4.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace4(3, dimsmt);
   memspace4.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d4.read(ne_in, PredType::NATIVE_DOUBLE, memspace4, dataspace4);
   
   DataSet d5 = file.openDataSet(datasets_n[1]);
   DataSpace dataspace5 = d5.getSpace();
   dataspace5.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace5(3, dimsmt);
   memspace5.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d5.read(na_in, PredType::NATIVE_DOUBLE, memspace5, dataspace5);

   DataSet d6 = file.openDataSet(datasets_n[2]);
   DataSpace dataspace6 = d6.getSpace();
   dataspace6.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace6(3, dimsmt);
   memspace6.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d6.read(nx_in, PredType::NATIVE_DOUBLE, memspace6, dataspace6);
   
   // Predicted instability regions
   DataSet d7 = file.openDataSet("/crossing_discriminant");
   DataSpace dataspace7 = d7.getSpace();
   dataspace7.selectHyperslab(H5S_SELECT_SET, countt, offsett);
   DataSpace memspace7(3, dimsmt);
   memspace7.selectHyperslab(H5S_SELECT_SET, count_outt, offset_outt);
   d7.read(delta, PredType::NATIVE_DOUBLE, memspace7, dataspace7);

 // }
 
  /*for (i = 0; i < xgrid; i++)
   {
   for (j = 0; j < ygrid; j++)
   {
   for (k = 0; k < zgrid; k++)
   {
   cout << fne_in[0][i][j][k] << endl;
   } } }*/
  //==================================================//
  // Create a sample tensor to pass through the model //
  //==================================================//
  // dimensions are [ngridzones, xyzt, nu/nubar, NF]
  
  torch::Tensor F4_in = torch::zeros({ngridzones,4,2,3});
  i = 0; 
  for (j = 0; j < xgrid; j++)
  {
  for (l = 0; l < ygrid; l++)
  {
  for (k = 0; k < zgrid; k++)
  {
  F4_in.index_put_({i,0,0,0}, fne_in[0][j][l][k]);
  F4_in.index_put_({i,1,0,0}, fne_in[1][j][l][k]);
  F4_in.index_put_({i,2,0,0}, fne_in[2][j][l][k]);
  F4_in.index_put_({i,3,0,0}, ne_in[j][l][k]);

  F4_in.index_put_({i,0,1,0}, fna_in[0][j][l][k]);
  F4_in.index_put_({i,1,1,0}, fna_in[1][j][l][k]);
  F4_in.index_put_({i,2,1,0}, fna_in[2][j][l][k]);
  F4_in.index_put_({i,3,1,0}, na_in[j][l][k]);
  
  F4_in.index_put_({i,0,0,1}, fnx_in[0][j][l][k]/4.0);
  F4_in.index_put_({i,1,0,1}, fnx_in[1][j][l][k]/4.0);
  F4_in.index_put_({i,2,0,1}, fnx_in[2][j][l][k]/4.0);
  F4_in.index_put_({i,3,0,1}, nx_in[j][l][k]/4.0);
  
  F4_in.index_put_({i,0,1,1}, fnx_in[0][j][l][k]/4.0);
  F4_in.index_put_({i,1,1,1}, fnx_in[1][j][l][k]/4.0);
  F4_in.index_put_({i,2,1,1}, fnx_in[2][j][l][k]/4.0);
  F4_in.index_put_({i,3,1,1}, nx_in[j][l][k]/4.0);
  
  F4_in.index_put_({i,0,0,2}, fnx_in[0][j][l][k]/4.0);
  F4_in.index_put_({i,1,0,2}, fnx_in[1][j][l][k]/4.0);
  F4_in.index_put_({i,2,0,2}, fnx_in[2][j][l][k]/4.0);
  F4_in.index_put_({i,3,0,2}, nx_in[j][l][k]/4.0);
  
  F4_in.index_put_({i,0,1,2}, fnx_in[0][j][l][k]/4.0);
  F4_in.index_put_({i,1,1,2}, fnx_in[1][j][l][k]/4.0);
  F4_in.index_put_({i,2,1,2}, fnx_in[2][j][l][k]/4.0);
  F4_in.index_put_({i,3,1,2}, nx_in[j][l][k]/4.0);
  
  i++;
  } } }
  //cout << endl;
  //cout << "input" << F4_in << endl;
 
  // put the input through the model maxi times
  auto F4_out = F4_in;
  torch::Tensor X, y;
  
  int maxi(0);
  maxi = 10;
  cout << "Number of iterations (> 0)?" << maxi << endl;
  //cin >> maxi;
  
  cout << "Start of ML calculation" << endl;
  auto time_start = high_resolution_clock::now();

  for(int it=0; it<maxi; it++){
    X = model.X_from_F4_Minkowski(F4_out);
    y = model.predict_y(X);
    F4_out = model.F4_from_y(F4_out, y);
    
    // Remove changes when no instability expected
    i = 0;
    for (j = 0; j < xgrid; j++)
    {
    for (l = 0; l < ygrid; l++)
    {
    for (k = 0; k < zgrid; k++)
    {
    if(delta[j][l][k] <= 0.){
        // If no crossing, no transformation
        F4_out.index_put_({i, Slice(), Slice(), Slice()}, (F4_in.index({i, Slice(), Slice(), Slice()})));
    }
    i++;
    } } }
  }
  
  cout << "End of ML calculation" << endl;
  auto time_end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(time_end - time_start);
  
  cout << "Duration of ML calculation: " << duration.count() << " ms" << endl;
  
  cout << "output" << F4_out << endl;
 // assert(torch::allclose(output, F4_expected, 1e-2, 1e-2));
 
 torch::save({F4_in, F4_out}, "tensor_rl2_z130.pt");
 return 0;
}
